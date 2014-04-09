//
//  test_AFC.h
//  SDM_MD_Tests
//
//  Created by Sam Marshall on 4/5/14.
//  Copyright (c) 2014 Sam Marshall. All rights reserved.
//

#ifndef SDM_MD_Tests_test_AFC_h
#define SDM_MD_Tests_test_AFC_h

#include "test_Type.h"

void Test_Compatibility_AFC(struct am_device *apple, SDMMD_AMDeviceRef sdm);
void Test_Functionality_AFC(SDMMD_AMDeviceRef sdm);


SDM_MD_TestResponse SDM_MD_Test_AFCConnectionCreate(struct am_device *apple, SDMMD_AMDeviceRef sdm, char *type);
SDM_MD_TestResponse SDM_MD_Test_AFCOperationCreateGetDeviceInfo(struct am_device *apple, SDMMD_AMDeviceRef sdm, char *type);
SDM_MD_TestResponse SDM_MD_Test_AFCOperationCreateGetConnectionInfo(struct am_device *apple, SDMMD_AMDeviceRef sdm, char *type);
SDM_MD_TestResponse SDM_MD_Test_AFCOperationCreateReadDirectory(struct am_device *apple, SDMMD_AMDeviceRef sdm, char *type);
SDM_MD_TestResponse SDM_MD_Test_AFCOperationCreateFileRefOpen(struct am_device *apple, SDMMD_AMDeviceRef sdm, char *type);

#endif
