//
//  test_AMService.h
//  SDM_MD_Tests
//
//  Created by Sam Marshall on 3/29/14.
//  Copyright (c) 2014 Sam Marshall. All rights reserved.
//

#ifndef SDM_MD_Tests_test_AMService_h
#define SDM_MD_Tests_test_AMService_h

#include "test_Type.h"

void Test_Compatibility_AMService(struct am_device *apple, SDMMD_AMDeviceRef sdm);
void Test_Functionality_AMService(struct am_device *apple, SDMMD_AMDeviceRef sdm);

SDM_MD_TestResponse SDM_MD_Test_AMDeviceStartService(struct am_device *apple, SDMMD_AMDeviceRef sdm, char *type);
SDM_MD_TestResponse SDM_MD_Test_AMDeviceSecureStartService(struct am_device *apple, SDMMD_AMDeviceRef sdm, char *type);
SDM_MD_TestResponse SDM_MD_Test_AMDeviceLookupApplications(struct am_device *apple, SDMMD_AMDeviceRef sdm, char *type);

#endif
