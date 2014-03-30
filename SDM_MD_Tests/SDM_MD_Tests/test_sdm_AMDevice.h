//
//  test_sdm_AMDevice.h
//  SDM_MD_Tests
//
//  Created by Sam Marshall on 3/30/14.
//  Copyright (c) 2014 Sam Marshall. All rights reserved.
//

#ifndef SDM_MD_Tests_test_sdm_AMDevice_h
#define SDM_MD_Tests_test_sdm_AMDevice_h

#include <SDMMobileDevice/SDMMobileDevice.h>

kern_return_t test_sdm_AMDeviceConnect(SDMMD_AMDeviceRef sdm);
kern_return_t test_sdm_AMDeviceDisconnect(SDMMD_AMDeviceRef sdm);
kern_return_t test_sdm_AMDeviceStartSession(SDMMD_AMDeviceRef sdm);
kern_return_t test_sdm_AMDeviceStopSession(SDMMD_AMDeviceRef sdm);
kern_return_t test_sdm_AMDeviceUSBDeviceID(SDMMD_AMDeviceRef sdm);
kern_return_t test_sdm_AMDeviceUSBProductID(SDMMD_AMDeviceRef sdm);
kern_return_t test_sdm_AMDeviceUSBLocationID(SDMMD_AMDeviceRef sdm);
kern_return_t test_sdm_AMDeviceValidatePairing(SDMMD_AMDeviceRef sdm);
kern_return_t test_sdm_AMDeviceCopyValue(SDMMD_AMDeviceRef sdm, CFTypeRef *value);
kern_return_t test_sdm_Sessioned_AMDeviceCopyValue(SDMMD_AMDeviceRef sdm, CFTypeRef *value);

#endif
