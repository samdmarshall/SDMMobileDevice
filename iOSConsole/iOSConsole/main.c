//
//  main.c
//  iOSConsole
//
//  Created by Sam Marshall on 1/1/14.
//  Copyright (c) 2014 Sam Marshall. All rights reserved.
//

#include <CoreFoundation/CoreFoundation.h>
#include "SDMMobileDevice.h"

static char *listArg = "--list";
static char *attachArg = "--attach";

#define SysLogBufferSize 0x80

void ListConnectedDevices() {
	CFArrayRef devices = SDMMD_AMDCreateDeviceList();
	CFIndex numberOfDevices = CFArrayGetCount(devices);
	
	if (numberOfDevices) {
		// return type (uint32_t) corresponds with known return codes (SDMMD_Error.h)
		sdmmd_return_t result;
		
		uint32_t index;
		// Iterating over connected devices
		for (index = 0; index < numberOfDevices; index++) {
			
			// getting the device object from the array of connected devices
			SDMMD_AMDeviceRef device = (SDMMD_AMDeviceRef)CFArrayGetValueAtIndex(devices, index);
			
			// attempting to connect to the device
			result = SDMMD_AMDeviceConnect(device);
			if (SDM_MD_CallSuccessful(result)) {
				CFTypeRef deviceName = SDMMD_AMDeviceCopyValue(device, NULL, CFSTR(kDeviceName));
				CFTypeRef deviceUDID = SDMMD_AMDeviceCopyValue(device, NULL, CFSTR(kUniqueDeviceID));
				printf("%i) %s : %s\n",index+0x1,CFStringGetCStringPtr(deviceName,kCFStringEncodingMacRoman),CFStringGetCStringPtr(deviceUDID,kCFStringEncodingMacRoman));
				CFRelease(deviceName);
				CFRelease(deviceUDID);
				SDMMD_AMDeviceDisconnect(device);
			}
		}
	}
	CFRelease(devices);
}

void AttachToDevice(char *udid) {
	CFArrayRef devices = SDMMD_AMDCreateDeviceList();
	CFIndex numberOfDevices = CFArrayGetCount(devices);
	
	if (numberOfDevices) {
		// return type (uint32_t) corresponds with known return codes (SDMMD_Error.h)
		sdmmd_return_t result;
		char *deviceId;
		uint32_t index;
		// Iterating over connected devices
		for (index = 0; index < numberOfDevices; index++) {
			
			// getting the device object from the array of connected devices
			SDMMD_AMDeviceRef device = (SDMMD_AMDeviceRef)CFArrayGetValueAtIndex(devices, index);
			
			// attempting to connect to the device
			result = SDMMD_AMDeviceConnect(device);
			if (SDM_MD_CallSuccessful(result)) {
				CFTypeRef deviceUDID = SDMMD_AMDeviceCopyValue(device, NULL, CFSTR(kUniqueDeviceID));
				deviceId = (char*)CFStringGetCStringPtr(deviceUDID,kCFStringEncodingMacRoman);
				CFRelease(deviceUDID);
				if (strncmp(udid, deviceId, strlen(deviceId)) == 0x0) {
					break;
				}
				SDMMD_AMDeviceDisconnect(device);
			}
		}
		SDMMD_AMConnectionRef syslog;
		SDMMD_AMDeviceRef device = (SDMMD_AMDeviceRef)CFArrayGetValueAtIndex(devices, index);
		result = SDMMD_AMDeviceStartSession(device);
		if (SDM_MD_CallSuccessful(result)) {
			result = SDMMD_AMDeviceStartService(device, CFSTR(AMSVC_SYSLOG_RELAY), NULL, &syslog);
			if (SDM_MD_CallSuccessful(result)) {
				CFTypeRef deviceName = SDMMD_AMDeviceCopyValue(device, NULL, CFSTR(kDeviceName));
				char *name = (char*)CFStringGetCStringPtr(deviceName,kCFStringEncodingMacRoman);
				if (!name) {
					name = "device";
				}
				printf("Connected to %s, loading syslog...\n",name);
				CFRelease(deviceName);
			}
			while (SDM_MD_CallSuccessful(result)) {
				unsigned char syslogRelayBuffer[SysLogBufferSize];
				CFDataRef syslogData = CFDataCreate(kCFAllocatorDefault, syslogRelayBuffer, SysLogBufferSize);
				result = SDMMD_DirectServiceReceive(SDMMD_TranslateConnectionToSocket(syslog), (CFDataRef*)&syslogData);
				fwrite(CFDataGetBytePtr(syslogData), sizeof(char), SysLogBufferSize, stdout);
				CFRelease(syslogData);
			}
		}
	}
	CFRelease(devices);
}

int main(int argc, const char * argv[]) {
	SDMMobileDevice;
	bool attachDevice = false;
	bool listDevice = false;
	switch (argc) {
		case 0x1: {
			printf("%s : list attached devices\n",listArg);
			printf("%s [UDID] : attach to the syslog of requested device\n",attachArg);
			break;
		}
		case 0x2:
		case 0x3: {
			if (strncmp(argv[0x1], listArg, strlen(listArg)) == 0x0) {
				listDevice = true;
			}
			if (strncmp(argv[0x1], attachArg, strlen(attachArg)) == 0x0 && argc == 0x3) {
				attachDevice = true;
			}
			break;
		}
		default:
			break;
	}
	if (listDevice) {
		ListConnectedDevices();
	}
	if (attachDevice) {
		AttachToDevice((char*)argv[0x2]);
		printf("\n\nLost Connection with Device\n");
	}
    return 0;
}

