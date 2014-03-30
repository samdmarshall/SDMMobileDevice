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
	double test_pass = 0;
	double test_fail = 0;
	double test_total = 0;
	
	// AMDeviceStartService Tests
	SDM_MD_TestResponse service = SDM_MD_Test_AMDeviceStartService(apple, sdm, kResponse);
	TestCount(service)
	SDM_MD_TestResponse service_sdm = SDM_MD_Test_AMDeviceStartService((struct am_device *)sdm, sdm, kSDMMD);
	TestCount(service_sdm)
	SDM_MD_TestResponse service_apple = SDM_MD_Test_AMDeviceStartService(apple, (SDMMD_AMDeviceRef)apple, kAMD);
	TestCount(service_apple)
	
	printf("\n");
	// AMDeviceSecureStartSessionedService Tests
	SDM_MD_TestResponse secure_service = SDM_MD_Test_AMDeviceSecureStartService(apple, sdm, kResponse);
	TestCount(secure_service)
	SDM_MD_TestResponse secure_service_sdm = SDM_MD_Test_AMDeviceSecureStartService((struct am_device *)sdm, sdm, kSDMMD);
	TestCount(secure_service_sdm)
	SDM_MD_TestResponse secure_service_apple = SDM_MD_Test_AMDeviceSecureStartService(apple, (SDMMD_AMDeviceRef)apple, kAMD);
	TestCount(secure_service_apple)
		
	double percent = floor((double)(test_pass/test_total)*100.f);
	printf("Passing: %0.f/%0.f %2.f%%\n\n",test_pass,test_total,percent);
}

SDM_MD_TestResponse SDM_MD_Test_AMDeviceStartService(struct am_device *apple, SDMMD_AMDeviceRef sdm, char *type) {
	SDM_MD_TestResponse response = SDM_MD_TestResponse_Invalid;
	
	AMDeviceConnect(apple);
	AMDeviceStartSession(apple);
	service_conn_t test_apple_conn;
	kern_return_t apple_return = AMDeviceStartService(apple, CFSTR(AMSVC_PURPLE_TEST), &test_apple_conn, NULL);
	if (apple_return != kAMDSuccess) {
		printf("\t\tAMDeviceStartService: %08x %s\n",apple_return,SDMMD_AMDErrorString(apple_return));
	}
	AMDeviceStopSession(apple);
	AMDeviceDisconnect(apple);
	
	SDMMD_AMDeviceConnect(sdm);
	SDMMD_AMDeviceStartSession(sdm);
	SDMMD_AMConnectionRef test_sdm_conn;
	kern_return_t sdm_return = SDMMD_AMDeviceStartService(sdm, CFSTR(AMSVC_PURPLE_TEST), NULL, &test_sdm_conn);
	if (sdm_return != kAMDSuccess) {
		printf("\t\tSDMMD_AMDeviceStartService: %08x %s\n",sdm_return,SDMMD_AMDErrorString(sdm_return));
	}
	SDMMD_AMDeviceStopSession(sdm);
	SDMMD_AMDeviceDisconnect(sdm);
	
	response = ((apple_return == sdm_return) ? SDM_MD_TestResponse_Success : SDM_MD_TestResponse_Failure);
	
	TEST_ASSET(type,response)
	
	return response;
}

SDM_MD_TestResponse SDM_MD_Test_AMDeviceSecureStartService(struct am_device *apple, SDMMD_AMDeviceRef sdm, char *type) {
	SDM_MD_TestResponse response = SDM_MD_TestResponse_Invalid;
	
	AMDeviceConnect(apple);
	AMDeviceStartSession(apple);
	service_conn_t test_apple_conn;
	kern_return_t apple_return = AMDeviceSecureStartService(apple, CFSTR(AMSVC_DEBUG_IMAGE_MOUNT), NULL, &test_apple_conn);
	if (apple_return != kAMDSuccess) {
		printf("\t\tAMDeviceSecureStartService: %08x %s\n",apple_return,SDMMD_AMDErrorString(apple_return));
	}
	AMDeviceStopSession(apple);
	AMDeviceDisconnect(apple);
	
	SDMMD_AMDeviceConnect(sdm);
	SDMMD_AMDeviceStartSession(sdm);
	SDMMD_AMConnectionRef test_sdm_conn;
	kern_return_t sdm_return = SDMMD_AMDeviceSecureStartService(sdm, CFSTR(AMSVC_DEBUG_IMAGE_MOUNT), NULL, &test_sdm_conn);
	if (sdm_return != kAMDSuccess) {
		printf("\t\tSDMMD_AMDeviceSecureStartService: %08x %s\n",sdm_return,SDMMD_AMDErrorString(sdm_return));
	}
	SDMMD_AMDeviceStopSession(sdm);
	SDMMD_AMDeviceDisconnect(sdm);
	
	response = ((apple_return == sdm_return) ? SDM_MD_TestResponse_Success : SDM_MD_TestResponse_Failure);
	
	TEST_ASSET(type,response)
	
	return response;}

#endif