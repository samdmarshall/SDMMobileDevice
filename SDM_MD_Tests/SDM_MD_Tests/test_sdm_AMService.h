//
//  test_sdm_AMService.h
//  SDM_MD_Tests
//
//  Created by Sam Marshall on 3/30/14.
//  Copyright (c) 2014 Sam Marshall. All rights reserved.
//

#ifndef SDM_MD_Tests_test_sdm_AMService_h
#define SDM_MD_Tests_test_sdm_AMService_h

#include <SDMMobileDevice/SDMMobileDevice.h>

kern_return_t test_sdm_AMDeviceStartService(SDMMD_AMDeviceRef sdm);
kern_return_t test_sdm_AMDeviceSecureStartService(SDMMD_AMDeviceRef sdm);
kern_return_t test_sdm_AMDeviceLookupApplications(SDMMD_AMDeviceRef sdm, CFTypeRef *value);

#endif
