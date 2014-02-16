/*
 *  SDMMD_AMDevice.c
 *  SDMMobileDevice
 *
 * Copyright (c) 2013, Sam Marshall
* All rights reserved.
* 
* Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
* 
* 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
* 
* 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
* 
* 3. Neither the name of Sam Marshall nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
* 
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
#include <sys/types.h>
#include <sys/select.h>
#include "CFRuntime.h"
#include <CoreFoundation/CFBase.h>
#include <CoreFoundation/CFString.h>
#include "Core.h"
#include "SDMMD_Functions.h"
#include "SDMMD_AFC.h"

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
		result = (CFStringCompare(device1->ivars.unique_device_id, device2->ivars.unique_device_id, 0x0) == 0x0);
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
	lockdown->connection = socket;
	return lockdown;
}

X509* SDMMD__decode_certificate(CFDataRef cert) {
	X509* result = NULL;
	if (cert) {
		BIO *newBIO = SDMMD__create_bio_from_data(cert);
		if (newBIO == 0) {
			printf("_decode_certificate: Could not create BIO from CFData.\n");
		} else {
			result = PEM_read_bio_X509(newBIO, 0x0, 0x0, 0x0);
			if (result == NULL) {
				printf("_decode_certificate: PEM_read_bio_X509 failed.\n");
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
					} else {
						result = false;
					}
				} else {
					result = false;
				}
			}
			
		} else {
			printf("_ssl_verify_callback: Error verifying cert: unable to compare.\n");
			result = false;
		}
		Safe(free,var_8);
		Safe(free,var_16);
	} else {
		printf("_ssl_verify_callback: Error verifying cert: (%d %s).\n", value, X509_verify_cert_error_string(X509_STORE_CTX_get_error(store)));
	}
	return result;
}

SSL* SDMMD_lockssl_handshake(SDMMD_lockdown_conn *lockdown_conn, CFTypeRef hostCert, CFTypeRef deviceCert, CFTypeRef hostPrivKey, uint32_t num) {
	SSL *ssl = NULL;
	SSL_CTX *sslCTX = NULL;
	sdmmd_return_t result = 0x0;
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
		} else {
			PEM_read_bio_RSAPrivateKey(dataBIO, &rsa, 0x0, 0x0);
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
					} else {
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
						} else {
							SSL_set_accept_state(ssl);
						}
						SSL_set_verify(ssl, SSL_VERIFY_PEER | SSL_VERIFY_FAIL_IF_NO_PEER_CERT, SDMMD__ssl_verify_callback);
						SSL_set_verify_depth(ssl, 0x0);
						SSL_set_bio(ssl, bioSocket, bioSocket);
						SSL_set_ex_data(ssl, (uint32_t)SDMMobileDevice->peer_certificate_data_index, (void*)deviceCert);
						result = SSL_do_handshake(ssl);
						if (result == 1) {
							SSL_CTX_free(sslCTX);
						} else {
							uint32_t err = SSL_get_error(ssl, result);
							if (err) {
								char *reason = SDMMD_ssl_strerror(ssl, err);
								printf("lockssl_handshake: SSL handshake fatal lower level error %d: %s.\n", err, reason);
							} else {
								char *reason = SDMMD_ssl_strerror(ssl, 0x0);
								printf("lockssl_handshake: SSL handshake controlled failure %d: %s.\n", err, reason);
							}
							Safe(SSL_free,ssl);
							ssl = 0x0;
						}
					} else {
						printf("_create_ssl: Could not create SSL thing.\n");
					}
				}
			} else {
				printf("_create_ssl_context: Could not decode private key.\n");
				Safe(X509_free,cert);
			}
		}
	} else {
		printf("lockssl_handshake: Could not create SSL bio.\n");
	}
	return ssl;
}

sdmmd_return_t SDMMD_lockconn_enable_ssl(SDMMD_lockdown_conn *lockdown_conn, CFTypeRef hostCert, CFTypeRef deviceCert, CFTypeRef hostPrivKey, uint32_t num) {
	sdmmd_return_t result = kAMDSuccess;
	lockdown_conn->ssl = SDMMD_lockssl_handshake(lockdown_conn, hostCert, deviceCert, hostPrivKey, num);
	return result;
}

sdmmd_return_t SDMMD_lockconn_disable_ssl(SDMMD_lockdown_conn *lockdown_conn) {
	sdmmd_return_t result = kAMDSuccess;
	if (lockdown_conn->ssl) {
		if (result == 0) {
			result = SSL_shutdown(lockdown_conn->ssl);
		}
		if (result == -1) {
			printf("lockconn_disable_ssl: Could not shutdown SSL connection %d.\n", -1);
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
			} else {
				conn = (SocketConnection){false, {.conn = (uint32_t)device->ivars.lockdown_conn->connection}};
			}
			
			SDMMD_ServiceSendMessage(conn, dict, kCFPropertyListXMLFormat_v1_0);
		} else {
			printf("SDMMD_lockconn_send_message: Could not encode message as XML.\n");
		}
	} else {
		result = SDMMD_AMDeviceIsValid(device);
		if (result == false) {
			result = kAMDSendMessageError;
		}
	}
	return result;
}

sdmmd_return_t SDMMD_lockconn_receive_message(SDMMD_AMDeviceRef device, CFMutableDictionaryRef *dict) {
	sdmmd_return_t result = 0x0;
	if (device->ivars.lockdown_conn) {
		bool useSSL = (device->ivars.lockdown_conn->ssl ? true : false);
		SocketConnection conn;
		if (useSSL) {
			conn = (SocketConnection){true, {.ssl = device->ivars.lockdown_conn->ssl}};
	 	} else {
			conn = (SocketConnection){false , {.conn = (uint32_t)device->ivars.lockdown_conn->connection}};
		}
		
		SDMMD_ServiceReceiveMessage(conn, (CFPropertyListRef*)dict);
	} else {
		result = SDMMD_AMDeviceIsValid(device);
		if (result == 0x0) {
			result = kAMDReadError;
		}
	}
	return result;
}

CFTypeRef SDMMD_copy_lockdown_value(SDMMD_AMDeviceRef device, CFStringRef domain, CFStringRef key, CFStringRef *err) {
	CFTypeRef value = NULL;
	sdmmd_return_t result = kAMDSuccess;
	if (device) {
		if (device->ivars.lockdown_conn) {
			CFMutableDictionaryRef getVal = SDMMD__CreateMessageDict(CFSTR("GetValue"));
			if (getVal) {
				CFMutableDictionaryRef response;
				if (domain == NULL) {
					domain = CFSTR("NULL");
				}
				if (key == NULL) {
					key = CFSTR("NULL");
				}
				
				if (CFStringCompare(domain, CFSTR("NULL"), 0) != 0) {
					CFDictionarySetValue(getVal, CFSTR("Domain"), domain);
				}
				if (CFStringCompare(key, CFSTR("NULL"), 0) != 0) {
					CFDictionarySetValue(getVal, CFSTR("Key"), key);
				}
				
				result = SDMMD_lockconn_send_message(device, getVal);
				CFSafeRelease(getVal);
				if (result == 0x0) {
					result = SDMMD_lockconn_receive_message(device, &response);
					if (result == 0x0 && response) {
						*err = CFDictionaryGetValue(response, CFSTR("Error"));
						if (*err) {
							if (CFGetTypeID(*err) == CFStringGetTypeID()) {
								result = (sdmmd_return_t)SDMMD__ConvertLockdowndError(*err);
							} else {
								result = kAMDInvalidResponseError;
							}
						} else {
							value = CFDictionaryGetValue(response, CFSTR("Value"));
							result = kAMDSuccess;
						}
					}
				}
			} else {
				result = kAMDNoResourcesError;
			}
		} else {
			result = kAMDNotConnectedError;
		}
	} else {
		result = kAMDInvalidArgumentError;
	}
	if (result != kAMDSuccess) {
		value = NULL;
	}
	return value;
}



sdmmd_return_t SDMMD_send_set_value(SDMMD_AMDeviceRef device, CFStringRef domain, CFStringRef key, CFTypeRef value) {
	sdmmd_return_t result = kAMDInvalidArgumentError;
    if (device) {
        result = kAMDNotConnectedError;
		if (device->ivars.lockdown_conn) {
            result = kAMDInvalidArgumentError;
            if (key && value) {
                CFMutableDictionaryRef setVal = SDMMD__CreateMessageDict(CFSTR("SetValue"));
                result = kAMDNoResourcesError;
                if (setVal) {
                    if (domain) {
                        CFDictionarySetValue(setVal, CFSTR("Domain"), domain);
                    }
                    CFDictionarySetValue(setVal, CFSTR("Key"), key);
                    CFDictionarySetValue(setVal, CFSTR("Value"), value);
                    result = SDMMD_lockconn_send_message(device, setVal);
                    CFSafeRelease(setVal);
                    if (result == kAMDSuccess) {
                        CFMutableDictionaryRef resultDict = NULL;
                        result = SDMMD_lockconn_receive_message(device, &resultDict);
                        if (result == kAMDSuccess) {
                            CFStringRef error = CFDictionaryGetValue(resultDict, CFSTR("Error"));
                            if (error) {
                                result = kAMDInvalidResponseError;
                                if (CFGetTypeID(error) == CFStringGetTypeID()) {
                                    result = (sdmmd_return_t)SDMMD__ConvertLockdowndError(error);
                                }
                            }
                            CFSafeRelease(resultDict);
                        }
                    }
                }
            }
        }
    }
    return result;
}

sdmmd_return_t SDMMD_lockdown_connection_destory(SDMMD_lockdown_conn *lockdownCon) {
	sdmmd_return_t result = 0x0;
	if (lockdownCon) {
		Safe(SSL_free,lockdownCon->ssl);
		lockdownCon->ssl = NULL;
		if (lockdownCon->connection != 0xff) {
			result = close((uint32_t)lockdownCon->connection);
			if (result == 0xff) {
				printf("SDMMD_lockdown_connection_destory: close(2) on socket %lld failed: %d.\n",lockdownCon->connection, result);
			}
			lockdownCon->connection = 0x0;
		}
		Safe(free,lockdownCon->pointer);
		result = 0x0;
		Safe(free,lockdownCon);
		lockdownCon = NULL;
	}
	return result;
}

sdmmd_return_t SDMMD_send_unpair(SDMMD_AMDeviceRef device, CFStringRef hostId) {
	sdmmd_return_t result = 0xe8000007;
	if (device) {
		if (device->ivars.lockdown_conn) {
			if (hostId) {
				CFMutableDictionaryRef dict = SDMMD__CreateMessageDict(CFSTR("Unpair"));
				if (dict) {
					const void *keys[1] = { CFSTR("HostID") };
					const void *values[1] = { hostId };
					CFDictionaryRef host = CFDictionaryCreate(kCFAllocatorDefault, keys, values, 0x1, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
					if (host) {
						CFDictionarySetValue(dict, CFSTR("PairRecord"), host);
						result = SDMMD_lockconn_send_message(device, dict);
						CFSafeRelease(host);
						if (result == 0x0) {
							CFMutableDictionaryRef response = NULL;
							result = SDMMD_lockconn_receive_message(device, &response);
							//PrintCFType(response);
							if (result == 0x0) {
								CFTypeRef error = CFDictionaryGetValue(response, CFSTR("Error"));
								if (!error) {
									result = 0x0;
								} else {
									if (CFGetTypeID(error) == CFStringGetTypeID()) {
										result = (sdmmd_return_t)SDMMD__ConvertLockdowndError(error);
									} else {
										result = kAMDInvalidResponseError;
									}
								}
							}
						}
					}
					CFSafeRelease(dict);
				} else {
					result = kAMDNoResourcesError;
				}
			} else {
				result = kAMDInvalidArgumentError;
			}
		} else {
			result = kAMDNotConnectedError;
		}
	} else {
		result = kAMDInvalidArgumentError;
	}
	return result;
}

sdmmd_return_t SDMMD_send_pair(SDMMD_AMDeviceRef device, CFMutableDictionaryRef pairRecord, CFTypeRef slip, CFTypeRef options, CFDataRef *escrowBag) {
    sdmmd_return_t result = 0xe8000007;
	CFMutableDictionaryRef response = NULL;
    if (device) {
		if (device->ivars.lockdown_conn) {
			if (pairRecord) {
				if (escrowBag) {
                    CFMutableDictionaryRef pRecord = SDMMD__CreateMessageDict(CFSTR("Pair"));
                    result = 0xe8000003;
                    if (pRecord) {
						CFDictionarySetValue(pRecord, CFSTR("PairRecord"), pairRecord);
						if (slip) {
							CFDictionarySetValue(pRecord, CFSTR("PermissionSlip"), slip);
						}
						if (options) {
							CFDictionarySetValue(pRecord, CFSTR("PairingOptions"), options);
						}
						result = SDMMD_lockconn_send_message(device, pRecord);
						if (result == 0x0) {
							result = SDMMD_lockconn_receive_message(device, &response);
							CFShow(response);
							if (result == 0x0) {
								CFTypeRef error = CFDictionaryGetValue(response, CFSTR("Error"));
								if (error) {
									result = 0xe8000013;
									if (CFGetTypeID(error) == CFStringGetTypeID()) {
										result = (sdmmd_return_t)SDMMD__ConvertLockdowndError(error);
									}
								} else {
									*escrowBag = CFDictionaryGetValue(response, CFSTR("EscrowBag"));
									if (escrowBag) {
										result = kAMDSuccess;
										CFRetain(*escrowBag);
									}
								}
							}
						}
                    }
				}
			}
		} else {
			result = kAMDNotConnectedError;
		}
    }
    if ((response) && (CFDictionaryContainsValue(response, CFSTR("ExtendedResponse")))) {
		*escrowBag = CFDictionaryGetValue(response, CFSTR("ExtendedResponse"));
		if (escrowBag) {
			result = kAMDSuccess;
		}
    }
    CFSafeRelease(response);
	
    return result;
}


sdmmd_return_t SDMMD_send_validate_pair(SDMMD_AMDeviceRef device, CFStringRef hostId) {
	sdmmd_return_t result = 0xe8000007;
	if (device) {
		if (device->ivars.lockdown_conn) {
			if (hostId) {
				CFMutableDictionaryRef dict = SDMMD__CreateMessageDict(CFSTR("ValidatePair"));
				if (dict) {
					const void *keys[1] = { CFSTR("HostID") };
					const void *values[1] = { hostId };
					CFDictionaryRef host = CFDictionaryCreate(kCFAllocatorDefault, keys, values, 0x1, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
					if (host) {
						CFDictionarySetValue(dict, CFSTR("PairRecord"), host);
						result = SDMMD_lockconn_send_message(device, dict);
						CFSafeRelease(host);
						if (result == 0x0) {
							CFMutableDictionaryRef response = NULL;
							result = SDMMD_lockconn_receive_message(device, &response);
							//PrintCFType(response);
							if (result == 0x0) {
								CFTypeRef error = CFDictionaryGetValue(response, CFSTR("Error"));
								if (!error) {
									result = 0x0;
								} else {
									if (CFGetTypeID(error) == CFStringGetTypeID()) {
										result = (sdmmd_return_t)SDMMD__ConvertLockdowndError(error);
									} else {
										result = kAMDInvalidResponseError;
									}
								}
							} 
						}
					}
					CFSafeRelease(dict);
				} else {
					result = kAMDNoResourcesError;
				}
			} else {
				result = kAMDInvalidArgumentError;
			}
		} else {
			result = kAMDNotConnectedError;
		}
	} else {
		result = kAMDInvalidArgumentError;
	}
	return result;
}

sdmmd_return_t SDMMD_copy_daemon_name(SDMMD_AMDeviceRef device, CFStringRef *name) {
	sdmmd_return_t result = 0;
	CFMutableDictionaryRef response = NULL;
	if (device) {
		if (device->ivars.lockdown_conn->connection) {
			if (name) {
				CFMutableDictionaryRef queryDict = SDMMD__CreateMessageDict(CFSTR("QueryType"));
				if (queryDict) {
					result = SDMMD_lockconn_send_message(device, queryDict);
					CFSafeRelease(queryDict);
					if (result == 0) {
						result = SDMMD_lockconn_receive_message(device, &response);
						if (result == 0) {
							//CFShow(response);
							if (response && CFDictionaryGetCount(response)) {
								CFTypeRef val = CFDictionaryGetValue(response, CFSTR("Error"));
								if (val == NULL) {
									val = CFDictionaryGetValue(response, CFSTR("Type"));
									if (val) {
										if (CFGetTypeID(val) == CFStringGetTypeID()) {
											CFRetain(val);
											*name = val;
										}
									} else {
										result = kAMDInvalidResponseError;
									}
								}
							} else {
								response = 0x0;
							}
						}
					}
				} else {
					result = kAMDNoResourcesError;
				}
			} else {
				result = kAMDInvalidArgumentError;
			}
		} else {
			result = kAMDNotConnectedError;
		}
	} else {
		result = kAMDInvalidArgumentError;
	}
	return result;
}

sdmmd_return_t SDMMD__CopyEscrowBag(SDMMD_AMDeviceRef device, CFDataRef *bag) {
	sdmmd_return_t result = kAMDInvalidArgumentError;
	if (device) {
		CFMutableDictionaryRef dict;
		result = SDMMD__CreatePairingRecordFromRecordOnDiskForIdentifier(device, &dict);
		if (result == 0) {
			CFTypeRef wifiValue = NULL, bagValue;
			if (!CFDictionaryContainsKey(dict, CFSTR("WiFiMACAddress"))) {
				wifiValue = SDMMD_AMDeviceCopyValue(device, CFSTR("NULL"), CFSTR("WiFiMACAddress"));
				if (CFGetTypeID(wifiValue) == CFStringGetTypeID()) {
					CFDictionarySetValue(dict, CFSTR("WiFiMACAddress"), wifiValue);
				}
			}
			bagValue = CFDictionaryGetValue(dict, CFSTR("EscrowBag"));
			if (bagValue) {
				if (CFGetTypeID(bagValue) == CFDataGetTypeID()) {
					CFRetain(bagValue);
					*bag = bagValue;
					char *path = calloc(1, 0x401);
					SDMMD__PairingRecordPathForIdentifier(device->ivars.unique_device_id, path);
					result = SDMMD_store_dict(dict, path, 0x1);
					if (result) {
						printf("SDMMD_CopyEscrowBag: Failed to store escrow bag to %s.\n",path);
					}
					Safe(free,path);
				}
			}
			CFSafeRelease(wifiValue);
		}
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
	} else {
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
					if (result == 0) {
						result = SDMMD_lockconn_receive_message(device, &message);
						if (result == 0) {
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
				if (result == 0) {
					result = SDMMD_lockconn_receive_message(device, &message);
					if (result == 0) {
						CFTypeRef msg = CFDictionaryGetValue(message, CFSTR("Error"));
						if (msg) {
							result = kAMDInvalidResponseError;
							if (CFGetTypeID(msg) == CFStringGetTypeID()) {
								result = (sdmmd_return_t)SDMMD__ConvertLockdowndError(msg);
							}
						}
					}
				}
			} else {
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
						isValidHostBUID = (CFStringCompare(bonjourId, systemBUID,  0x0) == kCFCompareEqualTo);
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
				if (result == 0) {
					CFMutableDictionaryRef recvDict = NULL;
					result = SDMMD_lockconn_receive_message(device, &recvDict);
					//PrintCFType(recvDict);
					if (result == 0) {
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
									} else {
										CFRetain(sessionId);
										*session = sessionId;
									}
								}
								//CFSafeRelease(sessionId);
							}
						} else {
							result = kAMDInvalidResponseError;
							if (CFGetTypeID(resultStr) == CFStringGetTypeID()) {
								result = (sdmmd_return_t)SDMMD__ConvertLockdowndError(resultStr);
							}
						}
					}
					CFSafeRelease(recvDict);
				}
			} else {
				printf("SDMMD_send_session_start: Mismatch between Host SystemBUID and Pairing Record SystemBUID, recreate pairing record to ensure host is trustworthy\n");
				result = kAMDInvalidHostIDError;
			}
		}
		CFSafeRelease(var32);
		CFSafeRelease(var20);
	}
	return result;
}

sdmmd_return_t SDMMD_send_session_stop(SDMMD_AMDeviceRef device, CFTypeRef session) {
	sdmmd_return_t result = kAMDInvalidArgumentError;
	if (device) {
		result = kAMDNotConnectedError;
		CFMutableDictionaryRef response = NULL;
		if (device->ivars.lockdown_conn) {
			result = kAMDInvalidArgumentError;
			if (session) {
				CFMutableDictionaryRef dict = SDMMD__CreateMessageDict(CFSTR("StopSession"));
				result = kAMDNoResourcesError;
				if (dict) {
					CFDictionarySetValue(dict, CFSTR("SessionID"), session);
					result = SDMMD_lockconn_send_message(device, dict);
					CFSafeRelease(dict);
					if (result == 0) {
						result = SDMMD_lockconn_receive_message(device, &response);
						if (result == 0) {
							CFTypeRef error = CFDictionaryGetValue(response, CFSTR("Error"));
							if (error && CFGetTypeID(error) == CFStringGetTypeID()) {
								result = (sdmmd_return_t)SDMMD__ConvertLockdowndError(error);
							} else {
								SDMMD_lockconn_disable_ssl(device->ivars.lockdown_conn);
								result = kAMDSuccess;
							}
						}
					}
				}
			}
		}
		CFSafeRelease(response);
	}
	return result;
}

sdmmd_return_t SDMMD_AMDeviceStartSession(SDMMD_AMDeviceRef device) {
	sdmmd_return_t result = kAMDSuccess;
	CFMutableDictionaryRef record = NULL;
	CFDataRef key = NULL;
	if (!device) {
		return kAMDInvalidArgumentError;
	}
	
	result = kAMDDeviceDisconnectedError;
	if (device->ivars.device_active) {
		SDMMD__mutex_lock(device->ivars.mutex_lock);
		result = SDMMD__CreatePairingRecordFromRecordOnDiskForIdentifier(device, &record);
		if (result == 0) {
			result = SDMMD_send_session_start(device, record, &device->ivars.session);
			if (result == 0 && device->ivars.session) {
				Boolean hasKey = CFDictionaryContainsKey(record, CFSTR("EscrowBag"));
				if (!hasKey) {
					hasKey = CFDictionaryContainsKey(record, CFSTR("WiFiMACAddress"));
				}
				if (hasKey) {
					SDMMD__CopyEscrowBag(device, &key);
				}
			} else {
				char *reason = SDMMD_AMDErrorString(result);
				printf("SDMMD_AMDeviceStartSession: Could not start session with device %u: %s\n",device->ivars.device_id,reason);
			}
		}
		SDMMD__mutex_unlock(device->ivars.mutex_lock);
	}
	CFSafeRelease(record);
	CFSafeRelease(key);
	return result;
}

sdmmd_return_t SDMMD_AMDeviceStopSession(SDMMD_AMDeviceRef device) {
	sdmmd_return_t result = kAMDSuccess;
	if (device) {
		result = kAMDDeviceDisconnectedError;
		if (device->ivars.device_active) {
			SDMMD__mutex_lock(device->ivars.mutex_lock);
			result = kAMDSessionInactiveError;
			if (device->ivars.session != 0) {
				result = SDMMD_send_session_stop(device, device->ivars.session);
				if (result) {
					char *reason = SDMMD_AMDErrorString(result);
					printf("SDMMD_AMDeviceStopSession: Could not stop session with device %u: %s\n",device->ivars.device_id,reason);
				}
				CFSafeRelease(device->ivars.session);
				device->ivars.session = NULL;
			}
			SDMMD__mutex_unlock(device->ivars.mutex_lock);
		}
	} else {
		result = kAMDInvalidArgumentError;
	}
	return result;
}

sdmmd_return_t SDMMD_AMDeviceActivate(SDMMD_AMDeviceRef device, CFDictionaryRef options) {
	sdmmd_return_t result = kAMDSuccess;
	if (device) {
		if (device->ivars.device_active) {
			SDMMD__mutex_lock(device->ivars.mutex_lock);
			result = SDMMD_send_activation(device, options);
			if (result != 0) {
				char *reason = SDMMD_AMDErrorString(result);
				printf("SDMMD_AMDeviceActivate: Could not activate device %u %s.\n",device->ivars.device_id,reason);
			}
			SDMMD__mutex_unlock(device->ivars.mutex_lock);
		} else {
			result = kAMDDeviceDisconnectedError;
		}
	} else {
		result = kAMDInvalidArgumentError;
	}
	return result;
}

sdmmd_return_t SDMMD_AMDeviceDeactivate(SDMMD_AMDeviceRef device) {
	sdmmd_return_t result = kAMDSuccess;
	if (device) {
		if (device->ivars.device_active) {
			SDMMD__mutex_lock(device->ivars.mutex_lock);
			result = SDMMD_send_deactivation(device);
			if (result != 0) {
				char *reason = SDMMD_AMDErrorString(result);
				printf("SDMMD_AMDeviceDeactivate: Could not deactivate device %u: %s\n",device->ivars.device_id,reason);
			}
			SDMMD__mutex_unlock(device->ivars.mutex_lock);
		} else {
			result = kAMDDeviceDisconnectedError;
		}
	} else {
		result = kAMDInvalidArgumentError;
	}
	return result;
}

sdmmd_return_t SDMMD__connect_to_port(SDMMD_AMDeviceRef device, uint32_t port, bool hasTimeout, uint32_t *socketConn, bool isSSL) {
	sdmmd_return_t result = kAMDSuccess;
	uint32_t sock = 0xffffffff;
	uint32_t mask = 0x1;
	if (device) {
		if (socket) {
			result = kAMDDeviceDisconnectedError;
			if (device->ivars.device_active) {
				if (device->ivars.connection_type == 1) {
					uint32_t dataLen = (uint32_t)CFDataGetLength(device->ivars.network_address);
					struct sockaddr *address = calloc(1, dataLen); 
					if (dataLen == 0x80) {
						CFDataGetBytes(device->ivars.network_address, CFRangeMake(0, dataLen), (UInt8*)address);
						sock = socket(0x2, 0x1, 0x0);
						if (setsockopt(sock, 0xffff, 0x1022, &mask, 0x4)) {
							
						}
						mask = 0x19;
						if (setsockopt(sock, 0xffff, 0x1005, &mask, 0x10)) {
						 
						}
						if (setsockopt(sock, 0xffff, 0x1006, &mask, 0x10)) {
						 
						}
						result = connect(sock, address, 0x10);
						printf("connection status: %i\n",result);
					} else {
						printf("_AMDeviceConnectByAddressAndPort: doesn't look like a sockaddr_storage.\n");
						result = kAMDMuxConnectError;
					}
				} else {
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
	if (device) {
		result = SDMMD_AMDevicePair(device);
		SDMMD_CondSuccessElse(result, {
			result = kAMDDeviceDisconnectedError;
			if (device->ivars.device_active && device->ivars.connection_type == 0) {
				SDMMD__mutex_lock(device->ivars.mutex_lock);
				if (device->ivars.lockdown_conn == 0) {
					uint32_t status = SDMMD__connect_to_port(device, 0x7ef2, 0x1, &socket, 0x0);
					if (status == 0) {
						result = kAMDNotConnectedError;
						if (socket != 0xff) {
							device->ivars.lockdown_conn = SDMMD_lockdown_connection_create(socket);
							result = kAMDNoResourcesError;
							if (device->ivars.lockdown_conn->connection) {
								CFStringRef daemon = NULL;
								status = SDMMD_copy_daemon_name(device, &daemon);
								if (daemon) {
									result = kAMDInvalidResponseError;
									if (CFStringCompare(daemon, CFSTR(AMSVC_LOCKDOWN), 0x0) != 0x0) {
										char *dname = SDMCFStringGetString(daemon);
										printf("SDMMD_AMDeviceConnect: This is not the droid you're looking for (is actually %s). move along,  move along.\n",dname);
										Safe(free,dname);
										SDMMD_AMDeviceDisconnect(device);
										result = kAMDWrongDroidError;
									} else {
										result = kAMDSuccess;
									}
								} else {
									result = kAMDNoResourcesError;
								}
							}
						}
					} else {
						printf("SDMMD_AMDeviceConnect: Could not connect to lockdown port (%d) on device %d - %s: 0x%x\n",0xf27e, device->ivars.device_id,"device with no name",result);
					}
				} else {
					bool valid = SDMMD_AMDeviceIsValid(device);
					if (!valid) {
						SDMMD_AMDeviceDisconnect(device);
						result = kAMDDeviceDisconnectedError;
					}
				}
				SDMMD__mutex_unlock(device->ivars.mutex_lock);
			}
		}, {
			if (result == kAMDPairingProhibitedError) {
				printf("SDMMD_AMDeviceConnect: Could not pair with device, please accept trust prompt on device.\n");
			}
		})
	} else {
		result = kAMDInvalidArgumentError;
	}
	/*if (socket != 0xff) {
		if (close(socket) == 0xff) {
			uint32_t errorNum = errno;
			printf("SDMMD_AMDeviceConnect: close(2) on socket %d socket, failed: %d.\n",socket, errorNum);
		}
	}*/
	return result;
}

sdmmd_return_t SDMMD_AMDeviceDisconnect(SDMMD_AMDeviceRef device) {
	sdmmd_return_t result = kAMDSuccess;
	if (device) {
		SDMMD__mutex_lock(device->ivars.mutex_lock);
		result = SDMMD_lockdown_connection_destory(device->ivars.lockdown_conn);
		device->ivars.lockdown_conn = NULL;
		CFSafeRelease(device->ivars.session);
		SDMMD__mutex_unlock(device->ivars.mutex_lock);
	} else {
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
		} else {
			result = true;
		}
	}
	return result;
}

sdmmd_return_t SDMMD_AMDeviceValidatePairing(SDMMD_AMDeviceRef device) {
	sdmmd_return_t result = kAMDSuccess;
	if (device) {
		if (device->ivars.device_active) {
			char *recordPath = calloc(0x1, 0x401);
			SDMMD__PairingRecordPathForIdentifier(device->ivars.unique_device_id, recordPath);
			CFMutableDictionaryRef dict = SDMMD__CreateDictFromFileContents(recordPath);
			if (dict) {
				CFStringRef host = CFDictionaryGetValue(dict, CFSTR("HostID"));
				if (host) {
					SDMMD__mutex_lock(device->ivars.mutex_lock);
					result = SDMMD_send_validate_pair(device, host);
					if (result) {
						printf("SDMMD_AMDeviceValidatePairing: Could not validate pairing with device %u: %s\n",device->ivars.device_id, SDMMD_AMDErrorString(result));
					}
					SDMMD__mutex_unlock(device->ivars.mutex_lock);
				} else {
					result = kAMDInvalidPairRecordError;
				}
				CFSafeRelease(dict);
			} else {
				result = kAMDMissingPairRecordError;
			}
			Safe(free,recordPath);
		} else {
			result = kAMDDeviceDisconnectedError;
		}
	} else {
		result = kAMDInvalidArgumentError;
	}
	return result;
}

sdmmd_return_t SDMMD_AMDeviceUnpair(SDMMD_AMDeviceRef device) {
	sdmmd_return_t result = kAMDSuccess;
	if (device) {
		if (device->ivars.device_active) {
			char *recordPath = calloc(0x1, 0x401);
			SDMMD__PairingRecordPathForIdentifier(device->ivars.unique_device_id, recordPath);
			CFMutableDictionaryRef dict = SDMMD__CreateDictFromFileContents(recordPath);
			if (dict) {
				CFStringRef host = CFDictionaryGetValue(dict, CFSTR("HostID"));
				if (host) {
					SDMMD__mutex_lock(device->ivars.mutex_lock);
					// SDM: remove pair record file
					remove(recordPath);
					result = SDMMD_send_unpair(device, host);
					if (result) {
						printf("SDMMD_AMDeviceUnpair: Could not unpair device %u: %s\n",device->ivars.device_id, SDMMD_AMDErrorString(result));
					}
					SDMMD__mutex_unlock(device->ivars.mutex_lock);
				} else {
					result = kAMDInvalidPairRecordError;
				}
				CFSafeRelease(dict);
			} else {
				result = kAMDMissingPairRecordError;
			}
			Safe(free,recordPath);
		} else {
			result = kAMDDeviceDisconnectedError;
		}
	} else {
		result = kAMDInvalidArgumentError;
	}
	return result;
}

bool SDMMD_AMDeviceIsPaired(SDMMD_AMDeviceRef device) {
	bool result = false;
	if (device) {
		SDMMD__mutex_lock(device->ivars.mutex_lock);
		char *path = calloc(0x1, 0x401);
		
		SDMMD__PairingRecordPathForIdentifier(device->ivars.unique_device_id, path);
		SDMMD__mutex_unlock(device->ivars.mutex_lock);
		struct stat buffer;
		int statResult = stat(path, &buffer);
		if (statResult) {
			uint32_t errorNum = errno;
			if (errorNum != 2) {
				errorNum = errno;
				char *errStr = strerror(errorNum);
				printf("SDMMD_AMDeviceIsPaired: Could not stat %s: %s\n",path, errStr);
			}
		} else {
			result = true;
		}
		Safe(free,path);
	} else {
		printf("SDMMD_AMDeviceIsPaired: No device.\n");
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
 	CFDataRef escrowBag = NULL;
	if (device) {
		result = kAMDDeviceDisconnectedError;
		if (device->ivars.device_active) {
			SDMMD__mutex_lock(device->ivars.mutex_lock);
			if (record) {
				CFDictionaryRef chapCert = CFDictionaryGetValue(record, CFSTR("ChaperoneCertificate"));
				if (chapCert) {
					if (CFPropertyListIsValid(chapCert, 0x64) || CFPropertyListIsValid(chapCert, 0xc8)) {
						CFIndex chapKeyCount = CFDictionaryGetCount(chapCert);
						if (chapKeyCount != 1) {
							chapCopy = CFDictionaryCreateMutableCopy(kCFAllocatorDefault, 0x0, chapCert);
							CFDictionaryRemoveValue(chapCopy, CFSTR("ChaperoneCertificate"));
						}
					} else {
						getValue = false;
					}
				}
			}
			if (getValue) {
				mutexIsLocked = true;
				result = kAMDPairingProhibitedError;
				CFStringRef err;
				CFTypeRef wifiAddress = SDMMD_copy_lockdown_value(device, NULL, CFSTR(kWiFiAddress), &err);
				CFTypeRef value = SDMMD_copy_lockdown_value(device, NULL, CFSTR(kDevicePublicKey), &err);
				CFShow(value);
				if (err) {
					result = kAMDInvalidResponseError;
				} else {
					if (value) {
						if (CFGetTypeID(value) == CFDataGetTypeID()) {
							record = SDMMD__CreatePairingMaterial(value);
							if (record) {
								CFTypeRef buid = SDMMD_AMDCopySystemBonjourUniqueID();
								if (buid) {
									CFDictionarySetValue(record, CFSTR("SystemBUID"), buid);
									CFMutableDictionaryRef sendPair = CFDictionaryCreateMutableCopy(kCFAllocatorDefault, 0x0, record);
									if (sendPair) {
										CFDictionaryRemoveValue(sendPair, CFSTR("RootPrivateKey"));
										CFDictionaryRemoveValue(sendPair, CFSTR("HostPrivateKey"));
										CFShow(sendPair);
										result = SDMMD_send_pair(device, sendPair, chapCopy, NULL, &escrowBag);
										if (!escrowBag) {
											printf("AMDeviceExtendedPairWithOptions: Could not pair with the device %u: 0x%x\n", device->ivars.device_id, result);
										} else {
											if (record) {
												CFDictionarySetValue(record, CFSTR("EscrowBag"), escrowBag);
											}
											if (record && wifiAddress) {
												if (CFGetTypeID(wifiAddress) == CFStringGetTypeID()) {
													CFDictionarySetValue(record, CFSTR("WiFiMACAddress"), wifiAddress);
												}
											}
											char *path = calloc(0x1, 0x401);
											SDMMD__PairingRecordPathForIdentifier(device->ivars.unique_device_id, path);
											result = SDMMD_store_dict(record, path, true);
											if (result) {
												printf("AMDeviceExtendedPairWithOptions: Could not store pairing record at '%s'.\n", path);
												result = kAMDPermissionError;
											} else {
												result = kAMDSuccess;
											}
											Safe(free,path);
										}
									} else {
										result = kAMDNoResourcesError;
									}
								} else {
									printf("SDMMD_AMDeviceExtendedPairWithOptions: Could not create system BUID.\n");
								}
							} else {
								printf("SDMMD_AMDeviceExtendedPairWithOptions: Could not create pairing material.\n");
							}
							CFSafeRelease(record);
						} else {
							result = kAMDInvalidResponseError;
						}
						CFSafeRelease(value);
					}
				}
			}
		}
	}
	if (mutexIsLocked) {
		SDMMD__mutex_unlock(device->ivars.mutex_lock);
	}
	return result;
}

uint32_t SDMMD_AMDeviceUSBDeviceID(SDMMD_AMDeviceRef device) {
	uint32_t result = 0x0;
	if (device) {
		SDMMD__mutex_lock(device->ivars.mutex_lock);
		result = device->ivars.device_id;
		SDMMD__mutex_unlock(device->ivars.mutex_lock);
	}
	return result;
}

uint32_t SDMMD_AMDeviceUSBLocationID(SDMMD_AMDeviceRef device) {
	uint32_t result = 0x0;
	if (device) {
		SDMMD__mutex_lock(device->ivars.mutex_lock);
		result = device->ivars.location_id;
		SDMMD__mutex_unlock(device->ivars.mutex_lock);
	} else {
		printf("SDMMD_AMDeviceUSBLocationID: No device\n");
	}
	return result;
}

uint16_t SDMMD_AMDeviceUSBProductID(SDMMD_AMDeviceRef device) {
	uint16_t result = 0x0;
	if (device) {
		SDMMD__mutex_lock(device->ivars.mutex_lock);
		result = device->ivars.product_id & 0xffff;
		SDMMD__mutex_unlock(device->ivars.mutex_lock);
	} else {
		printf("SDMMD_AMDeviceUSBProductID: No device\n");
	}
	return result;
}

uint32_t SDMMD_AMDeviceGetConnectionID(SDMMD_AMDeviceRef device) {
	return SDMMD_AMDeviceUSBDeviceID(device);
}

CFTypeRef SDMMD_AMDeviceCopyValue(SDMMD_AMDeviceRef device, CFStringRef domain, CFStringRef key) {
	CFTypeRef value = NULL;
	if (device->ivars.device_active) {
		if (domain == NULL) {
			domain = CFSTR("NULL");
		}
		if (key == NULL) {
			key = CFSTR("NULL");
		}
			
		SDMMD__mutex_lock(device->ivars.mutex_lock);
		CFStringRef err = NULL;
		value = SDMMD_copy_lockdown_value(device, domain, key, &err);
		if (err) {
			PrintCFType(err);
			value = NULL;
		}
		SDMMD__mutex_unlock(device->ivars.mutex_lock);
	}
	return value;
}

sdmmd_return_t SDMMD_AMDeviceSetValue(SDMMD_AMDeviceRef device, CFStringRef domain, CFStringRef key, CFTypeRef value) {
	sdmmd_return_t result = kAMDSuccess;
    if (device) {
        if (device->ivars.device_active) {
            SDMMD__mutex_lock(device->ivars.mutex_lock);
            if (!SDMMD_send_set_value(device, domain, key, value)) {
                printf("SDMMD_AMDeviceSetValue: Could not set value\n");
            } else {
                result = kAMDSuccess;
            }
            SDMMD__mutex_unlock(device->ivars.mutex_lock);
        } else {
            result = kAMDDeviceDisconnectedError;
        }
    } else {
        result = kAMDInvalidArgumentError;
    }
    
	return result;
}

SDMMD_AMDeviceRef SDMMD_AMDeviceCreateEmpty() {
	uint32_t extra = sizeof(AMDeviceClassBody);
	//SDMMD_AMDeviceRef device = calloc(0x1, sizeof(struct sdmmd_am_device));
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
				device->ivars.connection_type = 0x0;
				
				CFNumberRef productId = CFDictionaryGetValue(properties, CFSTR("ProductID"));
				CFNumberGetValue(productId, kCFNumberSInt16Type/*0x2*/, &device->ivars.product_id);
				
				CFNumberRef locationId = CFDictionaryGetValue(properties, CFSTR("LocationID"));
				CFNumberGetValue(locationId, kCFNumberSInt32Type/*0x4*/, &device->ivars.location_id);
				
			} else if (CFStringCompare(linkType, CFSTR("Network"), 0) == 0 || CFStringCompare(linkType, CFSTR("WiFi"), 0) == 0) {
				device->ivars.connection_type = 0x1;
				CFDataRef netAddress = CFDataCreateCopy(kCFAllocatorDefault, CFDictionaryGetValue(properties, CFSTR("NetworkAddress")));
				device->ivars.network_address = netAddress;
				device->ivars.unknown11 = netAddress;
				device->ivars.service_name = CFDictionaryGetValue(properties, CFSTR("EscapedFullServiceName"));
			} else {
				
			}
			
			device->ivars.device_active = 0x1;
			device->ivars.unknown8 = 0x0;
			sdmmd_mutex_init(device->ivars.mutex_lock);
		}
	}
	return device;
}

bool SDMMD_AMDeviceIsAttached(SDMMD_AMDeviceRef device) {
	bool result = false;
	struct USBMuxPacket *devicesPacket = SDMMD_USBMuxCreatePacketType(kSDMMD_USBMuxPacketListDevicesType, NULL);
	SDMMD_USBMuxListenerSend(SDMMobileDevice->usbmuxd, devicesPacket);
	for (uint32_t i = 0x0; i < CFArrayGetCount(SDMMobileDevice->deviceList); i++) {
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
	SDMMD_USBMuxListenerSend(SDMMobileDevice->usbmuxd, devicesPacket);
	USBMuxPacketRelease(devicesPacket);
	CFArrayRef deviceArray = CFArrayCreateCopy(kCFAllocatorDefault, SDMMobileDevice->deviceList);
	return deviceArray;
}

SDMMD_AMDeviceRef SDMMD_AMDeviceCreateCopy(SDMMD_AMDeviceRef device) {
	SDMMD_AMDeviceRef copy = (SDMMD_AMDeviceRef)malloc(sizeof(struct sdmmd_am_device));
	memcpy(copy, device, sizeof(struct sdmmd_am_device));
	return copy;
}

bool SDMMD_device_os_is_at_least(SDMMD_AMDeviceRef device, CFStringRef version) {
	bool result = false;
	if (device) {
		SDMMD_AMDeviceConnect(device);
		CFStringRef prodVers = SDMMD_AMDeviceCopyValue(device, NULL, CFSTR(kProductVersion));
		if (prodVers) {
				result = (CFStringCompare(prodVers, version, kCFCompareNumerically) != kCFCompareLessThan ? true : false);
		}
		CFSafeRelease(prodVers);
	}
	return result;
}


sdmmd_return_t SDMMD_copy_image(SDMMD_AMDeviceRef device, CFStringRef path) {
	sdmmd_return_t result = kAMDUndefinedError;
	if (device) {
		result = kAMDSuccess;//SDMMD_AMDeviceConnect(device);
		SDMMD_CondSuccessElse(result, {
			result = SDMMD_AMDeviceStartSession(device);
			SDMMD_CondSuccess(result, {
				SDMMD_AMConnectionRef copyConn = NULL;
				result = SDMMD_AMDeviceSecureStartService(device, CFSTR(AMSVC_AFC), NULL, &copyConn);
				SDMMD_CondSuccess(result, {
					SDMMD_AFCConnectionRef copyAFCConn = SDMMD_AFCConnectionCreate(copyConn);
					SDMMD_AFCOperationRef response = NULL;
					SDMMD_AFCOperationRef makeStaging = SDMMD_AFCOperationCreateMakeDirectory(CFSTR("PublicStaging"));
					result = SDMMD_AFCProcessOperation(copyAFCConn, makeStaging, &response);
					SDMMD_CondSuccess(result, {
						// SDM copy file AFC
						char *pathString = SDMCFStringGetString(path);
						result = SDMMD_AMDeviceCopyFile(NULL, NULL, NULL, copyAFCConn, pathString,"PublicStaging/staging.dimage");
						Safe(free, pathString);
					})
				})
			})
		}, {
			
		})
	}
	return result;
}

sdmmd_return_t SDMMD__hangup_with_image_mounter_service(SDMMD_AMConnectionRef connection) {
	sdmmd_return_t result = kAMDNoResourcesError;
	CFMutableDictionaryRef dict = SDMMD_create_dict();
	if (dict) {
		CFDictionarySetValue(dict, CFSTR("Command"), CFSTR("Hangup"));
	}
	if (connection) {
		SocketConnection socket = SDMMD_TranslateConnectionToSocket(connection);
		result = SDMMD_ServiceSendMessage(socket, dict, kCFPropertyListXMLFormat_v1_0);
		SDMMD_CondSuccess(result, {
			CFPropertyListRef response = NULL;
			result = SDMMD_ServiceReceiveMessage(socket, &response);
			PrintCFDictionary(response);
		})
	}
	return result;
}

#if 0
sdmmd_return_t SDMMD_AMDeviceMountImage(SDMMD_AMDeviceRef device, CFStringRef path, CFDictionaryRef dict, CallBack handle, void* unknown) {
	sdmmd_return_t result = kAMDMissingOptionsError;
	if (dict) {
		CFTypeRef digest = NULL;
		CFTypeRef imageType = CFDictionaryGetValue(dict, CFSTR("ImageType"));
		if (imageType) {
			CFTypeRef signature = CFDictionaryGetValue(dict, CFSTR("ImageSignature"));
			char *cpath = calloc(1, 0x401);
			Boolean pathCopy = CFStringGetCString(path, cpath, 0x400, kCFStringEncodingUTF8);
			if (pathCopy) {
				unsigned char *sumdigest = calloc(0x1, 0x20);
				result = SDMMD_AMDeviceDigestFile(cpath, 0x0, PtrCast(&sumdigest, unsigned char**));
				if (result) {
					SDMMD_AMDeviceRef deviceCopy = SDMMD_AMDeviceCreateCopy(device);
					result = kAMDUndefinedError;
					if (deviceCopy) {
						SDMMD_AMConnectionRef connection = NULL;
						result = SDMMD_AMDeviceSecureStartSessionedService(device, CFSTR(AMSVC_DEBUG_IMAGE_MOUNT), &connection);
						if (result == 0) {
							SDMMD_fire_callback(handle, unknown, CFSTR("LookingUpImage"));
							if (connection) {
								CFMutableDictionaryRef commandDict = SDMMD_create_dict();
								if (commandDict) {
									CFDictionarySetValue(commandDict, CFSTR("Command"), CFSTR("LookupImage"));
									CFDictionarySetValue(commandDict, CFSTR("ImageType"), imageType);
									CFShow(commandDict);
									result = SDMMD_ServiceSendMessage(SDMMD_TranslateConnectionToSocket(connection), commandDict, kCFPropertyListXMLFormat_v1_0);
									if (result == 0) {
										CFDictionaryRef response;
										result = SDMMD_ServiceReceiveMessage(SDMMD_TranslateConnectionToSocket(connection), (CFPropertyListRef*)&response);
										if (result == 0) {
											if (response) {
												CFTypeRef error = CFDictionaryGetValue(response, CFSTR("Error"));
												if (error) {
													CFShow(response);
													// convert error
													result = SDMMD__ConvertServiceError(error);
												} else {
													CFTypeRef image = CFDictionaryGetValue(response, CFSTR("ImagePresent"));
													if (image) {
														if (CFEqual(image, kCFBooleanTrue)) {
															digest = CFDictionaryGetValue(response, CFSTR("ImageDigest"));
														} else {
															result = kAMDMissingDigestError;
														}
													}
												}
											} else {
												result = kAMDReadError;
											}
										}
									}
								} else {
									result = kAMDNoResourcesError;
								}
							} else {
								result = kAMDInvalidArgumentError;
							}
							if (digest) {
								bool supported = SDMMD_device_os_is_at_least(device, CFSTR("7.0"));
								bool mounted = false;
								if (supported && result == kAMDSuccess) {
									SDMMD_fire_callback(handle, unknown, CFSTR("CopyingImage"));
									result = SDMMD_copy_image(device, path);
									if (result) {
										SDMMD_fire_callback(handle, unknown, CFSTR("StreamingImage"));
										char fspath;
										Boolean fsRep = CFStringGetFileSystemRepresentation(path, &fspath, 0x400);
										if (fsRep) {
											struct stat fileStat;
											fstat(fspath, &fileStat);
											CFNumberRef size = CFNumberCreate(kCFAllocatorDefault, 0xb, &fileStat.st_size);
											CFMutableDictionaryRef streamDict = SDMMD_create_dict();
											CFDictionarySetValue(streamDict, CFSTR("Command"), CFSTR("ReceiveBytes"));
											CFDictionarySetValue(streamDict, CFSTR("ImageType"), imageType);
											CFDictionarySetValue(streamDict, CFSTR("ImageSize"), size);
											result = SDMMD_ServiceSendMessage(SDMMD_TranslateConnectionToSocket(connection), streamDict, kCFPropertyListXMLFormat_v1_0);
											if (result == 0) {
												CFDictionaryRef response;
												result = SDMMD_ServiceReceiveMessage(SDMMD_TranslateConnectionToSocket(connection), (CFPropertyListRef*)&response);
												if (result == 0) {
													if (response) {
														CFTypeRef error = CFDictionaryGetValue(response, CFSTR("Error"));
														if (error) {
															// convert error
														} else {
															CFTypeRef status = CFDictionaryGetValue(response, CFSTR("Status"));
															if (status) {
																if (CFStringCompare(status, CFSTR("ReceiveBytesAck"), 0) == 0) {
																	// block code
																	CFShow(response);
																	/*
																	 uint32_t (^block)(uint32_t, uint32_t, CFTypeRef) = ^(uint32_t size, uint32_t code, CFTypeRef thing){return code;};
																	 result = SDMMD_read_file(fspath, 0x10000, block);
																	 if (result == 0) {
																	 CFDictionaryRef getStatus;
																	 result = SDMMD_ServiceReceiveMessage(SDMMD_TranslateConnectionToSocket(connection), (CFPropertyListRef*)&getStatus);
																	 if (result == 0) {
																	 CFTypeRef error = CFDictionaryGetValue(getStatus, CFSTR("Error"));
																	 if (error) {
																	 // convert error
																	 } else {
																	 CFTypeRef streamStatus = CFDictionaryGetValue(getStatus, CFSTR("Status"));
																	 if (streamStatus) {
																	 if (CFStringCompare(streamStatus, CFSTR("Complete"), 0x0) == 0) {
																	 result = 0x0;
																	 }
																	 }
																	 }
																	 }
																	 }
																	 */
																}
															} else {
																result = kAMDUndefinedError;
															}
														}
													} else {
														result = kAMDReadError;
													}
												}
											}
											CFSafeRelease(size);
										} else {
											result = kAMDNoResourcesError;
										}
									}
									if (!mounted) {
										SDMMD_fire_callback(handle, unknown, CFSTR("MountingImage"));
										CFMutableDictionaryRef mountDict = SDMMD_create_dict();
										if (mountDict) {
											CFDictionarySetValue(mountDict, CFSTR("Command"), CFSTR("MountImage"));
											CFDictionarySetValue(mountDict, CFSTR("ImageType"), imageType);
											CFDictionarySetValue(mountDict, CFSTR("ImagePath"), CFSTR("/var/mobile/Media/PublicStaging/staging.dimage"));
											CFDictionarySetValue(mountDict, CFSTR("ImageSignature"), signature);
											result = SDMMD_ServiceSendMessage(SDMMD_TranslateConnectionToSocket(connection), mountDict, kCFPropertyListXMLFormat_v1_0);
											if (result == 0) {
												CFDictionaryRef response;
												result = SDMMD_ServiceReceiveMessage(SDMMD_TranslateConnectionToSocket(connection), (CFPropertyListRef*)&response);
												if (result == 0) {
													if (response) {
														CFTypeRef error = CFDictionaryGetValue(response, CFSTR("Error"));
														if (error) {
															// convert error
														} else {
															CFTypeRef status = CFDictionaryGetValue(response, CFSTR("Status"));
															if (status) {
																if (CFEqual(status, CFSTR("Complete"))) {
																	mounted = true;
																} else {
																	result = kAMDMissingDigestError;
																}
															}
														}
													} else {
														result = kAMDReadError;
													}
												} else {
													result = kAMDReadError;
												}
											}
										}
									}
								}
								if (mounted) {
									result = SDMMD__hangup_with_image_mounter_service(connection);
									// insert error code
								}
							}
						}
					}
					CFSafeRelease(deviceCopy);
				} else {
					printf("SDMMD_AMDeviceMountImage: Could not digest %s\n",cpath);
					result = kAMDDigestFailedError;
				}
				Safe(free, sumdigest);
			} else {
				result = kAMDInvalidArgumentError;
			}
			Safe(free, cpath);
		} else {
			result = kAMDMissingImageTypeError;
		}
	}
	return result;
}
#endif

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
	//CFSafeRelease(deviceSIMStatus);
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
	//CFSafeRelease(deviceActivationState);
	return result;
}

sdmmd_interface_return_t SDMMD_AMDeviceGetInterfaceType(SDMMD_AMDeviceRef device) {
    sdmmd_interface_return_t result = kAMDInterfaceConnectionTypeInvalid;
    if (device) {
        result = kAMDInterfaceConnectionTypeDirect;
        if (device->ivars.connection_type > 0) {
            result = kAMDInterfaceConnectionTypeIndirect;
        }
    }
    return result;
}


#endif
