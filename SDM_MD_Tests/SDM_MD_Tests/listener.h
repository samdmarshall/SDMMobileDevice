//
//  listener.h
//  SDM_MD_Tests
//
//  Created by Sam Marshall on 3/29/14.
//  Copyright (c) 2014 Sam Marshall. All rights reserved.
//

#ifndef SDM_MD_Tests_listener_h
#define SDM_MD_Tests_listener_h

#include <CoreFoundation/CoreFoundation.h>
#include <SDMMobileDevice/SDMMobileDevice.h>

SDMMD_AMDeviceRef sdm_test_device;
struct am_device *apple_test_device;

kern_return_t StartListener(dispatch_semaphore_t sema);

#endif
