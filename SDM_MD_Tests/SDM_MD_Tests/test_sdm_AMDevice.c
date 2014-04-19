//
//  test_sdm_AMDevice.c
//  SDM_MD_Tests
//
//  Created by Sam Marshall on 3/30/14.
//  Copyright (c) 2014 Sam Marshall. All rights reserved.
//

#ifndef SDM_MD_Tests_test_sdm_AMDevice_c
#define SDM_MD_Tests_test_sdm_AMDevice_c

#include "test_sdm_AMDevice.h"

kern_return_t test_sdm_AMDeviceConnect(SDMMD_AMDeviceRef sdm) {
	kern_return_t sdm_return = SDMMD_AMDeviceConnect(sdm);
	if (sdm_return != kAMDSuccess) {
		printf("\t\tSDMMD_AMDeviceConnect: %08x %s\n",sdm_return,SDMMD_AMDErrorString(sdm_return));
	}
	else {
		SDMMD_AMDeviceDisconnect(sdm);
	}
	return sdm_return;
}

kern_return_t test_sdm_AMDeviceDisconnect(SDMMD_AMDeviceRef sdm) {
	kern_return_t sdm_return = kAMDUndefinedError;
	kern_return_t result = SDMMD_AMDeviceConnect(sdm);
	if (SDM_MD_CallSuccessful(result)) {
		sdm_return = SDMMD_AMDeviceDisconnect(sdm);
		if (sdm_return != kAMDSuccess) {
			printf("\t\tSDMMD_AMDeviceDisconnect: %08x %s\n",sdm_return,SDMMD_AMDErrorString(sdm_return));
		}
	}
	return sdm_return;
}

kern_return_t test_sdm_AMDeviceStartSession(SDMMD_AMDeviceRef sdm) {
	kern_return_t sdm_return = kAMDUndefinedError;
	kern_return_t result = SDMMD_AMDeviceConnect(sdm);
	if (SDM_MD_CallSuccessful(result)) {
		sdm_return = SDMMD_AMDeviceStartSession(sdm);
		if (sdm_return != kAMDSuccess) {
			printf("\t\tSDMMD_AMDeviceStartSession: %08x %s\n",sdm_return,SDMMD_AMDErrorString(sdm_return));
		}
		else {
			SDMMD_AMDeviceStopSession(sdm);
		}
		SDMMD_AMDeviceDisconnect(sdm);
	}
	return sdm_return;
}

kern_return_t test_sdm_AMDeviceStopSession(SDMMD_AMDeviceRef sdm) {
	kern_return_t sdm_return = kAMDUndefinedError;
	kern_return_t result = SDMMD_AMDeviceConnect(sdm);
	if (SDM_MD_CallSuccessful(result)) {
		result = SDMMD_AMDeviceStartSession(sdm);
		if (SDM_MD_CallSuccessful(result)) {
			sdm_return = SDMMD_AMDeviceStopSession(sdm);
			if (sdm_return != kAMDSuccess) {
				printf("\t\tSDMMD_AMDeviceStopSession: %08x %s\n",sdm_return,SDMMD_AMDErrorString(sdm_return));
			}
		}
		SDMMD_AMDeviceDisconnect(sdm);
	}
	return sdm_return;
}

kern_return_t test_sdm_AMDeviceUSBDeviceID(SDMMD_AMDeviceRef sdm) {
	kern_return_t sdm_return = kAMDUndefinedError;
	uint32_t sdm_device_id = SDMMD_AMDeviceUSBDeviceID(sdm);
	if (sdm_device_id == 0) {
		printf("\t\tSDMMD_AMDeviceUSBDeviceID: %i\n",sdm_return);
	}
	else {
		sdm_return = kAMDSuccess;
	}
	return sdm_return;
}

kern_return_t test_sdm_AMDeviceUSBProductID(SDMMD_AMDeviceRef sdm) {
	kern_return_t sdm_return = kAMDUndefinedError;
	uint32_t sdm_product_id = SDMMD_AMDeviceUSBProductID(sdm);
	if (sdm_product_id == 0) {
		printf("\t\tSDMMD_AMDeviceUSBProductID: %i\n",sdm_return);
	}
	else {
		sdm_return = kAMDSuccess;
	}
	return sdm_return;
}

kern_return_t test_sdm_AMDeviceUSBLocationID(SDMMD_AMDeviceRef sdm) {
	kern_return_t sdm_return = kAMDUndefinedError;
	uint32_t sdm_location_id = SDMMD_AMDeviceUSBLocationID(sdm);
	if (sdm_location_id == 0) {
		printf("\t\tSDMMD_AMDeviceUSBLocationID: %i\n",sdm_return);
	}
	else {
		sdm_return = kAMDSuccess;
	}
	return sdm_return;
}

kern_return_t test_sdm_AMDeviceValidatePairing(SDMMD_AMDeviceRef sdm) {
	kern_return_t sdm_return = kAMDUndefinedError;
	kern_return_t result = SDMMD_AMDeviceConnect(sdm);
	if (SDM_MD_CallSuccessful(result)) {
		sdm_return = SDMMD_AMDeviceValidatePairing(sdm);
		if (sdm_return != kAMDSuccess) {
			printf("\t\tSDMMD_AMDeviceValidatePairing: %i\n",sdm_return);
		}
		SDMMD_AMDeviceDisconnect(sdm);
	}
	return sdm_return;
}

kern_return_t test_sdm_AMDeviceCopyValue(SDMMD_AMDeviceRef sdm, CFTypeRef *value) {
	kern_return_t sdm_return = kAMDUndefinedError;
	CFTypeRef sdm_value = NULL;
	kern_return_t result = SDMMD_AMDeviceConnect(sdm);
	if (SDM_MD_CallSuccessful(result)) {
		sdm_value = SDMMD_AMDeviceCopyValue(sdm, NULL, CFSTR(kUniqueDeviceID));
		if (sdm_value == NULL || CFStringCompare(sdm_value, CFSTR("GetProhibited"), 0) == kCFCompareEqualTo) {
			printf("\t\tSDMMD_AMDeviceCopyValue: GetProhibited\n");
			sdm_return = kAMDGetProhibitedError;
			CFSafeRelease(sdm_value);
		}
		else {
			*value = sdm_value;
			sdm_return = kAMDSuccess;
		}
		SDMMD_AMDeviceDisconnect(sdm);
	}
	return sdm_return;
}

kern_return_t test_sdm_Sessioned_AMDeviceCopyValue(SDMMD_AMDeviceRef sdm, CFTypeRef *value) {
	kern_return_t sdm_return = kAMDUndefinedError;
	CFTypeRef sdm_value = NULL;
	kern_return_t result = SDMMD_AMDeviceConnect(sdm);
	if (SDM_MD_CallSuccessful(result)) {
		result = SDMMD_AMDeviceStartSession(sdm);
		if (SDM_MD_CallSuccessful(result)) {
			sdm_value = SDMMD_AMDeviceCopyValue(sdm, CFSTR(kInternationalDomain), CFSTR(kLanguage));
			if (sdm_value == NULL || CFStringCompare(sdm_value, CFSTR("GetProhibited"), 0) == kCFCompareEqualTo) {
				printf("\t\tSDMMD_AMDeviceCopyValue (w/ Session): GetProhibited\n");
				sdm_return = kAMDGetProhibitedError;
				CFSafeRelease(sdm_value);
			}
			else {
				*value = sdm_value;
				sdm_return = kAMDSuccess;
			}
			SDMMD_AMDeviceStopSession(sdm);
		}
		SDMMD_AMDeviceDisconnect(sdm);
	}
	return sdm_return;
}

#endif
