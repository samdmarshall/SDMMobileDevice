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

#include "test_apple_AMService.h"
#include "test_sdm_AMService.h"

void Test_Compatibility_AMService(struct am_device *apple, SDMMD_AMDeviceRef sdm) {
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
	
	printf("\n");
	// AMDeviceSecureStartSessionedService Tests
	SDM_MD_TestResponse lookup_apps = SDM_MD_Test_AMDeviceLookupApplications(apple, sdm, kResponse);
	TestCount(lookup_apps)
	SDM_MD_TestResponse lookup_apps_sdm = SDM_MD_Test_AMDeviceLookupApplications((struct am_device *)sdm, sdm, kSDMMD);
	TestCount(lookup_apps_sdm)
	SDM_MD_TestResponse lookup_apps_apple = SDM_MD_Test_AMDeviceLookupApplications(apple, (SDMMD_AMDeviceRef)apple, kAMD);
	TestCount(lookup_apps_apple)
		
	if (test_total) {
		double percent = floor((double)(test_pass/test_total)*100.f);
		printf("Passing: %0.f/%0.f %2.f%%\n\n",test_pass,test_total,percent);
	}
	else {
		printf("No active tests.\n");
	}
}

void Test_Functionality_AMService(struct am_device *apple, SDMMD_AMDeviceRef sdm) {
	LogTestName;
	double test_pass = 0;
	double test_fail = 0;
	double test_total = 0;
	
	// AMDeviceStartService Tests
	FunctionalityTestMacro(service,test_sdm_AMDeviceStartService,sdm)
	TestCount(service)
	
	printf("\n");
	// AMDeviceSecureStartSessionedService Tests
	FunctionalityTestMacro(secure_service,test_sdm_AMDeviceSecureStartService,sdm)
	TestCount(secure_service)
	
	printf("\n");
	// AMDeviceSecureStartSessionedService Tests
	CFTypeRef lookup_response = NULL;
	FunctionalityTestMacro(lookup_apps,test_sdm_AMDeviceLookupApplications,sdm,&lookup_response)
	TestCount(lookup_apps)
	CFSafeRelease(lookup_response);
	
	if (test_total) {
		double percent = floor((double)(test_pass/test_total)*100.f);
		printf("Passing: %0.f/%0.f %2.f%%\n\n",test_pass,test_total,percent);
	}
	else {
		printf("No active tests.\n");
	}
}

SDM_MD_TestResponse SDM_MD_Test_AMDeviceStartService(struct am_device *apple, SDMMD_AMDeviceRef sdm, char *type) {
	SDM_MD_TestResponse response = SDM_MD_TestResponse_Invalid;
	
	kern_return_t apple_return = test_apple_AMDeviceStartService(apple);
	
	kern_return_t sdm_return = test_sdm_AMDeviceStartService(sdm);
	
	response = ((SDM_MD_CallSuccessful(apple_return) && SDM_MD_CallSuccessful(sdm_return)) ? SDM_MD_TestResponse_Success : SDM_MD_TestResponse_Failure);
	
	CTEST_ASSERT(type,response)
	
	return response;
}

SDM_MD_TestResponse SDM_MD_Test_AMDeviceSecureStartService(struct am_device *apple, SDMMD_AMDeviceRef sdm, char *type) {
	SDM_MD_TestResponse response = SDM_MD_TestResponse_Invalid;
	
	kern_return_t apple_return = test_apple_AMDeviceSecureStartService(apple);
	
	kern_return_t sdm_return = test_sdm_AMDeviceSecureStartService(sdm);
	
	response = ((SDM_MD_CallSuccessful(apple_return) && SDM_MD_CallSuccessful(sdm_return)) ? SDM_MD_TestResponse_Success : SDM_MD_TestResponse_Failure);
	
	CTEST_ASSERT(type,response)
	
	return response;
}

SDM_MD_TestResponse SDM_MD_Test_AMDeviceLookupApplications(struct am_device *apple, SDMMD_AMDeviceRef sdm, char *type) {
	SDM_MD_TestResponse response = SDM_MD_TestResponse_Invalid;

	CFTypeRef apple_response = NULL;
	kern_return_t apple_return = test_apple_AMDeviceLookupApplications(apple, &apple_response);
	
	CFTypeRef sdm_response = NULL;
	kern_return_t sdm_return = test_sdm_AMDeviceLookupApplications(sdm, &sdm_response);
	
	response = (((SDM_MD_CallSuccessful(apple_return) && SDM_MD_CallSuccessful(sdm_return)) && (CFEqual(apple_response, sdm_response))) ? SDM_MD_TestResponse_Success : SDM_MD_TestResponse_Failure);
	CFSafeRelease(apple_response);
	CFSafeRelease(sdm_response);
	
	CTEST_ASSERT(type,response)
	
	return response;
}

#endif