//
//  attach.c
//  iOSConsole
//
//  Created by Sam Marshall on 1/4/14.
//  Copyright (c) 2014 Sam Marshall. All rights reserved.
//

#ifndef iOSConsole_attach_c
#define iOSConsole_attach_c

#include "attach.h"
#include "Core.h"

SDMMD_AMDeviceRef FindDeviceFromUDID(char *udid) {
	CFArrayRef devices = SDMMD_AMDCreateDeviceList();
	CFIndex numberOfDevices = CFArrayGetCount(devices);
	SDMMD_AMDeviceRef device = NULL;
	if (numberOfDevices) {
		// return type (uint32_t) corresponds with known return codes (SDMMD_Error.h)
		bool foundDevice = false;
		char *deviceId;
		uint32_t index;
		// Iterating over connected devices
		for (index = 0; index < numberOfDevices; index++) {
			SDMMD_AMDeviceRef device = (SDMMD_AMDeviceRef)CFArrayGetValueAtIndex(devices, index);
			CFTypeRef deviceUDID = device->ivars.unique_device_id;
			if (deviceUDID) {
				deviceId = (char*)CFStringGetCStringPtr(deviceUDID,kCFStringEncodingMacRoman);
				if (strncmp(udid, deviceId, strlen(deviceId)) == 0x0) {
					foundDevice = true;
					break;
				}
				CFSafeRelease(deviceUDID);
			}
		}
		if (foundDevice) {
			device = (SDMMD_AMDeviceRef)CFArrayGetValueAtIndex(devices, index);
		}
	} else {
		printf("No devices connected.\n");
	}
	return device;
}

SDMMD_AMConnectionRef AttachToDeviceAndService(SDMMD_AMDeviceRef device, char *service) {
	SDMMD_AMConnectionRef serviceCon = NULL;
	if (device) {
		sdmmd_return_t result = SDMMD_AMDeviceConnect(device);
		SDMMD_CondSuccess(result, {
			result = SDMMD_AMDeviceStartSession(device);
			SDMMD_CondSuccess(result, {
				CFStringRef serviceString = CFStringCreateWithCString(kCFAllocatorDefault, service, kCFStringEncodingMacRoman);
				result = SDMMD_AMDeviceStartService(device, serviceString, NULL, &serviceCon);
				SDMMD_CondSuccessElse(result, {
					CFTypeRef deviceName = SDMMD_AMDeviceCopyValue(device, NULL, CFSTR(kDeviceName));
					char *name = (char*)CFStringGetCStringPtr(deviceName,kCFStringEncodingMacRoman);
					if (!name) {
						name = "unnamed device";
					}
					printf("Connected to %s on \"%s\" ...\n",name,service);
					CFSafeRelease(deviceName);
				}, {
					SDMMD_AMDeviceStopSession(device);
					SDMMD_AMDeviceDisconnect(device);
					serviceCon = NULL;
				})
			})
		})
	} else {
		printf("Could not find device with that UDID\n");
	}
	return serviceCon;
}

#endif
