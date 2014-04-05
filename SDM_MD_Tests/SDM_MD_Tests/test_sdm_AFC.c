//
//  test_sdm_AFC.c
//  SDM_MD_Tests
//
//  Created by Sam Marshall on 4/5/14.
//  Copyright (c) 2014 Sam Marshall. All rights reserved.
//

#ifndef SDM_MD_Tests_test_sdm_AFC_c
#define SDM_MD_Tests_test_sdm_AFC_c

#include "test_sdm_AFC.h"

kern_return_t test_sdm_AFCConnectionCreate(SDMMD_AMDeviceRef sdm) {
	kern_return_t sdm_return = kAMDUndefinedError;
	kern_return_t result = SDMMD_AMDeviceConnect(sdm);
	if (SDM_MD_CallSuccessful(result)) {
		result = SDMMD_AMDeviceStartSession(sdm);
		if (SDM_MD_CallSuccessful(result)) {
			SDMMD_AMConnectionRef test_sdm_afc_conn;
			result = SDMMD_AMDeviceStartService(sdm, CFSTR(AMSVC_AFC), NULL, &test_sdm_afc_conn);
			if (SDM_MD_CallSuccessful(result)) {
				SDMMD_AFCConnectionRef afc = SDMMD_AFCConnectionCreate(test_sdm_afc_conn);
				if (afc) {
					sdm_return = kAMDSuccess;
					SDMMD_AMDServiceConnectionInvalidate(test_sdm_afc_conn);
					SDMMD_AFCConnectionRelease(afc);
				}
			}
			SDMMD_AMDeviceStopSession(sdm);
		}
		SDMMD_AMDeviceDisconnect(sdm);
	}
	return sdm_return;
}


kern_return_t test_sdm_AFCOperationCreateGetDeviceInfo(SDMMD_AMDeviceRef sdm) {
	kern_return_t sdm_return = kAMDUndefinedError;
	kern_return_t result = SDMMD_AMDeviceConnect(sdm);
	if (SDM_MD_CallSuccessful(result)) {
		result = SDMMD_AMDeviceStartSession(sdm);
		if (SDM_MD_CallSuccessful(result)) {
			SDMMD_AMConnectionRef test_sdm_afc_conn;
			result = SDMMD_AMDeviceStartService(sdm, CFSTR(AMSVC_AFC), NULL, &test_sdm_afc_conn);
			if (SDM_MD_CallSuccessful(result)) {
				SDMMD_AFCConnectionRef afc = SDMMD_AFCConnectionCreate(test_sdm_afc_conn);
				if (afc) {
					SDMMD_AFCOperationRef deviceInfo = SDMMD_AFCOperationCreateGetDeviceInfo();
					result = SDMMD_AFCProcessOperation(afc, &deviceInfo);
					if (SDM_MD_CallSuccessful(result)) {
						CFDataRef test = deviceInfo->packet->response_data;
						if (test) {
							sdm_return = kAMDSuccess;
						}
					}
					SDMMD_AFCConnectionRelease(afc);
				}
			}
			SDMMD_AMDeviceStopSession(sdm);
		}
		SDMMD_AMDeviceDisconnect(sdm);
	}
	return sdm_return;
}

kern_return_t test_sdm_AFCOperationCreateGetConnectionInfo(SDMMD_AMDeviceRef sdm) {
	kern_return_t sdm_return = kAMDUndefinedError;
	kern_return_t result = SDMMD_AMDeviceConnect(sdm);
	if (SDM_MD_CallSuccessful(result)) {
		result = SDMMD_AMDeviceStartSession(sdm);
		if (SDM_MD_CallSuccessful(result)) {
			SDMMD_AMConnectionRef test_sdm_afc_conn;
			result = SDMMD_AMDeviceStartService(sdm, CFSTR(AMSVC_AFC), NULL, &test_sdm_afc_conn);
			if (SDM_MD_CallSuccessful(result)) {
				SDMMD_AFCConnectionRef afc = SDMMD_AFCConnectionCreate(test_sdm_afc_conn);
				if (afc) {
					SDMMD_AFCOperationRef conn_info = SDMMD_AFCOperationCreateGetConnectionInfo();
					result = SDMMD_AFCProcessOperation(afc, &conn_info);
					if (SDM_MD_CallSuccessful(result)) {
						CFDataRef test = conn_info->packet->response_data;
						if (test) {
							sdm_return = kAMDSuccess;
						}
					}
					SDMMD_AFCConnectionRelease(afc);
				}
			}
			SDMMD_AMDeviceStopSession(sdm);
		}
		SDMMD_AMDeviceDisconnect(sdm);
	}
	return sdm_return;
}

kern_return_t test_sdm_AFCOperationCreateReadDirectory(SDMMD_AMDeviceRef sdm) {
	kern_return_t sdm_return = kAMDUndefinedError;
	kern_return_t result = SDMMD_AMDeviceConnect(sdm);
	if (SDM_MD_CallSuccessful(result)) {
		result = SDMMD_AMDeviceStartSession(sdm);
		if (SDM_MD_CallSuccessful(result)) {
			SDMMD_AMConnectionRef test_sdm_afc_conn;
			result = SDMMD_AMDeviceStartService(sdm, CFSTR(AMSVC_AFC), NULL, &test_sdm_afc_conn);
			if (SDM_MD_CallSuccessful(result)) {
				SDMMD_AFCConnectionRef afc = SDMMD_AFCConnectionCreate(test_sdm_afc_conn);
				if (afc) {
					SDMMD_AFCOperationRef read_dir = SDMMD_AFCOperationCreateReadDirectory(CFSTR("Safari"));
					result = SDMMD_AFCProcessOperation(afc, &read_dir);
					if (SDM_MD_CallSuccessful(result)) {
						CFDataRef test = read_dir->packet->response_data;
						if (test) {
							sdm_return = kAMDSuccess;
						}
					}
					SDMMD_AFCConnectionRelease(afc);
				}
			}
			SDMMD_AMDeviceStopSession(sdm);
		}
		SDMMD_AMDeviceDisconnect(sdm);
	}
	return sdm_return;
}


#endif
