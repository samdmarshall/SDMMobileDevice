//
//  tests.c
//  SDM_MD_Tests
//
//  Created by Sam Marshall on 3/30/14.
//  Copyright (c) 2014 Sam Marshall. All rights reserved.
//

#ifndef SDM_MD_Tests_tests_c
#define SDM_MD_Tests_tests_c

#include "tests.h"

void RunCompatibilityTests(struct am_device *apple_test_device, SDMMD_AMDeviceRef sdm_test_device) {
	printf("Compatibility Tests:\n====================\n\n");
	
	Test_Compatibility_AMDevice(apple_test_device, sdm_test_device);
	
	Test_Compatibility_AMService(apple_test_device, sdm_test_device);
	
	
	Test_Compatibility_AFC(apple_test_device, sdm_test_device);
}

void RunFunctionalityTests(struct am_device *apple_test_device, SDMMD_AMDeviceRef sdm_test_device) {
	printf("Functionality Tests:\n====================\n\n");
	
	Test_Functionality_AMDevice(apple_test_device, sdm_test_device);
	
	Test_Functionality_AMService(apple_test_device, sdm_test_device);
	
	
	Test_Functionality_AFC(sdm_test_device);

}

#endif