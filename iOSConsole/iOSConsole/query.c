//
//  query.c
//  iOSConsole
//
//  Created by Sam Marshall on 1/4/14.
//  Copyright (c) 2014 Sam Marshall. All rights reserved.
//

#ifndef iOSConsole_query_c
#define iOSConsole_query_c

#include "query.h"
#include "attach.h"

void PerformQuery(char *udid, char *domain, char *key) {
	if ((udid && strlen(udid) == 0x28) && key) {
		SDMMD_AMDeviceRef device = FindDeviceFromUDID(udid);
		sdmmd_return_t result = SDMMD_AMDeviceConnect(device);
		if (SDM_MD_CallSuccessful(result)) {
			CFStringRef domainString = NULL;
			if (strncmp(domain, "null", strlen("null")) != 0x0) {
				domainString = CFStringCreateWithCString(kCFAllocatorDefault, domain, kCFStringEncodingUTF8);
			}
			CFStringRef keyString = CFStringCreateWithCString(kCFAllocatorDefault, key, kCFStringEncodingUTF8);
			
			result = SDMMD_AMDeviceStartSession(device);
			if (SDM_MD_CallSuccessful(result)) {
				CFTypeRef queryResult = SDMMD_AMDeviceCopyValue(device, domainString, keyString);
				CFShow(queryResult);
				result = SDMMD_AMDeviceStopSession(device);
			}
			
			CFRelease(keyString);
			if (domainString) {
				CFRelease(domainString);
			}
			SDMMD_AMDeviceDisconnect(device);
		}
	}
}

#endif