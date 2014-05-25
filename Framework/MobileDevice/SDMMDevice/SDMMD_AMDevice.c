/*
 *  SDMMD_AMDevice.c
 *  SDMMobileDevice
 *
 * Copyright (c) 2014, Sam Marshall
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the 
 * following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer 
 * 		in the documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of Sam Marshall nor the names of its contributors may be used to endorse or promote products derived from this
 * 		software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef _SDM_MD_AMDEVICE_C_
#define _SDM_MD_AMDEVICE_C_

// Ignore OS X SSL deprecation warnings
#pragma clang diagnostic ignored "-Wdeprecated-declarations"

#include "SDMMD_AMDevice.h"
#include "SDMMD_Functions.h"
#include "SDMMD_Service.h"
#include "SDMMD_USBMuxListener.h"
#include <string.h>
#include <errno.h>
#include <openssl/bio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/time.h>
#include "CFRuntime.h"
#include <CoreFoundation/CFBase.h>
#include <CoreFoundation/CFString.h>
#include "Core.h"
#include "SDMMD_Functions.h"
#include "SDMMD_AppleFileConduit.h"

#include <IOKit/IOKitLib.h>
#include <IOKit/usb/IOUSBLib.h>
#include <IOKit/IOCFPlugIn.h>
#include <mach/mach_port.h>

static Boolean SDMMD_AMDeviceRefEqual(CFTypeRef cf1, CFTypeRef cf2) {
    SDMMD_AMDeviceRef device1 = (SDMMD_AMDeviceRef)cf1;
    SDMMD_AMDeviceRef device2 = (SDMMD_AMDeviceRef)cf2;
	Boolean result = (device1->ivars.device_id == device2->ivars.device_id);
	if (!result) {
		// evaluate for usb vs wifi
		result = (CFStringCompare(device1->ivars.unique_device_id, device2->ivars.unique_device_id, 0) == kCFCompareEqualTo);
	}
	return result;
}

static CFStringRef SDMMD_AMDeviceRefCopyFormattingDesc(CFTypeRef cf, CFDictionaryRef formatOpts) {
    SDMMD_AMDeviceRef device = (SDMMD_AMDeviceRef)cf;
    return CFStringCreateWithFormat(CFGetAllocator(device), NULL, CFSTR("<SDMMD_AMDeviceRef %p>{device = %d}"), device, device->ivars.device_id);
}

static CFStringRef SDMMD_AMDeviceRefCopyDebugDesc(CFTypeRef cf) {
    SDMMD_AMDeviceRef device = (SDMMD_AMDeviceRef)cf;
    return CFStringCreateWithFormat(CFGetAllocator(device), NULL, CFSTR("<SDMMD_AMDeviceRef %p>{device = %d}"), device, device->ivars.device_id);
}

static void SDMMD_AMDeviceRefFinalize(CFTypeRef cf) {
    SDMMD_AMDeviceRef device = (SDMMD_AMDeviceRef)cf;
	CFSafeRelease(device->ivars.unique_device_id);
	
	if (device->ivars.lockdown_conn) {
		Safe(close,(uint32_t)device->ivars.lockdown_conn->connection);
		Safe(SSL_free,device->ivars.lockdown_conn->ssl);
	}
	CFSafeRelease(device->ivars.session);
	CFSafeRelease(device->ivars.service_name);
	CFSafeRelease(device->ivars.network_address);
	CFSafeRelease(device->ivars.unknown11);
	Safe(free,device);
}

static CFTypeID _kSDMMD_AMDeviceRefID = _kCFRuntimeNotATypeID;

static CFRuntimeClass _kSDMMD_AMDeviceRefClass = {0};

void SDMMD_AMDeviceRefClassInitialize(void) {
    _kSDMMD_AMDeviceRefClass.version = 0;
    _kSDMMD_AMDeviceRefClass.className = "SDMMD_AMDeviceRef";
    _kSDMMD_AMDeviceRefClass.init = NULL;
    _kSDMMD_AMDeviceRefClass.copy = NULL;
    _kSDMMD_AMDeviceRefClass.finalize = SDMMD_AMDeviceRefFinalize;
    _kSDMMD_AMDeviceRefClass.equal = SDMMD_AMDeviceRefEqual;
    _kSDMMD_AMDeviceRefClass.hash = NULL;
    _kSDMMD_AMDeviceRefClass.copyFormattingDesc = SDMMD_AMDeviceRefCopyFormattingDesc;
    _kSDMMD_AMDeviceRefClass.copyDebugDesc = SDMMD_AMDeviceRefCopyDebugDesc;
	_kSDMMD_AMDeviceRefClass.reclaim = NULL;
    _kSDMMD_AMDeviceRefID = _CFRuntimeRegisterClass((const CFRuntimeClass * const)&_kSDMMD_AMDeviceRefClass);
}

CFTypeID SDMMD_AMDeviceRefGetTypeID(void) {
    return _kSDMMD_AMDeviceRefID;
}

SDMMD_lockdown_conn* SDMMD_lockdown_connection_create(uint32_t socket) {
	SDMMD_lockdown_conn *lockdown = calloc(0x1, sizeof(SDMMD_lockdown_conn));
	if (socket != 0) {
		lockdown->connection = socket;
		Safe(free,lockdown->pointer);
		lockdown->length = 0;
	}
	return lockdown;
}

X509* SDMMD__decode_certificate(CFDataRef cert) {
	X509* result = NULL;
	if (cert) {
		BIO *newBIO = SDMMD__create_bio_from_data(cert);
		if (newBIO == 0) {
			printf("%s: Could not create BIO from CFData.\n",__FUNCTION__);
		}
		else {
			result = PEM_read_bio_X509(newBIO, NULL, NULL, NULL);
			if (result == NULL) {
				printf("%s: PEM_read_bio_X509 failed.\n",__FUNCTION__);
			}
			Safe(BIO_free,newBIO);
		}
		
	}
	return result;
}

int SDMMD__ssl_verify_callback(int value, X509_STORE_CTX *store) {
	bool result = true;
	X509 *cert = NULL, *decoded = NULL;
//	if (value || (X509_STORE_CTX_get_error(store) + 0xffffffffffffffec < 0x2)) {
	if (value ||
		X509_STORE_CTX_get_error(store) == X509_V_ERR_UNABLE_TO_GET_ISSUER_CERT_LOCALLY ||
		X509_STORE_CTX_get_error(store) == X509_V_ERR_UNABLE_TO_VERIFY_LEAF_SIGNATURE) {
		unsigned char* var_8 = NULL;
		unsigned char* var_16 = NULL;
		cert = X509_STORE_CTX_get_current_cert(store);
		if (cert) {
			SSL *storeSSL = (SSL *)X509_STORE_CTX_get_ex_data(store, SSL_get_ex_data_X509_STORE_CTX_idx());
			CFDataRef data = SSL_get_ex_data(storeSSL, (uint32_t)SDMMobileDevice->peer_certificate_data_index);
			decoded = SDMMD__decode_certificate(data);
			uint32_t data1 = i2d_X509(cert, NULL);
			uint32_t data2 = i2d_X509(decoded, NULL);
			if (data1 == data2) {
				uint32_t length1 = i2d_X509(cert, &var_16);
				uint32_t length2 = i2d_X509(decoded, &var_8);
				if (length1 == length2) {
					if (memcmp(var_8, var_16, length1) == 0) {
						result = true;
					}
					else {
						result = false;
					}
				}
				else {
					result = false;
				}
			}
			
		}
		else {
			printf("%s: Error verifying cert: unable to compare.\n",__FUNCTION__);
			result = false;
		}
		Safe(free,var_8);
		Safe(free,var_16);
	}
	else {
		printf("%s: Error verifying cert: (%d %s).\n",__FUNCTION__, value, X509_verify_cert_error_string(X509_STORE_CTX_get_error(store)));
	}
	return result;
}

SSL* SDMMD_lockssl_handshake(SDMMD_lockdown_conn *lockdown_conn, CFTypeRef hostCert, CFTypeRef deviceCert, CFTypeRef hostPrivKey, uint32_t num) {
	SSL *ssl = NULL;
	SSL_CTX *sslCTX = NULL;
	sdmmd_return_t result = kAMDSuccess;
	BIO_METHOD *bioMethod = BIO_s_socket();
	BIO *bioSocket = BIO_new(bioMethod);
	if (bioSocket) {
		BIO_set_fd(bioSocket, lockdown_conn->connection, 0);
		X509 *cert = SDMMD__decode_certificate(hostCert);
		if (cert == 0) {
			printf("_create_ssl_context: Could not certificate.\n");
		}
		RSA *rsa = NULL;
		BIO *dataBIO = SDMMD__create_bio_from_data(hostPrivKey);
		if (dataBIO == 0) {
			printf("_create_ssl_context: Could not decode host private key.\n");
			Safe(X509_free,cert);
		}
		else {
			PEM_read_bio_RSAPrivateKey(dataBIO, &rsa, NULL, NULL);
			Safe(BIO_free,dataBIO);
			if (rsa) {
				if (hostCert) {
					sslCTX = SSL_CTX_new(SSLv3_method());
					if (sslCTX) {
						result = SSL_CTX_use_certificate(sslCTX, cert);
						if (result == 0) {
							printf("_create_ssl_context: Could not set certificate.\n");
						}
						result = SSL_CTX_use_RSAPrivateKey(sslCTX, rsa);
						if (result == 0) {
							printf("_create_ssl_context: Could not set private key.\n");
						}
					}
					else {
						printf("_create_ssl_context: Could not create SSLv3 context.\n");
					}
				}
				//Safe(RSA_free,rsa);
				//Safe(X509_free,cert);
				if (sslCTX) {
					ssl = SSL_new(sslCTX);
					if (ssl) {
						if (num) {
							SSL_set_connect_state(ssl);
						}
						else {
							SSL_set_accept_state(ssl);
						}
						SSL_set_verify(ssl, SSL_VERIFY_PEER | SSL_VERIFY_FAIL_IF_NO_PEER_CERT, SDMMD__ssl_verify_callback);
						SSL_set_verify_depth(ssl, 0);
						SSL_set_bio(ssl, bioSocket, bioSocket);
						SSL_set_ex_data(ssl, (uint32_t)SDMMobileDevice->peer_certificate_data_index, (void*)deviceCert);
						result = SSL_do_handshake(ssl);
						if (result == 1) {
							SSL_CTX_free(sslCTX);
						}
						else {
							uint32_t err = SSL_get_error(ssl, result);
							if (err) {
								char *reason = SDMMD_ssl_strerror(ssl, err);
								printf("lockssl_handshake: SSL handshake fatal lower level error %d: %s.\n", err, reason);
							}
							else {
								char *reason = SDMMD_ssl_strerror(ssl, 0);
								printf("lockssl_handshake: SSL handshake controlled failure %d: %s.\n", err, reason);
							}
							Safe(SSL_free,ssl);
							ssl = 0x0;
						}
					}
					else {
						printf("_create_ssl: Could not create SSL thing.\n");
					}
				}
			}
			else {
				printf("_create_ssl_context: Could not decode private key.\n");
				Safe(X509_free,cert);
			}
		}
	}
	else {
		printf("%s: Could not create SSL bio.\n",__FUNCTION__);
	}
	return ssl;
}

sdmmd_return_t SDMMD_lockconn_enable_ssl(SDMMD_lockdown_conn *lockdown_conn, CFTypeRef hostCert, CFTypeRef deviceCert, CFTypeRef hostPrivKey, uint32_t num) {
	sdmmd_return_t result = kAMDSuccess;
	SSL *handshake = SDMMD_lockssl_handshake(lockdown_conn, hostCert, deviceCert, hostPrivKey, num);
	if (handshake) {
		lockdown_conn->ssl = handshake;
	}
	else {
		result = kAMDErrorError;
	}
	return result;
}

sdmmd_return_t SDMMD_lockconn_disable_ssl(SDMMD_lockdown_conn *lockdown_conn) {
	sdmmd_return_t result = kAMDSuccess;
	if (lockdown_conn->ssl) {
		result = SSL_shutdown(lockdown_conn->ssl);
		
		if (result == -1) {
			printf("%s: Could not shutdown SSL connection %d.\n",__FUNCTION__, -1);
		}
		SSL_free(lockdown_conn->ssl);
		lockdown_conn->ssl = NULL;
	}
	return result;
}

sdmmd_return_t SDMMD_lockconn_send_message(SDMMD_AMDeviceRef device, CFDictionaryRef dict) {
	sdmmd_return_t result = kAMDSuccess;
	if (device->ivars.lockdown_conn) {
		if (dict) {
			bool useSSL = (device->ivars.lockdown_conn->ssl ? true : false);
			SocketConnection conn;
			if (useSSL) {
				conn = (SocketConnection){true, {.ssl = device->ivars.lockdown_conn->ssl}};
			}
			else {
				conn = (SocketConnection){false, {.conn = (uint32_t)device->ivars.lockdown_conn->connection}};
			}
			
			SDMMD_ServiceSendMessage(conn, dict, kCFPropertyListXMLFormat_v1_0);
		}
		else {
			printf("%s: Could not encode message as XML.\n",__FUNCTION__);
		}
	}
	else {
		result = SDMMD_AMDeviceIsValid(device);
		if (result == false) {
			result = kAMDSendMessageError;
		}
	}
	return result;
}

sdmmd_return_t SDMMD_lockconn_receive_message(SDMMD_AMDeviceRef device, CFMutableDictionaryRef *dict) {
	sdmmd_return_t result = kAMDSuccess;
	if (device->ivars.lockdown_conn) {
		bool useSSL = (device->ivars.lockdown_conn->ssl ? true : false);
		SocketConnection conn;
		if (useSSL) {
			conn = (SocketConnection){true, {.ssl = device->ivars.lockdown_conn->ssl}};
	 	}
		else {
			conn = (SocketConnection){false , {.conn = (uint32_t)device->ivars.lockdown_conn->connection}};
		}
		
		result = SDMMD_ServiceReceiveMessage(conn, (CFPropertyListRef*)dict);
	}
	else {
		result = SDMMD_AMDeviceIsValid(device);
		if (result == kAMDSuccess) {
			result = kAMDReadError;
		}
	}
	return result;
}

CFTypeRef SDMMD_copy_lockdown_value(SDMMD_AMDeviceRef device, CFStringRef domain, CFStringRef key, CFStringRef *err) {
	CFTypeRef value = NULL;
	sdmmd_return_t result = kAMDSuccess;
	
	if (!device) {
		result = kAMDInvalidArgumentError;
	}
	CheckErrorAndReturn(result);
	
	if (device->ivars.lockdown_conn == 0) {
		result = kAMDNotConnectedError;
	}
	CheckErrorAndReturn(result);
	
	CFMutableDictionaryRef request = SDMMD__CreateMessageDict(CFSTR("GetValue"));
	if (request == NULL) {
		result = kAMDNoResourcesError;
	}
	CheckErrorAndReturn(result);
	
	CFMutableDictionaryRef response = NULL;
	if (domain && CFStringCompare(domain, CFSTR("NULL"), 0) != 0) {
		CFDictionarySetValue(request, CFSTR("Domain"), domain);
	}
	if (key && CFStringCompare(key, CFSTR("NULL"), 0) != 0) {
		CFDictionarySetValue(request, CFSTR("Key"), key);
	}
	
	result = SDMMD_lockconn_send_message(device, request);
	CFSafeRelease(request);
	CheckErrorAndReturn(result);
	
	result = SDMMD_lockconn_receive_message(device, &response);
	CheckErrorAndReturn(result);
	if (response) {
		CFStringRef error = CFDictionaryGetValue(response, CFSTR("Error"));
		if (error) {
			if (CFGetTypeID(error) == CFStringGetTypeID()) {
				if (err) {
					// Retain error if it is being passed to caller
					*err = CFRetain(error);
				}
				result = (sdmmd_return_t)SDMMD__ConvertLockdowndError(error);
			}
			else {
				result = kAMDInvalidResponseError;
			}
		}
		else {
			// Retain returned value from response
			value = CFRetain(CFDictionaryGetValue(response, CFSTR("Value")));
			result = kAMDSuccess;
		}
	}
	CFSafeRelease(response);
	
ExitLabel:
	if (!SDM_MD_CallSuccessful(result)) {
		printf("%s: Could not copy value (%x)\n", __FUNCTION__, result);
	}
	return value;
}

sdmmd_return_t SDMMD_send_set_value(SDMMD_AMDeviceRef device, CFStringRef domain, CFStringRef key, CFTypeRef value) {
	sdmmd_return_t result = kAMDSuccess;
	if (!device) {
		result = kAMDInvalidArgumentError;
	}
	CheckErrorAndReturn(result);
	
	if (device->ivars.lockdown_conn == NULL) {
		result = kAMDNotConnectedError;
	}
	CheckErrorAndReturn(result);
	
	if (key == NULL || value == NULL) {
		result = kAMDInvalidArgumentError;
	}
	CheckErrorAndReturn(result);
	
	CFMutableDictionaryRef setVal = SDMMD__CreateMessageDict(CFSTR("SetValue"));
	if (setVal == NULL) {
		result = kAMDNoResourcesError;
	}
	CheckErrorAndReturn(result);
	
	if (domain) {
		CFDictionarySetValue(setVal, CFSTR("Domain"), domain);
	}
	CFDictionarySetValue(setVal, CFSTR("Key"), key);
	CFDictionarySetValue(setVal, CFSTR("Value"), value);
	result = SDMMD_lockconn_send_message(device, setVal);
	CFSafeRelease(setVal);
	CheckErrorAndReturn(result);
	
	
	CFMutableDictionaryRef resultDict = NULL;
	result = SDMMD_lockconn_receive_message(device, &resultDict);
	if (result == kAMDSuccess) {
		result = SDMMD__ErrorHandler(SDMMD__ConvertLockdowndError, resultDict);
		CFSafeRelease(resultDict);
	}
	
	ExitLabelAndReturn(result);
}

sdmmd_return_t SDMMD_lockdown_connection_destory(SDMMD_lockdown_conn *lockdownCon) {
	sdmmd_return_t result = kAMDSuccess;
	if (lockdownCon) {
		Safe(SSL_free,lockdownCon->ssl);
		lockdownCon->ssl = NULL;
		if (lockdownCon->connection != 0xff) {
			result = close((uint32_t)lockdownCon->connection);
			if (result == 0xff) {
				printf("%s: close(2) on socket %lld failed: %d.\n",__FUNCTION__,lockdownCon->connection, result);
			}
			lockdownCon->connection = 0;
		}
		Safe(free,lockdownCon->pointer);
		result = kAMDSuccess;
		Safe(free,lockdownCon);
		lockdownCon = NULL;
	}
	return result;
}

sdmmd_return_t SDMMD_send_unpair(SDMMD_AMDeviceRef device, CFStringRef hostId) {
	sdmmd_return_t result = kAMDSuccess;
	
	if (!device) {
		result = kAMDInvalidArgumentError;
	}
	CheckErrorAndReturn(result);
	
	if (device->ivars.lockdown_conn == NULL) {
		result = kAMDNotConnectedError;
	}
	CheckErrorAndReturn(result);
	
	if (hostId == NULL) {
		result = kAMDInvalidArgumentError;
	}
	CheckErrorAndReturn(result);
	
	CFMutableDictionaryRef dict = SDMMD__CreateMessageDict(CFSTR("Unpair"));
	if (dict == NULL) {
		result = kAMDNoResourcesError;
	}
	CheckErrorAndReturn(result);
	
	const void *keys[1] = { CFSTR("HostID") };
	const void *values[1] = { hostId };
	CFDictionaryRef host = CFDictionaryCreate(kCFAllocatorDefault, keys, values, 1, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
	if (host) {
		CFDictionarySetValue(dict, CFSTR("PairRecord"), host);
		result = SDMMD_lockconn_send_message(device, dict);
		CFSafeRelease(host);
		if (SDM_MD_CallSuccessful(result)) {
			CFMutableDictionaryRef response = NULL;
			result = SDMMD_lockconn_receive_message(device, &response);
			//PrintCFType(response);
			if (SDM_MD_CallSuccessful(result)) {
				result = SDMMD__ErrorHandler(SDMMD__ConvertLockdowndError, response);
			}
		}
	}
	CFSafeRelease(dict);
	
	ExitLabelAndReturn(result);
}

sdmmd_return_t SDMMD_send_pair(SDMMD_AMDeviceRef device, CFMutableDictionaryRef pairRecord, CFTypeRef slip, CFTypeRef options, CFDataRef *escrowBag) {
    sdmmd_return_t result = kAMDSuccess;
	CFMutableDictionaryRef response = NULL;
	
	if (!device) {
		result = kAMDInvalidArgumentError;
	}
	CheckErrorAndReturn(result);
	
	if (device->ivars.lockdown_conn == NULL) {
		result = kAMDNotConnectedError;
	}
	CheckErrorAndReturn(result);
	
	if (pairRecord == NULL) {
		result = kAMDInvalidArgumentError;
	}
	CheckErrorAndReturn(result);
	
	if (escrowBag == NULL) {
		result = kAMDInvalidArgumentError;
	}
	CheckErrorAndReturn(result);
	
	CFMutableDictionaryRef pRecord = SDMMD__CreateMessageDict(CFSTR("Pair"));
	if (pRecord == NULL) {
		result = kAMDNoResourcesError;
	}
	CheckErrorAndReturn(result);
	
	CFDictionarySetValue(pRecord, CFSTR("PairRecord"), pairRecord);
	if (slip) {
		CFDictionarySetValue(pRecord, CFSTR("PermissionSlip"), slip);
	}
	if (options) {
		CFDictionarySetValue(pRecord, CFSTR("PairingOptions"), options);
	}
	result = SDMMD_lockconn_send_message(device, pRecord);
	CFSafeRelease(pRecord);
	CheckErrorAndReturn(result);
	
	result = SDMMD_lockconn_receive_message(device, &response);
	CheckErrorAndReturn(result);
	
	result = SDMMD__ErrorHandler(SDMMD__ConvertLockdowndError, response);
	
	if (SDM_MD_CallSuccessful(result)) {
		
		CFDataRef bagData = NULL;
		if (CFDictionaryContainsValue(response, CFSTR("ExtendedResponse"))) {
			bagData = CFDictionaryGetValue(response, CFSTR("ExtendedResponse"));
		}
		else {
			bagData = CFDictionaryGetValue(response, CFSTR("EscrowBag"));
		}
		
		if (bagData) {
			*escrowBag = CFRetain(bagData);
		}
		else {
			result = kAMDInvalidResponseError;
		}
	}
	CFSafeRelease(response);
	
	ExitLabelAndReturn(result);
}


sdmmd_return_t SDMMD_send_validate_pair(SDMMD_AMDeviceRef device, CFStringRef hostId) {
	sdmmd_return_t result = kAMDSuccess;
	
	if (!device) {
		result = kAMDInvalidArgumentError;
	}
	CheckErrorAndReturn(result);
	
	if (device->ivars.lockdown_conn == NULL) {
		result = kAMDNotConnectedError;
	}
	CheckErrorAndReturn(result);
	
	if (hostId == NULL) {
		result = kAMDInvalidArgumentError;
	}
	CheckErrorAndReturn(result);
	
	CFMutableDictionaryRef dict = SDMMD__CreateMessageDict(CFSTR("ValidatePair"));
	if (dict == NULL) {
		result = kAMDNoResourcesError;
	}
	CheckErrorAndReturn(result);
	
	const void *keys[1] = { CFSTR("HostID") };
	const void *values[1] = { hostId };
	CFDictionaryRef host = CFDictionaryCreate(kCFAllocatorDefault, keys, values, 1, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
	if (host) {
		CFDictionarySetValue(dict, CFSTR("PairRecord"), host);
		result = SDMMD_lockconn_send_message(device, dict);
		CFSafeRelease(host);
		if (SDM_MD_CallSuccessful(result)) {
			CFMutableDictionaryRef response = NULL;
			result = SDMMD_lockconn_receive_message(device, &response);
			//PrintCFType(response);
			if (SDM_MD_CallSuccessful(result)) {
				result = SDMMD__ErrorHandler(SDMMD__ConvertLockdowndError, response);
			}
			CFSafeRelease(response);
		}
	}
	CFSafeRelease(dict);
	
	ExitLabelAndReturn(result);
}

sdmmd_return_t SDMMD_copy_daemon_name(SDMMD_AMDeviceRef device, CFStringRef *name) {
	sdmmd_return_t result = kAMDSuccess;
	CFMutableDictionaryRef response = NULL;
	
	if (!device) {
		result = kAMDInvalidArgumentError;
	}
	CheckErrorAndReturn(result);
	
	if (device->ivars.lockdown_conn == NULL) {
		result = kAMDNotConnectedError;
	}
	CheckErrorAndReturn(result);
	
	if (name == NULL) {
		result = kAMDInvalidArgumentError;
	}
	CheckErrorAndReturn(result);
	
	CFMutableDictionaryRef queryDict = SDMMD__CreateMessageDict(CFSTR("QueryType"));
	if (queryDict == NULL) {
		result = kAMDNoResourcesError;
	}
	CheckErrorAndReturn(result);
	
	result = SDMMD_lockconn_send_message(device, queryDict);
	CFSafeRelease(queryDict);
	CheckErrorAndReturn(result);
	
	result = SDMMD_lockconn_receive_message(device, &response);
	CheckErrorAndReturn(result);
	if (response && CFDictionaryGetCount(response)) {
		CFTypeRef val = CFDictionaryGetValue(response, CFSTR("Error"));
		if (val == NULL) {
			val = CFDictionaryGetValue(response, CFSTR("Type"));
			if (val) {
				if (CFGetTypeID(val) == CFStringGetTypeID()) {
					CFRetain(val);
					*name = val;
				}
			}
			else {
				result = kAMDInvalidResponseError;
			}
		}
	}
	CFSafeRelease(response);
	
	ExitLabelAndReturn(result);
}

sdmmd_return_t SDMMD__CopyEscrowBag(SDMMD_AMDeviceRef device, CFDataRef *bag) {
	sdmmd_return_t result = kAMDInvalidArgumentError;
	if (device) {
		CFMutableDictionaryRef dict = NULL;
		result = SDMMD__CreatePairingRecordFromRecordOnDiskForIdentifier(device, &dict);
		if (result == kAMDSuccess) {
			CFTypeRef wifiValue = NULL;
			if (!CFDictionaryContainsKey(dict, CFSTR("WiFiMACAddress"))) {
				wifiValue = SDMMD_AMDeviceCopyValue(device, CFSTR("NULL"), CFSTR("WiFiMACAddress"));
				if (CFGetTypeID(wifiValue) == CFStringGetTypeID()) {
					CFDictionarySetValue(dict, CFSTR("WiFiMACAddress"), wifiValue);
				}
				
			}
			CFSafeRelease(wifiValue);
			
			CFTypeRef bagValue = CFDictionaryGetValue(dict, CFSTR("EscrowBag"));
			if (bagValue) {
				if (CFGetTypeID(bagValue) == CFDataGetTypeID()) {
					CFRetain(bagValue);
					*bag = bagValue;
					char *path = calloc(1, 0x401);
					SDMMD__PairingRecordPathForIdentifier(device->ivars.unique_device_id, path);
					result = SDMMD_store_dict(dict, path, true);
					if (result) {
						printf("%s: Failed to store escrow bag to %s.\n",__FUNCTION__,path);
					}
					Safe(free,path);
				}
			}
		}
		CFSafeRelease(dict);
	}
	return result;
}

bool SDMMD_isDeviceAttachedUSB(uint32_t location_id) {
	bool foundDevice = false;
	io_iterator_t iterator;
	mach_port_t masterPort;
	kern_return_t kr = IOMasterPort(MACH_PORT_NULL, &masterPort);
	if (kr == kIOReturnSuccess && masterPort) {
		CFMutableDictionaryRef matchingDict = IOServiceMatching(kIOUSBDeviceClassName);
		IOServiceGetMatchingServices(kIOMasterPortDefault, matchingDict, &iterator);
		io_service_t usbDevice;
		while ((usbDevice = IOIteratorNext(iterator))) {
			CFTypeRef supportsIPhoneOS = IORegistryEntrySearchCFProperty(usbDevice, kIOServicePlane, CFSTR("SupportsIPhoneOS"), kCFAllocatorDefault, kIORegistryIterateRecursively);
			if (supportsIPhoneOS) {
				CFTypeRef locationId = IORegistryEntrySearchCFProperty(usbDevice, kIOServicePlane, CFSTR("locationID"), kCFAllocatorDefault, kIORegistryIterateRecursively);
				if (locationId) {
					uint32_t loc_id;
					CFNumberGetValue(locationId, kCFNumberSInt32Type, &loc_id);
					foundDevice = (loc_id == location_id);
					CFSafeRelease(locationId);
				}
				CFSafeRelease(supportsIPhoneOS);
			}
			Safe(IOObjectRelease,usbDevice);
			if (foundDevice) {
				break;
			}
		}
	}
	else {
		printf("Error: Couldn't create a master I/O Kit port(%08x)\n", kr);
	}
	mach_port_deallocate(mach_task_self(), masterPort);
	return foundDevice;
}

bool SDMMD_isDeviceAttached(uint32_t device_id) {
	// this needs to be changed to query against USBMuxd for device dictionaries
	bool result = false;
	if (SDMMobileDevice->deviceList) {
		CFArrayRef devices = CFArrayCreateCopy(kCFAllocatorDefault, SDMMobileDevice->deviceList);
		if (devices) {
			for (uint32_t i = 0; i < CFArrayGetCount(devices); i++) {
				SDMMD_AMDeviceRef device = (SDMMD_AMDeviceRef)CFArrayGetValueAtIndex(devices, i);
				if (device) {
					uint32_t fetched_id = SDMMD_AMDeviceGetConnectionID(device);
					result = (fetched_id == device_id ? SDMMD_isDeviceAttachedUSB(device->ivars.location_id) : false);
					if (result) {
						break;
					}
				}
			}
			CFSafeRelease(devices);
		}
	}
	return result;
}

sdmmd_return_t SDMMD_send_activation(SDMMD_AMDeviceRef device, CFDictionaryRef dict) {
	sdmmd_return_t result = kAMDInvalidArgumentError;
	CFMutableDictionaryRef message = NULL;
	if (device) {
		result = kAMDNotConnectedError;
		if (device->ivars.device_active) {
			result = kAMDInvalidArgumentError;
			if (dict) {
				result = kAMDNoResourcesError;
				CFMutableDictionaryRef messageDict = SDMMD__CreateMessageDict(CFSTR("Activate"));
				if (messageDict) {
					CFDictionarySetValue(messageDict, CFSTR("ActivationRecord"), dict);
					result = SDMMD_lockconn_send_message(device, messageDict);
					if (result == kAMDSuccess) {
						result = SDMMD_lockconn_receive_message(device, &message);
						if (result == kAMDSuccess) {
							CFTypeRef msg = CFDictionaryGetValue(message, CFSTR("Error"));
							if (msg) {
								result = kAMDInvalidResponseError;
								if (CFGetTypeID(msg) == CFStringGetTypeID()) {
									result = (sdmmd_return_t)SDMMD__ConvertLockdowndError(msg);
								}
							}
						}
					}
				}
				CFSafeRelease(messageDict);
			}
		}
	}
	CFSafeRelease(message);
	return result;
}

sdmmd_return_t SDMMD_send_deactivation(SDMMD_AMDeviceRef device) {
	sdmmd_return_t result = kAMDInvalidArgumentError;
	CFMutableDictionaryRef message = NULL;
	if (device) {
		result = kAMDNotConnectedError;
		if (device->ivars.device_active) {
			//result = kAMDInvalidArgumentError;
			CFMutableDictionaryRef messageDict = SDMMD__CreateMessageDict(CFSTR("Deactivate"));
			if (messageDict) {
				result = SDMMD_lockconn_send_message(device, messageDict);
				if (result == kAMDSuccess) {
					result = SDMMD_lockconn_receive_message(device, &message);
					if (result == kAMDSuccess) {
						CFTypeRef msg = CFDictionaryGetValue(message, CFSTR("Error"));
						if (msg) {
							result = kAMDInvalidResponseError;
							if (CFGetTypeID(msg) == CFStringGetTypeID()) {
								result = (sdmmd_return_t)SDMMD__ConvertLockdowndError(msg);
							}
						}
					}
				}
			}
			else {
				result = kAMDNoResourcesError;
			}
			CFSafeRelease(messageDict);
		}
	}
	CFSafeRelease(message);
	return result;
}

sdmmd_return_t SDMMD_send_session_start(SDMMD_AMDeviceRef device, CFDictionaryRef record, CFStringRef *session) {
	sdmmd_return_t result = kAMDInvalidArgumentError;
	CFTypeRef var32 = NULL;
	bool isValidHostBUID = false;
	CFMutableDictionaryRef message = NULL;
	if (device) {
		CFTypeRef var20 = NULL;
		result = kAMDNotConnectedError;
		if (device->ivars.lockdown_conn) {
			result = kAMDInvalidArgumentError;
			if (record && session) {
				message = SDMMD__CreateMessageDict(CFSTR("StartSession"));
				result = kAMDNoResourcesError;
				if (message) {
					CFTypeRef hostId = CFDictionaryGetValue(record, CFSTR("HostID"));
					CFDictionarySetValue(message, CFSTR("HostID"), hostId);
					CFTypeRef bonjourId = CFDictionaryGetValue(record, CFSTR("SystemBUID"));
					if (bonjourId) {
						CFTypeRef systemBUID = SDMMD_AMDCopySystemBonjourUniqueID();
						isValidHostBUID = (CFStringCompare(bonjourId, systemBUID,  0) == kCFCompareEqualTo);
						//CFDictionarySetValue(message, CFSTR("HostID"), bonjourId);
						result = kAMDSuccess;
						CFSafeRelease(systemBUID);
					}

				}
			}
			if (isValidHostBUID && result == kAMDSuccess) { // SDM: this is a check against the host BUID and the BUID of the pairing record. this is a security measure.
				result = SDMMD_lockconn_send_message(device, message);
				//PrintCFType(message);
				CFSafeRelease(message);
				if (result == kAMDSuccess) {
					CFMutableDictionaryRef recvDict = NULL;
					result = SDMMD_lockconn_receive_message(device, &recvDict);
					//PrintCFType(recvDict);
					if (result == kAMDSuccess) {
						//CFShow(recvDict);
						CFTypeRef resultStr = CFDictionaryGetValue(recvDict, CFSTR("Error"));
						if (!resultStr) {
							CFTypeRef sessionId = CFDictionaryGetValue(recvDict, CFSTR("SessionID"));
							result = kAMDMissingSessionIDError;
							if (sessionId) {
								CFRetain(sessionId);
								CFTypeID typeId = CFGetTypeID(sessionId);
								result = kAMDMissingSessionIDError;
								if (typeId == CFStringGetTypeID()) {
									CFTypeRef hostCert = CFDictionaryGetValue(record, CFSTR("HostCertificate"));
									CFTypeRef hostPriKey = CFDictionaryGetValue(record, CFSTR("HostPrivateKey"));
									CFTypeRef deviceCert = CFDictionaryGetValue(record, CFSTR("DeviceCertificate"));
									result = SDMMD_lockconn_enable_ssl(device->ivars.lockdown_conn, hostCert, deviceCert, hostPriKey, 1);
									if (result != 0) {
										bool isValid = SDMMD_AMDeviceIsValid(device);
										result = kAMDDeviceDisconnectedError;
										if (isValid) {
											SDMMD_AMDeviceDisconnect(device);
											SDMMD_AMDeviceConnect(device);
											result = kAMDInvalidResponseError;
										}
									}
									else {
										CFRetain(sessionId);
										*session = sessionId;
									}
								}
								CFSafeRelease(sessionId);
							}
						}
						else {
							result = kAMDInvalidResponseError;
							if (CFGetTypeID(resultStr) == CFStringGetTypeID()) {
								result = (sdmmd_return_t)SDMMD__ConvertLockdowndError(resultStr);
							}
						}
					}
					CFSafeRelease(recvDict);
				}
			}
			else {
				printf("%s: Mismatch between Host SystemBUID and Pairing Record SystemBUID, recreate pairing record to ensure host is trustworthy.\n",__FUNCTION__);
				result = kAMDInvalidHostIDError;
			}
		}
		CFSafeRelease(var32);
		CFSafeRelease(var20);
	}
	return result;
}

sdmmd_return_t SDMMD_send_session_stop(SDMMD_AMDeviceRef device, CFTypeRef session) {
	sdmmd_return_t result = kAMDSuccess;
	CFMutableDictionaryRef response = NULL;
	
	if (!device) {
		result = kAMDInvalidArgumentError;
	}
	CheckErrorAndReturn(result);
	
	if (device->ivars.lockdown_conn == NULL) {
		result = kAMDNotConnectedError;
	}
	CheckErrorAndReturn(result);
	
	if (!session) {
		result = kAMDInvalidArgumentError;
	}
	CheckErrorAndReturn(result);
	
	CFMutableDictionaryRef dict = SDMMD__CreateMessageDict(CFSTR("StopSession"));
	if (!dict) {
		result = kAMDNoResourcesError;
	}
	CheckErrorAndReturn(result);
	
	CFDictionarySetValue(dict, CFSTR("SessionID"), session);
	result = SDMMD_lockconn_send_message(device, dict);
	CFSafeRelease(dict);
	CheckErrorAndReturn(result);
	
	result = SDMMD_lockconn_receive_message(device, &response);
	CheckErrorAndReturn(result);
	
	CFTypeRef error = CFDictionaryGetValue(response, CFSTR("Error"));
	if (error && CFGetTypeID(error) == CFStringGetTypeID()) {
		result = (sdmmd_return_t)SDMMD__ConvertLockdowndError(error);
	}
	else {
		SDMMD_lockconn_disable_ssl(device->ivars.lockdown_conn);
		result = kAMDSuccess;
	}
	
	ExitLabelAndReturn(result);
}

sdmmd_return_t SDMMD_AMDeviceStartSession(SDMMD_AMDeviceRef device) {
	sdmmd_return_t result = kAMDSuccess;
	CFMutableDictionaryRef record = NULL;
	CFDataRef key = NULL;
	if (!device) {
		result = kAMDInvalidArgumentError;
	}
	CheckErrorAndReturn(result);
	
	if (device->ivars.device_active == 0) {
		result = kAMDDeviceDisconnectedError;
	}
	CheckErrorAndReturn(result);
	
	SDMMD__mutex_lock(device->ivars.mutex_lock);
	result = SDMMD__CreatePairingRecordFromRecordOnDiskForIdentifier(device, &record);
	CheckErrorAndReturn(result);
	
	result = SDMMD_send_session_start(device, record, &device->ivars.session);
	if (result == kAMDSuccess && device->ivars.session) {
		Boolean hasKey = CFDictionaryContainsKey(record, CFSTR("EscrowBag"));
		if (!hasKey) {
			hasKey = CFDictionaryContainsKey(record, CFSTR("WiFiMACAddress"));
		}
		if (hasKey) {
			SDMMD__CopyEscrowBag(device, &key);
		}
	}
	else {
		char *reason = SDMMD_AMDErrorString(result);
		printf("%s: Could not start session with device %u: %s\n",__FUNCTION__,device->ivars.device_id,reason);
	}
	SDMMD__mutex_unlock(device->ivars.mutex_lock);
	CFSafeRelease(record);
	CFSafeRelease(key);
	
	ExitLabelAndReturn(result);
}

sdmmd_return_t SDMMD_AMDeviceStopSession(SDMMD_AMDeviceRef device) {
	sdmmd_return_t result = kAMDSuccess;
	if (!device) {
		result = kAMDInvalidArgumentError;
	}
	CheckErrorAndReturn(result);
	
	if (device->ivars.device_active == 0) {
		result = kAMDDeviceDisconnectedError;
	}
	CheckErrorAndReturn(result);
	
	SDMMD__mutex_lock(device->ivars.mutex_lock);
	result = kAMDSessionInactiveError;
	if (device->ivars.session != 0) {
		result = SDMMD_send_session_stop(device, device->ivars.session);
		if (result) {
			char *reason = SDMMD_AMDErrorString(result);
			printf("%s: Could not stop session with device %u: %s\n",__FUNCTION__,device->ivars.device_id,reason);
		}
		CFSafeRelease(device->ivars.session);
		device->ivars.session = NULL;
	}
	SDMMD__mutex_unlock(device->ivars.mutex_lock);

	ExitLabelAndReturn(result);
}

sdmmd_return_t SDMMD_AMDeviceActivate(SDMMD_AMDeviceRef device, CFDictionaryRef options) {
	sdmmd_return_t result = kAMDSuccess;
	if (!device) {
		result = kAMDInvalidArgumentError;
	}
	CheckErrorAndReturn(result);
	
	if (device->ivars.device_active == 0) {
		result = kAMDDeviceDisconnectedError;
	}
	CheckErrorAndReturn(result);
	
	SDMMD__mutex_lock(device->ivars.mutex_lock);
	result = SDMMD_send_activation(device, options);
	if (result != 0) {
		char *reason = SDMMD_AMDErrorString(result);
		printf("%s: Could not activate device %u %s.\n",__FUNCTION__,device->ivars.device_id,reason);
	}
	SDMMD__mutex_unlock(device->ivars.mutex_lock);

	ExitLabelAndReturn(result);
}

sdmmd_return_t SDMMD_AMDeviceDeactivate(SDMMD_AMDeviceRef device) {
	sdmmd_return_t result = kAMDSuccess;
	if (device) {
		if (device->ivars.device_active) {
			SDMMD__mutex_lock(device->ivars.mutex_lock);
			result = SDMMD_send_deactivation(device);
			if (result != 0) {
				char *reason = SDMMD_AMDErrorString(result);
				printf("%s: Could not deactivate device %u: %s\n",__FUNCTION__,device->ivars.device_id,reason);
			}
			SDMMD__mutex_unlock(device->ivars.mutex_lock);
		}
		else {
			result = kAMDDeviceDisconnectedError;
		}
	}
	else {
		result = kAMDInvalidArgumentError;
	}
	return result;
}

sdmmd_return_t SDMMD__connect_to_port(SDMMD_AMDeviceRef device, uint32_t port, bool hasTimeout, uint32_t *socketConn, bool isSSL) {
	sdmmd_return_t result = kAMDSuccess;
	uint32_t sock = 0xffffffff;
	uint32_t mask = true;
	struct timeval timeout = { .tv_sec = 25, .tv_usec = 0 };
	if (device) {
		if (socket) {
			result = kAMDDeviceDisconnectedError;
			if (device->ivars.device_active) {
				if (device->ivars.connection_type == kAMDeviceConnectionTypeWiFi) {
					size_t dataLen = CFDataGetLength(device->ivars.network_address);
					struct sockaddr_storage address = {0};
					if (dataLen == sizeof(struct sockaddr_storage)) {
						CFDataGetBytes(device->ivars.network_address, CFRangeMake(0, dataLen), (UInt8*)&address);
						sock = socket(AF_INET, SOCK_STREAM, 0);
						if (setsockopt(sock, SOL_SOCKET, SO_NOSIGPIPE, &mask, sizeof(mask))) {
							
						}
						if (setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout))) {
						 
						}
						if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout))) {
						 
						}
						result = connect(sock, (const struct sockaddr *)&address, sizeof(struct sockaddr_in));
						printf("connection status: %i\n",result);
					}
					else {
						printf("_AMDeviceConnectByAddressAndPort: doesn't look like a sockaddr_storage.\n");
						result = kAMDMuxConnectError;
					}
				}
				else {
					result = SDMMD_USBMuxConnectByPort(device, port, &sock);
					if (result) {
						result = kAMDMuxConnectError;
					}
				}
				/*if (setsockopt(sock, 0xffff, 0x1022, &mask, 0x4)) {
					
				}
				mask = 0x19;
				if (setsockopt(sock, 0xffff, 0x1005, &mask, 0x10)) {
					
				}
				if (setsockopt(sock, 0xffff, 0x1006, &mask, 0x10)) {
					
				}*/
				*socketConn = sock;
			}
			/*if (sock != 0xff) {
				if (close(sock) == 0xff) {
					printf("_connect_to_port: close(2) on socket %d failed: %d.\n",sock,errno);
				}
			}*/
		}
	}
	return result;
}

sdmmd_return_t SDMMD_AMDeviceConnect(SDMMD_AMDeviceRef device) {
	sdmmd_return_t result = kAMDSuccess;
	uint32_t socket = 0xffffffff;
	if (!device) {
		result = kAMDInvalidArgumentError;
	}
	CheckErrorAndReturn(result);
	
	result = SDMMD_AMDevicePair(device);	
	if (SDM_MD_CallSuccessful(result)) {
		result = kAMDDeviceDisconnectedError;
		if (device->ivars.device_active && device->ivars.connection_type == kAMDeviceConnectionTypeUSB) {
			SDMMD__mutex_lock(device->ivars.mutex_lock);
			if (device->ivars.lockdown_conn == 0) {
				uint32_t status = SDMMD__connect_to_port(device, 0x7ef2, true, &socket, false);
				if (status == kAMDSuccess) {
					result = kAMDNotConnectedError;
					if (socket != 0xff) {
						device->ivars.lockdown_conn = SDMMD_lockdown_connection_create(socket);
						result = kAMDNoResourcesError;
						if (device->ivars.lockdown_conn->connection) {
							CFStringRef daemon = NULL;
							status = SDMMD_copy_daemon_name(device, &daemon);
							if (daemon && status == 0) {
								//result = kAMDInvalidResponseError;
								if (CFStringCompare(daemon, CFSTR(AMSVC_LOCKDOWN), 0) != kCFCompareEqualTo) {
									char *dname = SDMCFStringGetString(daemon);
									printf("%s: This is not the droid you're looking for (is actually %s). move along,  move along.\n",__FUNCTION__,dname);
									Safe(free,dname);
									SDMMD_AMDeviceDisconnect(device);
									result = kAMDWrongDroidError;
								}
								else {
									result = kAMDSuccess;
								}
								CFSafeRelease(daemon);
							}
							else {
								result = kAMDNoResourcesError;
							}
						}
					}
				}
				else {
					printf("%s: Could not connect to lockdown port (%d) on device %d - %s: 0x%x\n",__FUNCTION__,0xf27e, device->ivars.device_id,"device with no name",result);
				}
			}
			else {
				bool valid = SDMMD_AMDeviceIsValid(device);
				if (!valid) {
					SDMMD_AMDeviceDisconnect(device);
					result = kAMDDeviceDisconnectedError;
				}
			}
			SDMMD__mutex_unlock(device->ivars.mutex_lock);
		}
	}
	else {
		if (result == kAMDPairingProhibitedError) {
			printf("%s: Could not pair with device, please accept trust prompt on device.\n",__FUNCTION__);
		}
	}
	/*if (socket != 0xff) {
		if (close(socket) == 0xff) {
			uint32_t errorNum = errno;
			printf("SDMMD_AMDeviceConnect: close(2) on socket %d socket, failed: %d.\n",socket, errorNum);
		}
	}*/
	ExitLabelAndReturn(result);
}

sdmmd_return_t SDMMD_AMDeviceDisconnect(SDMMD_AMDeviceRef device) {
	sdmmd_return_t result = kAMDSuccess;
	if (device) {
		SDMMD__mutex_lock(device->ivars.mutex_lock);
		result = SDMMD_lockdown_connection_destory(device->ivars.lockdown_conn);
		device->ivars.lockdown_conn = NULL;
		CFSafeRelease(device->ivars.session);
		SDMMD__mutex_unlock(device->ivars.mutex_lock);
	}
	else {
		result = kAMDInvalidArgumentError;
	}
	return result;
}

bool SDMMD_AMDeviceIsValid(SDMMD_AMDeviceRef device) {
	bool result = false;
	if (device && device->ivars.device_active != 0) {
		bool attached = SDMMD_isDeviceAttached(device->ivars.device_id);
		if (!attached) {
			device->ivars.device_active = 0;
		}
		else {
			result = true;
		}
	}
	return result;
}

sdmmd_return_t SDMMD_AMDeviceValidatePairing(SDMMD_AMDeviceRef device) {
	sdmmd_return_t result = kAMDSuccess;
	if (!device) {
		result = kAMDInvalidArgumentError;
	}
	CheckErrorAndReturn(result);
	
	if (device->ivars.device_active == 0) {
		result = kAMDDeviceDisconnectedError;
	}
	CheckErrorAndReturn(result);
	
	char *recordPath = calloc(1, sizeof(char[1025]));
	SDMMD__PairingRecordPathForIdentifier(device->ivars.unique_device_id, recordPath);
	CFMutableDictionaryRef dict = SDMMD__CreateDictFromFileContents(recordPath);
	if (!dict) {
		result = kAMDMissingPairRecordError;
	}
	Safe(free,recordPath);
	CheckErrorAndReturn(result);
	
	CFStringRef host = CFDictionaryGetValue(dict, CFSTR("HostID"));
	if (!host) {
		result = kAMDInvalidPairRecordError;
	}
	else {
		CFRetain(host);
	}
	CFSafeRelease(dict);
	CheckErrorAndReturn(result);
	
	SDMMD__mutex_lock(device->ivars.mutex_lock);
	result = SDMMD_send_validate_pair(device, host);
	if (result) {
		printf("%s: Could not validate pairing with device %u: %s\n",__FUNCTION__,device->ivars.device_id, SDMMD_AMDErrorString(result));
	}
	SDMMD__mutex_unlock(device->ivars.mutex_lock);
	CFSafeRelease(host);
	
	ExitLabelAndReturn(result);
}

sdmmd_return_t SDMMD_AMDeviceUnpair(SDMMD_AMDeviceRef device) {
	sdmmd_return_t result = kAMDSuccess;
	if (!device) {
		result = kAMDInvalidArgumentError;
	}
	CheckErrorAndReturn(result);
	
	if (device->ivars.device_active == 0) {
		result = kAMDDeviceDisconnectedError;
	}
	CheckErrorAndReturn(result);
	
	char *recordPath = calloc(1, sizeof(char[1025]));
	SDMMD__PairingRecordPathForIdentifier(device->ivars.unique_device_id, recordPath);
	CFMutableDictionaryRef dict = SDMMD__CreateDictFromFileContents(recordPath);
	if (!dict) {
		result = kAMDMissingPairRecordError;
		Safe(free,recordPath);
	}
	CheckErrorAndReturn(result);
	
	CFStringRef host = CFDictionaryGetValue(dict, CFSTR("HostID"));
	if (!host) {
		result = kAMDInvalidPairRecordError;
	}
	else {
		CFRetain(host);
	}
	CFSafeRelease(dict);
	CheckErrorAndReturn(result);
	
	SDMMD__mutex_lock(device->ivars.mutex_lock);
	// SDM: remove pair record file
	remove(recordPath);
	result = SDMMD_send_unpair(device, host);
	if (result) {
		printf("%s: Could not unpair device %u: %s\n",__FUNCTION__,device->ivars.device_id, SDMMD_AMDErrorString(result));
	}
	SDMMD__mutex_unlock(device->ivars.mutex_lock);
	CFSafeRelease(host);
	Safe(free,recordPath);
	
	ExitLabelAndReturn(result);
}

bool SDMMD_AMDeviceIsPaired(SDMMD_AMDeviceRef device) {
	bool result = false;
	if (device) {
		SDMMD__mutex_lock(device->ivars.mutex_lock);
		char *path = calloc(1, sizeof(char[1025]));
		
		SDMMD__PairingRecordPathForIdentifier(device->ivars.unique_device_id, path);
		SDMMD__mutex_unlock(device->ivars.mutex_lock);
		struct stat buffer;
		int statResult = stat(path, &buffer);
		if (statResult) {
			uint32_t errorNum = errno;
			if (errorNum != 2) {
				errorNum = errno;
				char *errStr = strerror(errorNum);
				printf("%s: Could not stat %s: %s\n",__FUNCTION__,path, errStr);
			}
		}
		else {
			result = true;
		}
		Safe(free,path);
	}
	else {
		printf("%s: No device.\n",__FUNCTION__);
	}
	return result;
}

sdmmd_return_t SDMMD_AMDevicePair(SDMMD_AMDeviceRef device) {
	sdmmd_return_t result = kAMDSuccess;
	if (!SDMMD_AMDeviceIsPaired(device)) {
		result = SDMMD_AMDevicePairWithOptions(device, NULL);
	}
	return result;
}

sdmmd_return_t SDMMD_AMDevicePairWithOptions(SDMMD_AMDeviceRef device, CFMutableDictionaryRef record) {
 	sdmmd_return_t result = kAMDInvalidArgumentError;
	bool mutexIsLocked = false;
	bool getValue = true;
	CFMutableDictionaryRef chapCopy = NULL;
 	
	if (device) {
		result = kAMDDeviceDisconnectedError;
		if (device->ivars.device_active) {
			SDMMD__mutex_lock(device->ivars.mutex_lock);
			if (record) {
				CFDictionaryRef chapCert = CFDictionaryGetValue(record, CFSTR("ChaperoneCertificate"));
				if (chapCert) {
					if (CFPropertyListIsValid(chapCert, kCFPropertyListXMLFormat_v1_0) || CFPropertyListIsValid(chapCert, kCFPropertyListBinaryFormat_v1_0)) {
						CFIndex chapKeyCount = CFDictionaryGetCount(chapCert);
						if (chapKeyCount != 1) {
							chapCopy = CFDictionaryCreateMutableCopy(kCFAllocatorDefault, 0, chapCert);
							CFDictionaryRemoveValue(chapCopy, CFSTR("ChaperoneCertificate"));
						}
					}
					else {
						getValue = false;
					}
				}
			}
			if (getValue) {
				mutexIsLocked = true;
				result = kAMDPairingProhibitedError;
				CFStringRef err = NULL;
				CFTypeRef wifiAddress = SDMMD_copy_lockdown_value(device, NULL, CFSTR(kWiFiAddress), &err);
				CFTypeRef value = SDMMD_copy_lockdown_value(device, NULL, CFSTR(kDevicePublicKey), &err);
				//CFShow(value);
				if (err) {
					result = kAMDInvalidResponseError;
				}
				else {
					if (value) {
						if (CFGetTypeID(value) == CFDataGetTypeID()) {
							record = SDMMD__CreatePairingMaterial(value);
							if (record) {
								CFTypeRef buid = SDMMD_AMDCopySystemBonjourUniqueID();
								if (buid) {
									CFDictionarySetValue(record, CFSTR("SystemBUID"), buid);
									CFMutableDictionaryRef sendPair = CFDictionaryCreateMutableCopy(kCFAllocatorDefault, 0, record);
									if (sendPair) {
										CFDictionaryRemoveValue(sendPair, CFSTR("RootPrivateKey"));
										CFDictionaryRemoveValue(sendPair, CFSTR("HostPrivateKey"));
										
										CFDataRef escrowBag = NULL;
										// escrowBag is returned by reference with +1 retain
										result = SDMMD_send_pair(device, sendPair, chapCopy, NULL, &escrowBag);
										if (!escrowBag) {
											printf("%s: Could not pair with the device %u: 0x%x\n",__FUNCTION__, device->ivars.device_id, result);
										}
										else {
											if (record) {
												CFDictionarySetValue(record, CFSTR("EscrowBag"), escrowBag);
											}
											if (record && wifiAddress) {
												if (CFGetTypeID(wifiAddress) == CFStringGetTypeID()) {
													CFDictionarySetValue(record, CFSTR("WiFiMACAddress"), wifiAddress);
												}
											}
											char *path = calloc(1, sizeof(char[1025]));
											SDMMD__PairingRecordPathForIdentifier(device->ivars.unique_device_id, path);
											result = SDMMD_store_dict(record, path, true);
											if (result) {
												printf("%s: Could not store pairing record at '%s'.\n", __FUNCTION__, path);
												result = kAMDPermissionError;
											}
											else {
												result = kAMDSuccess;
											}
											Safe(free,path);
										}
										CFSafeRelease(escrowBag);
									}
									else {
										result = kAMDNoResourcesError;
									}
									CFSafeRelease(sendPair);
								}
								else {
									printf("%s: Could not create system BUID.\n",__FUNCTION__);
								}
								CFSafeRelease(buid);
							}
							else {
								printf("%s: Could not create pairing material.\n",__FUNCTION__);
							}
							CFSafeRelease(record);
						}
						else {
							result = kAMDInvalidResponseError;
						}
					}
				}
				CFSafeRelease(value);
				CFSafeRelease(wifiAddress);
			}
		}
	}
	if (mutexIsLocked) {
		SDMMD__mutex_unlock(device->ivars.mutex_lock);
	}
	return result;
}

uint32_t SDMMD_AMDeviceUSBDeviceID(SDMMD_AMDeviceRef device) {
	uint32_t result = 0;
	if (device) {
		SDMMD__mutex_lock(device->ivars.mutex_lock);
		result = device->ivars.device_id;
		SDMMD__mutex_unlock(device->ivars.mutex_lock);
	}
	return result;
}

uint32_t SDMMD_AMDeviceUSBLocationID(SDMMD_AMDeviceRef device) {
	uint32_t result = 0;
	if (device) {
		SDMMD__mutex_lock(device->ivars.mutex_lock);
		result = device->ivars.location_id;
		SDMMD__mutex_unlock(device->ivars.mutex_lock);
	}
	else {
		printf("%s: No device\n",__FUNCTION__);
	}
	return result;
}

uint16_t SDMMD_AMDeviceUSBProductID(SDMMD_AMDeviceRef device) {
	uint16_t result = 0;
	if (device) {
		SDMMD__mutex_lock(device->ivars.mutex_lock);
		result = device->ivars.product_id & 0xffff;
		SDMMD__mutex_unlock(device->ivars.mutex_lock);
	}
	else {
		printf("%s: No device\n",__FUNCTION__);
	}
	return result;
}

uint32_t SDMMD_AMDeviceGetConnectionID(SDMMD_AMDeviceRef device) {
	return SDMMD_AMDeviceUSBDeviceID(device);
}

CFTypeRef SDMMD_AMDeviceCopyValue(SDMMD_AMDeviceRef device, CFStringRef domain, CFStringRef key) {
	CFTypeRef value = NULL;
	if (device->ivars.device_active) {
		
		SDMMD__mutex_lock(device->ivars.mutex_lock);
		CFStringRef err = NULL;
		value = SDMMD_copy_lockdown_value(device, domain, key, &err);
		if (err) {
			CFSafeRelease(value);
			value = NULL;
		}
		CFSafeRelease(err);
		SDMMD__mutex_unlock(device->ivars.mutex_lock);
	}
	return value;
}

sdmmd_return_t SDMMD_AMDeviceSetValue(SDMMD_AMDeviceRef device, CFStringRef domain, CFStringRef key, CFTypeRef value) {
	sdmmd_return_t result = kAMDSuccess;
    if (!device) {
		result = kAMDInvalidArgumentError;
	}
	CheckErrorAndReturn(result);
	
	if (device->ivars.device_active == 0) {
		result = kAMDDeviceDisconnectedError;
	}
	CheckErrorAndReturn(result);
	
	SDMMD__mutex_lock(device->ivars.mutex_lock);
	result = SDMMD_send_set_value(device, domain, key, value);
	if (result != kAMDSuccess) {
		printf("%s: Could not set value\n",__FUNCTION__);
	}
	SDMMD__mutex_unlock(device->ivars.mutex_lock);
    
	ExitLabelAndReturn(result);
}

SDMMD_AMDeviceRef SDMMD_AMDeviceCreateEmpty() {
	uint32_t extra = sizeof(AMDeviceClassBody);
	SDMMD_AMDeviceRef device = (SDMMD_AMDeviceRef)_CFRuntimeCreateInstance(kCFAllocatorDefault, _kSDMMD_AMDeviceRefID, extra, NULL);
	return device;
}

SDMMD_AMDeviceRef SDMMD_AMDeviceCreateFromProperties(CFDictionaryRef dictionary) {
	SDMMD_AMDeviceRef device = NULL;
	if (dictionary) {
		device = SDMMD_AMDeviceCreateEmpty();
		if (device) {
			
			CFDictionaryRef properties = (CFDictionaryContainsKey(dictionary, CFSTR("Properties")) ? CFDictionaryGetValue(dictionary, CFSTR("Properties")) : dictionary);
			
			CFNumberRef deviceId = CFDictionaryGetValue(properties, CFSTR("DeviceID"));
			CFNumberGetValue(deviceId, kCFNumberSInt32Type/*0x4*/, &device->ivars.device_id);
			
			CFStringRef serialNumber = CFDictionaryGetValue(properties, CFSTR("SerialNumber"));
			device->ivars.unique_device_id = serialNumber;
			
			CFStringRef linkType = CFDictionaryGetValue(properties, CFSTR("ConnectionType"));
			if (CFStringCompare(linkType, CFSTR("USB"), 0) == 0) {
				device->ivars.connection_type = kAMDeviceConnectionTypeUSB;
				
				CFNumberRef productId = CFDictionaryGetValue(properties, CFSTR("ProductID"));
				CFNumberGetValue(productId, kCFNumberSInt16Type/*0x2*/, &device->ivars.product_id);
				
				CFNumberRef locationId = CFDictionaryGetValue(properties, CFSTR("LocationID"));
				CFNumberGetValue(locationId, kCFNumberSInt32Type/*0x4*/, &device->ivars.location_id);
				
			}
			else if (CFStringCompare(linkType, CFSTR("Network"), 0) == 0 || CFStringCompare(linkType, CFSTR("WiFi"), 0) == 0) {
				device->ivars.connection_type = kAMDeviceConnectionTypeWiFi;
				CFDataRef netAddress = CFDataCreateCopy(kCFAllocatorDefault, CFDictionaryGetValue(properties, CFSTR("NetworkAddress")));
				device->ivars.network_address = netAddress;
				device->ivars.unknown11 = netAddress;
				device->ivars.service_name = CFDictionaryGetValue(properties, CFSTR("EscapedFullServiceName"));
			}
			else {
				
			}
			
			device->ivars.device_active = true;
			device->ivars.unknown8 = 0;
			sdmmd_mutex_init(device->ivars.mutex_lock);
		}
	}
	return device;
}

bool SDMMD_AMDeviceIsAttached(SDMMD_AMDeviceRef device) {
	bool result = false;
	struct USBMuxPacket *devicesPacket = SDMMD_USBMuxCreatePacketType(kSDMMD_USBMuxPacketListDevicesType, NULL);
	SDMMD_USBMuxListenerSend(SDMMobileDevice->usbmuxd, &devicesPacket);
	for (uint32_t i = 0; i < CFArrayGetCount(SDMMobileDevice->deviceList); i++) {
		SDMMD_AMDeviceRef deviceCheck = (SDMMD_AMDeviceRef)CFArrayGetValueAtIndex(SDMMobileDevice->deviceList, i);
		if (SDMMD_AMDeviceGetConnectionID(device) == SDMMD_AMDeviceGetConnectionID(deviceCheck)) {
			result = true;
			break;
		}
	}
	USBMuxPacketRelease(devicesPacket);
	return result;
}

CFArrayRef SDMMD_AMDCreateDeviceList() {
	struct USBMuxPacket *devicesPacket = SDMMD_USBMuxCreatePacketType(kSDMMD_USBMuxPacketListDevicesType, NULL);
	SDMMD_USBMuxListenerSend(SDMMobileDevice->usbmuxd, &devicesPacket);
	USBMuxPacketRelease(devicesPacket);
	CFArrayRef deviceArray = CFArrayCreateCopy(kCFAllocatorDefault, SDMMobileDevice->deviceList);
	return deviceArray;
}

SDMMD_AMDeviceRef SDMMD_AMDeviceCreateCopy(SDMMD_AMDeviceRef device) {
	SDMMD_AMDeviceRef copy = (SDMMD_AMDeviceRef)calloc(1, sizeof(struct sdmmd_am_device));
	memcpy(copy, device, sizeof(struct sdmmd_am_device));
	return copy;
}

sdmmd_sim_return_t SDMMD_GetSIMStatusCode(SDMMD_AMDeviceRef device) {
	sdmmd_sim_return_t result = KnownSIMCodes[0];
	CFStringRef deviceSIMStatus = SDMMD_AMDeviceCopyValue(device, NULL, CFSTR(kSIMStatus));
	if (deviceSIMStatus) {
		for (uint32_t i = 1; i < kKnownSIMCodesNum; i++) {
			if (CFStringCompare(deviceSIMStatus, KnownSIMCodes[i].codeName, 0) == kCFCompareEqualTo) {
				result = KnownSIMCodes[i];
				break;
			}
		}
	}
	CFSafeRelease(deviceSIMStatus);
	return result;
}

sdmmd_activation_return_t SDMMD_GetActivationStatus(SDMMD_AMDeviceRef device) {
	sdmmd_activation_return_t result = KnownActivationStates[0];
	CFStringRef deviceActivationState = SDMMD_AMDeviceCopyValue(device, NULL, CFSTR(kActivationState));
	if (deviceActivationState) {
		for (uint32_t i = 1; i < kKnownActivationStatesNum; i++) {
			if (CFStringCompare(deviceActivationState, KnownActivationStates[i].statusName, 0) == kCFCompareEqualTo) {
				result = KnownActivationStates[i];
				break;
			}
		}
	}
	CFSafeRelease(deviceActivationState);
	return result;
}

sdmmd_interface_return_t SDMMD_AMDeviceGetInterfaceType(SDMMD_AMDeviceRef device) {
	sdmmd_interface_return_t result = kAMDInterfaceConnectionTypeInvalid;
	if (device) {
		SDMMD__mutex_lock(device->ivars.mutex_lock);
		
		if (device->ivars.connection_type == kAMDeviceConnectionTypeUSB) {
			result = kAMDInterfaceConnectionTypeDirect;
		}
		else if (device->ivars.connection_type == kAMDeviceConnectionTypeWiFi) {
			result = kAMDInterfaceConnectionTypeIndirect;
		}
		
		SDMMD__mutex_unlock(device->ivars.mutex_lock);
	}
	else {
		printf("%s: No device\n", __FUNCTION__);
	}
	return result;
}


#endif
