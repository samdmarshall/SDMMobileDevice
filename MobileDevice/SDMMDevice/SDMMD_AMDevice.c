/*
 *  SDMMD_AMDevice.c
 *  SDMMobileDevice
 *
 *  Copyright (c) 2013, Sam Marshall
 *  All rights reserved.
 * 
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 *  1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
 *  3. All advertising materials mentioning features or use of this software must display the following acknowledgement:
 *  	This product includes software developed by the Sam Marshall.
 *  4. Neither the name of the Sam Marshall nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
 * 
 *  THIS SOFTWARE IS PROVIDED BY Sam Marshall ''AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL Sam Marshall BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 */

#ifndef _SDM_MD_AMDEVICE_C_
#define _SDM_MD_AMDEVICE_C_

#include "SDMMD_AMDevice.h"
#include "SDMMD_Functions.h"
#include "SDMMD_Service.h"
#include <string.h>
#include <errno.h>
#include <openssl/bio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/select.h>

SDMMD_lockdown_conn* SDMMD_lockdown_connection_create(SDMMD_lockdown_conn *lockdown) {
	if (lockdown)
		free(lockdown);
	lockdown = calloc(0x1, 0x20);
	return lockdown;
}

BIO* SDMMD__create_bio_from_data(CFDataRef data) {
	BIO *bio = NULL;
	if (data) {
		bio = BIO_new_mem_buf((void*)CFDataGetBytePtr(data),CFDataGetLength(data));
	}
	return bio;
}

X509* SDMMD__decode_certificate(CFTypeRef cert) {
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
			BIO_free(newBIO);
		}
		
	}
	return result;
}

int SDMMD__ssl_verify_callback(int, X509_STORE_CTX*);

SSL* SDMMD_lockssl_handshake(SDMMD_lockdown_conn *lockdown_conn, CFTypeRef hostCert, CFTypeRef deviceCert, CFTypeRef hostPrivKey, uint32_t num) {
	SSL *ssl;
	SSL_CTX *sslCTX;
	sdmmd_return_t result = 0x0;
	BIO_METHOD *bioMethod = BIO_s_socket();
	BIO *bioSocket = BIO_new(bioMethod);
	if (bioSocket) {
		BIO_int_ctrl(bioSocket, 0x68, 0x0, lockdown_conn->connection);
		X509 *cert = SDMMD__decode_certificate(hostCert);
		if (cert == 0) {
			printf("_create_ssl_context: Could not certificate.\n");
		}
		RSA *rsa = NULL;
		BIO *dataBIO = SDMMD__create_bio_from_data(hostPrivKey);
		if (dataBIO == 0) {
			printf("_create_ssl_context: Could not decode private key.\n");
			if (cert) {
				X509_free(cert);
				result = 0x0;
			}
		} else {
			PEM_read_bio_RSAPrivateKey(bioSocket, &rsa, 0x0, 0x0);
			BIO_free(bioSocket);
			if (rsa) {
				if (hostCert) {
					sslCTX = SSL_CTX_new(SSLv3_method());
					if (sslCTX) {
						result = SSL_CTX_use_certificate(sslCTX, cert);
						if (result == 0)
							printf("_create_ssl_context: Could not set certificate.\n");
						result = SSL_CTX_use_RSAPrivateKey(sslCTX, rsa);
						if (result == 0)
							printf("_create_ssl_context: Could not set private key.\n");
					} else {
						printf("_create_ssl_context: Could not create SSLv3 context.\n");
					}
				}
				RSA_free(rsa);
				if (cert)
					X509_free(cert);
				if (sslCTX) {
					ssl = SSL_new(sslCTX);
					if (ssl) {
						if (num) {
							SSL_set_connect_state(ssl);
						} else {
							SSL_set_accept_state(ssl);
						}
						SSL_set_verify(ssl, 0x3, SDMMD__ssl_verify_callback);
						SSL_set_verify_depth(ssl, 0x0);
						SSL_set_bio(ssl, bioSocket, bioSocket);
						SSL_set_ex_data(ssl, 0x0, deviceCert);
						result = SSL_do_handshake(ssl);
						if (result == 1) {
							SSL_CTX_free(sslCTX);
						} else {
							uint32_t err = SSL_get_error(ssl, result);
							if (result) {
								char *reason = SDMMD_ssl_strerror(ssl, result);
								printf("lockssl_handshake: SSL handshake fatal lower level error %d: %s.\n", result, reason);
							} else {
								char *reason = SDMMD_ssl_strerror(ssl, 0x0);
								printf("lockssl_handshake: SSL handshake controlled failure %d: %s.\n", result, reason);
							}
							SSL_free(ssl);
						}
					} else {
						printf("_create_ssl: Could not create SSL thing.\n");
					}
				}
			} else {
				printf("_create_ssl_context: Could not decode private key.\n");
				if (cert) {
					X509_free(cert);
					result = 0x0;
				}
			}
		}
	} else {
		printf("lockssl_handshake: Could not create SSL bio.\n");
	}
	return ssl;
}

sdmmd_return_t SDMMD_lockconn_enable_ssl(SDMMD_lockdown_conn *lockdown_conn, CFTypeRef hostCert, CFTypeRef deviceCert, CFTypeRef hostPrivKey, uint32_t num) {
	sdmmd_return_t result = 0x0;
	lockdown_conn->ssl = SDMMD_lockssl_handshake(lockdown_conn, hostCert, hostPrivKey, deviceCert, num);
	return result;
}

sdmmd_return_t SDMMD_lockconn_send_message(SDMMD_AMDeviceRef device, CFDictionaryRef dict) {
	sdmmd_return_t result = 0xe800002d;
	if (device->ivars.lockdown_conn && dict) {
		CFDataRef xml = CFPropertyListCreateXMLData(kCFAllocatorDefault, dict);
		if (xml) {			
			//uint32_t xmlLength = CFDataGetLength(xml);
			//char *xmlPtr = (char*)CFDataGetBytePtr(xml);
			//uint32_t sentLen = 0;
			SDMMD_ServiceSendMessage((SocketConnection){device->ivars.lockdown_conn->ssl, device->ivars.lockdown_conn->connection}, xml);
			
			
			/*if (device->ivars.lockdown_conn->ssl) {
				sentLen = SSL_write(device->ivars.lockdown_conn->ssl, &xmlLength, 0x4);
				sentLen = SSL_write(device->ivars.lockdown_conn->ssl, &xmlPtr, xmlLength);
			} else {
				SDMMD_ServiceSendMessage(device->ivars.lockdown_conn->connection, xml);
			}*/
		} else {
			printf("SDMMD_lockconn_send_message: Could not encode message as XML.\n");
		}
	} else {
		result = SDMMD_AMDeviceIsValid(device);
		if (result == 0x0) {
			result = 0xe800002d;
		}
	}
	return result;
}

sdmmd_return_t SDMMD_lockconn_receive_message(SDMMD_AMDeviceRef device, CFDictionaryRef *dict) {
	sdmmd_return_t result = 0x0;
	if (device->ivars.lockdown_conn) {
		
	} else {
		result = SDMMD_AMDeviceIsValid(device);
		if (result == 0x0) {
			result = 0xe8000004;
		}
	}
	return result;
}

sdmmd_return_t SDMMD_copy_lockdown_value(SDMMD_AMDeviceRef device, CFStringRef domain, CFStringRef key, CFErrorRef *err) {
	sdmmd_return_t result = 0x0;
	if (device) {
		if (device->ivars.lockdown_conn) {
			CFMutableDictionaryRef getVal = SDMMD__CreateMessageDict(CFSTR("GetValue"));
			if (getVal) {
				CFMutableDictionaryRef response;
				if (domain)
					CFDictionarySetValue(getVal, CFSTR("Domain"), domain);
				if (key)
					CFDictionarySetValue(getVal, CFSTR("Key"), domain);
					
				result = SDMMD_lockconn_send_message(device, getVal);
				if (result == 0x0) {
					result = SDMMD_lockconn_receive_message(device, &response);
					if (result == 0x0) {
						CFTypeRef err = CFDictionaryGetValue(response, CFSTR("Error"));
						if (err) {
							if (CFGetTypeID(err) == CFStringGetTypeID())
								result = SDMMD__ConvertLockdowndError(err);
							else
								result = 0xe8000013;
						} else {
							CFTypeRef value = CFDictionaryGetValue(response, CFSTR("Value"));
							if (value)
								result = value;
							else
								result = 0x0;
						}
					}
				}
			} else {
				result = 0xe8000003;
			}
		} else {
			result = 0xe800000b;
		}
	} else {
		result = 0xe8000007;
	}
	return result;
}

sdmmd_return_t SDMMD_lockdown_connection_destory(SDMMD_lockdown_conn *lockdownCon) {
	sdmmd_return_t result = 0x0;
	if (lockdownCon) {
		if (lockdownCon->ssl != 0x0) {
			SSL_free(lockdownCon->ssl);
			lockdownCon->ssl = NULL;
		}
		if (lockdownCon->connection != 0xff) {
			result = close(lockdownCon->connection);
			if (result == 0xff) {
				printf("SDMMD_lockdown_connection_destory: close(2) on socket %d failed: %d.\n",lockdownCon->connection, result);
			}
			lockdownCon->connection = 0x0;
		}
		if (lockdownCon->unknown0 != 0x0) {
			free(lockdownCon->unknown0);
		}
		result = 0x0;
		free(lockdownCon);
	}
	return result;
}

sdmmd_return_t SDMMD_copy_daemon_name(SDMMD_AMDeviceRef device, CFStringRef *name) {
	sdmmd_return_t result = 0;
	CFDictionaryRef response = CFDictionaryCreateMutable(kCFAllocatorDefault, 0x0, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
	if (device) {
		if (device->ivars.lockdown_conn) {
			if (name) {
				CFMutableDictionaryRef queryDict = SDMMD__CreateMessageDict(CFSTR("QueryType"));
				if (queryDict) {
					result = SDMMD_lockconn_send_message(device, queryDict);
					if (result == 0) {
						result = SDMMD_lockconn_receive_message(device, &response);
						if (result == 0) {
							CFTypeRef val = CFDictionaryGetValue(response, CFSTR("Error"));
							if (val == NULL) {
								val = CFDictionaryGetValue(response, CFSTR("Type"));
								if (val) {
									if (CFGetTypeID(val) == CFStringGetTypeID()) {
										CFRetain(val);
										*name = val;
									}
								} else {
									result = 0xe8000013;
								}
							}
						}
					}
				} else {
					result = 0xe8000003;
				}
			} else {
				result = 0xe8000007;
			}
		} else {
			result = 0xe800000b;
		}
	} else {
		result = 0xe8000007;
	}
	return result;
}

bool SDMMD_isDeviceAttached(uint32_t device_id) {
	bool result = false;
	CFArrayRef devices = CFArrayCreateCopy(kCFAllocatorDefault, SDMMD_MCP->deviceList);
	if (devices) {
		for (uint32_t i = 0; i < CFArrayGetCount(devices); i++) {
			CFDictionaryRef device = CFArrayGetValueAtIndex(devices, i);
			if (device) {
				CFNumberRef idNumber = CFDictionaryGetValue(device, CFSTR("DeviceID"));
				if (idNumber) {
					uint32_t fetched_id = 0;
					CFNumberGetValue(idNumber, 0x3, &fetched_id);
					if (fetched_id == device_id) {
						result = true;
						break;
					}
				}
			}
		}
	}
	return result;
}

void SDMMD__PairingRecordPathForIdentifier(CFStringRef udid, char *path) {
	
}

sdmmd_return_t SDMMD__CreatePairingRecordFromRecordOnDiskForIdentifier(SDMMD_AMDeviceRef device, CFDictionaryRef *dict) {
	sdmmd_return_t result = 0xe8000007;
	if (device) {
		if (dict) {
			result = 0xe8000003;
			CFTypeRef bonjourId = SDMMD_AMDCopySystemBonjourUniqueID();
			if (bonjourId) {
				char *path = calloc(1, sizeof(char)*0x400);
				SDMMD__PairingRecordPathForIdentifier(device->ivars.unique_device_id, path);
				CFDictionaryRef fileDict = SDMMD__CreateDictFromFileContents(path);
				result = 0xe8000025;
				if (fileDict) {
					CFTypeRef systemId = CFDictionaryGetValue(fileDict, CFSTR("SystemBUID"));
					if (systemId) {
						if (CFGetTypeID(systemId) == CFStringGetTypeID()) {
							printf("SDMMD__CreatePairingRecordFromRecordOnDiskForIdentifier: Could not store pairing record at '%s'.\n",path);
							result = 0xe800000a;
						}
					} else {
						CFDictionarySetValue(fileDict, systemId, bonjourId);
						SDMMD_store_dict(fileDict, path, 1);
					}
					CFRelease(fileDict);
				}
				free(path);
				CFRelease(bonjourId);
			}
		}
	}
	return result;
}

sdmmd_return_t SDMMD_send_activation(SDMMD_AMDeviceRef device, CFDictionaryRef dict) {
	sdmmd_return_t result = 0xe8000007;
	CFMutableDictionaryRef message = NULL;
	if (device) {
		result = 0xe800000b;
		if (device->ivars.device_active) {
			result = 0xe8000007;
			if (dict) {
				result = 0xe8000003;
				CFMutableDictionaryRef messageDict = SDMMD__CreateMessageDict(CFSTR("Activate"));
				if (messageDict) {
					CFDictionarySetValue(messageDict, CFSTR("ActivationRecord"), dict);
					result = SDMMD_lockconn_send_message(device, messageDict);
					if (result == 0) {
						result = SDMMD_lockconn_receive_message(device, &message);
						if (result == 0) {
							CFTypeRef msg = CFDictionaryGetValue(message, CFSTR("Error"));
							if (msg) {
								result = 0xe8000013;
								if (CFGetTypeID(msg) == CFStringGetTypeID()) {
									result = SDMMD__ConvertLockdowndError(msg);
								}
							}
						}
					}
				}
				if (messageDict)
					CFRelease(messageDict);
			}
		}
	}
	if (message)
		CFRelease(message);
	return result;
}

sdmmd_return_t SDMMD_send_deactivation(SDMMD_AMDeviceRef device) {
	sdmmd_return_t result = 0xe8000007;
	CFMutableDictionaryRef message = NULL;
	if (device) {
		result = 0xe800000b;
		if (device->ivars.device_active) {
			result = 0xe8000007;
			CFMutableDictionaryRef messageDict = SDMMD__CreateMessageDict(CFSTR("Deactivate"));
			result = 0xe8000003;
			if (messageDict) {
				result = SDMMD_lockconn_send_message(device, messageDict);
				if (result == 0) {
					result = SDMMD_lockconn_receive_message(device, &message);
					if (result == 0) {
						CFTypeRef msg = CFDictionaryGetValue(message, CFSTR("Error"));
						if (msg) {
							result = 0xe8000013;
							if (CFGetTypeID(msg) == CFStringGetTypeID()) {
								result = SDMMD__ConvertLockdowndError(msg);
							}
						}
					}
				}
			}
			if (messageDict)
				CFRelease(messageDict);
		}
	}
	if (message)
		CFRelease(message);
	return result;
}

sdmmd_return_t SDMMD_send_session_start(SDMMD_AMDeviceRef device, CFTypeRef record, uint32_t session) {
	sdmmd_return_t result = 0xe8000007;
	CFTypeRef var32 = NULL;
	if (device) {
		CFTypeRef var20 = NULL;
		result = 0xe800000b;
		if (device->ivars.lockdown_conn) {
			result = 0xe8000007;
			if (record) {
				if (session) {
					CFMutableDictionaryRef message = SDMMD__CreateMessageDict(CFSTR("StartSession"));
					result = 0xe8000003;
					if (message) {
						CFTypeRef hostId = CFDictionaryGetValue(record, CFSTR("HostID"));
						CFDictionarySetValue(message, CFSTR("HostID"),hostId);
						CFTypeRef bonjourId = CFDictionaryGetValue(record, CFSTR("SystemBUID"));
						if (bonjourId)
							 CFDictionarySetValue(message, CFSTR("HostID"), bonjourId);
						result = SDMMD_lockconn_send_message(device, message);
						if (result == 0) {
							CFDictionaryRef recvDict;
							result = SDMMD_lockconn_receive_message(device, &recvDict);
							if (result == 0) {
								CFTypeRef resultStr = CFDictionaryGetValue(recvDict, CFSTR("Error"));
								if (!resultStr) {
									CFTypeRef sessionId = CFDictionaryGetValue(recvDict, CFSTR("SessionID"));
									result = 0xe800001f;
									if (sessionId) {
										CFRetain(sessionId);
										CFTypeID typeId = CFGetTypeID(sessionId);
										result = 0xe800001f;
										if (typeId == CFStringGetTypeID()) {
											CFTypeRef hostCert = CFDictionaryGetValue(record, CFSTR("HostCertificate"));
											CFTypeRef hostPriKey = CFDictionaryGetValue(record, CFSTR("HostPrivateKey"));
											CFTypeRef deviceCert = CFDictionaryGetValue(record, CFSTR("DeviceCertificate"));
											result = SDMMD_lockconn_enable_ssl(device->ivars.lockdown_conn, hostCert, deviceCert, hostPriKey, 1);
											if (result != 0) {
												bool isValid = SDMMD_AMDeviceIsValid(device);
												result = 0xe8000084;
												if (isValid) {
													SDMMD_AMDeviceDisconnect(device);
													SDMMD_AMDeviceConnect(device);
													result = 0xe8000013;
												}
												if (sessionId)
													CFRelease(sessionId);													
											}
										}
									}
								} else {
									result = 0xe8000013;
									if (CFGetTypeID(resultStr) == CFStringGetTypeID()) {
										result = SDMMD__ConvertLockdowndError(resultStr);
									}
								}
							}
						}
					}
				}
			}
		}
		if (var32)
			CFRelease(var32);
		if (var20)
			CFRelease(var20);
	}
	return result;
}

sdmmd_return_t SDMMD_send_session_stop(SDMMD_AMDeviceRef device, uint32_t session) {
	sdmmd_return_t result = 0xe8000007;
	if (device) {
		result = 0xe800000b;
		CFDictionaryRef response = NULL;
		if (device->ivars.lockdown_conn) {
			result = 0xe8000007;
			if (session) {
				CFMutableDictionaryRef dict = SDMMD__CreateMessageDict(CFSTR("StopSession"));
				result = 0xe8000003;
				if (dict) {
					CFDictionarySetValue(dict, CFSTR("SessionID"), session);
					result = SDMMD_lockconn_send_message(device, dict);
					if (result == 0) {
						result = SDMMD_lockconn_receive_message(device, &response);
						if (result == 0) {
							CFTypeRef error = CFDictionaryGetValue(response, CFSTR("Error"));
							if (error && CFGetTypeID(error) == CFStringGetTypeID()) {
								result = SDMMD__ConvertLockdowndError(error);
							} else {
								SDMMD_lockconn_disable_ssl(device->ivars.lockdown_conn);
								result = 0x0;
							}
						}
					}
				}
			}
		}
		if (response)
			CFRelease(response);
	}
	return result;
}

sdmmd_return_t SDMMD_AMDeviceStartSession(SDMMD_AMDeviceRef device) {
	sdmmd_return_t result = 0xe8000007;
	CFMutableDictionaryRef record = NULL;
	CFTypeRef key = NULL;
	if (device) {
		result = 0xe8000084;
		if (device->ivars.device_active) {
			SDMMD__mutex_lock(&(device->ivars.mutex_lock));
			result = SDMMD__CreatePairingRecordFromRecordOnDiskForIdentifier((device->ivars.unique_device_id), &record);
			if (result == 0) {
				result = SDMMD_send_session_start(device, record, device->ivars.session);
				if (result == 0 && device->ivars.session != 0) {
					Boolean hasKey = CFDictionaryContainsKey(record, CFSTR("EscrowBag"));
					if (!hasKey)
						hasKey = CFDictionaryContainsKey(record, CFSTR("WiFiMACAddress"));
					if (hasKey)
						SDMMD__CopyEscrowBag(record, &key);
				} else {
					char *reason = SDMMD_AMDErrorString(result);
					printf("SDMMD_AMDeviceStartSession: Could not start session with device %u: %s\n",device->ivars.device_id,reason);
				}
			} 
			SDMMD__mutex_unlock(&(device->ivars.mutex_lock));
		}
	}
	return result;
}

sdmmd_return_t SDMMD_AMDeviceStopSession(SDMMD_AMDeviceRef device) {
	sdmmd_return_t result = 0x0;
	if (device) {
		result = 0xe8000084;
		if (device->ivars.device_active) {
			SDMMD__mutex_lock(&(device->ivars.mutex_lock));
			result = 0xe800001e;
			if (device->ivars.session != 0) {
				device->ivars.session = NULL;
				result = SDMMD_send_session_stop(device, device->ivars.session);
				if (!result) {
					char *reason = SDMMD_AMDErrorString(result);
					printf("SDMMD_AMDeviceStartSession: Could not stop session with device %u: %s\n",device->ivars.device_id,reason);
				}
				CFRelease(device->ivars.session);
			}
			SDMMD__mutex_unlock(&(device->ivars.mutex_lock));
		}
	} else {
		result = 0xe8000007;
	}
	printf("SDMMD_AMDeviceStopSession: returned 0x%x for device %u\n",result,device->ivars.device_id);
	return result;
}

sdmmd_return_t SDMMD_AMDeviceActivate(SDMMD_AMDeviceRef device, CFDictionaryRef options) {
	sdmmd_return_t result = 0x0;
	if (device) {
		if (device->ivars.device_active) {
			SDMMD__mutex_lock(&(device->ivars.mutex_lock));
			result = SDMMD_send_activation(device, options);
			if (result != 0) {
				char *reason = SDMMD_AMDErrorString(result);
				printf("SDMMD_AMDeviceActivate: Could not activate device %u %s.\n",device->ivars.device_id,reason);
			}
			SDMMD__mutex_unlock(&(device->ivars.mutex_lock));
		} else {
			result = 0xe8000084;
		}
	} else {
		result = 0xe8000007;
	}
	return result;
}

sdmmd_return_t SDMMD_AMDeviceDeactivate(SDMMD_AMDeviceRef device) {
	sdmmd_return_t result = 0x0;
	if (device) {
		if (device->ivars.device_active) {
			SDMMD__mutex_lock(&(device->ivars.mutex_lock));
			result = SDMMD_send_deactivation(device);
			if (result != 0) {
				char *reason = SDMMD_AMDErrorString(result);
				printf("SDMMD_AMDeviceDeactivate: Could not deactivate device %u: %s\n",device->ivars.device_id,reason);
			}
			SDMMD__mutex_unlock(&(device->ivars.mutex_lock));
		} else {
			result = 0xe8000084;
		}
	} else {
		result = 0xe8000007;
	}
	return result;
}

sdmmd_return_t SDMMD_AMDeviceConnect(SDMMD_AMDeviceRef device) {
	sdmmd_return_t result = 0x0;
	uint32_t socket = 0xffffffff;
	if (device) {
		result = 0xe8000084;
		if (device->ivars.device_active == 1) {
			SDMMD__mutex_lock(&(device->ivars.mutex_lock));
			if (device->ivars.lockdown_conn == 0) {
				uint32_t status = SDMMD__connect_to_port(device, 0x7ef2, 0x1, &socket, 0x0);
				if (status == 0) {
					result = 0xe800000b;
					if (socket != 0xff) {
						device->ivars.lockdown_conn = SDMMD_lockdown_connection_create(device->ivars.lockdown_conn);
						result = 0xe8000003;
						if (device->ivars.lockdown_conn != 0) {
							CFStringRef daemon = NULL;
							status = SDMMD_copy_daemon_name(device, &daemon);
							if (daemon) {
								result = 0xe8000013;
								if (CFStringCompare(daemon, CFSTR(AMSVC_LOCKDOWN), 0x0) != 0x0) {
									char *dname = SDMCFStringGetString(daemon);
									printf("SDMMD_AMDeviceConnect: This is not the droid you're looking for (is actually %s). move along,  move along.\n",dname);
									free(dname);
									SDMMD_AMDeviceDisconnect(device);
									result = 0xe8000028;
								}
							}
						}
					}
				} else {
					printf("SDMMD_AMDeviceConnect: Could not connect to lockdown port (%d) on device %d - %s: 0x%x",0xf27e, device->ivars.device_id,"device with no name",result);
				}
			} else {
				bool valid = SDMMD_AMDeviceIsValid(device);
				if (!valid) {
					SDMMD_AMDeviceDisconnect(device);
					result = 0xe8000084;
				}
			}
			SDMMD__mutex_unlock(&(device->ivars.mutex_lock));
		}
	} else {
		result = 0xe8000007;
	}
	if (socket != 0xff) {
		result = close(socket);
		if (result == 0) {
			uint32_t errorNum = errno;
			printf("SDMMD_AMDeviceConnect: close(2) on socket %d socket, failed: %d.\n",socket, errorNum);
		}
	}
	return result;
}

sdmmd_return_t SDMMD_AMDeviceDisconnect(SDMMD_AMDeviceRef device) {
	sdmmd_return_t result = 0x0;
	if (device) {
		SDMMD__mutex_lock(&(device->ivars.mutex_lock));
		result = SDMMD_lockdown_connection_destory(device->ivars.lockdown_conn);
		device->ivars.lockdown_conn = NULL;
		if (device->ivars.session) {
			CFRelease((CFTypeRef)(device->ivars.session));
			device->ivars.session = NULL;
		}
		SDMMD__mutex_unlock(&(device->ivars.mutex_lock));
	} else {
		result = 0xe8000007;
	}
	return result;
}

bool SDMMD_AMDeviceIsValid(SDMMD_AMDeviceRef device) {
	bool result = false;
	if (device && device->ivars.device_active != 0) {
		bool attached = SDMMD_isDeviceAttached(device->ivars.device_id);
		if (!attached)
			device->ivars.device_active = 0;
		else
			result = true;
	}
	return result;
}

bool SDMMD_AMDeviceIsPaired(SDMMD_AMDeviceRef device) {
	bool result = false;
	if (device) {
		SDMMD__mutex_lock(&(device->ivars.mutex_lock));
		char *path;
		SDMMD__PairingRecordPathForIdentifier(device->ivars.unique_device_id, path);
		SDMMD__mutex_unlock(&(device->ivars.mutex_lock));
		char *buffer;
		bool statResult = stat$INODE64(path, buffer);
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
	} else {
		printf("SDMMD_AMDeviceIsPaired: No device.\n");
	}
	return result;
}

sdmmd_return_t SDMMD_AMDevicePairWithOptions(SDMMD_AMDeviceRef device, CFDictionaryRef record) {
 	sdmmd_return_t result = 0xe8000007;
	if (device) {
		if (device->ivars.device_active) {
			SDMMD__mutex_lock(&(device->ivars.mutex_lock));
			if (record) {
				CFTypeRef chapCert = CFDictionaryGetValue(record, CFSTR("ChaperoneCertificate"));
				
			}
			result = 0xe8000082;
			if (device->ivars.connection_type == 1) {
				CFErrorRef err;
				result = SDMMD_copy_lockdown_value(device, NULL, CFSTR(""), &err);
				
			}
			SDMMD__mutex_unlock(&(device->ivars.mutex_lock));
		}
	}
	return result;
}

uint32_t SDMMD_AMDeviceUSBDeviceID(SDMMD_AMDeviceRef device) {
	uint32_t result = 0x0;
	if (device) {
		SDMMD__mutex_lock(&(device->ivars.mutex_lock));
		result = device->ivars.device_id;
		SDMMD__mutex_unlock(&(device->ivars.mutex_lock));
	}
	return result;
}

uint32_t SDMMD_AMDeviceUSBLocationID(SDMMD_AMDeviceRef device) {
	uint32_t result = 0x0;
	if (device) {
		SDMMD__mutex_lock(&(device->ivars.mutex_lock));
		result = device->ivars.location_id;
		SDMMD__mutex_unlock(&(device->ivars.mutex_lock));
	} else {
		printf("SDMMD_AMDeviceUSBLocationID: No device\n");
	}
	return result;
}

uint16_t SDMMD_AMDeviceUSBProductID(SDMMD_AMDeviceRef device) {
	uint16_t result = 0x0;
	if (device) {
		SDMMD__mutex_lock(&(device->ivars.mutex_lock));
		result = device->ivars.product_id & 0xffff;
		SDMMD__mutex_unlock(&(device->ivars.mutex_lock));
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
		if (!domain)
			domain = CFStringGetCString(domain, "NULL", 256, kCFStringEncodingUTF8);
		if (!key)
			key = CFStringGetCString(domain, "NULL", 256, kCFStringEncodingUTF8);
			
		SDMMD__mutex_lock(&(device->ivars.mutex_lock));
		CFErrorRef err;
		value = SDMMD_copy_lockdown_value(device, domain, key, &err);
		SDMMD__mutex_unlock(&(device->ivars.mutex_lock));
	}
	return value;
}

SDMMD_AMDeviceRef SDMMD_AMDeviceCreateEmpty() {
	return (SDMMD_AMDeviceRef)malloc(sizeof(SDM_AMDeviceClass));
}

SDMMD_AMDeviceRef SDMMD_AMDeviceCreateFromProperties(CFDictionaryRef dictionary) {
	SDMMD_AMDeviceRef device = NULL;
	if (dictionary) {
		device = SDMMD_AMDeviceCreateEmpty();
		if (device) {
			CFDictionaryRef properties = (CFDictionaryContainsKey(dictionary, CFSTR("Properties")) ? CFDictionaryGetValue(dictionary, CFSTR("Properties")) : dictionary);
			
			CFNumberRef deviceId = CFDictionaryGetValue(properties, CFSTR("DeviceID"));
			CFNumberGetValue(deviceId, 0x3, &device->ivars.device_id);
			
			CFStringRef serialNumber = CFDictionaryGetValue(properties, CFSTR("SerialNumber"));
			device->ivars.unique_device_id = serialNumber;
			
			CFStringRef linkType = CFDictionaryGetValue(properties, CFSTR("ConnectionType"));
			if (CFStringCompare(linkType, CFSTR("USB"), 0) == 0) {
				device->ivars.connection_type = 0x1;
				
				CFNumberRef productId = CFDictionaryGetValue(properties, CFSTR("ProductID"));
				CFNumberGetValue(productId, 0x2, &device->ivars.product_id);
				
				CFNumberRef locationId = CFDictionaryGetValue(properties, CFSTR("LocationID"));
				CFNumberGetValue(locationId, 0x4, &device->ivars.location_id);
				
			} else if (CFStringCompare(linkType, CFSTR("WiFi"), 0) == 0) {
				device->ivars.connection_type = 0x2;
								
			} else {
				
			}
			
			device->ivars.device_active = 0x1;
			device->ivars.unknown8 = 0x0;
			device->ivars.network_address = NULL;
			kern_return_t result = sdmmd_mutex_init(device->ivars.mutex_lock);
			printf("%x\n",result);
		}
	}
	return device;
}

bool SDMMD_AMDeviceIsAttached(SDMMD_AMDeviceRef device) {
	bool result = false;
	struct USBMuxPacket *devicesPacket = SDMMD_USBMuxCreatePacketType(kSDMMD_USBMuxPacketListDevicesType, NULL);
	SDMMD_USBMuxListenerSend(SDMMD_MCP->usbmuxd, devicesPacket);
	for (uint32_t i = 0x0; i < CFArrayGetCount(SDMMD_MCP->deviceList); i++) {
		SDMMD_AMDeviceRef deviceCheck = (SDMMD_AMDeviceRef)CFArrayGetValueAtIndex(SDMMD_MCP->deviceList, i);
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
	SDMMD_USBMuxListenerSend(SDMMD_MCP->usbmuxd, devicesPacket);
	USBMuxPacketRelease(devicesPacket);
	return SDMMD_MCP->deviceList;
}

SDMMD_AMDeviceRef SDMMD_AMDeviceCreateCopy(SDMMD_AMDeviceRef device) {
	SDMMD_AMDeviceRef copy = (SDMMD_AMDeviceRef)malloc(sizeof(SDM_AMDeviceClass));
	memcpy(copy, device, sizeof(SDM_AMDeviceClass));
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
	if (deviceSIMStatus)
		CFRelease(deviceSIMStatus);
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
	if(deviceActivationState)
		CFRelease(deviceActivationState);
	return result;
}

#endif