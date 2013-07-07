/*
 *  SDMMD_AMDevice.c
 *  SDM_MD_Demo
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
#include <string.h>

void* SDMMD_lockdown_connection_destory(SDMMD_lockdown_conn *lockdownCon) {
	void* result = 0x0;
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

SDMMD_lockdown_conn* SDMMD_lockdown_connection_create(SDMMD_lockdown_conn *lockdown) {
	if (lockdown)
		free(lockdown);
	lockdown = calloc(0x1, 0x20);
	return lockdown;
}

bool SDMMD_isDeviceAttached(uint32_t device_id) {
	bool result = false;
	CFArrayRef devices = SDMMD_USBMuxCopyDeviceArray();
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

void* SDMMD__CreatePairingRecordFromRecordOnDiskForIdentifier(SDMMD_AMDeviceRef device, CFDictionaryRef *dict) {
	void* result = 0xe8000007;
	if (device) {
		if (dict) {
			result = 0xe8000003;
			CFTypeRef bonjourId = SDMMD_AMDCopySystemBonjourUniqueID();
			if (bonjourId) {
				char *path;
				bzero(path, 0x400);
				SDMMD__PairingRecordPathForIdentifier(device->ivars.unique_device_id,path);
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
				CFRelease(bonjourId);
			}
		}
	}
	return result;
}

void* SDMMD_send_activation(SDMMD_AMDeviceRef device, CFDictionaryRef dict) {
	void* result = 0xe8000007;
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
									SDMMD__ConvertLockdowndError(message);
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

void* SDMMD_send_deactivation(SDMMD_AMDeviceRef device) {
	void* result = 0xe8000007;
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
								SDMMD__ConvertLockdowndError(message);
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

void* SDMMD_send_session_start(SDMMD_AMDeviceRef device, CFTypeRef record, uint32_t session) {
	void* result = 0xe8000007;
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

void* SDMMD_AMDeviceStartSession(SDMMD_AMDeviceRef device) {
	void* result = 0xe8000007;
	CFTypeRef record = NULL;
	CFTypeRef key = NULL;
	if (device) {
		result = 0xe8000084;
		if (device->ivars.device_active) {
			SDMMD__mutex_lock(&(device->ivars.mutex_lock));
			result = SDMMD__CreatePairingRecordFromRecordOnDiskForIdentifier(device->ivars.unique_device_id, &record);
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

void* SDMMD_AMDeviceStopSession(SDMMD_AMDeviceRef device) {
	void* result = 0x0;
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

void* SDMMD_AMDeviceStartService(SDMMD_AMDeviceRef device, CFStringRef service, CFDictionaryRef options, uint32_t *handle) {
	void* result = 0xe8000007;
	CFTypeRef var16 = NULL;
	CFTypeRef var13 = NULL;
	uint32_t socket = 0xffffffff;
	if (device && handle) {
		SSL *ssl_enabled = NULL;
		if (service) {
			if (device->ivars.device_active) {
				CFMutableDictionaryRef optionsCopy;
				if (options)
				 	optionsCopy = CFDictionaryCreateMutableCopy(kCFAllocatorDefault, NULL, options);
				else
					optionsCopy = CFDictionaryCreateMutable(kCFAllocatorDefault, NULL, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
				if (optionsCopy) {
					CFDictionarySetValue(optionsCopy, CFSTR("CloseOnInvalidate"), kCFBooleanFalse);
					result = SDMMD_AMDeviceSecureStartService(device, service, optionsCopy, &var16);
					if (result == 0) {
						socket = SDMMD_AMDServiceConnectionGetSocket(var16);
						ssl_enabled = SDMMD_AMDServiceConnectionGetSecureIOContext(var16);
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
		if (var16)
			CFRelease(var16);
		if (var13)
			CFRelease(var13);
		if (ssl_enabled)
			SSL_free(ssl_enabled);
		if (socket != 0xff)  {
			if(close(socket)) {
				printf("SDMMD_AMDeviceStartService: close(2) con socket %d failed: %d\n",socket,SDMMD__error());
			}
		}
	}
	printf("SDMMD_AMDeviceStartService: returning 0x%x, socket is %d.\n",result, socket);
	return result;
}

void* SDMMD_AMDeviceActivate(SDMMD_AMDeviceRef device, CFDictionaryRef options) {
	void* result = 0x0;
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

void* SDMMD_AMDeviceDeactivate(SDMMD_AMDeviceRef device) {
	void* result = 0x0;
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

void* SDMMD_AMDeviceConnect(SDMMD_AMDeviceRef device) {
	void* result = 0x0;
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
							status = SDMMD_copy_daemon_name(device, daemon);
							if (daemon) {
								result = 0xe8000013;
								if (CFStringCompare(daemon, CFSTR("com.apple.mobile.lockdown"), 0x0) != 0x0) {
									char *dname = calloc(1, CFStringGetLength(daemon)+1);
									CFStringGetCString(daemon, dname, CFStringGetLength(daemon), CFStringGetFastestEncoding(daemon));
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
			uint32_t errorNum = SDMMD__error();
			printf("SDMMD_AMDeviceConnect: close(2) on socket %d socket, faild: %d.\n",socket, errorNum);
		}
	}
	return result;
}

void* SDMMD_AMDeviceDisconnect(SDMMD_AMDeviceRef device) {
	void* result = 0x0;
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
			uint32_t errorNum = error();
			if (errorNum != 2) {
				errorNum = error();
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

void* SDMMD_AMDevicePairWithOptions(SDMMD_AMDeviceRef device, CFDictionaryRef record) {
	void* result = 0xe8000007;
	if (device) {
		if (device->ivars.device_active) {
			SDMMD__mutex_lock(&(device->ivars.mutex_lock));
			if (record) {
				CFTypeRef chapCert = CFDictionaryGetValue(record, CFSTR("ChaperoneCertificate"));
				
			}
			result = 0xe8000082;
			if (device->ivars.connection_type == 1) {
				uint32_t var76;
				result = SDMMD_copy_lockdown_value(device, 0x0, CFSTR(""), &var76);
				
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

#endif