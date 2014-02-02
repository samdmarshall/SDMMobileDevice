/*
 *  SDMMD_Connection.c
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

#ifndef _SDM_MD_CONNECTION_C_
#define _SDM_MD_CONNECTION_C_

#include "SDMMD_Connection.h"
#include "SDMMD_Service.h"
#include "SDMMD_Functions.h"
#include "SDMMD_AMDevice.h"
#include "Core.h"

sdmmd_return_t SDMMD_perform_command(SDMMD_AMConnectionRef conn, CFStringRef command, uint64_t code, CallBack handle, uint32_t argsCount, void* paramStart, ...) {
	sdmmd_return_t result = 0x0;
	CFMutableDictionaryRef message = SDMMD_create_dict();
	if (message) {
		CFDictionarySetValue(message, CFSTR("Command"), command);
		va_list args;
		va_start(args, paramStart);
		CFTypeRef key, value;
		for (uint32_t i = 0; i < argsCount; i++) {
			key = va_arg(args, CFTypeRef);
			value = va_arg(args, CFTypeRef);
			CFDictionarySetValue(message, key, value);
			i++;
		}
		va_end(args);
		SocketConnection sock = SDMMD_TranslateConnectionToSocket(conn);			
		result = SDMMD_ServiceSendStream(sock, message, kCFPropertyListXMLFormat_v1_0);
		if (result == 0) {
			CFDictionaryRef response = NULL;
			result = SDMMD_ServiceReceiveStream(sock, (CFPropertyListRef*)&response);
			if (result == 0 && response) {
				while (result == 0) {
					CFTypeRef error = CFDictionaryGetValue(response, CFSTR("Error"));
					if (error) {
						PrintCFType(response);
						result = SDMMD__ConvertServiceError(error);
						printf("call_and_response: GOT AN ERROR 0x%08x %s.\n",result, SDMMD_AMDErrorString(result));
					} else {
						CFTypeRef status = CFDictionaryGetValue(response, CFSTR("Status"));
						if (status) {
							if (CFStringCompare(status, CFSTR("Complete"), 0) != 0) {
								CFArrayRef responses = CFDictionaryGetValue(response, CFSTR("CurrentList"));
								if (responses) {
									uint64_t count = CFArrayGetCount(responses);
									for (uint32_t i = 0; i < count; i++) {
										CFDictionaryRef value = CFArrayGetValueAtIndex(responses, i);
										handle(value, paramStart);
									}
								} else {
									handle(response, 0);
								}
							} else {
								break;
							}
						}
					}
					SDMMD_ServiceReceiveStream(sock, (CFPropertyListRef*)&response);
				}
			} else {
				result = kAMDReceiveMessageError;
				printf("call_and_response: Could not receive response from proxy.\n");
			}
		} else {
			result = kAMDSendMessageError;
			printf("call_and_response: Could not send request to proxy.\n");
		}
	} else {
		result = kAMDUndefinedError;
	}
	return result;
}

SDMMD_AMConnectionRef SDMMD__CreateTemporaryServConn(uint32_t socket, SSL *ssl) {
	SDMMD_AMConnectionRef handle = NULL;
	CFStringRef closeInvalid = CFSTR("CloseOnInvalidate");
	CFDictionaryRef dict = CFDictionaryCreate(kCFAllocatorDefault, (const void**)&closeInvalid, (const void**)&kCFBooleanFalse, 0x1, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
	if (dict) {
		handle = SDMMD_AMDServiceConnectionCreate(socket, ssl, dict);
		CFSafeRelease(dict);
	}
	return handle;
}

SDMMD_AMConnectionRef SDMMD_AMDServiceConnectionCreate(uint32_t socket, SSL* ssl, CFDictionaryRef dict) {
	SDMMD_AMConnectionRef handle = calloc(0x1, sizeof(struct am_connection));
	handle->ivars.socket = socket;
	handle->ivars.ssl = ssl;
	handle->ivars.closeOnInvalid = true;
	handle->ivars.one1 = 0x1;
	if (dict) {
		CFTypeRef value = CFDictionaryGetValue(dict, CFSTR("CloseOnInvalidate"));
		if (value && CFEqual(value, kCFBooleanFalse)) {
			handle->ivars.closeOnInvalid = false;
		}
	}
	return handle;
}

sdmmd_return_t SDMMD_send_service_start(SDMMD_AMDeviceRef device, CFStringRef service, CFTypeRef escrowBag, uint32_t *port, bool *enableSSL) {
	sdmmd_return_t result = kAMDInvalidArgumentError;
	if (device) {
		result = kAMDNotConnectedError;
		if (device->ivars.lockdown_conn) {
			result = kAMDInvalidArgumentError;
			if (service) {
				if (port != 0 && enableSSL != 0) {
					CFMutableDictionaryRef dict = SDMMD__CreateMessageDict(CFSTR("StartService"));
					result = kAMDNoResourcesError;
					if (dict) {
						CFDictionarySetValue(dict, CFSTR("Service"), service);
						if (escrowBag) {
							CFDictionarySetValue(dict, CFSTR("EscrowBag"), escrowBag);
						}
						result = SDMMD_lockconn_send_message(device, dict);
						CFSafeRelease(dict);
						if (result == 0) {
							CFMutableDictionaryRef response = NULL;
							result = SDMMD_lockconn_receive_message(device, &response);
							if (result == 0 && response) {
								result = kAMDInvalidResponseError;
								CFTypeRef error = CFDictionaryGetValue(response, CFSTR("Error"));
								if (error) {
									if (CFGetTypeID(error) == CFStringGetTypeID()) {
										result = (sdmmd_return_t)SDMMD__ConvertLockdowndError(error);
									}
								} else {
									CFTypeRef portNumber = CFDictionaryGetValue(response, CFSTR("Port"));
									if (portNumber) {
										if (CFGetTypeID(portNumber) == CFNumberGetTypeID()) {
											CFNumberGetValue(portNumber, 0x9, port);
										}
									}
									CFTypeRef sslService = CFDictionaryGetValue(response, CFSTR("EnableServiceSSL"));
									if (sslService) {
										*enableSSL = (CFEqual(sslService, kCFBooleanTrue) ? true : false);
									} else {
										*enableSSL = false;
									}
									result = 0x0;
								}
							}
						}
					}
				}
			}
		}
	}
	return result;
}


sdmmd_return_t SDMMD_AMDeviceSecureStartService(SDMMD_AMDeviceRef device, CFStringRef service, CFDictionaryRef options, SDMMD_AMConnectionRef *connection) {
    uint32_t port = 0x0;
    uint32_t socket = 0xffffffff;
    CFDataRef escrowBag = NULL;
	sdmmd_return_t result = 0x0;
	bool enableSSL = false;
	char *cservice = calloc(1, sizeof(char)*0xc8);
    if (service) {
		CFStringGetCString(service, cservice, 0xc8, kCFStringEncodingUTF8);
    }
	SSL *ssl = NULL;
	bool mutexLock = false;
    if (device) {
		result = kAMDDeviceDisconnectedError;
		if (device->ivars.device_active) {
			if ((device->ivars.session) && (service) && (connection)) {
				SDMMD__mutex_lock(device->ivars.mutex_lock);
				bool timeoutConnection = false;
				bool closeOnInvalidate = true;
				bool directSocket = false;
				bool canConnect = true;
				if (options) {
					CFTypeRef closeVal = CFDictionaryGetValue(options, CFSTR("CloseOnInvalidate"));
					if (closeVal) {
						Boolean closeValResult = CFEqual(closeVal, kCFBooleanTrue);
						if (closeValResult == 0x0) {
							closeOnInvalidate = (CFEqual(closeVal, kCFBooleanFalse) == 0x0 ? 0xff : 0x0);
						}
					}
					CFTypeRef timeoutVal = CFDictionaryGetValue(options, CFSTR("TimeoutConnection"));
					if (timeoutVal) {
						timeoutConnection = (CFEqual(timeoutVal, kCFBooleanTrue) != 0x0 ? 0xff : 0x0) & 0xff;
					}
					CFTypeRef bag = CFDictionaryGetValue(options, CFSTR("UnlockEscrowBag"));
					if (bag) {
						if (CFEqual(bag, kCFBooleanTrue)) {
							result = SDMMD__CopyEscrowBag(device, &escrowBag);
							if (result) {
								printf("AMDeviceSecureStartService: Could not get escrow keybag for device %s!\n", (device->ivars.unique_device_id ? SDMCFStringGetString(device->ivars.unique_device_id) : "device with no name"));
								mutexLock = true;
								ssl = NULL;
								CFSafeRelease(escrowBag);
								Safe(SSL_free,ssl);
								
								canConnect = false;
							}
						}
					}
					CFTypeRef socketVal = CFDictionaryGetValue(options, CFSTR("DirectSocket"));
					if (socketVal) {
						directSocket = (CFEqual(socketVal, kCFBooleanTrue) != 0x0 ? 0xff : 0x0);
					}
				}
				if (canConnect) {
					bool appSocket = false;
					CFTypeRef curAppSocket = CFPreferencesCopyAppValue(CFSTR("DirectSocket"), kCFPreferencesCurrentApplication);
					if (curAppSocket) {
						appSocket = CFEqual(curAppSocket, kCFBooleanTrue);
						CFSafeRelease(curAppSocket);
						if (appSocket == 0) {
							curAppSocket = CFPreferencesCopyAppValue(CFSTR("DirectSocket"), CFSTR("com.apple.usbmuxd"));
							if (curAppSocket) {
								appSocket = CFEqual(curAppSocket, kCFBooleanTrue);
								CFSafeRelease(curAppSocket);
							}
						}
					}
					
					result = SDMMD_send_service_start(device, service, escrowBag, &port, &enableSSL);
					mutexLock = true;
					if (result) {
						if (result != kAMDPasswordProtectedError) {
							ssl = NULL;
							CFSafeRelease(escrowBag);
							Safe(SSL_free,ssl);
						} else {
							if (escrowBag) {
								printf("AMDeviceSecureStartService: Escrow bag mismatch for device %s!", (device->ivars.unique_device_id ? SDMCFStringGetString(device->ivars.unique_device_id) : "device with no name"));
								char *path = calloc(1, sizeof(char)*0x400);
								SDMMD__PairingRecordPathForIdentifier(device->ivars.unique_device_id, path);
								CFMutableDictionaryRef fileDict = SDMMD__CreateDictFromFileContents(path);
								ssl = NULL;
								if (fileDict) {
									CFDictionaryRemoveValue(fileDict, CFSTR("EscrowBag"));
									result = SDMMD_store_dict(fileDict, path, 0x1);
									if (result) {
										printf("_DestroyEscrowBag: Failed to store escrow bag to %s.\n", path);
									}
								}
								CFSafeRelease(fileDict);
								Safe(free,path);
								CFSafeRelease(escrowBag);
								Safe(SSL_free,ssl);
							}
						}
					} else {
						result = SDMMD__connect_to_port(device, port, timeoutConnection, &socket, ((uint32_t)ssl & 0x1) & 0xff);
						if (result == 0x0) {
							if (enableSSL) {
								printf("AMDeviceSecureStartService: SSL requested for service %s with device %s.\n", cservice, (device->ivars.unique_device_id ? SDMCFStringGetString(device->ivars.unique_device_id) : "device with no name"));
								CFMutableDictionaryRef record = NULL;
								if (socket != 0xff) {
									result = kAMDInvalidArgumentError;
									if (device->ivars.device_active) {
										result = SDMMD__CreatePairingRecordFromRecordOnDiskForIdentifier(device, &record);
										if (result == 0x0) {
											CFTypeRef rootCertVal = CFDictionaryGetValue(record, CFSTR("RootCertificate"));
											result = kAMDMissingPairRecordError;
											if (rootCertVal == 0x0) {
												ssl = NULL;
											} else {
												CFTypeRef deviceCertVal = CFDictionaryGetValue(record, CFSTR("DeviceCertificate"));
												CFTypeRef rootPrivKeyVal = CFDictionaryGetValue(record, CFSTR("RootPrivateKey"));
												if (deviceCertVal && rootPrivKeyVal) {
													ssl = SDMMD_lockssl_handshake((device->ivars.lockdown_conn), rootCertVal, deviceCertVal, rootPrivKeyVal, 0x1);
													if (ssl) {
														result = kAMDSuccess;
													} else {
														printf("_TurnOnSSLOverSocket: Could not perform SSL handshake.\n");
														result = kAMDNoWifiSyncSupportError;
													}
												}
											}
										}
									}
									CFSafeRelease(record);
									if (result == 0x0) {
										CFMutableDictionaryRef connDict = SDMMD_create_dict();
										result = kAMDNoResourcesError;
										if (connDict) {
											CFDictionarySetValue(connDict, CFSTR("CloseOnInvalidate"), (closeOnInvalidate ? kCFBooleanTrue : kCFBooleanFalse));
											SDMMD_AMConnectionRef conn = SDMMD_AMDServiceConnectionCreate(socket, ssl, connDict);
											result = kAMDNoResourcesError;
											if (conn) {
												SDMMD_AMDServiceConnectionSetDevice(&conn, device);
												SDMMD_AMDServiceConnectionSetServiceName(&conn, service);
												*connection = conn;
												socket = 0xffffffff;
												ssl = NULL;
												result = kAMDSuccess;
											}
											CFSafeRelease(connDict);
										}
									}
								} else {
									result = kAMDInvalidArgumentError;
								}
							} else {
								CFMutableDictionaryRef connDict = SDMMD_create_dict();
								result = kAMDNoResourcesError;
								if (connDict) {
									CFDictionarySetValue(connDict, CFSTR("CloseOnInvalidate"), (closeOnInvalidate ? kCFBooleanTrue : kCFBooleanFalse));
									SDMMD_AMConnectionRef conn = SDMMD_AMDServiceConnectionCreate(socket, ssl, connDict);
									result = kAMDNoResourcesError;
									if (conn) {
										SDMMD_AMDServiceConnectionSetDevice(&conn, device);
										SDMMD_AMDServiceConnectionSetServiceName(&conn, service);
										*connection = conn;
										result = kAMDSuccess;
									}
									CFSafeRelease(connDict);
								}
							}
						} else {
							printf("SDMMD_AMDeviceSecureStartService: Could not connect to \"%s\" service on port %d, device %d - %s.", cservice, port, device->ivars.device_id, SDMCFStringGetString(device->ivars.unique_device_id));
							ssl = NULL;
						}
						CFSafeRelease(escrowBag);
						Safe(SSL_free,ssl);
					}
				}
			} else {
				result = kAMDInvalidArgumentError;
				CFSafeRelease(escrowBag);
				Safe(SSL_free,ssl);
			}
		} else {
			CFSafeRelease(escrowBag);
			Safe(SSL_free,ssl);
		}
	} else {
		ssl = NULL;
		mutexLock = false;
		result = kAMDInvalidArgumentError;
		CFSafeRelease(escrowBag);
		Safe(SSL_free,ssl);
	}
	//loc_6eb42;
	/*if (socket != 0xff) {
		if (close(socket) == 0xff) {
			printf("SDMMD_AMDeviceSecureStartService: close(2) on socket %d failed: %d.\n", socket, errno);
		}
	}*/
	if (mutexLock) {
		SDMMD__mutex_unlock(device->ivars.mutex_lock);
	}
	//printf("SDMMD_AMDeviceSecureStartService: Returned %x starting service %s on device at port %d, out fd = %d.\n", result, cservice, port, socket);
	Safe(free,cservice);
	return result;
}

sdmmd_return_t SDMMD_AMDeviceStartService(SDMMD_AMDeviceRef device, CFStringRef service, CFDictionaryRef options, SDMMD_AMConnectionRef *connection) {
	sdmmd_return_t result = kAMDInvalidArgumentError;
	//uint32_t socket = 0xffffffff;
	if (device && connection) {
		SSL *ssl_enabled = NULL;
		if (service) {
			if (device->ivars.device_active) {
				CFMutableDictionaryRef optionsCopy;
				if (options) {
				 	optionsCopy = CFDictionaryCreateMutableCopy(kCFAllocatorDefault, 0x0, options);
				} else {
					optionsCopy = CFDictionaryCreateMutable(kCFAllocatorDefault, 0x0, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
				}
				if (optionsCopy) {
					CFDictionarySetValue(optionsCopy, CFSTR("CloseOnInvalidate"), kCFBooleanFalse);
					result = SDMMD_AMDeviceSecureStartService(device, service, optionsCopy, connection);
					CFSafeRelease(optionsCopy);
					if (result == 0) {
						//socket = SDMMD_AMDServiceConnectionGetSocket(*connection);
						ssl_enabled = SDMMD_AMDServiceConnectionGetSecureIOContext(*connection);
						if (ssl_enabled) {
							result = kAMDNoWifiSyncSupportError;
						} else {
							result = kAMDSuccess;
						}
					}
				} else {
					result = kAMDNoResourcesError;
				}
			} else {
				result = kAMDDeviceDisconnectedError;
				ssl_enabled = NULL;
			}
		}
		/*if (ssl_enabled)
			SSL_free(ssl_enabled);
		if (socket != 0xff)  {
			if(close(socket) == 0xff) {
				printf("SDMMD_AMDeviceStartService: close(2) con socket %d failed: %d\n",socket,errno);
			}
		}*/
	}
	//printf("SDMMD_AMDeviceStartService: returning 0x%x, socket is %d.\n",result, socket);
	return result;
}

void SDMMD_AMDServiceConnectionSetDevice(SDMMD_AMConnectionRef *connection, SDMMD_AMDeviceRef device) {
	(*connection)->ivars.device = device;
}

void SDMMD_AMDServiceConnectionSetServiceName(SDMMD_AMConnectionRef *connection, CFStringRef service) {
	if (service) {
		CFStringGetCString(service, (*connection)->ivars.service, 0x80, kCFStringEncodingUTF8);
	}
}

uint32_t SDMMD_AMDServiceConnectionGetSocket(SDMMD_AMConnectionRef connection) {
	return connection->ivars.socket;
}

SSL* SDMMD_AMDServiceConnectionGetSecureIOContext(SDMMD_AMConnectionRef connection) {
	return connection->ivars.ssl;
}

sdmmd_return_t SDMMD_AMDServiceConnectionInvalidate(SDMMD_AMConnectionRef connection) {
	return 0x0;
}

sdmmd_return_t SDMMD_AMDeviceSecureStartSessionedService(SDMMD_AMDeviceRef device, CFStringRef service, SDMMD_AMConnectionRef *connection) {
	sdmmd_return_t result = 0x0;
	if (device) {
		result = SDMMD_AMDeviceConnect(device);
		if (result == 0) {
			result = SDMMD_AMDeviceStartSession(device);
			if (result == 0) {
				result = SDMMD_AMDeviceSecureStartService(device, service, NULL, connection);
				if (result) {
					char *serviceString = SDMCFStringGetString(service);
					printf("SDMMD_AMDeviceSecureStartSessionedService: Could not start service %s for device %i.\n", serviceString, device->ivars.device_id);
					Safe(free, serviceString);
				}
				SDMMD_AMDeviceStopSession(device);
			}
			SDMMD_AMDeviceDisconnect(device);
		}
	}
	return result;
}

#endif

