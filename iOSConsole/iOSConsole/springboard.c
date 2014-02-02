//
//  springboard.c
//  iOSConsole
//
//  Created by Sam Marshall on 2/2/14.
//  Copyright (c) 2014 Sam Marshall. All rights reserved.
//

#ifndef iOSConsole_springboard_c
#define iOSConsole_springboard_c

#include "springboard.h"
#include "attach.h"

#include <CoreFoundation/CoreFoundation.h>

#include "SDMMobileDevice.h"
#include "Core.h"


enum SpringboardIconType ResolveSpringboardItemType(CFDictionaryRef item);
bool PageContainsBundleID(CFArrayRef page, CFStringRef bundleID, CFDictionaryRef *appInfo);
bool FolderContainsBundleID(CFDictionaryRef folder, CFStringRef bundleID, CFDictionaryRef *appInfo);
bool AppHasBundleID(CFDictionaryRef app, CFStringRef bundleID, CFDictionaryRef *appInfo);


bool AppHasBundleID(CFDictionaryRef app, CFStringRef bundleID, CFDictionaryRef *appInfo) {
	bool foundBundleID = false;
	Boolean isApp = CFDictionaryContainsKey(app, CFSTR("bundleIdentifier"));
	if (isApp) {
		CFStringRef appID = CFDictionaryGetValue(app, CFSTR("bundleIdentifier"));
		if (CFStringCompare(appID, bundleID, 0x0) == 0x0) {
			foundBundleID = true;
			*appInfo = CFDictionaryCreateCopy(kCFAllocatorDefault, app);
		}
	}
	return foundBundleID;
}

bool FolderContainsBundleID(CFDictionaryRef folder, CFStringRef bundleID, CFDictionaryRef *appInfo) {
	bool foundBundleID = false;
	if (CFDictionaryContainsKey(folder, CFSTR(kListType))) {
		CFStringRef type = CFDictionaryGetValue(folder, CFSTR(kListType));
		if (CFStringCompare(type, CFSTR("folder"), 0x0) == 0x0) {
			CFArrayRef folderPages = CFDictionaryGetValue(folder, CFSTR(kIconLists));
			CFIndex pageCount = CFArrayGetCount(folderPages);
			for (CFIndex index = 0x0; index < pageCount; index++) {
				CFArrayRef page = CFArrayGetValueAtIndex(folderPages, index);
				foundBundleID = PageContainsBundleID(page, bundleID, appInfo);
				if (foundBundleID) {
					break;
				}
			}
		}
		if (CFStringCompare(type, CFSTR("newsstand"), 0x0) == 0x0) {
			// SDM: searching newsstand...
			printf("This is newsstand, skipping...\n");
		}
	}
	return foundBundleID;
}

enum SpringboardIconType ResolveSpringboardItemType(CFDictionaryRef item) {
	uint8_t type = SpringboardIconTypeInvalid;
	Boolean isApp = CFDictionaryContainsKey(item, CFSTR("bundleIdentifier"));
	if (isApp) {
		return SpringboardIconTypeApp;
	}
	Boolean isFolder = CFDictionaryContainsKey(item, CFSTR(kListType));
	if (isFolder) {
		return SpringboardIconTypeFolder;
	}
	return type;
}

bool PageContainsBundleID(CFArrayRef page, CFStringRef bundleID, CFDictionaryRef *appInfo) {
	bool foundBundleID = false;
	CFIndex itemsCount = CFArrayGetCount(page);
	for (CFIndex index = 0x0; index < itemsCount; index++) {
		CFDictionaryRef pageItem = CFArrayGetValueAtIndex(page, index);
		uint8_t type = ResolveSpringboardItemType(pageItem);
		switch (type) {
			case SpringboardIconTypeInvalid: {
				printf("This item is of an unknown type, skipping...\n");
				break;
			};
			case SpringboardIconTypeApp: {
				foundBundleID = AppHasBundleID(pageItem, bundleID, appInfo);
				break;
			};
			case SpringboardIconTypeFolder: {
				foundBundleID = FolderContainsBundleID(pageItem, bundleID, appInfo);
				break;
			};
			default: {
				break;
			};
		}
		if (foundBundleID) {
			break;
		}
		
	}
	return foundBundleID;
}

CFPropertyListRef FormatHomescreen(CFArrayRef dock, CFArrayRef pages) {
	CFPropertyListRef newFormat = CFPropertyListCreateXMLData(kCFAllocatorDefault, NULL);
	
	return newFormat;
}

void SpringboardQuery(char *udid) {
	SDMMD_AMDeviceRef device = FindDeviceFromUDID(udid);
	if (device) {
		SDMMD_AMConnectionRef springboard = AttachToDeviceAndService(device, AMSVC_SPRINGBOARD_SERVICES);
		CFMutableDictionaryRef request = SDMMD_create_dict();
		CFDictionarySetValue(request, CFSTR(kCommand), CFSTR(kCommandGetIconState));
		CFDictionarySetValue(request, CFSTR(kFormatVersion), CFSTR("2"));
		SocketConnection socket = SDMMD_TranslateConnectionToSocket(springboard);
		sdmmd_return_t result = SDMMD_ServiceSendMessage(socket, request, kCFPropertyListBinaryFormat_v1_0);
		if (result == kAMDSuccess) {
			CFPropertyListRef response = NULL;
			result = SDMMD_ServiceReceiveMessage(socket, &response);
			if (result == kAMDSuccess && response) {
				PrintCFType(response);
			}
		}
	}
}

#endif