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
#include "Core.h"
#include "SDMMobileDevice.h"

void RunQueryOnDevice(SDMMD_AMDeviceRef device, char *domain, char *key, sdmmd_return_t result) {
	if (SDM_MD_CallSuccessful(result)) {
		CFStringRef domainString = NULL;
		CFStringRef keyString = NULL;
		if (strncmp(domain, "null", strlen("null")) != 0x0) {
			domainString = CFStringCreateWithCString(kCFAllocatorDefault, domain, kCFStringEncodingUTF8);
		}
		keyString = CFStringCreateWithCString(kCFAllocatorDefault, key, kCFStringEncodingUTF8);
		
		printf("[%s][%s]: ",domain,key);
		CFTypeRef queryResult = SDMMD_AMDeviceCopyValue(device, domainString, keyString);
		if (queryResult) {
			PrintCFType(queryResult);
		}
		CFSafeRelease(queryResult);

		CFSafeRelease(keyString);
		CFSafeRelease(domainString);
	}
}

void PerformQuery(char *udid, char *domain, char *key) {
	if ((udid && strlen(udid) == 0x28) && key) {
		SDMMD_AMDeviceRef device = FindDeviceFromUDID(udid);
		if (device) {
			sdmmd_return_t result = SDMMD_AMDeviceConnect(device);
			if (SDM_MD_CallSuccessful(result)) {
				result = SDMMD_AMDeviceStartSession(device);
				if (SDM_MD_CallSuccessful(result)) {
					if (strncmp(domain, kAllDomains, strlen(kAllDomains)) == 0x0 && strncmp(key, kAllKeys, strlen(kAllKeys)) == 0x0) {
						for (uint32_t i = 0x0; i < SDM_MD_Domain_Count; i++) {
							printf("%s\n",SDMMDKnownDomain[i].domain);
							for (uint32_t j = 0x0; j < SDMMDKnownDomain[i].keyCount; j++) {
								RunQueryOnDevice(device, SDMMDKnownDomain[i].domain, SDMMDKnownDomain[i].keys[j], result);
							}
							printf("\n");
						}
					} else {
						RunQueryOnDevice(device, domain, key,result);
					}
					SDMMD_AMDeviceStopSession(device);
				}
				SDMMD_AMDeviceDisconnect(device);
			}
		}
	}
}

#endif