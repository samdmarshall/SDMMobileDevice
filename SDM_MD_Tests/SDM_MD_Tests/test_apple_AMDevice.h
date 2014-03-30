//
//  test_apple_AMDevice.h
//  SDM_MD_Tests
//
//  Created by Sam Marshall on 3/30/14.
//  Copyright (c) 2014 Sam Marshall. All rights reserved.
//

#ifndef SDM_MD_Tests_test_apple_AMDevice_h
#define SDM_MD_Tests_test_apple_AMDevice_h

#include <CoreFoundation/CoreFoundation.h>
#include "MobileDevice.h"

kern_return_t test_apple_AMDeviceConnect(struct am_device *apple);
kern_return_t test_apple_AMDeviceDisconnect(struct am_device *apple);
kern_return_t test_apple_AMDeviceStartSession(struct am_device *apple);
kern_return_t test_apple_AMDeviceStopSession(struct am_device *apple);
kern_return_t test_apple_AMDeviceUSBDeviceID(struct am_device *apple);
kern_return_t test_apple_AMDeviceUSBProductID(struct am_device *apple);
kern_return_t test_apple_AMDeviceUSBLocationID(struct am_device *apple);
kern_return_t test_apple_AMDeviceValidatePairing(struct am_device *apple);
kern_return_t test_apple_AMDeviceCopyValue(struct am_device *apple, CFTypeRef *value);
kern_return_t test_apple_Sessioned_AMDeviceCopyValue(struct am_device *apple, CFTypeRef *value);

#endif
