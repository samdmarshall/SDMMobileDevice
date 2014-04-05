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

#include "test_apple_AMDevice.h"
#include "test_sdm_AMDevice.h"

void Test_Compatibility_AMDevice(struct am_device *apple, SDMMD_AMDeviceRef sdm) {
	LogTestName;
	double test_pass = 0;
	double test_fail = 0;
	double test_total = 0;
	
	// AMDeviceUSBDeviceID Tests
	SDM_MD_TestResponse usb_id = SDM_MD_Test_AMDeviceUSBDeviceID(apple, sdm, kResponse);
	TestCount(usb_id)
	SDM_MD_TestResponse usb_id_sdm = SDM_MD_Test_AMDeviceUSBDeviceID((struct am_device *)sdm, sdm, kSDMMD);
	TestCount(usb_id_sdm)
	SDM_MD_TestResponse usb_id_apple = SDM_MD_Test_AMDeviceUSBDeviceID(apple, (SDMMD_AMDeviceRef)apple, kAMD);
	TestCount(usb_id_apple)
	
	printf("\n");
	// AMDeviceUSBProductID Tests
	SDM_MD_TestResponse product_id = SDM_MD_Test_AMDeviceUSBProductID(apple, sdm, kResponse);
	TestCount(product_id)
	SDM_MD_TestResponse product_id_sdm = SDM_MD_Test_AMDeviceUSBProductID((struct am_device *)sdm, sdm, kSDMMD);
	TestCount(product_id_sdm)
	SDM_MD_TestResponse product_id_apple = SDM_MD_Test_AMDeviceUSBProductID(apple, (SDMMD_AMDeviceRef)apple, kAMD);
	TestCount(product_id_apple)
	
	printf("\n");
	// AMDeviceUSBLocationID Tests
	SDM_MD_TestResponse location_id = SDM_MD_Test_AMDeviceUSBLocationID(apple, sdm, kResponse);
	TestCount(location_id)
	SDM_MD_TestResponse location_id_sdm = SDM_MD_Test_AMDeviceUSBLocationID((struct am_device *)sdm, sdm, kSDMMD);
	TestCount(location_id_sdm)
	SDM_MD_TestResponse location_id_apple = SDM_MD_Test_AMDeviceUSBLocationID(apple, (SDMMD_AMDeviceRef)apple, kAMD);
	TestCount(location_id_apple)
	
	printf("\n");
	// AMDeviceConnect Tests
	SDM_MD_TestResponse connect = SDM_MD_Test_AMDeviceConnect(apple, sdm, kResponse);
	TestCount(connect)
	SDM_MD_TestResponse connect_sdm = SDM_MD_Test_AMDeviceConnect((struct am_device *)sdm, sdm, kSDMMD);
	TestCount(connect_sdm)
	SDM_MD_TestResponse connect_apple = SDM_MD_Test_AMDeviceConnect(apple, (SDMMD_AMDeviceRef)apple, kAMD);
	TestCount(connect_apple)
	
	printf("\n");
	// AMDeviceDisconnect Tests
	SDM_MD_TestResponse disconnect = SDM_MD_Test_AMDeviceDisconnect(apple, sdm, kResponse);
	TestCount(disconnect)
	SDM_MD_TestResponse disconnect_sdm = SDM_MD_Test_AMDeviceDisconnect((struct am_device *)sdm, sdm, kSDMMD);
	TestCount(disconnect_sdm)
	SDM_MD_TestResponse disconnect_apple = SDM_MD_Test_AMDeviceDisconnect(apple, (SDMMD_AMDeviceRef)apple, kAMD);
	TestCount(disconnect_apple)
	
	printf("\n");
	// AMDeviceStartSession Tests
	SDM_MD_TestResponse start_session = SDM_MD_Test_AMDeviceStartSession(apple, sdm, kResponse);
	TestCount(start_session)
	SDM_MD_TestResponse start_session_sdm = SDM_MD_Test_AMDeviceStartSession((struct am_device *)sdm, sdm, kSDMMD);
	TestCount(start_session_sdm)
	SDM_MD_TestResponse start_session_apple = SDM_MD_Test_AMDeviceStartSession(apple, (SDMMD_AMDeviceRef)apple, kAMD);
	TestCount(start_session_apple)
	
	printf("\n");
	// AMDeviceStopSession Tests
	SDM_MD_TestResponse stop_session = SDM_MD_Test_AMDeviceStopSession(apple, sdm, kResponse);
	TestCount(stop_session)
	SDM_MD_TestResponse stop_session_sdm = SDM_MD_Test_AMDeviceStopSession((struct am_device *)sdm, sdm, kSDMMD);
	TestCount(stop_session_sdm)
	SDM_MD_TestResponse stop_session_apple = SDM_MD_Test_AMDeviceStopSession(apple, (SDMMD_AMDeviceRef)apple, kAMD);
	TestCount(stop_session_apple)
	
	printf("\n");
	// AMDeviceValidatePairing Tests
	SDM_MD_TestResponse validate_pair = SDM_MD_Test_AMDeviceValidatePairing(apple, sdm, kResponse);
	TestCount(validate_pair)
	SDM_MD_TestResponse validate_pair_sdm = SDM_MD_Test_AMDeviceValidatePairing((struct am_device *)sdm, sdm, kSDMMD);
	TestCount(validate_pair_sdm)
	SDM_MD_TestResponse validate_pair_apple = SDM_MD_Test_AMDeviceValidatePairing(apple, (SDMMD_AMDeviceRef)apple, kAMD);
	TestCount(validate_pair_apple)
	
	printf("\n");
	// AMDeviceCopyValue Tests
	SDM_MD_TestResponse copy_value = SDM_MD_Test_AMDeviceCopyValue(apple, sdm, kResponse);
	TestCount(copy_value)
	SDM_MD_TestResponse copy_value_sdm = SDM_MD_Test_AMDeviceCopyValue((struct am_device *)sdm, sdm, kSDMMD);
	TestCount(copy_value_sdm)
	SDM_MD_TestResponse copy_value_apple = SDM_MD_Test_AMDeviceCopyValue(apple, (SDMMD_AMDeviceRef)apple, kAMD);
	TestCount(copy_value_apple)
	
	printf("\n");
	// AMDeviceCopyValue Tests (w/ Session)
	SDM_MD_TestResponse session_copy_value = SDM_MD_Test_Sessioned_AMDeviceCopyValue(apple, sdm, kResponse);
	TestCount(session_copy_value)
	SDM_MD_TestResponse session_copy_value_sdm = SDM_MD_Test_Sessioned_AMDeviceCopyValue((struct am_device *)sdm, sdm, kSDMMD);
	TestCount(session_copy_value_sdm)
	SDM_MD_TestResponse session_copy_value_apple = SDM_MD_Test_Sessioned_AMDeviceCopyValue(apple, (SDMMD_AMDeviceRef)apple, kAMD);
	TestCount(session_copy_value_apple)
	
	if (test_total) {
		double percent = floor((double)(test_pass/test_total)*100.f);
		printf("Passing: %0.f/%0.f %2.f%%\n\n",test_pass,test_total,percent);
	}
	else {
		printf("No active tests.\n");
	}
}

void Test_Functionality_AMDevice(struct am_device *apple, SDMMD_AMDeviceRef sdm) {
	LogTestName;
	double test_pass = 0;
	double test_fail = 0;
	double test_total = 0;
	
	// AMDeviceUSBDeviceID Tests
	FunctionalityTestMacro(usb_id,test_sdm_AMDeviceUSBDeviceID,sdm)
	TestCount(usb_id)
	
	printf("\n");
	// AMDeviceUSBProductID Tests
	FunctionalityTestMacro(product_id,test_sdm_AMDeviceUSBProductID,sdm)
	TestCount(product_id)
	
	printf("\n");
	// AMDeviceUSBLocationID Tests
	FunctionalityTestMacro(location_id,test_sdm_AMDeviceUSBLocationID,sdm)
	TestCount(location_id)
	
	printf("\n");
	// AMDeviceConnect Tests
	FunctionalityTestMacro(connect,test_sdm_AMDeviceConnect,sdm)
	TestCount(connect)
	
	printf("\n");
	// AMDeviceDisconnect Tests
	FunctionalityTestMacro(disconnect,test_sdm_AMDeviceDisconnect,sdm)
	TestCount(disconnect)
	
	printf("\n");
	// AMDeviceStartSession Tests
	FunctionalityTestMacro(start_session,test_sdm_AMDeviceStartSession,sdm)
	TestCount(start_session)
	
	printf("\n");
	// AMDeviceStopSession Tests
	FunctionalityTestMacro(stop_session,test_sdm_AMDeviceStopSession,sdm)
	TestCount(stop_session)
	
	printf("\n");
	// AMDeviceValidatePairing Tests
	FunctionalityTestMacro(validate_pair,test_sdm_AMDeviceValidatePairing,sdm)
	TestCount(validate_pair)
	
	printf("\n");
	// AMDeviceCopyValue Tests
	CFTypeRef sdm_unsession_copy_value = NULL;
	FunctionalityTestMacro(copy_value,test_sdm_AMDeviceCopyValue,sdm, &sdm_unsession_copy_value)
	TestCount(copy_value)
	CFSafeRelease(sdm_unsession_copy_value);
	
	printf("\n");
	// AMDeviceCopyValue Tests (w/ Session)
	CFTypeRef sdm_session_copy_value = NULL;
	FunctionalityTestMacro(session_copy_value,test_sdm_Sessioned_AMDeviceCopyValue,sdm, &sdm_session_copy_value)
	TestCount(session_copy_value)
	CFSafeRelease(sdm_session_copy_value);
	
	if (test_total) {
		double percent = floor((double)(test_pass/test_total)*100.f);
		printf("Passing: %0.f/%0.f %2.f%%\n\n",test_pass,test_total,percent);
	}
	else {
		printf("No active tests.\n");
	}
}

SDM_MD_TestResponse SDM_MD_Test_AMDeviceConnect(struct am_device *apple, SDMMD_AMDeviceRef sdm, char *type) {
	SDM_MD_TestResponse response = SDM_MD_TestResponse_Invalid;
	
	kern_return_t apple_return = test_apple_AMDeviceConnect(apple);
	
	kern_return_t sdm_return = test_sdm_AMDeviceConnect(sdm);
	
	response = ((SDM_MD_CallSuccessful(apple_return) && SDM_MD_CallSuccessful(sdm_return)) ? SDM_MD_TestResponse_Success : SDM_MD_TestResponse_Failure);
	
	CTEST_ASSERT(type,response)
	
	return response;
}

SDM_MD_TestResponse SDM_MD_Test_AMDeviceDisconnect(struct am_device *apple, SDMMD_AMDeviceRef sdm, char *type) {
	SDM_MD_TestResponse response = SDM_MD_TestResponse_Invalid;
	
	kern_return_t apple_return = test_apple_AMDeviceDisconnect(apple);
	
	kern_return_t sdm_return = test_sdm_AMDeviceDisconnect(sdm);
	
	response = ((SDM_MD_CallSuccessful(apple_return) && SDM_MD_CallSuccessful(sdm_return)) ? SDM_MD_TestResponse_Success : SDM_MD_TestResponse_Failure);
	
	CTEST_ASSERT(type,response)
	
	return response;
}


SDM_MD_TestResponse SDM_MD_Test_AMDeviceStartSession(struct am_device *apple, SDMMD_AMDeviceRef sdm, char *type) {
	SDM_MD_TestResponse response = SDM_MD_TestResponse_Invalid;
	
	kern_return_t apple_return = test_apple_AMDeviceStartSession(apple);
	
	kern_return_t sdm_return = test_sdm_AMDeviceStartSession(sdm);
	
	response = ((SDM_MD_CallSuccessful(apple_return) && SDM_MD_CallSuccessful(sdm_return)) ? SDM_MD_TestResponse_Success : SDM_MD_TestResponse_Failure);
	
	CTEST_ASSERT(type,response)
	
	return response;
}

SDM_MD_TestResponse SDM_MD_Test_AMDeviceStopSession(struct am_device *apple, SDMMD_AMDeviceRef sdm, char *type) {
	SDM_MD_TestResponse response = SDM_MD_TestResponse_Invalid;
	
	kern_return_t apple_return = test_apple_AMDeviceStopSession(apple);
	
	kern_return_t sdm_return = test_sdm_AMDeviceStopSession(sdm);
	
	response = ((SDM_MD_CallSuccessful(apple_return) && SDM_MD_CallSuccessful(sdm_return)) ? SDM_MD_TestResponse_Success : SDM_MD_TestResponse_Failure);
	
	CTEST_ASSERT(type,response)
	
	return response;
}

SDM_MD_TestResponse SDM_MD_Test_AMDeviceUSBDeviceID(struct am_device *apple, SDMMD_AMDeviceRef sdm, char *type) {
	SDM_MD_TestResponse response = SDM_MD_TestResponse_Invalid;
	
	uint32_t apple_return = AMDeviceUSBDeviceID(apple);
	if (apple_return == 0) {
		printf("\t\tAMDeviceUSBDeviceID: %i\n",apple_return);
	}
	
	uint32_t sdm_return = SDMMD_AMDeviceUSBDeviceID(sdm);
	if (sdm_return == 0) {
		printf("\t\tSDMMD_AMDeviceUSBDeviceID: %i\n",sdm_return);
	}
	
	response = ((apple_return == sdm_return) ? SDM_MD_TestResponse_Success : SDM_MD_TestResponse_Failure);
	
	CTEST_ASSERT(type,response)
	
	return response;
}

SDM_MD_TestResponse SDM_MD_Test_AMDeviceUSBProductID(struct am_device *apple, SDMMD_AMDeviceRef sdm, char *type) {
	SDM_MD_TestResponse response = SDM_MD_TestResponse_Invalid;
	
	uint32_t apple_return = AMDeviceUSBProductID(apple);
	if (apple_return == 0) {
		printf("\t\tAMDeviceUSBProductID: %i\n",apple_return);
	}
	
	uint32_t sdm_return = SDMMD_AMDeviceUSBProductID(sdm);
	if (sdm_return == 0) {
		printf("\t\tSDMMD_AMDeviceUSBProductID: %i\n",sdm_return);
	}
	
	response = ((apple_return == sdm_return) ? SDM_MD_TestResponse_Success : SDM_MD_TestResponse_Failure);
	
	CTEST_ASSERT(type,response)
	
	return response;
}

SDM_MD_TestResponse SDM_MD_Test_AMDeviceUSBLocationID(struct am_device *apple, SDMMD_AMDeviceRef sdm, char *type) {
	SDM_MD_TestResponse response = SDM_MD_TestResponse_Invalid;
	
	uint32_t apple_return = AMDeviceUSBLocationID(apple);
	if (apple_return == 0) {
		printf("\t\tAMDeviceUSBLocationID: %i\n",apple_return);
	}
	
	uint32_t sdm_return = SDMMD_AMDeviceUSBLocationID(sdm);
	if (sdm_return == 0) {
		printf("\t\tSDMMD_AMDeviceUSBLocationID: %i\n",sdm_return);
	}
	
	response = ((apple_return == sdm_return) ? SDM_MD_TestResponse_Success : SDM_MD_TestResponse_Failure);
	
	CTEST_ASSERT(type,response)
	
	return response;
}

SDM_MD_TestResponse SDM_MD_Test_AMDeviceValidatePairing(struct am_device *apple, SDMMD_AMDeviceRef sdm, char *type) {
	SDM_MD_TestResponse response = SDM_MD_TestResponse_Invalid;

	kern_return_t apple_return = test_apple_AMDeviceValidatePairing(apple);
	
	kern_return_t sdm_return = test_sdm_AMDeviceValidatePairing(sdm);

	response = ((SDM_MD_CallSuccessful(apple_return) && SDM_MD_CallSuccessful(sdm_return)) ? SDM_MD_TestResponse_Success : SDM_MD_TestResponse_Failure);
	
	CTEST_ASSERT(type,response)
	
	return response;
}

SDM_MD_TestResponse SDM_MD_Test_AMDeviceCopyValue(struct am_device *apple, SDMMD_AMDeviceRef sdm, char *type) {
	SDM_MD_TestResponse response = SDM_MD_TestResponse_Invalid;
	
	CFTypeRef apple_value = NULL;
	kern_return_t apple_return = test_apple_AMDeviceCopyValue(apple, &apple_value);
	
	CFTypeRef sdm_value = NULL;
	kern_return_t sdm_return = test_sdm_AMDeviceCopyValue(sdm, &sdm_value);
	
	response = (((SDM_MD_CallSuccessful(apple_return) && SDM_MD_CallSuccessful(sdm_return)) && (apple_value != NULL && sdm_value != NULL) && (CFStringCompare(apple_value, sdm_value, 0) == kCFCompareEqualTo)) ? SDM_MD_TestResponse_Success : SDM_MD_TestResponse_Failure);
	
	CTEST_ASSERT(type,response)
	
	return response;
}

SDM_MD_TestResponse SDM_MD_Test_Sessioned_AMDeviceCopyValue(struct am_device *apple, SDMMD_AMDeviceRef sdm, char *type) {
	SDM_MD_TestResponse response = SDM_MD_TestResponse_Invalid;
	
	CFTypeRef apple_value = NULL;
	kern_return_t apple_return = test_apple_Sessioned_AMDeviceCopyValue(apple, &apple_value);
	
	CFTypeRef sdm_value = NULL;
	kern_return_t sdm_return = test_sdm_Sessioned_AMDeviceCopyValue(sdm, &sdm_value);
	
	response = (((SDM_MD_CallSuccessful(apple_return) && SDM_MD_CallSuccessful(sdm_return)) && (apple_value != NULL && sdm_value != NULL) && (CFStringCompare(apple_value, sdm_value, 0) == kCFCompareEqualTo)) ? SDM_MD_TestResponse_Success : SDM_MD_TestResponse_Failure);
	
	CTEST_ASSERT(type,response)
	
	return response;
}


#endif
