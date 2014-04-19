//
//  test_AFC.c
//  SDM_MD_Tests
//
//  Created by Sam Marshall on 4/5/14.
//  Copyright (c) 2014 Sam Marshall. All rights reserved.
//

#ifndef SDM_MD_Tests_test_AFC_c
#define SDM_MD_Tests_test_AFC_c

#include "test_AFC.h"

#include "test_apple_AFC.h"
#include "test_sdm_AFC.h"

void Test_Compatibility_AFC(struct am_device *apple, SDMMD_AMDeviceRef sdm) {
	LogTestName;
	double test_pass = 0;
	double test_fail = 0;
	double test_total = 0;
	
	SDM_MD_TestResponse afc_conn = SDM_MD_Test_AFCConnectionCreate(apple, sdm, kResponse);
	TestCount(afc_conn)
	
	printf("\n");
	SDM_MD_TestResponse afc_device_info = SDM_MD_Test_AFCOperationCreateGetDeviceInfo(apple, sdm, kResponse);
	TestCount(afc_device_info)

	printf("\n");
	SDM_MD_TestResponse afc_conn_info = SDM_MD_Test_AFCOperationCreateGetConnectionInfo(apple, sdm, kResponse);
	TestCount(afc_conn_info)
	
	printf("\n");
	SDM_MD_TestResponse afc_read_dir = SDM_MD_Test_AFCOperationCreateReadDirectory(apple, sdm, kResponse);
	TestCount(afc_read_dir)
	
	if (test_total) {
		double percent = floor((double)(test_pass/test_total)*100.f);
		printf("Passing: %0.f/%0.f %2.f%%\n\n",test_pass,test_total,percent);
	}
	else {
		printf("No active tests.\n");
	}
}

void Test_Functionality_AFC(SDMMD_AMDeviceRef sdm) {
	LogTestName;
	double test_pass = 0;
	double test_fail = 0;
	double test_total = 0;
		
	// AFCConnectionCreate Tests
	FunctionalityTestMacro(afc_conn,test_sdm_AFCConnectionCreate,sdm)
	TestCount(afc_conn)
	
	printf("\n");
	// AFCOperationCreateGetDeviceInfo Tests
	CFTypeRef afc_device_info_response = NULL;
	FunctionalityTestMacro(afc_device_info,test_sdm_AFCOperationCreateGetDeviceInfo,sdm,&afc_device_info_response)
	TestCount(afc_device_info)
	CFSafeRelease(afc_device_info_response);

	printf("\n");
	// AFCOperationCreateGetConnectionInfo Tests
	CFTypeRef afc_conn_info_response = NULL;
	FunctionalityTestMacro(afc_conn_info,test_sdm_AFCOperationCreateGetConnectionInfo,sdm,&afc_conn_info_response)
	TestCount(afc_conn_info)
	CFSafeRelease(afc_conn_info_response);
	
	printf("\n");
	// AFCOperationCreateReadDirectory Tests
	CFTypeRef afc_read_dir_response = NULL;
	FunctionalityTestMacro(afc_read_dir,test_sdm_AFCOperationCreateReadDirectory,sdm,&afc_read_dir_response)
	TestCount(afc_read_dir)
	CFSafeRelease(afc_read_dir_response);
	
	printf("\n");
	// AFCOperationCreateGetFileInfo Tests
	CFTypeRef afc_file_info_response = NULL;
	FunctionalityTestMacro(afc_file_info,test_sdm_AFCOperationCreateGetFileInfo,sdm,&afc_file_info_response)
	TestCount(afc_file_info)
	CFSafeRelease(afc_file_info_response);
	
	printf("\n");
	// AFCOperationCreateFileRefOpen Tests
	CFTypeRef afc_file_ref_open_response = NULL;
	FunctionalityTestMacro(afc_file_ref_open,test_sdm_AFCOperationCreateFileRefOpen,sdm,&afc_file_ref_open_response)
	TestCount(afc_file_ref_open)
	CFSafeRelease(afc_file_ref_open_response);
	
	printf("\n");
	// AFCOperationCreateWriteOperation Tests
	CFTypeRef afc_file_write_response = NULL;
	FunctionalityTestMacro(afc_file_write,test_sdm_AFCFileDescriptorCreateWriteOperation,sdm,&afc_file_write_response)
	TestCount(afc_file_write)
	CFSafeRelease(afc_file_write_response);
	
	printf("\n");
	// AFCOperationCreateReadOperation Tests
	CFTypeRef afc_file_read_response = NULL;
	FunctionalityTestMacro(afc_file_read,test_sdm_AFCFileDescriptorCreateReadOperation,sdm,&afc_file_read_response)
	TestCount(afc_file_read)
	CFSafeRelease(afc_file_read_response);
	
	if (test_total) {
		double percent = floor((double)(test_pass/test_total)*100.f);
		printf("Passing: %0.f/%0.f %2.f%%\n\n",test_pass,test_total,percent);
	}
	else {
		printf("No active tests.\n");
	}
}

SDM_MD_TestResponse SDM_MD_Test_AFCConnectionCreate(struct am_device *apple, SDMMD_AMDeviceRef sdm, char *type) {
	SDM_MD_TestResponse response = SDM_MD_TestResponse_Invalid;
	
	kern_return_t apple_return = test_apple_AFCConnectionCreate(apple);
	
	kern_return_t sdm_return = test_sdm_AFCConnectionCreate(sdm);
	
	response = ((SDM_MD_CallSuccessful(apple_return) && SDM_MD_CallSuccessful(sdm_return)) ? SDM_MD_TestResponse_Success : SDM_MD_TestResponse_Failure);
	
	CTEST_ASSERT(type,response)
	
	return response;
}

// this test will sometimes fail due to differences in FSFreeBytes value.
SDM_MD_TestResponse SDM_MD_Test_AFCOperationCreateGetDeviceInfo(struct am_device *apple, SDMMD_AMDeviceRef sdm, char *type) {
	SDM_MD_TestResponse response = SDM_MD_TestResponse_Invalid;
	
	CFTypeRef apple_response = NULL;
	kern_return_t apple_return = test_apple_AFCOperationCreateGetDeviceInfo(apple, &apple_response);
	
	CFTypeRef sdm_response = NULL;
	kern_return_t sdm_return = test_sdm_AFCOperationCreateGetDeviceInfo(sdm, &sdm_response);
	
	response = ((SDM_MD_CallSuccessful(apple_return) && SDM_MD_CallSuccessful(sdm_return)) && CFEqual(apple_response, sdm_response) ? SDM_MD_TestResponse_Success : SDM_MD_TestResponse_Failure);
	
	CFSafeRelease(apple_response);
	CFSafeRelease(sdm_response);
	
	CTEST_ASSERT(type,response)
	
	return response;
}

SDM_MD_TestResponse SDM_MD_Test_AFCOperationCreateGetConnectionInfo(struct am_device *apple, SDMMD_AMDeviceRef sdm, char *type) {
	SDM_MD_TestResponse response = SDM_MD_TestResponse_Invalid;
	
	CFTypeRef apple_response = NULL;
	kern_return_t apple_return = test_apple_AFCOperationCreateGetConnectionInfo(apple, &apple_response);
	
	CFTypeRef sdm_response = NULL;
	kern_return_t sdm_return = test_sdm_AFCOperationCreateGetConnectionInfo(sdm, &sdm_response);
	
	response = ((SDM_MD_CallSuccessful(apple_return) && SDM_MD_CallSuccessful(sdm_return)) && CFEqual(apple_response, sdm_response) ? SDM_MD_TestResponse_Success : SDM_MD_TestResponse_Failure);
	
	CFSafeRelease(apple_response);
	CFSafeRelease(sdm_response);
	
	CTEST_ASSERT(type,response)
	
	return response;
}

SDM_MD_TestResponse SDM_MD_Test_AFCOperationCreateReadDirectory(struct am_device *apple, SDMMD_AMDeviceRef sdm, char *type) {
	SDM_MD_TestResponse response = SDM_MD_TestResponse_Invalid;
	
	CFTypeRef apple_response = NULL;
	kern_return_t apple_return = test_apple_AFCOperationCreateReadDirectory(apple, &apple_response);
	
	CFTypeRef sdm_response = NULL;
	kern_return_t sdm_return = test_sdm_AFCOperationCreateReadDirectory(sdm, &sdm_response);
	
	response = ((SDM_MD_CallSuccessful(apple_return) && SDM_MD_CallSuccessful(sdm_return)) && CFEqual(apple_response, sdm_response) ? SDM_MD_TestResponse_Success : SDM_MD_TestResponse_Failure);
	
	CFSafeRelease(apple_response);
	CFSafeRelease(sdm_response);
	
	CTEST_ASSERT(type,response)
	
	return response;
}

SDM_MD_TestResponse SDM_MD_Test_AFCOperationCreateGetFileInfo(struct am_device *apple, SDMMD_AMDeviceRef sdm, char *type) {
	SDM_MD_TestResponse response = SDM_MD_TestResponse_Invalid;
	
	CFTypeRef apple_response = NULL;
	kern_return_t apple_return = kAMDSuccess; //test_apple_AFCOperationCreateReadDirectory(apple, &apple_response);
	
	CFTypeRef sdm_response = NULL;
	kern_return_t sdm_return = test_sdm_AFCOperationCreateGetFileInfo(sdm, &sdm_response);
	
	response = ((SDM_MD_CallSuccessful(apple_return) && SDM_MD_CallSuccessful(sdm_return)) && CFEqual(apple_response, sdm_response) ? SDM_MD_TestResponse_Success : SDM_MD_TestResponse_Failure);
	
	CFSafeRelease(apple_response);
	CFSafeRelease(sdm_response);
	
	CTEST_ASSERT(type,response)
	
	return response;
}

SDM_MD_TestResponse SDM_MD_Test_AFCOperationCreateFileRefOpen(struct am_device *apple, SDMMD_AMDeviceRef sdm, char *type) {
	SDM_MD_TestResponse response = SDM_MD_TestResponse_Invalid;
	
	CFTypeRef apple_response = NULL;
	kern_return_t apple_return = kAMDSuccess; //test_apple_AFCOperationCreateReadDirectory(apple, &apple_response);
	
	CFTypeRef sdm_response = NULL;
	kern_return_t sdm_return = test_sdm_AFCOperationCreateFileRefOpen(sdm, &sdm_response);
	
	response = ((SDM_MD_CallSuccessful(apple_return) && SDM_MD_CallSuccessful(sdm_return)) && CFEqual(apple_response, sdm_response) ? SDM_MD_TestResponse_Success : SDM_MD_TestResponse_Failure);
	
	CFSafeRelease(apple_response);
	CFSafeRelease(sdm_response);
	
	CTEST_ASSERT(type,response)
	
	return response;
}

#endif