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
#include <openssl/ssl.h>

void* SDMMD__CreatePairingRecordFromRecordOnDiskForIdentifier(SDMMD_AMDeviceRef device, CFDictionaryRef *dict) {
	void* result = 0xe8000007;
	if (device) {
		if (dict) {
			result = 0xe8000003;
			CFTypeRef bonjourId = SDMMD_AMDCopySystemBonjourUniqueID();
			if (bonjourId) {
				SDMMD__PairingRecordPathForIdentifier(,);
				CFDictionaryRef fileDict = SDMMD__CreateDictFromFileContents();
				result = 0xe8000025;
				if (dictCreate) {
					CFTypeRef systemId = CFDictionaryGetValue(fileDict, CFSTR("SystemBUID"));
					if (systemId) {
						if (CFGetTypeID(systemId) == CFStringGetTypeID()) {
							
						}
					} else {
						CFDictionarySetValue(fileDict, , bonjourId);
						SDMMD_store_dict(fileDict, , 1);
					}
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

void* SDMMD_send_activation(SDMMD_AMDeviceRef device) {
	void* result = 0xe8000007;
	CFMutableDictionaryRef message = NULL;
	if (device) {
		result = 0xe800000b;
		if (device->ivars.device_active) {
			result = 0xe8000007;
			if (dict) {
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
			SDMMD__mutex_lock(device->ivars.mutex_lock);
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
			SDMMD__mutex_unlock(device->ivars.mutex_lock);
		}
	}
	return result;
}

void* SDMMD_AMDeviceStopSession(SDMMD_AMDeviceRef device) {
	void* result = 0x0;
	if (device) {
		result = 0xe8000084;
		if (device->ivars.device_active) {
			SDMMD__mutex_lock(device->ivars.mutex_lock);
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
			SDMMD__mutex_unlock(device->ivars.mutex_lock);
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
			SDMMD__mutex_lock(device->ivars.mutex_lock);
			result = SDMMD_send_activation(device, options);
			if (result != 0) {
				char *reason = SDMMD_AMDErrorString(result);
				printf("SDMMD_AMDeviceActivate: Could not activate device %u %s.\n",device->ivars.device_id,reason);
			}
			SDMMD__mutex_unlock(device->ivars.mutex_lock);
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
			SDMMD__mutex_lock(device->ivars.mutex_lock);
			result = SDMMD_send_deactivation(device);
			if (result != 0) {
				char *reason = SDMMD_AMDErrorString(result);
				printf("SDMMD_AMDeviceDeactivate: Could not deactivatedevice %u: %s\n",device->ivars.device_id,reason);
			}
			SDMMD__mutex_unlock(device->ivars.mutex_lock);
		} else {
			result = 0xe8000084;
		}
	} else {
		result = 0xe8000007;
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

#endif