//
//  test.c
//  iOSConsole
//
//  Created by Sam Marshall on 5/18/14.
//  Copyright (c) 2014 Sam Marshall. All rights reserved.
//

#ifndef iOSConsole_test_c
#define iOSConsole_test_c

#include "test.h"
#include "SDMMobileDevice.h"
#include <CoreFoundation/CoreFoundation.h>
#include "attach.h"
#include "Core.h"

void WhatDoesThisDo(char *udid) {
	SDMMD_AMDeviceRef device = FindDeviceFromUDID(udid);
	if (device) {
		sdmmd_return_t result = kAMDInvalidArgumentError;
		SDMMD_AMDeviceConnect(device);
		SDMMD_AMDeviceStartSession(device);
		CFDictionaryRef dict = NULL;
		SDMMD_AMConnectionRef conn = SDMMD_AMDServiceConnectionCreate(0, NULL, dict);
		result = SDMMD_AMDeviceSecureStartService(device, CFSTR(AMSVC_AIT_AITD), NULL, &conn);
		if (result == 0) {
			
			CFMutableDictionaryRef response;
			result = SDMMD_ServiceReceiveMessage(SDMMD_TranslateConnectionToSocket(conn), &response);
			if (SDM_MD_CallSuccessful(result)) {
				PrintCFType(response);
			}
			
			result = SDMMD_ServiceReceiveMessage(SDMMD_TranslateConnectionToSocket(conn), &response);
			if (SDM_MD_CallSuccessful(result)) {
				PrintCFType(response);
			}
			
			result = SDMMD_ServiceReceiveMessage(SDMMD_TranslateConnectionToSocket(conn), &response);
			if (SDM_MD_CallSuccessful(result)) {
				PrintCFType(response);
			}

		}
		else {
			printf("%s: Was unable to start the service on the device: %i\n",__FUNCTION__,device->ivars.device_id);
		}
	}
}


#endif