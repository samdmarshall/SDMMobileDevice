//
//  springboard.h
//  iOSConsole
//
//  Created by Sam Marshall on 2/2/14.
//  Copyright (c) 2014 Sam Marshall. All rights reserved.
//

#ifndef iOSConsole_springboard_h
#define iOSConsole_springboard_h

#include <CoreFoundation/CoreFoundation.h>
#include "SDMMobileDevice.h"

enum SpringboardIconType {
	SpringboardIconTypeInvalid = 0x0,
	SpringboardIconTypeApp,
	SpringboardIconTypeFolder
};

struct SpringboardDeviceInfo {
	uint32_t dockCount;
	uint32_t screenRow;
	uint32_t screenColumn;
	uint32_t screenMaxPage;
	uint32_t folderRow;
	uint32_t folderColumn;
	uint32_t folderMaxPage;
};

void SpringboardQuery(char *udid);
struct SpringboardDeviceInfo* CreateSpringboardInfoFromDevice(SDMMD_AMDeviceRef device);
CFDictionaryRef CreateSpringboardApp(CFStringRef bundleID);
CFDictionaryRef CreateSpringboardFolder(CFStringRef name, CFArrayRef contents);

#endif
