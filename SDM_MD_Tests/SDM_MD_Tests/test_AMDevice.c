//
//  test_AMDevice.c
//  SDM_MD_Tests
//
//  Created by Sam Marshall on 3/29/14.
//  Copyright (c) 2014 Sam Marshall. All rights reserved.
//

#ifndef SDM_MD_Tests_test_AMDevice_c
#define SDM_MD_Tests_test_AMDevice_c

#include <CoreFoundation/CoreFoundation.h>
#include "test_AMDevice.h"

void Test_AMDevice(struct am_device *apple, SDMMD_AMDeviceRef sdm) {
	LogTestName;
	uint32_t test_pass = 0;
	uint32_t test_fail = 0;
	uint32_t test_total = 0;
	
	SDM_MD_TestResponse connect = SDM_MD_Test_AMDeviceConnect(apple, sdm);
	TestCount(connect)
	
	SDM_MD_TestResponse disconnect = SDM_MD_Test_AMDeviceDisconnect(apple, sdm);
	TestCount(disconnect)
	
	printf("Passing: %i/%i %i%%\n",test_pass,test_total,(int)(floor(test_pass/test_total)*100.f));
}

SDM_MD_TestResponse SDM_MD_Test_AMDeviceConnect(struct am_device *apple, SDMMD_AMDeviceRef sdm) {
	SDM_MD_TestResponse response = SDM_MD_TestResponse_Invalid;
	
	kern_return_t apple_return = AMDeviceConnect(apple);
	if (apple_return != kAMDSuccess) {
		printf("AMDeviceConnect: %08x %s\n",apple_return,SDMMD_AMDErrorString(apple_return));
	}
	AMDeviceDisconnect(apple);
	
	kern_return_t sdm_return = SDMMD_AMDeviceConnect(sdm);
	if (sdm_return != kAMDSuccess) {
		printf("SDMMD_AMDeviceConnect: %08x %s\n",sdm_return,SDMMD_AMDErrorString(sdm_return));
	}
	SDMMD_AMDeviceDisconnect(sdm);
	
	response = ((apple_return == sdm_return) ? SDM_MD_TestResponse_Success : SDM_MD_TestResponse_Failure);
	
	TEST_ASSET(response)
	
	return response;
}

SDM_MD_TestResponse SDM_MD_Test_AMDeviceDisconnect(struct am_device *apple, SDMMD_AMDeviceRef sdm) {
	SDM_MD_TestResponse response = SDM_MD_TestResponse_Invalid;
	
	AMDeviceConnect(apple);
	kern_return_t apple_return = AMDeviceDisconnect(apple);
	if (apple_return != kAMDSuccess) {
		printf("AMDeviceDisconnect: %08x %s\n",apple_return,SDMMD_AMDErrorString(apple_return));
	}
	
	SDMMD_AMDeviceConnect(sdm);
	kern_return_t sdm_return = SDMMD_AMDeviceDisconnect(sdm);
	if (sdm_return != kAMDSuccess) {
		printf("SDMMD_AMDeviceDisconnect: %08x %s\n",sdm_return,SDMMD_AMDErrorString(sdm_return));
	}
	
	response = ((apple_return == sdm_return) ? SDM_MD_TestResponse_Success : SDM_MD_TestResponse_Failure);
	
	TEST_ASSET(response)
	
	return response;
}

#endif
