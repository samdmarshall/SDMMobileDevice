//
//  test_apple_AMService.h
//  SDM_MD_Tests
//
//  Created by Samantha Marshall on 3/30/14.
//  Copyright (c) 2014 Samantha Marshall. All rights reserved.
//

#ifndef SDM_MD_Tests_test_apple_AMService_h
#define SDM_MD_Tests_test_apple_AMService_h

#include <CoreFoundation/CoreFoundation.h>
#include "MobileDevice.h"

kern_return_t test_apple_AMDeviceStartService(struct am_device *apple);
kern_return_t test_apple_AMDeviceSecureStartService(struct am_device *apple);
kern_return_t test_apple_AMDeviceLookupApplications(struct am_device *apple, CFTypeRef *value);

#endif
