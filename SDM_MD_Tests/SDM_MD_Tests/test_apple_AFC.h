//
//  test_apple_AFC.h
//  SDM_MD_Tests
//
//  Created by Sam Marshall on 4/5/14.
//  Copyright (c) 2014 Sam Marshall. All rights reserved.
//

#ifndef SDM_MD_Tests_test_apple_AFC_h
#define SDM_MD_Tests_test_apple_AFC_h

#include "MobileDevice.h"

kern_return_t test_apple_AFCConnectionCreate(struct am_device *apple);
kern_return_t test_apple_AFCOperationCreateGetDeviceInfo(struct am_device *apple, CFTypeRef *response);
kern_return_t test_apple_AFCOperationCreateGetConnectionInfo(struct am_device *apple, CFTypeRef *response);
kern_return_t test_apple_AFCOperationCreateReadDirectory(struct am_device *apple, CFTypeRef *response);

#endif
