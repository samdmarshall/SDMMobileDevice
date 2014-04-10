/*
 *  SDMMD_Applications.c
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

#ifndef _SDM_MD_APPLICATIONS_C_
#define _SDM_MD_APPLICATIONS_C_

#include "SDMMD_Applications.h"
#include "SDMMD_Service.h"
#include "SDMMD_Functions.h"
#include "SDMMD_AMDevice.h"
#include "SDMMD_AppleFileConduit.h"

#define kAppLookupMasterKey "ReturnAttributes"

void SDMMD_browse_callback(CFDictionaryRef dict, void* response) {
	if (dict) {
		if (CFDictionaryContainsKey(dict, CFSTR(kAppLookupKeyCFBundleIdentifier))) {
			CFTypeRef bundleId = CFDictionaryGetValue(dict, CFSTR(kAppLookupKeyCFBundleIdentifier));
			CFDictionarySetValue(response, bundleId, dict);
		}
	}
}

void SDMMD_lookup_callback(CFDictionaryRef dict, void* response) {
	response = (void*)dict;
}

sdmmd_return_t SDMMD_AMDeviceLookupAppInfo(SDMMD_AMDeviceRef device, CFDictionaryRef options, CFDictionaryRef *response) {
	sdmmd_return_t result = kAMDInvalidArgumentError;
	if (device) {
		if (options) {
			CFDictionaryRef dict = NULL;
			SDMMD_AMConnectionRef conn = SDMMD_AMDServiceConnectionCreate(0, NULL, dict);
			result = SDMMD_AMDeviceSecureStartService(device, CFSTR(AMSVC_INSTALLATION_PROXY), 0x0, &conn);
			if (result == 0) {
				CFMutableDictionaryRef dict = SDMMD_create_dict();
				result = kAMDNoResourcesError;
				if (dict) {
					result = SDMMD_perform_command(conn, CFSTR("Lookup"), 0x0, SDMMD_lookup_callback, 0x2, dict, CFSTR("ClientOptions"), options);
					if (!result) {
						*response = dict;
					}
				}
			}
			else {
				printf("%s: Was unable to start the install service on the device: %i\n",__FUNCTION__,device->ivars.device_id);
			}
		}
	}
	return result;
}


sdmmd_return_t SDMMD_AMDeviceLookupApplications(SDMMD_AMDeviceRef device, CFDictionaryRef options, CFDictionaryRef *response) {
	sdmmd_return_t result = kAMDInvalidArgumentError;
	if (device) {
		if (options) {
			CFDictionaryRef dict = NULL;
			SDMMD_AMConnectionRef conn = SDMMD_AMDServiceConnectionCreate(0, NULL, dict);
			result = SDMMD_AMDeviceSecureStartService(device, CFSTR(AMSVC_INSTALLATION_PROXY), 0x0, &conn);
			if (result == 0) {
				CFMutableDictionaryRef dict = SDMMD_create_dict();
				result = kAMDNoResourcesError;
				if (dict) {
					result = SDMMD_perform_command(conn, CFSTR("Browse"), 0x0, SDMMD_browse_callback, 2, dict, CFSTR("ClientOptions"), options);
					if (!result) {
						*response = dict;
					}
				}
			}
			else {
				printf("%s: Was unable to start the install service on the device: %i\n",__FUNCTION__,device->ivars.device_id);
			}
		}
	}
	return result;
}

void SDMMD_preflight_transfer(char *path, struct stat *statRef, char *rStatRef) {
	int statResult = stat(path, statRef);
	if (statResult != 0xff) {
		if ((*(int16_t *)(statRef + 0x4) & 0xffff & 0xf000) != 0x4000) {
			rStatRef[0] = 0x1;
			rStatRef[8] = (char)(statRef + 0x60);
		}
		else {
			// error!
		}
	}
}

sdmmd_return_t SDMMD_AMDeviceTransferApplication(SDMMD_AMConnectionRef conn, CFStringRef path, CFDictionaryRef options, CallBack transferCallback, void* unknown) {
	sdmmd_return_t result = kAMDInvalidArgumentError;
	if (path) {
		if (conn) {
			char *cpath = calloc(0x1, 0x401);
			ATR_UNUSED struct stat pathStat, remoteStat;
			Boolean status = CFStringGetCString(path, cpath, 0x401, kCFStringEncodingUTF8);
			if (status) {
				CFURLRef deviceURL = SDMMD__AMDCFURLCreateFromFileSystemPathWithSmarts(path);
				if (deviceURL) {
					CFStringRef lastComp = CFURLCopyLastPathComponent(deviceURL);
					if (lastComp) {
						CFURLRef base = SDMMD__AMDCFURLCreateWithFileSystemPathRelativeToBase(kCFAllocatorDefault, CFSTR("PublicStaging"), 0x0, 0x1);
						CFURLRef copy = CFURLCreateCopyAppendingPathComponent(kCFAllocatorDefault, base, lastComp, true);
						char *copyPath = NULL;
						SDMMD__AMDCFURLGetCStringForFileSystemPath(copy, copyPath);
						SDMMD_fire_callback_767f4(transferCallback, unknown, 0x0, CFSTR("PreflightingTransfer"));
						//SDMMD_preflight_transfer(&cpath, &pathStat, &remoteStat);
						SDMMD_fire_callback_767f4(transferCallback, unknown, 0x0, CFSTR("TransferingPackage"));
						ATR_UNUSED SDMMD_AFCConnectionRef afcConn = SDMMD_AFCConnectionCreate(conn);//(r12, conn, 0x0, 0x0, &var_72);
						// loop that iterates through all of file contents
						
						
						/*if (result == 0) {
 							CFDataRef touchResponse;
							result = SDMMD_check_can_touch(afcConn, &touchResponse);
							if (result == 0) {
								CFURLRef parent = CFURLCreateCopyDeletingLastPathComponent(r12, r13);
								result = SDMMD_make_path(afcConn, parent);
								if (result == 0) {
									result = SDMMD_nuke_path(afcConn, r13);
									if ((result | 0x8) == 0x8) {
										int statResult = lstat(cpath, &pathStat);
										if (statResult != 0xff) {
											f ((var_84 & 0xffff & 0xf000) != 0xa000) {
												if (rax == 0x8000) {
													rbx = SDMMD_copy_touch_file(&remoteStat, transferCallback, unknown, afcConn, &cpath, &copyPath);
												} else {
													if (rax == 0x4000) {
														rbx = SDMMD_copy_directory(&remoteStat, transferCallback, unknown, afcConn, &cpath, &copyPath);
													} else {
														printf("transfer_package: Don't know how to copy this type of file: %s\n", cpath);
													}
												}
											} else {
												rbx = SDMMD_copy_symlink(afcConn, &cpath, &copyPath);
											}
											r14 = 0x0;
											if (rbx != 0x0) {
												r9 = SDMMD_AFCErrorString(rbx);
												printf("transfer_package: Could not copy %s to %s on the device.\n", cpath, copyPath);
												result = kAMDUndefinedError;
											}
											result = SDMMD_AFCConnectionClose(afcConn);
											if (result) {
												printf("transfer_package: Could not close AFC connection. error: %i\n", result);
											}
											CFSafeRelease(r12);
										}
									}
								} else {
									// error!
								}
							}
						}*/
						CFSafeRelease(base);
						CFSafeRelease(copy);
					}
					else {
						result = kAMDUndefinedError;
					}
					CFSafeRelease(lastComp);
				}
				CFSafeRelease(deviceURL);
			}
			else {
				result = kAMDUndefinedError;
			}
			Safe(free, cpath);
		}
	}
	return result;
}

sdmmd_return_t SDMMD_AMDeviceSecureInstallApplication(SDMMD_AMConnectionRef conn, SDMMD_AMDeviceRef device, CFURLRef path, CFDictionaryRef options, CallBack installCallback, void* unknown) {
	sdmmd_return_t result = kAMDSuccess;
	SDMMD_AMConnectionRef connection = NULL;
	bool hasConnection = (conn ? true : false);
	if (device) {
		result = SDMMD_AMDeviceSecureStartService(device, CFSTR(AMSVC_INSTALLATION_PROXY), 0x0, &connection);
		if (result == 0) {
			hasConnection = true;
		}
		else {
			printf("%s: Was unable to start the install service on the device 0x%x.\n",__FUNCTION__,result);
		}
	}
	if (hasConnection) {
		CFStringRef lastComp = CFURLCopyLastPathComponent(path);
		if (CFStringGetLength(lastComp)==0) {
			CFSafeRelease(lastComp);
			CFURLRef pathURL = CFURLCreateCopyDeletingLastPathComponent(kCFAllocatorDefault, path);
			lastComp = CFURLCopyLastPathComponent(pathURL);
			CFSafeRelease(pathURL);
		}
		if (lastComp) {
			CFStringRef format = CFStringCreateWithFormat(kCFAllocatorDefault, 0x0, CFSTR("%s%c%@"), "PublicStaging", 0x2f, lastComp);
			if (format) {
				char *appName = SDMCFStringGetString(format);
				printf("%s: Attempting install of %s.\n",__FUNCTION__,appName);
				result = SDMMD_perform_command(connection, CFSTR("Install"), 0x0, installCallback, 4, unknown, CFSTR("PackagePath"), format, CFSTR("ClientOptions"), options);
				if (result) {
					printf("%s: Old style of install failed for (%s).\n",__FUNCTION__,appName);
				}
				Safe(free, appName);
			}
			else {
				printf("%s: Unable to create CFString!\n",__FUNCTION__);
			}
			CFSafeRelease(format);
		}
		else {
			char *lastCompString = SDMCFStringGetString(lastComp);
			printf("%s: Could not copy last path component from url %s.\n",__FUNCTION__,lastCompString);
			Safe(free, lastCompString);
		}
		CFSafeRelease(lastComp);
	}
	CFSafeRelease(connection);
	char *pathString = SDMCFURLGetString(path);
	printf("%s: Installation of package %s returned 0x%x.\n",__FUNCTION__,pathString,result);
	Safe(free, pathString);
	return result;
}

sdmmd_return_t SDMMD_AMDeviceInstallApplication(SDMMD_AMDeviceRef device, CFStringRef path, CFDictionaryRef options, CallBack installCallback, void* unknown) {
	printf("%s: Entry.\n",__FUNCTION__);
	sdmmd_return_t result = kAMDSuccess;
	uint32_t socket = -1;
	SDMMD_AMConnectionRef conn = SDMMD__CreateTemporaryServConn(socket, 0x0);
	if (conn) {
		CFURLRef url = SDMMD__AMDCFURLCreateFromFileSystemPathWithSmarts(path);
		char *urlString = SDMCFURLGetString(url);
		if (url) {
			result = SDMMD_AMDeviceSecureInstallApplication(conn, device, url, options, installCallback, unknown);
		}
		else {
			printf("%s: SDMMD_AMDCFURLCreateFromFileSystemPathWithSmarts failed on %s.\n",__FUNCTION__,urlString);
		}
		Safe(free, urlString);
		CFSafeRelease(url);
		CFSafeRelease(conn);
	}
	else {
		result = kAMDUndefinedError;
	}
	return result;
}

#endif