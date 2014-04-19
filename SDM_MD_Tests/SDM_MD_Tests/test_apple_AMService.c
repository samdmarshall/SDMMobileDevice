//
//  test_apple_AMService.c
//  SDM_MD_Tests
//
//  Created by Sam Marshall on 3/30/14.
//  Copyright (c) 2014 Sam Marshall. All rights reserved.
//

#ifndef SDM_MD_Tests_test_apple_AMService_c
#define SDM_MD_Tests_test_apple_AMService_c

#include <SDMMobileDevice/SDMMobileDevice.h>
#include "test_apple_AMService.h"

kern_return_t test_apple_AMDeviceStartService(struct am_device *apple) {
	kern_return_t apple_return = kAMDUndefinedError;
	kern_return_t result = AMDeviceConnect(apple);
	if (SDM_MD_CallSuccessful(result)) {
		result = AMDeviceStartSession(apple);
		if (SDM_MD_CallSuccessful(result)) {
			service_conn_t test_apple_conn;
			apple_return = AMDeviceStartService(apple, CFSTR(AMSVC_AFC), &test_apple_conn, NULL);
			if (apple_return != kAMDSuccess) {
				printf("\t\tAMDeviceStartService: %08x %s\n",apple_return,SDMMD_AMDErrorString(apple_return));
			}
			AMDeviceStopSession(apple);
		}
		AMDeviceDisconnect(apple);
	}
	return apple_return;
}

kern_return_t test_apple_AMDeviceSecureStartService(struct am_device *apple) {
	kern_return_t apple_return = kAMDUndefinedError;
	kern_return_t result = AMDeviceConnect(apple);
	if (SDM_MD_CallSuccessful(result)) {
		result = AMDeviceStartSession(apple);
		if (SDM_MD_CallSuccessful(result)) {
			service_conn_t test_apple_conn;
			apple_return = AMDeviceSecureStartService(apple, CFSTR(AMSVC_MOBILE_IMAGE_MOUNT), NULL, &test_apple_conn);
			if (apple_return != kAMDSuccess) {
				printf("\t\tAMDeviceSecureStartService: %08x %s\n",apple_return,SDMMD_AMDErrorString(apple_return));
			}
			AMDeviceStopSession(apple);
		}
		AMDeviceDisconnect(apple);
	}
	return apple_return;
}

kern_return_t test_apple_AMDeviceLookupApplications(struct am_device *apple, CFTypeRef *value) {
	CFDictionaryRef apple_response = NULL;
	kern_return_t apple_return = kAMDUndefinedError;
	kern_return_t result = AMDeviceConnect(apple);
	if (SDM_MD_CallSuccessful(result)) {
		result = AMDeviceStartSession(apple);
		if (SDM_MD_CallSuccessful(result)) {
			CFArrayRef lookupValues = SDMMD_ApplicationLookupDictionary();
			CFMutableDictionaryRef optionsDict = SDMMD_create_dict();
			CFDictionarySetValue(optionsDict, CFSTR("ReturnAttributes"), lookupValues);
			apple_return = AMDeviceLookupApplications(apple, optionsDict, &apple_response);
			if (apple_return != kAMDSuccess) {
				printf("\t\tAMDeviceLookupApplications: %08x %s\n",apple_return,SDMMD_AMDErrorString(apple_return));
			}
			else {
				*value = apple_response;
			}
			CFSafeRelease(optionsDict);
			CFSafeRelease(lookupValues);
			AMDeviceStopSession(apple);
		}
		AMDeviceDisconnect(apple);
	}
	return apple_return;
}

#endif
