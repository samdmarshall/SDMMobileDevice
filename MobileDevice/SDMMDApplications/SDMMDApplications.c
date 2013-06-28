/*
 *  SDMMDApplications.c
 *  SDM_MD_Demo
 *
 *  Created by sam on 6/6/13.
 *  Copyright 2013 Sam Marshall. All rights reserved.
 *
 */

#ifndef _SDMMDAPPLICATIONS_C_
#define _SDMMDAPPLICATIONS_C_

#include "SDMMDApplications.h"

#define kAppLookupMasterKey								"ReturnAttributes"

sdmmd_return_t AMDeviceLookupApplications(SDM_AMDeviceRef device, CFDictionaryRef options, CFDictionaryRef *results);

sdmmd_return_t SDM_AMDeviceLookupApplications(SDM_AMDeviceRef device, CFArrayRef options, CFDictionaryRef *results) {
	sdmmd_return_t result = AMDeviceConnect(device);
	if (SDM_MD_CallSuccessful(result)) {
		result = AMDeviceStartSession(device);
		if (SDM_MD_CallSuccessful(result)) {
			CFMutableDictionaryRef optionsDict = CFDictionaryCreateMutable(kCFAllocatorDefault, 0, NULL, NULL);
			CFDictionaryAddValue(optionsDict, CFSTR(kAppLookupMasterKey), options);
			result = AMDeviceLookupApplications(device, optionsDict, results);
			CFRelease(optionsDict);
			AMDeviceStopSession(device);
		}
		AMDeviceDisconnect(device);
	}
	return result;
}

#endif