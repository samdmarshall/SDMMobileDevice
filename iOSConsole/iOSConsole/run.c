//
//  run.c
//  iOSConsole
//
//  Created by Sam Marshall on 1/4/14.
//  Copyright (c) 2014 Sam Marshall. All rights reserved.
//

#ifndef iOSConsole_run_c
#define iOSConsole_run_c

#include "run.h"
#include "apps.h"
#include "attach.h"
#include <CoreFoundation/CoreFoundation.h>
#include "SDMMobileDevice.h"

void RunAppOnDeviceWithIdentifier(char *udid, char* identifier) {
	SDMMD_AMDeviceRef device = FindDeviceFromUDID(udid);
	if (device) {
		SDMMD_AMDebugConnectionRef connection;
		sdmmd_return_t result = SDMMD_AMDeviceConnect(device);
		SDMMD_CondSuccess(result, {
			result = SDMMD_AMDeviceStartSession(device);
			SDMMD_CondSuccess(result, {
				CFDictionaryRef response;
				CFArrayRef lookupValues = SDMMD_ApplicationLookupDictionary();
				CFMutableDictionaryRef optionsDict = SDMMD_create_dict();
				CFDictionarySetValue(optionsDict, CFSTR("ReturnAttributes"), lookupValues);
				
				result = SDMMD_AMDeviceLookupApplications(device, optionsDict, &response);
				SDMMD_CondSuccess(result, {
					CFStringRef bundleIdentifier = CFStringCreateWithCString(kCFAllocatorDefault, identifier, kCFStringEncodingUTF8);
					CFDictionaryRef details = NULL;
					if (CFDictionaryContainsKey(response, bundleIdentifier)) {
						details = CFDictionaryGetValue(response, bundleIdentifier);
					}
					CFSafeRelease(bundleIdentifier);
					if (details) {
						SDMMD_AMDeviceStopSession(device);
						SDMMD_AMDeviceDisconnect(device);
						sdmmd_return_t result = SDMMD_StartDebuggingSessionOnDevice(device, &connection);
						SDMMD_CondSuccess(result, {
							bool launchSuccess = false;
							CFStringRef path = CFDictionaryGetValue(details, CFSTR("Path"));
							CFStringRef encodedPath = SDMMD_CreateEncodeForDebuggingCommand(path);
							CFStringRef container = CFDictionaryGetValue(details, CFSTR("Container"));
							if (!container) {
								CFURLRef pathURL = CFURLCreateWithString(kCFAllocatorDefault, path, NULL);
								CFURLRef containerURL = CFURLCreateCopyDeletingLastPathComponent(kCFAllocatorDefault, pathURL);
								container = CFURLGetString(containerURL);
								CFSafeRelease(pathURL);
								//CFSafeRelease(containerURL);
							}
							if (container) {
								CFStringRef containerPath = SDMMD_CreateEncodeForDebuggingCommand(container);
								sdmmd_debug_return_t dresult;
								CFStringRef maxPacket = SDMMD_CreateEncodeForDebuggingCommand(CFSTR("1024"));
								dresult = SDMMD_DebuggingSend(connection, KnownDebugCommands[kDebugQSetMaxPacketSize], maxPacket);
								CFSafeRelease(maxPacket);
								dresult = SDMMD_DebuggingSend(connection, KnownDebugCommands[kDebugQSetWorkingDir], containerPath);
								CFSafeRelease(containerPath);
								CFStringRef commandFormat = CFStringCreateWithFormat(kCFAllocatorDefault, NULL, CFSTR("%d,0,%s"), (uint32_t)CFStringGetLength(encodedPath), CFStringGetCStringPtr(encodedPath, kCFStringEncodingUTF8));
								dresult = SDMMD_DebuggingSend(connection, KnownDebugCommands[kDebugA], commandFormat);
								CFSafeRelease(commandFormat);
								dresult = SDMMD_DebuggingSend(connection, KnownDebugCommands[kDebugH], CFSTR("c0"));
								dresult = SDMMD_DebuggingSend(connection, KnownDebugCommands[kDebugc], CFSTR(""));
								launchSuccess = true;
							}
							CFSafeRelease(encodedPath);
							if (launchSuccess) {
								CFRunLoopRun();
							}
						})
					}
				})
			})
		})
	}
}

#endif