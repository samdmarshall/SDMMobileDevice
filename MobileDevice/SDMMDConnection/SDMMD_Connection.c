/*
 *  SDMMD_Connection.c
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

#ifndef _SDM_MD_CONNECTION_C_
#define _SDM_MD_CONNECTION_C_

#include "SDMMD_Connection.h"
#include "SDMMD_Service.h"
#include "SDMMD_Functions.h"
#include "SDMMD_AMDevice.h"

sdmmd_return_t SDMMD_perform_command(CFSocketRef socket, CFStringRef command, void* unknown, void (*callback)(CFTypeRef type, void* data), void* unknown1, ...) {
	sdmmd_return_t result = 0x0;
	CFMutableDictionaryRef dict = SDMMD_create_dict();
	if (dict) {
		CFDictionarySetValue(dict, CFSTR("Command"), command);
		va_list args;
		va_start(args, unknown1);
		CFTypeRef key, value;
		while (key = va_arg(args, CFTypeRef)) {
			value = va_arg(args, CFTypeRef);
			CFDictionarySetValue(dict, key, value);
		}
		va_end(args);
		result = SDMMD_ServiceSendMessage((SocketConnection){false, {.conn = CFSocketGetNative(socket)}}, dict, kCFPropertyListXMLFormat_v1_0);
		if (result == 0) {
			CFDictionaryRef response;
			result = SDMMD_ServiceReceiveMessage((SocketConnection){false, {.conn = CFSocketGetNative(socket)}}, (CFPropertyListRef*)&response);
			if (result == 0) {
				CFTypeRef error = CFDictionaryGetValue(response, CFSTR("Error"));
				if (error) {
					result = SDMMD__ConvertServiceError(error);
					printf("call_and_response: GOT AN ERROR 0x%08x.\n",result);
				} else {
					CFTypeRef status = CFDictionaryGetValue(response, CFSTR("Status"));
					if (status) {
						if (CFStringCompare(status, CFSTR("Complete"), 0) == 0) {
							CFTypeRef responseValue = CFDictionaryGetValue(response, CFSTR("LookupResult"));
							if (responseValue) {
								&callback(responseValue, unknown1);
							}
						}
					}
				}
			} else {
				result = 0xe800002e;
				printf("call_and_response: Could not receive response from proxy.\n");
			}
		} else {
			result = 0xe800002d;
			printf("call_and_response: Could not send request to proxy.\n");
		}
	} else {
		result = 0xe8000001;
	}
	return result;
}

SDMMD_AMConnectionRef SDMMD__CreateTemporaryServConn(uint32_t socket, SSL *ssl) {
	SDMMD_AMConnectionRef handle = NULL;
	CFStringRef closeInvalid = CFSTR("CloseOnInvalidate");
	CFDictionaryRef dict = CFDictionaryCreate(kCFAllocatorDefault, (const void**)&closeInvalid, (const void**)&kCFBooleanFalse, 0x1, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
	if (dict) {
		handle = SDMMD_AMDServiceConnectionCreate(socket, ssl, dict);
		CFRelease(dict);
	}
	return handle;
}

SDMMD_AMConnectionRef SDMMD_AMDServiceConnectionCreate(uint32_t socket, SSL* ssl, CFDictionaryRef dict) {
	SDMMD_AMConnectionRef handle = calloc(0x1, sizeof(SDMMD_AMConnectionClass));
	handle->ivars.socket = socket;
	handle->ivars.secure_context = ssl;
	handle->ivars.closeOnInvalid = true;
	handle->ivars.one1 = 0x1;
	if (dict) {
		CFTypeRef value = CFDictionaryGetValue(dict, CFSTR("CloseOnInvalidate"));
		if (value && CFEqual(value, kCFBooleanFalse))
			handle->ivars.closeOnInvalid = false;
	}
	return handle;
}

sdmmd_return_t SDMMD_send_service_start(SDMMD_AMDeviceRef device, CFStringRef service, CFTypeRef escrowBag, uint32_t *port, bool *enableSSL) {
	sdmmd_return_t result = 0xe8000007;
	if (device) {
		result = 0xe800000b;
		if (device->ivars.lockdown_conn) {
			result = 0xe8000007;
			if (service) {
				if (port != 0 && enableSSL != 0) {
					CFMutableDictionaryRef dict = SDMMD__CreateMessageDict(CFSTR("StartService"));
					result = 0xe8000003;
					if (dict) {
						CFDictionarySetValue(dict, CFSTR("Service"), service);
						if (escrowBag)
							CFDictionarySetValue(dict, CFSTR("EscrowBag"), escrowBag);
						result = SDMMD_lockconn_send_message(device, dict);
						CFRelease(dict);
						if (result == 0) {
							CFMutableDictionaryRef response;
							result = SDMMD_lockconn_receive_message(device, &response);
							CFShow(response);
							if (result == 0) {
								result = 0xe8000013;
								CFTypeRef error = CFDictionaryGetValue(response, CFSTR("Error"));
								if (error) {
									if (CFGetTypeID(error) == CFStringGetTypeID()) {
										result = SDMMD__ConvertLockdowndError(error);
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
		CFStringGetCString(service, cservice, 0xc8, 0x8000100);
    }
	SSL *ssl = NULL;
	bool mutexLock = false;
    if (device) {
		//loc_6e986;
		result = 0xe8000084;
		if (device->ivars.device_active) {
			if ((device->ivars.session) && (service) && (connection)) {
				//loc_0x6e9d0;
				SDMMD__mutex_lock(device->ivars.mutex_lock);
				bool timeoutConnection = false;
				bool closeOnInvalidate = true;
				bool directSocket = false;
				bool canConnect = true;
				if (options) {
					//loc_6e9fc;
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
						//loc_6ea99;
						if (CFEqual(bag, kCFBooleanTrue)) {
							result = SDMMD__CopyEscrowBag(device, &escrowBag);
							if (result) {
								printf("AMDeviceSecureStartService: Could not get escrow keybag for device %s!\n", (device->ivars.unique_device_id ? SDMCFStringGetString(device->ivars.unique_device_id) : "device with no name"));
								mutexLock = true;
								//loc_6ee29;
								ssl = NULL;
								//loc_6eb24;
								if (escrowBag) {
									CFRelease(escrowBag);
								}
								if (ssl) {
									SSL_free(ssl);
								}
								canConnect = false;
							}
						}
					}
					//loc_0x6ebdd;
					CFTypeRef socketVal = CFDictionaryGetValue(options, CFSTR("DirectSocket"));
					if (socketVal) {
						directSocket = (CFEqual(socketVal, kCFBooleanTrue) != 0x0 ? 0xff : 0x0);
					}
				}
				//loc_6ec1c;
				if (canConnect) {
					bool appSocket = false;
					CFTypeRef curAppSocket = CFPreferencesCopyAppValue(CFSTR("DirectSocket"), kCFPreferencesCurrentApplication);
					if (curAppSocket) {
						appSocket = CFEqual(curAppSocket, kCFBooleanTrue);
						CFRelease(curAppSocket);
						if (appSocket == 0) {
							curAppSocket = CFPreferencesCopyAppValue(CFSTR("DirectSocket"), CFSTR("com.apple.usbmuxd"));
							if (curAppSocket) {
								appSocket = CFEqual(curAppSocket, kCFBooleanTrue);
								CFRelease(curAppSocket);
							}
						}
					}
					result = SDMMD_send_service_start(device, service, escrowBag, &port, &enableSSL);
					mutexLock = true;
					if (result) {
						//loc_0x6ed50;
						if (result != 0xe800001a) {
							//loc_0x6ee29;
							ssl = NULL;
							//loc_6eb24;
							if (escrowBag) {
								CFRelease(escrowBag);
							}
							if (ssl) {
								SSL_free(ssl);
							}
						} else {
							if (escrowBag) {
								//loc_6ed6b;
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
									CFRelease(fileDict);
								}
								free(path);
								//loc_6eb24;
								if (escrowBag) {
									CFRelease(escrowBag);
								}
								if (ssl) {
									SSL_free(ssl);
								}
							}
						}
					} else {
						//loc_6ecd2;
						result = SDMMD__connect_to_port(device, port & 0xffff, timeoutConnection, &socket, ((uint32_t)ssl & 0x1) & 0xff);
						if (result == 0x0) {
							//loc_0x6ee60;
							if (enableSSL) {
								//loc_6ee70;
								printf("AMDeviceSecureStartService: SSL requested for service %s with device %s.\n", cservice, (device->ivars.unique_device_id ? SDMCFStringGetString(device->ivars.unique_device_id) : "device with no name"));
								CFMutableDictionaryRef record = NULL;
								if (socket != 0xff) {
									result = 0xe8000007;
									if (device->ivars.device_active) {
										//loc_0x6efb4;
										result = SDMMD__CreatePairingRecordFromRecordOnDiskForIdentifier(device, &record);
										if (result == 0x0) {
											//loc_6efd0;
											CFTypeRef rootCertVal = CFDictionaryGetValue(record, CFSTR("RootCertificate"));
											result = 0xe8000025;
											if (rootCertVal == 0x0) {
												//loc_0x6f06b;
												ssl = NULL;
											} else {
												//loc_6eff1;
												CFTypeRef deviceCertVal = CFDictionaryGetValue(record, CFSTR("DeviceCertificate"));
												CFTypeRef rootPrivKeyVal = CFDictionaryGetValue(record, CFSTR("RootPrivateKey"));
												if (deviceCertVal && rootPrivKeyVal) {
													ssl = SDMMD_lockssl_handshake((device->ivars.lockdown_conn), rootCertVal, deviceCertVal, rootPrivKeyVal, 0x1);
													if (ssl) {
														result = 0x0;
													} else {
														printf("_TurnOnSSLOverSocket: Could not perform SSL handshake.\n");
														result = 0xe800007f;
													}
												}
											}
										}
									}
									//loc_6eee2;
									if (record) {
										CFRelease(record);
									}
									if (result == 0x0) {
								    	//loc_6eefc;
										CFMutableDictionaryRef connDict = SDMMD_create_dict();
										result = 0xe8000003;
										if (connDict) {
											CFDictionarySetValue(connDict, CFSTR("CloseOnInvalidate"), (closeOnInvalidate ? kCFBooleanTrue : kCFBooleanFalse));
											SDMMD_AMConnectionRef conn = SDMMD_AMDServiceConnectionCreate(socket, ssl, connDict);
											result = 0xe8000003;
											if (conn) {
												SDMMD_AMDServiceConnectionSetDevice(&conn, device);
												SDMMD_AMDServiceConnectionSetServiceName(&conn, service);
												*connection = conn;
												socket = 0xffffffff;
												ssl = NULL;
												result = 0x0;
											}
											CFRelease(connDict);
										}
									}
								} else {
									//loc_0x6eb1e;
									result = 0xe8000007;
								}
							} else {
								//loc_0x6eefc;
								CFMutableDictionaryRef connDict = SDMMD_create_dict();
								result = 0xe8000003;
								if (connDict) {
									CFDictionarySetValue(connDict, CFSTR("CloseOnInvalidate"), (closeOnInvalidate ? kCFBooleanTrue : kCFBooleanFalse));
									SDMMD_AMConnectionRef conn = SDMMD_AMDServiceConnectionCreate(socket, ssl, connDict);
									result = 0xe8000003;
									if (conn) {
										SDMMD_AMDServiceConnectionSetDevice(&conn, device);
										SDMMD_AMDServiceConnectionSetServiceName(&conn, service);
										*connection = conn;
										result = 0x0;
									}
									CFRelease(connDict);
								}
							}
						} else {
							//loc_6ed09;
							printf("SDMMD_AMDeviceSecureStartService: Could not connect to \"%s\" service on port %d, device %d - %s.", cservice, port, device->ivars.device_id, SDMCFStringGetString(device->ivars.unique_device_id));
							//loc_6ee29;
							ssl = NULL;
						}
						//loc_6eb24;
						if (escrowBag) {
							CFRelease(escrowBag);
						}
						if (ssl) {
							SSL_free(ssl);
						}
					}
				}
			} else {
				//loc_6eb1e;
				result = 0xe8000007;
				//loc_6eb24;
				if (escrowBag) {
					CFRelease(escrowBag);
				}
				if (ssl) {
					SSL_free(ssl);
				}
			}
		} else {
			//loc_6eb24;
			if (escrowBag) {
				CFRelease(escrowBag);
			}
			if (ssl) {
				SSL_free(ssl);
			}
		}
	} else {
		//loc_0x6eb06;
		ssl = NULL;
		mutexLock = false;
		//loc_6eb1e;
		result = 0xe8000007;
		//loc_6eb24;
		if (escrowBag) {
			CFRelease(escrowBag);
		}
		if (ssl) {
			SSL_free(ssl);
		}
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
	printf("SDMMD_AMDeviceSecureStartService: Returned %x starting service %s on device at port %d, out fd = %d.\n", result, cservice, port, socket);
	free(cservice);
	return result;
}

sdmmd_return_t SDMMD_AMDeviceStartService(SDMMD_AMDeviceRef device, CFStringRef service, CFDictionaryRef options, SDMMD_AMConnectionRef *connection) {
	sdmmd_return_t result = 0xe8000007;
	uint32_t socket = 0xffffffff;
	if (device && connection) {
		SSL *ssl_enabled = NULL;
		if (service) {
			if (device->ivars.device_active) {
				CFMutableDictionaryRef optionsCopy;
				if (options)
				 	optionsCopy = CFDictionaryCreateMutableCopy(kCFAllocatorDefault, 0x0, options);
				else
					optionsCopy = CFDictionaryCreateMutable(kCFAllocatorDefault, 0x0, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
				if (optionsCopy) {
					CFDictionarySetValue(optionsCopy, CFSTR("CloseOnInvalidate"), kCFBooleanFalse);
					result = SDMMD_AMDeviceSecureStartService(device, service, optionsCopy, connection);
					CFRelease(optionsCopy);
					if (result == 0) {
						socket = SDMMD_AMDServiceConnectionGetSocket(*connection);
						ssl_enabled = SDMMD_AMDServiceConnectionGetSecureIOContext(*connection);
						if (ssl_enabled) {
							result = 0xe800007f;
						} else {
							result = 0x0;
						}
					}
				} else {
					result = 0xe8000003;
				}
			} else {
				result = 0xe8000084;
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
	printf("SDMMD_AMDeviceStartService: returning 0x%x, socket is %d.\n",result, socket);
	return result;
}

void SDMMD_AMDServiceConnectionSetDevice(SDMMD_AMConnectionRef *connection, SDMMD_AMDeviceRef device) {
	(*connection)->ivars.device = device;
}

void SDMMD_AMDServiceConnectionSetServiceName(SDMMD_AMConnectionRef *connection, CFStringRef service) {
	if (service)
		CFStringGetCString(service, (*connection)->ivars.service, 0x80, 0x8000100);
}

uint32_t SDMMD_AMDServiceConnectionGetSocket(SDMMD_AMConnectionRef connection) {
	return connection->ivars.socket;
}

SSL* SDMMD_AMDServiceConnectionGetSecureIOContext(SDMMD_AMConnectionRef connection) {
	return connection->ivars.secure_context;
}

sdmmd_return_t SDMMD_AMDServiceConnectionInvalidate(SDMMD_AMConnectionRef connection) {
	return 0x0;
}


#endif

