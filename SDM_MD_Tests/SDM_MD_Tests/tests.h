//
//  tests.h
//  SDM_MD_Tests
//
//  Created by Sam Marshall on 3/29/14.
//  Copyright (c) 2014 Sam Marshall. All rights reserved.
//

#ifndef SDM_MD_Tests_tests_h
#define SDM_MD_Tests_tests_h

#include "test_AMDevice.h"
#include "test_AMService.h"

#include "test_AFC.h"

void RunCompatibilityTests(struct am_device *apple_test_device, SDMMD_AMDeviceRef sdm_test_device);
void RunFunctionalityTests(struct am_device *apple_test_device, SDMMD_AMDeviceRef sdm_test_device);

#endif
