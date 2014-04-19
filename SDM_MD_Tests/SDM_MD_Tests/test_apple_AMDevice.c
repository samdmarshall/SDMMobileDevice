//
//  test_apple_AMDevice.c
//  apple_MD_Tests
//
//  Created by Sam Marshall on 3/30/14.
//  Copyright (c) 2014 Sam Marshall. All rights reserved.
//

#ifndef apple_MD_Tests_test_apple_AMDevice_c
#define apple_MD_Tests_test_apple_AMDevice_c

#include <SDMMobileDevice/SDMMobileDevice.h>
#include "test_apple_AMDevice.h"

kern_return_t test_apple_AMDeviceConnect(struct am_device *apple) {
	kern_return_t apple_return = AMDeviceConnect(apple);
	if (apple_return != kAMDSuccess) {
		printf("\t\tAMDeviceConnect: %08x %s\n",apple_return,SDMMD_AMDErrorString(apple_return));
	}
	else {
		AMDeviceDisconnect(apple);
	}
	return apple_return;
}

kern_return_t test_apple_AMDeviceDisconnect(struct am_device *apple) {
	kern_return_t apple_return = kAMDUndefinedError;
	kern_return_t result = AMDeviceConnect(apple);
	if (SDM_MD_CallSuccessful(result)) {
		apple_return = AMDeviceDisconnect(apple);
		if (apple_return != kAMDSuccess) {
			printf("\t\tAMDeviceDisconnect: %08x %s\n",apple_return,SDMMD_AMDErrorString(apple_return));
		}
	}
	return apple_return;
}

kern_return_t test_apple_AMDeviceStartSession(struct am_device *apple) {
	kern_return_t apple_return = kAMDUndefinedError;
	kern_return_t result = AMDeviceConnect(apple);
	if (SDM_MD_CallSuccessful(result)) {
		apple_return = AMDeviceStartSession(apple);
		if (apple_return != kAMDSuccess) {
			printf("\t\tAMDeviceStartSession: %08x %s\n",apple_return,SDMMD_AMDErrorString(apple_return));
		}
		else {
			AMDeviceStopSession(apple);
		}
		AMDeviceDisconnect(apple);
	}
	return apple_return;
}

kern_return_t test_apple_AMDeviceStopSession(struct am_device *apple) {
	kern_return_t apple_return = kAMDUndefinedError;
	kern_return_t result = AMDeviceConnect(apple);
	if (SDM_MD_CallSuccessful(result)) {
		result = AMDeviceStartSession(apple);
		if (SDM_MD_CallSuccessful(result)) {
			apple_return = AMDeviceStopSession(apple);
			if (apple_return != kAMDSuccess) {
				printf("\t\tAMDeviceStopSession: %08x %s\n",apple_return,SDMMD_AMDErrorString(apple_return));
			}
		}
		AMDeviceDisconnect(apple);
	}
	return apple_return;
}

kern_return_t test_apple_AMDeviceUSBDeviceID(struct am_device *apple) {
	kern_return_t apple_return = kAMDUndefinedError;
	uint32_t apple_device_id = AMDeviceUSBDeviceID(apple);
	if (apple_device_id == 0) {
		printf("\t\tAMDeviceUSBDeviceID: %i\n",apple_return);
	}
	else {
		apple_return = kAMDSuccess;
	}
	return apple_return;
}

kern_return_t test_apple_AMDeviceUSBProductID(struct am_device *apple) {
	kern_return_t apple_return = kAMDUndefinedError;
	uint32_t apple_product_id = AMDeviceUSBProductID(apple);
	if (apple_product_id == 0) {
		printf("\t\tAMDeviceUSBProductID: %i\n",apple_return);
	}
	else {
		apple_return = kAMDSuccess;
	}
	return apple_return;
}

kern_return_t test_apple_AMDeviceUSBLocationID(struct am_device *apple) {
	kern_return_t apple_return = kAMDUndefinedError;
	uint32_t apple_location_id = AMDeviceUSBLocationID(apple);
	if (apple_location_id == 0) {
		printf("\t\tAMDeviceUSBLocationID: %i\n",apple_return);
	}
	else {
		apple_return = kAMDSuccess;
	}
	return apple_return;
}

kern_return_t test_apple_AMDeviceValidatePairing(struct am_device *apple) {
	kern_return_t apple_return = kAMDUndefinedError;
	kern_return_t result = AMDeviceConnect(apple);
	if (SDM_MD_CallSuccessful(result)) {
		apple_return = AMDeviceValidatePairing(apple);
		if (apple_return != kAMDSuccess) {
			printf("\t\tAMDeviceValidatePairing: %i\n",apple_return);
		}
		AMDeviceDisconnect(apple);
	}
	return apple_return;
}

kern_return_t test_apple_AMDeviceCopyValue(struct am_device *apple, CFTypeRef *value) {
	kern_return_t apple_return = kAMDUndefinedError;
	CFTypeRef apple_value = NULL;
	kern_return_t result = AMDeviceConnect(apple);
	if (SDM_MD_CallSuccessful(result)) {
		apple_value = AMDeviceCopyValue(apple, NULL, CFSTR(kUniqueDeviceID));
		if (apple_value == NULL || CFStringCompare(apple_value, CFSTR("GetProhibited"), 0) == kCFCompareEqualTo) {
			printf("\t\tAMDeviceCopyValue: GetProhibited\n");
			apple_return = kAMDGetProhibitedError;
			CFSafeRelease(apple_value);
		}
		else {
			*value = apple_value;
			apple_return = kAMDSuccess;
		}
		AMDeviceDisconnect(apple);
	}
	return apple_return;
}

kern_return_t test_apple_Sessioned_AMDeviceCopyValue(struct am_device *apple, CFTypeRef *value) {
	kern_return_t apple_return = kAMDUndefinedError;
	CFTypeRef apple_value = NULL;
	kern_return_t result = AMDeviceConnect(apple);
	if (SDM_MD_CallSuccessful(result)) {
		result = AMDeviceStartSession(apple);
		if (SDM_MD_CallSuccessful(result)) {
			apple_value = AMDeviceCopyValue(apple, CFSTR(kInternationalDomain), CFSTR(kLanguage));
			if (apple_value == NULL || CFStringCompare(apple_value, CFSTR("GetProhibited"), 0) == kCFCompareEqualTo) {
				printf("\t\tappleMD_AMDeviceCopyValue (w/ Session): GetProhibited\n");
				apple_return = kAMDGetProhibitedError;
				CFSafeRelease(apple_value);
			}
			else {
				*value = apple_value;
				apple_return = kAMDSuccess;
			}
			AMDeviceStopSession(apple);
		}
		AMDeviceDisconnect(apple);
	}
	return apple_return;
}

#endif
