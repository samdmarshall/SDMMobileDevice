/*
 *  MobileDevice.c
 *  SDM_MD_Demo
 *
 *  Created by sam on 5/24/13.
 *  Copyright 2013 Sam Marshall. All rights reserved.
 *
 */

#ifndef _SDMMOBILEDEVICE_C_
#define _SDMMOBILEDEVICE_C_

#include "MobileDevice.h"

SDMMobileDeviceRef InitializeSDMMobileDevice() {
	static SDMMobileDeviceRef controller = nil;
	static dispatch_once_t once;
	dispatch_once(&once, ^{
		if (!controller) {
			controller = (SDMMobileDeviceRef)malloc(sizeof(struct sdm_mobiledevice));
			controller->usbmuxd = SDMUSBMuxCreate();
			SDMUSBMuxStartListener(&controller->usbmuxd);
			controller->deviceList = CFArrayCreate(kCFAllocatorDefault, NULL, 0, &kCFTypeArrayCallBacks);
			controller->lookupTable = SDMSTLoadLibrary("/System/Library/PrivateFrameworks/MobileDevice.framework/Versions/A/MobileDevice");
		}
	});
	return controller;
}

#endif