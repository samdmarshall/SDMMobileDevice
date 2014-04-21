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

void InstallProfileToDevice(char *udid, char *path) {
	SDMMD_AMDeviceRef device = FindDeviceFromUDID(udid);
	if (device) {
		SDMMD_AMDeviceConnect(device);
		SDMMD_AMDeviceStartSession(device);
		SDMMD_AMConnectionRef conn = NULL;
		SDMMD_AMDeviceStartService(device, CFSTR(AMSVC_MCINSTALL), NULL, &conn);
		CFDataRef config_data = CFDataCreateFromFilePath(path);
		
		CFMutableDictionaryRef request = SDMMD_create_dict();
		CFDictionaryAddValue(request, CFSTR("RequestType"), CFSTR("InstallProfile"));
		CFDictionaryAddValue(request, CFSTR("Payload"), config_data);
		
		SDMMD_ServiceSendMessage(SDMMD_TranslateConnectionToSocket(conn), request, kCFPropertyListXMLFormat_v1_0);
		CFSafeRelease(request);
		CFSafeRelease(config_data);
		
		CFPropertyListRef response = NULL;
		SDMMD_ServiceReceiveMessage(SDMMD_TranslateConnectionToSocket(conn), &response);
		CFSafeRelease(response);
		
		SDMMD_AMDServiceConnectionInvalidate(conn);
		SDMMD_AMDeviceStopSession(device);
		SDMMD_AMDeviceDisconnect(device);
		
	
	}
}

#endif
