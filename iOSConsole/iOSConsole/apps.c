//
//  apps.c
//  iOSConsole
//
//  Created by Sam Marshall on 1/4/14.
//  Copyright (c) 2014 Sam Marshall. All rights reserved.
//

#ifndef iOSConsole_apps_c
#define iOSConsole_apps_c

#include "apps.h"
#include "SDMMobileDevice.h"
#include <CoreFoundation/CoreFoundation.h>
#include "attach.h"
#include "Core.h"

void LookupAppsOnDevice(char *udid) {
	SDMMD_AMDeviceRef device = FindDeviceFromUDID(udid);
	if (device) {
		sdmmd_return_t result = SDMMD_AMDeviceConnect(device);
		if (SDM_MD_CallSuccessful(result)) {
			result = SDMMD_AMDeviceStartSession(device);
			if (SDM_MD_CallSuccessful(result)) {
				CFDictionaryRef response;
				
				//CFMutableArrayRef lookupValues = CFArrayCreateMutable(kCFAllocatorDefault, 0x0, &kCFTypeArrayCallBacks);
				//CFArrayAppendValue(lookupValues, CFSTR(kAppLookupKeyCFBundleIdentifier));
				//CFArrayAppendValue(lookupValues, CFSTR("SequenceNumber"));
				CFArrayRef lookupValues = SDMMD_ApplicationLookupDictionary();
				CFMutableDictionaryRef optionsDict = SDMMD_create_dict();
				CFDictionarySetValue(optionsDict, CFSTR("ReturnAttributes"), lookupValues);
				
				//CFDictionarySetValue(optionsDict, CFSTR("com.apple.mobile_installation.metadata"), kCFBooleanTrue);
				//CFDictionarySetValue(optionsDict, CFSTR("BundleIDs"), kCFBooleanTrue);
				
				result = SDMMD_AMDeviceLookupApplications(device, optionsDict, &response);
				if (SDM_MD_CallSuccessful(result)) {
					PrintCFDictionary(response);
				}
				SDMMD_AMDeviceStopSession(device);
			}
			SDMMD_AMDeviceDisconnect(device);
		}
	}
}

#endif