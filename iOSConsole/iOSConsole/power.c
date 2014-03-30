//
//  power.c
//  iOSConsole
//
//  Created by Sam Marshall on 1/25/14.
//  Copyright (c) 2014 Sam Marshall. All rights reserved.
//

#ifndef iOSConsole_power_c
#define iOSConsole_power_c

#include "power.h"
#include "Core.h"
#include "attach.h"

void SendDeviceCommand(char *udid, CFDictionaryRef request) {
	SDMMD_AMDeviceRef device = FindDeviceFromUDID(udid);
	if (device) {
		SDMMD_AMConnectionRef powerDiag = AttachToDeviceAndService(device, AMSVC_DIAG_RELAY);
		if (request) {
			sdmmd_return_t result = SDMMD_ServiceSendMessage(SDMMD_TranslateConnectionToSocket(powerDiag), request, kCFPropertyListXMLFormat_v1_0);
			if (SDM_MD_CallSuccessful(result)) {
				CFStringRef command = CFDictionaryGetValue(request, CFSTR("Request"));
				char *commandString = SDMCFStringGetString(command);
				printf("Sent %s command to device, this could take up to 5 seconds.\n",commandString);
				CFDictionaryRef response;
				result = SDMMD_ServiceReceiveMessage(SDMMD_TranslateConnectionToSocket(powerDiag), PtrCast(&response, CFPropertyListRef*));
				if (SDM_MD_CallSuccessful(result)) {
					PrintCFDictionary(response);
				}
				Safe(free, commandString);
			}
		}
	}
}

void SendSleepToDevice(char *udid) {
	CFMutableDictionaryRef request = SDMMD__CreateRequestDict(CFSTR("Sleep"));
	SendDeviceCommand(udid, request);
}

void SendRebootToDevice(char *udid) {
	CFMutableDictionaryRef request = SDMMD__CreateRequestDict(CFSTR("Restart"));
	CFDictionarySetValue(request, CFSTR("DisplayPass"), kCFBooleanTrue);
	CFDictionarySetValue(request, CFSTR("WaitForDisconnect"), kCFBooleanFalse);
	SendDeviceCommand(udid, request);
}

void SendShutdownToDevice(char *udid) {
	CFMutableDictionaryRef request = SDMMD__CreateRequestDict(CFSTR("Shutdown"));
	CFDictionarySetValue(request, CFSTR("DisplayPass"), kCFBooleanTrue);
	CFDictionarySetValue(request, CFSTR("WaitForDisconnect"), kCFBooleanFalse);
	SendDeviceCommand(udid, request);
}

#endif