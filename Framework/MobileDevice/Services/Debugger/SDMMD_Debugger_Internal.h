//
//  SDMMD_Debugger_Internal.h
//  SDMMobileDevice-Framework
//
//  Created by Sam Marshall on 4/21/14.
//
//

#ifndef SDMMobileDevice_Framework_SDMMD_Debugger_Internal_h
#define SDMMobileDevice_Framework_SDMMD_Debugger_Internal_h

#include <CoreFoundation/CoreFoundation.h>
#include "SDMMD_AMDevice.h"

CFStringRef SDMMD_CopyDeviceSupportPathFromXCRUN();
CFStringRef SDMMD_PathToDeviceSupport(SDMMD_AMDeviceRef device);
CFStringRef SDMMD_PathToDeviceSupportDiskImage(CFStringRef device_support);
CFDictionaryRef SDMMD_CreateImageDictionary(CFStringRef device_support_image);

#endif
