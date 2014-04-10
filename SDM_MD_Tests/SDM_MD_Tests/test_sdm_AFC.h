//
//  test_sdm_AFC.h
//  SDM_MD_Tests
//
//  Created by Sam Marshall on 4/5/14.
//  Copyright (c) 2014 Sam Marshall. All rights reserved.
//

#ifndef SDM_MD_Tests_test_sdm_AFC_h
#define SDM_MD_Tests_test_sdm_AFC_h

#include <SDMMobileDevice/SDMMobileDevice.h>

kern_return_t test_sdm_AFCConnectionCreate(SDMMD_AMDeviceRef sdm);
kern_return_t test_sdm_AFCOperationCreateGetDeviceInfo(SDMMD_AMDeviceRef sdm, CFTypeRef *response);
kern_return_t test_sdm_AFCOperationCreateGetConnectionInfo(SDMMD_AMDeviceRef sdm, CFTypeRef *response);
kern_return_t test_sdm_AFCOperationCreateReadDirectory(SDMMD_AMDeviceRef sdm, CFTypeRef *response);
kern_return_t test_sdm_AFCOperationCreateGetFileInfo(SDMMD_AMDeviceRef sdm, CFTypeRef *response);
kern_return_t test_sdm_AFCOperationCreateFileRefOpen(SDMMD_AMDeviceRef sdm, CFTypeRef *response);
kern_return_t test_sdm_AFCFileDescriptorCreateWriteOperation(SDMMD_AMDeviceRef sdm, CFTypeRef *response);
kern_return_t test_sdm_AFCFileDescriptorCreateReadOperation(SDMMD_AMDeviceRef sdm, CFTypeRef *response);

#endif
