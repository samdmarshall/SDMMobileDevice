//
//  dev.c
//  iOSConsole
//
//  Created by Sam Marshall on 4/17/14.
//  Copyright (c) 2014 Sam Marshall. All rights reserved.
//

#ifndef iOSConsole_dev_c
#define iOSConsole_dev_c

#include "dev.h"
#include "attach.h"

void SetupDeviceForDevelopment(char *udid) {
	SDMMD_AMDeviceRef device = FindDeviceFromUDID(udid);
	if (device) {
		SDMMD_AMDeviceMountDeveloperImage(device);
	}
}

#endif