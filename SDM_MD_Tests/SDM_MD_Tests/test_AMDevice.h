//
//  test_AMDevice.h
//  SDM_MD_Tests
//
//  Created by Sam Marshall on 3/29/14.
//  Copyright (c) 2014 Sam Marshall. All rights reserved.
//

#ifndef SDM_MD_Tests_test_AMDevice_h
#define SDM_MD_Tests_test_AMDevice_h

#include "test_Type.h"

void Test_AMDevice(struct am_device *apple, SDMMD_AMDeviceRef sdm);

SDM_MD_TestResponse SDM_MD_Test_AMDeviceConnect(struct am_device *apple, SDMMD_AMDeviceRef sdm, char *type);
SDM_MD_TestResponse SDM_MD_Test_AMDeviceDisconnect(struct am_device *apple, SDMMD_AMDeviceRef sdm, char *type);
SDM_MD_TestResponse SDM_MD_Test_AMDeviceStartSession(struct am_device *apple, SDMMD_AMDeviceRef sdm, char *type);
SDM_MD_TestResponse SDM_MD_Test_AMDeviceStopSession(struct am_device *apple, SDMMD_AMDeviceRef sdm, char *type);

#endif
