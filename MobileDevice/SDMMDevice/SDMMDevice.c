/*
 *  SDMMDevice.c
 *  SDM_MD_Demo
 *
 *  Created by sam on 5/24/13.
 *  Copyright 2013 Sam Marshall. All rights reserved.
 *
 */

#ifndef _SDMMDEVICE_C_
#define _SDMMDEVICE_C_

#import "SDMMDevice.h"
#import "SDMMDeviceKeys.h"
#import <dlfcn.h>
#import "SDMMDGeneralFunctions.h"
#import "MobileDevice.h"

typedef void* (*functionCall)();

CFRuntimeClass* SDM_AMDeviceRuntimeClassCreate() {
	static CFRuntimeClass *runtimeClass = NULL;
	static dispatch_once_t once;
	dispatch_once(&once, ^{
		if (!runtimeClass) {
			functionCall dealloc, equal, description, debug;
			runtimeClass = malloc(sizeof(CFRuntimeClass));
			dealloc = SDMSTSymbolLookup(SDMMDController->lookupTable, "AMDeviceFinalize");
			equal = SDMSTSymbolLookup(SDMMDController->lookupTable, "AMDeviceEqual");
			description = SDMSTSymbolLookup(SDMMDController->lookupTable, "AMDeviceCopyFormattingDescription");
			debug = SDMSTSymbolLookup(SDMMDController->lookupTable, "AMDeviceCopyDebugDescription");
			runtimeClass = (CFRuntimeClass *){0x0, "AMDevice", NULL, NULL, dealloc, equal, NULL, description, debug, NULL};
		}
	});
	return runtimeClass;
}

SDM_AMDeviceRef SDM_AMDeviceCreateEmpty() {
	SDM_AMDeviceRef device = (SDM_AMDeviceRef)malloc(sizeof(SDM_AMDeviceClass));
	functionCall createDevice = SDMSTSymbolLookup(SDMMDController->lookupTable, "CreateEmptyDeviceRef");
	if (createDevice) {
		device = (SDM_AMDeviceRef)createDevice();			
	}	
	return device;
}

SDM_AMDeviceRef SDM_AMDeviceCreateFromProperties(CFDictionaryRef dictionary) {
	SDM_AMDeviceRef device = NULL;
	if (dictionary) {
		device = SDM_AMDeviceCreateEmpty();
		if (device) {
			CFDictionaryRef properties = (CFDictionaryContainsKey(dictionary, CFSTR("Properties")) ? CFDictionaryGetValue(dictionary, CFSTR("Properties")) : dictionary);
			
			CFNumberRef deviceId = CFDictionaryGetValue(properties, CFSTR("DeviceID"));
			CFNumberGetValue(deviceId, 0x3, &device->ivars.device_id);
			
			CFStringRef serialNumber = CFDictionaryGetValue(properties, CFSTR("SerialNumber"));
			device->ivars.unique_device_id = serialNumber;
			
			CFStringRef linkType = CFDictionaryGetValue(properties, CFSTR("ConnectionType"));
			if (CFStringCompare(linkType, CFSTR("USB"), 0) == 0) {
				device->ivars.connection_type = 0x1;
				
				CFNumberRef productId = CFDictionaryGetValue(properties, CFSTR("ProductID"));
				CFNumberGetValue(productId, 0x2, &device->ivars.product_id);
				
				CFNumberRef locationId = CFDictionaryGetValue(properties, CFSTR("LocationID"));
				CFNumberGetValue(locationId, 0x4, &device->ivars.location_id);
				
			} else if (CFStringCompare(linkType, CFSTR("WiFi"), 0) == 0) {
				device->ivars.connection_type = 0x2;
								
			} else {
				
			}
			
			device->ivars.device_active = 0x1;
			device->ivars.unknown8 = 0x0;
			device->ivars.network_address = NULL;
			kern_return_t result = sdmmd_mutex_init(device->ivars.mutex_lock);
			printf("%x\n",result);
		}
	}
	return device;
}

bool SDM_AMDeviceIsAttached(SDM_AMDeviceRef device) {
	bool result = false;
	struct USBMuxPacket *devicesPacket = SDMUSBMuxCreatePacketType(kSDMUSBMuxPacketListDevicesType, NULL);
	SDMUSBMuxListenerSend(SDMMDController->usbmuxd, devicesPacket);
	for (uint32_t i = 0x0; i < CFArrayGetCount(SDMMDController->deviceList); i++) {
		SDM_AMDeviceRef deviceCheck = (SDM_AMDeviceRef)CFArrayGetValueAtIndex(SDMMDController->deviceList, i);
		if (AMDeviceGetConnectionID(device) == AMDeviceGetConnectionID(deviceCheck)) {
			result = true;
			break;
		}
	}
	USBMuxPacketRelease(devicesPacket);
	return result;
}

CFArrayRef SDM_AMDCreateDeviceList() {
	struct USBMuxPacket *devicesPacket = SDMUSBMuxCreatePacketType(kSDMUSBMuxPacketListDevicesType, NULL);
	SDMUSBMuxListenerSend(SDMMDController->usbmuxd, devicesPacket);
	USBMuxPacketRelease(devicesPacket);
	return SDMMDController->deviceList;
}

sdmmd_return_t SDM_AMDeviceConnectByAddressAndPort(SDM_AMDeviceRef device) {
	sdmmd_return_t result = 0x0;
	functionCall deviceConnectByAddressAndPort = SDMSTSymbolLookup(SDMMDController->lookupTable, "AMDeviceConnectByAddressAndPort");
	if (deviceConnectByAddressAndPort) {
		result = (sdmmd_return_t)deviceConnectByAddressAndPort(device);
	}
	return result;
}

SDM_AMDeviceRef SDM_AMDeviceCreateCopy(SDM_AMDeviceRef device) {
	SDM_AMDeviceRef result = NULL;
	functionCall deviceCreateCopy = SDMSTSymbolLookup(SDMMDController->lookupTable, "AMDeviceCreateCopy");
	if (deviceCreateCopy) {
		result = (SDM_AMDeviceRef)deviceCreateCopy(device);
	}
	return result;
}

sdmmd_sim_return_t SDMMDSIMStatusCode(SDM_AMDeviceRef device) {
	sdmmd_sim_return_t result = KnownSIMCodes[0];
	CFStringRef deviceSIMStatus = SDMDeviceCopyValue(device, NULL, CFSTR(kSIMStatus));
	if (deviceSIMStatus) {
		for (uint32_t i = 1; i < kKnownSIMCodesNum; i++) {
			if (CFStringCompare(deviceSIMStatus, KnownSIMCodes[i].codeName, 0) == kCFCompareEqualTo) {
				result = KnownSIMCodes[i];
				break;
			}
		}
	}
	if (deviceSIMStatus)
		CFRelease(deviceSIMStatus);
	return result;
}

sdmmd_activation_return_t SDMMDActivationStatus(SDM_AMDeviceRef device) {
	sdmmd_activation_return_t result = KnownActivationStates[0];
	CFStringRef deviceActivationState = SDMDeviceCopyValue(device, NULL, CFSTR(kActivationState));
	if (deviceActivationState) {
		for (uint32_t i = 1; i < kKnownActivationStatesNum; i++) {
			if (CFStringCompare(deviceActivationState, KnownActivationStates[i].statusName, 0) == kCFCompareEqualTo) {
				result = KnownActivationStates[i];
				break;
			}
		}
	}
	if(deviceActivationState)
		CFRelease(deviceActivationState);
	return result;
}

#endif