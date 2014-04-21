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
#include "SDMMD_Debugger_Internal.h"

void SetupDeviceForDevelopment(char *udid) {
	SDMMD_AMDeviceRef device = FindDeviceFromUDID(udid);
	if (device) {
		SDMMD_AMDeviceMountDeveloperImage(device);
	}
}

void TranferAppForInstallOnDevice(char *udid, char *app_path) {
	SDMMD_AMDeviceRef device = FindDeviceFromUDID(udid);
	if (device) {
		CFStringRef path = CFStringCreateWithCharacters(kCFAllocatorDefault, (UniChar *)app_path, strlen(app_path));
		SDMMD_AMDeviceCopyApplication(device, path);
		CFSafeRelease(path);
	}
}

void InstallApplicationOnDevice(char *udid, char *app_path) {
	SDMMD_AMDeviceRef device = FindDeviceFromUDID(udid);
	if (device) {
		CFStringRef path = CFStringCreateWithCharacters(kCFAllocatorDefault, (UniChar *)app_path, strlen(app_path));
		SDMMD_AMDeviceInstallApp(device, path);
		CFSafeRelease(path);
	}
}

void StartDebuggingAndDetach(char *udid) {
	SDMMD_AMDeviceRef device = FindDeviceFromUDID(udid);
	if (device) {
		CFStringRef device_support_path = SDMMD_PathToDeviceSupport(device);
		CFStringRef symbols_path = CFStringCreateWithFormat(kCFAllocatorDefault, NULL, CFSTR("%@/Symbols"),device_support_path);
		
	}
}

#endif