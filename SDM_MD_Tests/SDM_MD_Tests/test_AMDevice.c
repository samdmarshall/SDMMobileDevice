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
	double test_pass = 0;
	double test_fail = 0;
	double test_total = 0;
	
	// AMDeviceConnect Tests
	SDM_MD_TestResponse connect = SDM_MD_Test_AMDeviceConnect(apple, sdm, kResponse);
	TestCount(connect)
	SDM_MD_TestResponse connect_sdm = SDM_MD_Test_AMDeviceConnect((struct am_device *)sdm, sdm, kSDMMD);
	TestCount(connect_sdm)
	SDM_MD_TestResponse connect_apple = SDM_MD_Test_AMDeviceConnect(apple, (SDMMD_AMDeviceRef)apple, kAMD);
	TestCount(connect_apple)
	
	// AMDeviceDisconnect Tests
	SDM_MD_TestResponse disconnect = SDM_MD_Test_AMDeviceDisconnect(apple, sdm, kResponse);
	TestCount(disconnect)
	SDM_MD_TestResponse disconnect_sdm = SDM_MD_Test_AMDeviceDisconnect((struct am_device *)sdm, sdm, kSDMMD);
	TestCount(disconnect_sdm)
	SDM_MD_TestResponse disconnect_apple = SDM_MD_Test_AMDeviceDisconnect(apple, (SDMMD_AMDeviceRef)apple, kAMD);
	TestCount(disconnect_apple)
	
	// AMDeviceStartSession Tests
	SDM_MD_TestResponse start_session = SDM_MD_Test_AMDeviceStartSession(apple, sdm, kResponse);
	TestCount(start_session)
	SDM_MD_TestResponse start_session_sdm = SDM_MD_Test_AMDeviceStartSession((struct am_device *)sdm, sdm, kSDMMD);
	TestCount(start_session_sdm)
	SDM_MD_TestResponse start_session_apple = SDM_MD_Test_AMDeviceStartSession(apple, (SDMMD_AMDeviceRef)apple, kAMD);
	TestCount(start_session_apple)
	
	// AMDeviceStopSession Tests
	SDM_MD_TestResponse stop_session = SDM_MD_Test_AMDeviceStopSession(apple, sdm, kResponse);
	TestCount(stop_session)
	SDM_MD_TestResponse stop_session_sdm = SDM_MD_Test_AMDeviceStopSession((struct am_device *)sdm, sdm, kSDMMD);
	TestCount(stop_session_sdm)
	SDM_MD_TestResponse stop_session_apple = SDM_MD_Test_AMDeviceStopSession(apple, (SDMMD_AMDeviceRef)apple, kAMD);
	TestCount(stop_session_apple)
	
	double percent = floor((double)(test_pass/test_total)*100.f);
	printf("Passing: %0.f/%0.f %2.f%%\n\n",test_pass,test_total,percent);
}

SDM_MD_TestResponse SDM_MD_Test_AMDeviceConnect(struct am_device *apple, SDMMD_AMDeviceRef sdm, char *type) {
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
	
	TEST_ASSET(type,response)
	
	return response;
}

SDM_MD_TestResponse SDM_MD_Test_AMDeviceDisconnect(struct am_device *apple, SDMMD_AMDeviceRef sdm, char *type) {
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
	
	TEST_ASSET(type,response)
	
	return response;
}


SDM_MD_TestResponse SDM_MD_Test_AMDeviceStartSession(struct am_device *apple, SDMMD_AMDeviceRef sdm, char *type) {
	SDM_MD_TestResponse response = SDM_MD_TestResponse_Invalid;
	
	AMDeviceConnect(apple);
	kern_return_t apple_return = AMDeviceStartSession(apple);
	if (apple_return != kAMDSuccess) {
		printf("AMDeviceStartSession: %08x %s\n",apple_return,SDMMD_AMDErrorString(apple_return));
	}
	AMDeviceStopSession(apple);
	AMDeviceDisconnect(apple);
	
	SDMMD_AMDeviceConnect(sdm);
	kern_return_t sdm_return = SDMMD_AMDeviceStartSession(sdm);
	if (sdm_return != kAMDSuccess) {
		printf("SDMMD_AMDeviceStartSession: %08x %s\n",sdm_return,SDMMD_AMDErrorString(sdm_return));
	}
	SDMMD_AMDeviceStopSession(sdm);
	SDMMD_AMDeviceDisconnect(sdm);
	
	response = ((apple_return == sdm_return) ? SDM_MD_TestResponse_Success : SDM_MD_TestResponse_Failure);
	
	TEST_ASSET(type,response)
	
	return response;
}

SDM_MD_TestResponse SDM_MD_Test_AMDeviceStopSession(struct am_device *apple, SDMMD_AMDeviceRef sdm, char *type) {
	SDM_MD_TestResponse response = SDM_MD_TestResponse_Invalid;
	
	AMDeviceConnect(apple);
	AMDeviceStartSession(apple);
	kern_return_t apple_return = AMDeviceStopSession(apple);
	if (apple_return != kAMDSuccess) {
		printf("AMDeviceStopSession: %08x %s\n",apple_return,SDMMD_AMDErrorString(apple_return));
	}
	AMDeviceDisconnect(apple);
	
	SDMMD_AMDeviceConnect(sdm);
	SDMMD_AMDeviceStartSession(sdm);
	kern_return_t sdm_return = SDMMD_AMDeviceStopSession(sdm);
	if (sdm_return != kAMDSuccess) {
		printf("SDMMD_AMDeviceStopSession: %08x %s\n",sdm_return,SDMMD_AMDErrorString(sdm_return));
	}
	SDMMD_AMDeviceDisconnect(sdm);
	
	response = ((apple_return == sdm_return) ? SDM_MD_TestResponse_Success : SDM_MD_TestResponse_Failure);
	
	TEST_ASSET(type,response)
	
	return response;
}

#endif
