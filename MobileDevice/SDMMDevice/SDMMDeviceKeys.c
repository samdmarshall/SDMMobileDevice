/*
 *  SDMMDeviceKeys.c
 *  SDM_MD_Demo
 *
 *  Created by sam on 5/25/13.
 *  Copyright 2013 Sam Marshall. All rights reserved.
 *
 */

#ifndef _SDMMDEVICEKEYS_C_
#define _SDMMDEVICEKEYS_C_

#include "SDMMDeviceKeys.h"

CFTypeRef SDMDeviceCopyValue(SDM_AMDeviceRef device, CFStringRef domain, CFStringRef key) {
	CFTypeRef value = NULL;
	if (SDM_MD_CallSuccessful(AMDeviceConnect(device))) {
		AMDeviceStartSession(device);
		value = AMDeviceCopyValue(device, domain, key);
		AMDeviceStopSession(device);
		AMDeviceDisconnect(device);
	}
	return value;
}

#endif