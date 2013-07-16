/*
 *  SDMMD_Applications.c
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

#ifndef _SDM_MD_APPLICATIONS_C_
#define _SDM_MD_APPLICATIONS_C_

#include "SDMMD_Applications.h"
#include "SDMMD_Service.h"
#include "SDMMD_Functions.h"
#include "SDMMD_AMDevice.h"

#define kAppLookupMasterKey "ReturnAttributes"

sdmmd_return_t AMDeviceLookupApplications(SDMMD_AMDeviceRef device, CFDictionaryRef options, CFDictionaryRef *results);

/*sdmmd_return_t SDM_AMDeviceLookupApplications(SDM_AMDeviceRef device, CFArrayRef options, CFDictionaryRef *results) {
	sdmmd_return_t result = SDMMD_AMDeviceConnect(device);
	if (SDM_MD_CallSuccessful(result)) {
		result = SDMMD_AMDeviceStartSession(device);
		if (SDM_MD_CallSuccessful(result)) {
			CFMutableDictionaryRef optionsDict = CFDictionaryCreateMutable(kCFAllocatorDefault, 0, NULL, NULL);
			CFDictionaryAddValue(optionsDict, CFSTR(kAppLookupMasterKey), options);
			result = AMDeviceLookupApplications(device, optionsDict, results);
			CFRelease(optionsDict);
			SDMMD_AMDeviceStopSession(device);
		}
		AMDeviceDisconnect(device);
	}
	return result;
}*/

/*sdmmd_return_t SDMMD_AMDeviceTransferApplication(SDMMD_AMConnectionRef conn, CFStringRef path, CFDictionaryRef options, void* transferCallback, void* unknown) {
	sdmmd_return_t result = 0xe8000007;
	if (path) {
		if (conn) {
			char *cpath = calloc(0x1, 0x401);
			Boolean status = CFStringGetCString(path, cpath, 0x401, 0x8000100);
			if (status) {
				CFURLRef deviceURL = SDMMD__AMDCFURLCreateFromFileSystemPathWithSmarts(path);
				if (deviceURL) {
					CFStringRef lastComp = CFURLCopyLastPathComponent(deviceURL);
					if (lastComp) {
						CFURLRef base = SDMMD__AMDCFURLCreateWithFileSystemPathRelativeToBase(kCFAllocatorDefault, CFSTR("PublicStaging"), 0x0, 0x1);
						CFURLRef copy = CFURLCreateCopyAppendingPathComponent(kCFAllocatorDefault, base, lastComp, true);
						char *copyPath;
						result = SDMMD__AMDCFURLGetCStringForFileSystemPath(copy, copyPath);
						SDMMD_fire_callback_767f4(transferCallback, unknown, 0x0, CFSTR("PreflightingTransfer"));
						SDMMD_preflight_transfer();
						SDMMD_fire_callback_767f4(transferCallback, unknown, 0x0, CFSTR("TransferingPackage"));
						result = SDMMD_AFCConnectionCreate();
						if (result == 0) {
							result = SDMMD_check_can_touch();
							if (result == 0) {
								CFURLRef parent = CFURLCreateCopyDeletingLastPathComponent(r12, r13);
								result = SDMMD_make_path(r15, parent);
							}
						}
					} else {
						result = 0xe8000001;
					}
				}
			} else {
				result = 0xe8000001;
			}
		}
	}
	return result;
}*/

sdmmd_return_t SDMMD_AMDeviceSecureInstallApplication(SDMMD_AMConnectionRef conn, uint32_t socket, CFURLRef path, CFDictionaryRef options, void* installCallback, void* unknown) {
	sdmmd_return_t result = 0x0;
	SDMMD_AMConnectionRef connection = NULL;
	bool hasConnection = (conn ? true : false);
	if (!socket) {
		result = SDMMD_AMDeviceSecureStartService(socket, CFSTR(AMSVC_INSTALLATION_PROXY), 0x0, &connection);
		if (result == 0) {
			hasConnection = true;
		} else {
			printf("SDMMD_AMDeviceSecureInstallApplication: Was unable to start the install service on the device 0x%x.\n",result);
		}
	}
	if (hasConnection) {
		CFURLRef lastComp = CFURLCopyLastPathComponent(path);
		if (lastComp) {
			CFStringRef format = CFStringCreateWithFormat(kCFAllocatorDefault, 0x0, CFSTR("%s%c%s"), "PublicStaging", 0x2f, SDMCFURLGetString(lastComp));
			if (format) {
				printf("SDMMD_AMDeviceSecureInstallApplication: Attempting install of %s.\n",SDMCFStringGetString(format));
				result = SDMMD_perform_command(connection, CFSTR("Install"), 0x0, installCallback, unknown, CFSTR("PackagePath"), format, CFSTR("ClientOptions"), options);
				if (result) {
					printf("SDMMD_AMDeviceSecureInstallApplication: Old style of install failed for (%s).\n",SDMCFStringGetString(format));
				}
				CFRelease(format);
			} else {
				printf("SDMMD_AMDeviceSecureInstallApplication: Unable to create CFString!\n");
			}
			CFRelease(lastComp);
		} else {
			printf("SDMMD_AMDeviceSecureInstallApplication: Could not copy last path component from url %s.\n",SDMCFURLGetString(lastComp));
		}
	}
	if (connection)
		CFRelease(connection);
	if (!path)
		path = CFSTR("NULL");
	printf("SDMMD_AMDeviceSecureInstallApplication: Installation of package %s returned 0x%x.\n",SDMCFURLGetString(path),result);
	return result;
}

sdmmd_return_t SDMMD_AMDeviceInstallApplication(uint32_t socket, CFStringRef path, CFDictionaryRef options, void* installCallback, void* unknown) {
	printf("SDMMD_AMDeviceInstallApplication: Entry.\n");
	sdmmd_return_t result = 0x0;
	SDMMD_AMConnectionRef conn = SDMMD__CreateTemporaryServConn(socket, 0x0);
	if (conn) {
		CFURLRef url = SDMMD__AMDCFURLCreateFromFileSystemPathWithSmarts(path);
		if (url) {
			result = SDMMD_AMDeviceSecureInstallApplication(conn, NULL, url, options, installCallback, unknown);
			CFRelease(url);
		} else {
			printf("AMDeviceInstallApplication: SDMMD_AMDCFURLCreateFromFileSystemPathWithSmarts failed on %s.\n",SDMCFURLGetString(url));
		}
		CFRelease(conn);
	} else {
		result = 0xe8000001;
	}
	return result;
}

#endif