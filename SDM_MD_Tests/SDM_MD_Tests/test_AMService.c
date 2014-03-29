//
//  test_AMService.c
//  SDM_MD_Tests
//
//  Created by Sam Marshall on 3/29/14.
//  Copyright (c) 2014 Sam Marshall. All rights reserved.
//

#ifndef SDM_MD_Tests_test_AMService_c
#define SDM_MD_Tests_test_AMService_c

#include <CoreFoundation/CoreFoundation.h>
#include "test_AMService.h"

void Test_AMService(struct am_device *apple, SDMMD_AMDeviceRef sdm) {
	LogTestName;
	uint32_t test_pass = 0;
	uint32_t test_fail = 0;
	uint32_t test_total = 0;
	
	SDM_MD_TestResponse service = SDM_MD_Test_AMDeviceStartService(apple, sdm);
	TestCount(service);
		
	printf("Passing: %i/%i %i%%\n\n",test_pass,test_total,(int)(floor(test_pass/test_total)*100.f));
}

SDM_MD_TestResponse SDM_MD_Test_AMDeviceStartService(struct am_device *apple, SDMMD_AMDeviceRef sdm) {
	SDM_MD_TestResponse response = SDM_MD_TestResponse_Invalid;
	
	AMDeviceConnect(apple);
	AMDeviceStartSession(apple);
	service_conn_t test_apple_conn;
	kern_return_t apple_return = AMDeviceStartService(apple, CFSTR(AMSVC_PURPLE_TEST), &test_apple_conn, NULL);
	if (apple_return != kAMDSuccess) {
		printf("AMDeviceStopSession: %08x %s\n",apple_return,SDMMD_AMDErrorString(apple_return));
	}
	AMDeviceStopSession(apple);
	AMDeviceDisconnect(apple);
	
	SDMMD_AMDeviceConnect(sdm);
	SDMMD_AMDeviceStartSession(sdm);
	SDMMD_AMConnectionRef test_sdm_conn;
	kern_return_t sdm_return = SDMMD_AMDeviceStartService(sdm, CFSTR(AMSVC_PURPLE_TEST), NULL, &test_sdm_conn);
	if (sdm_return != kAMDSuccess) {
		printf("SDMMD_AMDeviceStartService: %08x %s\n",sdm_return,SDMMD_AMDErrorString(sdm_return));
	}
	SDMMD_AMDeviceStopSession(sdm);
	SDMMD_AMDeviceDisconnect(sdm);
	
	response = ((apple_return == sdm_return) ? SDM_MD_TestResponse_Success : SDM_MD_TestResponse_Failure);
	
	TEST_ASSET(response)
	
	return response;
}

#endif