//
//  install.c
//  iOSConsole
//
//  Created by Sam Marshall on 4/21/14.
//  Copyright (c) 2014 Sam Marshall. All rights reserved.
//

#ifndef iOSConsole_install_c
#define iOSConsole_install_c

#include "install.h"
#include "SDMMobileDevice.h"
#include <CoreFoundation/CoreFoundation.h>
#include "attach.h"
#include "Core.h"

void InstallAppToDevice(char *udid, char *path) {
	SDMMD_AMDeviceRef device = FindDeviceFromUDID(udid);
	if (device) {
		CFStringRef app_path = CFStringCreateWithFormat(kCFAllocatorDefault, NULL, CFSTR("%s"), path);
		SDMMD_AMDeviceCopyApplication(device, app_path);
		SDMMD_AMDeviceInstallApp(device, app_path);
		CFSafeRelease(app_path);
	}
}

#endif
