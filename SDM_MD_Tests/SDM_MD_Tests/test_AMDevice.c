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
	
	double percent = floor((double)(test_pass/test_total)*100.f);
	printf("Passing: %0.f/%0.f %2.f%%\n\n",test_pass,test_total,percent);
}

SDM_MD_TestResponse SDM_MD_Test_AMDeviceConnect(struct am_device *apple, SDMMD_AMDeviceRef sdm, char *type) {
	SDM_MD_TestResponse response = SDM_MD_TestResponse_Invalid;
	
	kern_return_t apple_return = AMDeviceConnect(apple);
	if (apple_return != kAMDSuccess) {
		printf("\t\tAMDeviceConnect: %08x %s\n",apple_return,SDMMD_AMDErrorString(apple_return));
	}
	AMDeviceDisconnect(apple);
	
	kern_return_t sdm_return = SDMMD_AMDeviceConnect(sdm);
	if (sdm_return != kAMDSuccess) {
		printf("\t\tSDMMD_AMDeviceConnect: %08x %s\n",sdm_return,SDMMD_AMDErrorString(sdm_return));
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
		printf("\t\tAMDeviceDisconnect: %08x %s\n",apple_return,SDMMD_AMDErrorString(apple_return));
	}
	
	SDMMD_AMDeviceConnect(sdm);
	kern_return_t sdm_return = SDMMD_AMDeviceDisconnect(sdm);
	if (sdm_return != kAMDSuccess) {
		printf("\t\tSDMMD_AMDeviceDisconnect: %08x %s\n",sdm_return,SDMMD_AMDErrorString(sdm_return));
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
		printf("\t\tAMDeviceStartSession: %08x %s\n",apple_return,SDMMD_AMDErrorString(apple_return));
	}
	AMDeviceStopSession(apple);
	AMDeviceDisconnect(apple);
	
	SDMMD_AMDeviceConnect(sdm);
	kern_return_t sdm_return = SDMMD_AMDeviceStartSession(sdm);
	if (sdm_return != kAMDSuccess) {
		printf("\t\tSDMMD_AMDeviceStartSession: %08x %s\n",sdm_return,SDMMD_AMDErrorString(sdm_return));
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
		printf("\t\tAMDeviceStopSession: %08x %s\n",apple_return,SDMMD_AMDErrorString(apple_return));
	}
	AMDeviceDisconnect(apple);
	
	SDMMD_AMDeviceConnect(sdm);
	SDMMD_AMDeviceStartSession(sdm);
	kern_return_t sdm_return = SDMMD_AMDeviceStopSession(sdm);
	if (sdm_return != kAMDSuccess) {
		printf("\t\tSDMMD_AMDeviceStopSession: %08x %s\n",sdm_return,SDMMD_AMDErrorString(sdm_return));
	}
	SDMMD_AMDeviceDisconnect(sdm);
	
	response = ((apple_return == sdm_return) ? SDM_MD_TestResponse_Success : SDM_MD_TestResponse_Failure);
	
	TEST_ASSET(type,response)
	
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
	
	TEST_ASSET(type,response)
	
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
	
	TEST_ASSET(type,response)
	
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
	
	TEST_ASSET(type,response)
	
	return response;
}

SDM_MD_TestResponse SDM_MD_Test_AMDeviceValidatePairing(struct am_device *apple, SDMMD_AMDeviceRef sdm, char *type) {
	SDM_MD_TestResponse response = SDM_MD_TestResponse_Invalid;
	
	AMDeviceConnect(apple);
	kern_return_t apple_return = AMDeviceValidatePairing(apple);
	if (apple_return != kAMDSuccess) {
		printf("\t\tAMDeviceValidatePairing: %i\n",apple_return);
	}
	AMDeviceDisconnect(apple);
	
	SDMMD_AMDeviceConnect(sdm);
	kern_return_t sdm_return = SDMMD_AMDeviceValidatePairing(sdm);
	if (sdm_return != kAMDSuccess) {
		printf("\t\tSDMMD_AMDeviceValidatePairing: %i\n",sdm_return);
	}
	SDMMD_AMDeviceDisconnect(sdm);
	
	response = ((apple_return == sdm_return) ? SDM_MD_TestResponse_Success : SDM_MD_TestResponse_Failure);
	
	TEST_ASSET(type,response)
	
	return response;
}

SDM_MD_TestResponse SDM_MD_Test_AMDeviceCopyValue(struct am_device *apple, SDMMD_AMDeviceRef sdm, char *type) {
	SDM_MD_TestResponse response = SDM_MD_TestResponse_Invalid;
	
	AMDeviceConnect(apple);
	CFTypeRef apple_return = AMDeviceCopyValue(apple, NULL, CFSTR(kUniqueDeviceID));
	if (apple_return == NULL || CFStringCompare(apple_return, CFSTR("GetProhibited"), 0) == kCFCompareEqualTo) {
		printf("\t\tAMDeviceCopyValue: GetProhibited\n");
	}
	AMDeviceDisconnect(apple);
	
	SDMMD_AMDeviceConnect(sdm);
	CFTypeRef sdm_return = SDMMD_AMDeviceCopyValue(sdm, NULL, CFSTR(kUniqueDeviceID));
	if (sdm_return == NULL || CFStringCompare(apple_return, CFSTR("GetProhibited"), 0) == kCFCompareEqualTo) {
		printf("\t\tSDMMD_AMDeviceCopyValue: GetProhibited\n");
	}
	SDMMD_AMDeviceDisconnect(sdm);
	
	response = (((apple_return != NULL && sdm_return != NULL) && (CFStringCompare(apple_return, sdm_return, 0) == kCFCompareEqualTo)) ? SDM_MD_TestResponse_Success : SDM_MD_TestResponse_Failure);
	
	TEST_ASSET(type,response)
	
	return response;
}

SDM_MD_TestResponse SDM_MD_Test_Sessioned_AMDeviceCopyValue(struct am_device *apple, SDMMD_AMDeviceRef sdm, char *type) {
	SDM_MD_TestResponse response = SDM_MD_TestResponse_Invalid;
	
	AMDeviceConnect(apple);
	AMDeviceStartSession(apple);
	CFTypeRef apple_return = AMDeviceCopyValue(apple, CFSTR(kiTunesDomain), CFSTR(kMinMacOSVersion));
	if (apple_return == NULL || CFStringCompare(apple_return, CFSTR("GetProhibited"), 0) == kCFCompareEqualTo) {
		printf("\t\tAMDeviceCopyValue (w/ Session): GetProhibited\n");
	}
	AMDeviceStopSession(apple);
	AMDeviceDisconnect(apple);
	
	SDMMD_AMDeviceConnect(sdm);
	SDMMD_AMDeviceStartSession(sdm);
	CFTypeRef sdm_return = SDMMD_AMDeviceCopyValue(sdm, CFSTR(kiTunesDomain), CFSTR(kMinMacOSVersion));
	if (sdm_return == NULL || CFStringCompare(apple_return, CFSTR("GetProhibited"), 0) == kCFCompareEqualTo) {
		printf("\t\tSDMMD_AMDeviceCopyValue (w/ Session): GetProhibited\n");
	}
	SDMMD_AMDeviceStopSession(sdm);
	SDMMD_AMDeviceDisconnect(sdm);
	
	response = ((CFStringCompare(apple_return, sdm_return, 0) == kCFCompareEqualTo) ? SDM_MD_TestResponse_Success : SDM_MD_TestResponse_Failure);
	
	TEST_ASSET(type,response)
	
	return response;
}


#endif
