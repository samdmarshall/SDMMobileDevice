//
//  list.c
//  iOSConsole
//
//  Created by Sam Marshall on 1/4/14.
//  Copyright (c) 2014 Sam Marshall. All rights reserved.
//

#ifndef iOSConsole_list_c
#define iOSConsole_list_c

#include <CoreFoundation/CoreFoundation.h>

#include "SDMMobileDevice.h"
#include "Core.h"

#include "list.h"

void ListConnectedDevices() {
	CFArrayRef devices = SDMMD_AMDCreateDeviceList();
	CFIndex numberOfDevices = CFArrayGetCount(devices);
	LogLine(PrintCode_Norm,"Currently connected devices: (%ld)",numberOfDevices);
	if (numberOfDevices) {
		// return type (uint32_t) corresponds with known return codes (SDMMD_Error.h)
		sdmmd_return_t result;
		
		uint32_t index;
		// Iterating over connected devices
		for (index = 0; index < numberOfDevices; index++) {
			
			// getting the device object from the array of connected devices
			SDMMD_AMDeviceRef device = (SDMMD_AMDeviceRef)CFArrayGetValueAtIndex(devices, index);
			bool validDevice = SDMMD_AMDeviceIsValid(device);
			if (validDevice) {
				// attempting to connect to the device
				result = SDMMD_AMDeviceConnect(device);
				SDMMD_CondSuccess(result, {
					printf("%d) ",index+0x1);
					CFTypeRef deviceName = SDMMD_AMDeviceCopyValue(device, NULL, CFSTR(kDeviceName));
					CFTypeRef deviceUDID = SDMMD_AMDeviceCopyValue(device, NULL, CFSTR(kUniqueDeviceID));
					printf("%s : %s",CFStringGetCStringPtr(deviceName,kCFStringEncodingMacRoman),CFStringGetCStringPtr(deviceUDID,kCFStringEncodingMacRoman));
					if (deviceName) {
						CFRelease(deviceName);
					}
					
					if (deviceUDID) {
						CFRelease(deviceUDID);
					}
					
					SDMMD_AMDeviceDisconnect(device);
					printf("\n");
				})
			}
		}
	}
	CFRelease(devices);
}

#endif
