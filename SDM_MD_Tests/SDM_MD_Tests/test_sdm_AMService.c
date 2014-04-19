//
//  test_sdm_AMService.c
//  SDM_MD_Tests
//
//  Created by Sam Marshall on 3/30/14.
//  Copyright (c) 2014 Sam Marshall. All rights reserved.
//

#ifndef SDM_MD_Tests_test_sdm_AMService_c
#define SDM_MD_Tests_test_sdm_AMService_c

#include "test_sdm_AMService.h"

kern_return_t test_sdm_AMDeviceStartService(SDMMD_AMDeviceRef sdm) {
	kern_return_t sdm_return = kAMDUndefinedError;
	kern_return_t result = SDMMD_AMDeviceConnect(sdm);
	if (SDM_MD_CallSuccessful(result)) {
		result = SDMMD_AMDeviceStartSession(sdm);
		if (SDM_MD_CallSuccessful(result)) {
			SDMMD_AMConnectionRef test_sdm_conn;
			sdm_return = SDMMD_AMDeviceStartService(sdm, CFSTR(AMSVC_AFC), NULL, &test_sdm_conn);
			if (sdm_return != kAMDSuccess) {
				printf("\t\tSDMMD_AMDeviceStartService: %08x %s\n",sdm_return,SDMMD_AMDErrorString(sdm_return));
			}
			SDMMD_AMDeviceStopSession(sdm);
		}
		SDMMD_AMDeviceDisconnect(sdm);
	}
	return sdm_return;
}

kern_return_t test_sdm_AMDeviceSecureStartService(SDMMD_AMDeviceRef sdm) {
	kern_return_t sdm_return = kAMDUndefinedError;
	kern_return_t result = SDMMD_AMDeviceConnect(sdm);
	if (SDM_MD_CallSuccessful(result)) {
		result = SDMMD_AMDeviceStartSession(sdm);
		if (SDM_MD_CallSuccessful(result)) {
			SDMMD_AMConnectionRef test_sdm_conn;
			sdm_return = SDMMD_AMDeviceSecureStartService(sdm, CFSTR(AMSVC_MOBILE_IMAGE_MOUNT), NULL, &test_sdm_conn);
			if (sdm_return != kAMDSuccess) {
				printf("\t\tSDMMD_AMDeviceSecureStartService: %08x %s\n",sdm_return,SDMMD_AMDErrorString(sdm_return));
			}
			SDMMD_AMDeviceStopSession(sdm);
		}
		SDMMD_AMDeviceDisconnect(sdm);
	}
	return sdm_return;
}

kern_return_t test_sdm_AMDeviceLookupApplications(SDMMD_AMDeviceRef sdm, CFTypeRef *value) {
	CFDictionaryRef sdm_response = NULL;
	kern_return_t sdm_return = kAMDUndefinedError;
	kern_return_t result = SDMMD_AMDeviceConnect(sdm);
	if (SDM_MD_CallSuccessful(result)) {
		result = SDMMD_AMDeviceStartSession(sdm);
		if (SDM_MD_CallSuccessful(result)) {
			CFArrayRef lookupValues = SDMMD_ApplicationLookupDictionary();
			CFMutableDictionaryRef optionsDict = SDMMD_create_dict();
			CFDictionarySetValue(optionsDict, CFSTR("ReturnAttributes"), lookupValues);
			sdm_return = SDMMD_AMDeviceLookupApplications(sdm, optionsDict, &sdm_response);
			if (sdm_return != kAMDSuccess) {
				printf("\t\tSDMMD_AMDeviceLookupApplications: %08x %s\n",sdm_return,SDMMD_AMDErrorString(sdm_return));
			}
			else {
				*value = sdm_response;
			}
			CFSafeRelease(optionsDict);
			CFSafeRelease(lookupValues);
			SDMMD_AMDeviceStopSession(sdm);
		}
		SDMMD_AMDeviceDisconnect(sdm);
	}
	return sdm_return;
}


#endif
