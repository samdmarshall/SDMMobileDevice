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

#include "Core.h"

bool AppHasBundleID(CFDictionaryRef app, CFStringRef bundleID, CFDictionaryRef *appInfo);
bool FolderContainsBundleID(CFDictionaryRef folder, CFStringRef bundleID, CFDictionaryRef *appInfo);
enum SpringboardIconType ResolveSpringboardItemTypeInternal(CFDictionaryRef item);
enum SpringboardIconType ResolveSpringboardItemType(CFDictionaryRef item);
bool PageContainsBundleID(CFArrayRef page, CFStringRef bundleID, CFDictionaryRef *appInfo);
bool HomescreenHasApp(CFPropertyListRef homescreen, CFStringRef bundleID, CFDictionaryRef *appInfo);
CFMutableDictionaryRef CreateEmptyFolder(CFStringRef name);
CFMutableArrayRef AppendApp(struct SpringboardDeviceInfo *info, CFPropertyListRef homescreen, CFMutableArrayRef screen, CFDictionaryRef item);
CFMutableArrayRef AppendFolder(struct SpringboardDeviceInfo *info, CFPropertyListRef homescreen, CFMutableArrayRef screen, CFDictionaryRef item);
CFMutableArrayRef AppendItem(struct SpringboardDeviceInfo *info, CFPropertyListRef homescreen, CFMutableArrayRef screen, CFDictionaryRef item);
CFArrayRef CreateSpringboardDock(struct SpringboardDeviceInfo *info, CFPropertyListRef homescreen, CFArrayRef springboardItems);
CFArrayRef CreateSpringboardScreen(struct SpringboardDeviceInfo *info, CFPropertyListRef homescreen, CFArrayRef springboardItems);
CFDictionaryRef CreateSpringboardItem(CFStringRef type, CFTypeRef value);
CFDictionaryRef CreateSpringboardApp(CFStringRef bundleID);
CFDictionaryRef CreateSpringboardFolder(CFStringRef name, CFArrayRef contents);
CFPropertyListRef FormatHomescreen(struct SpringboardDeviceInfo *info, CFPropertyListRef homescreen, CFArrayRef dock, CFArrayRef pages);

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

enum SpringboardIconType ResolveSpringboardItemTypeInternal(CFDictionaryRef item) {
	uint8_t type = SpringboardIconTypeInvalid;
	CFStringRef typeValue = CFDictionaryGetValue(item, CFSTR("Type"));
	if (CFStringCompare(typeValue, CFSTR("Application"), 0x0) == 0x0) {
		return SpringboardIconTypeApp;
	}
	if (CFStringCompare(typeValue, CFSTR("Folder"), 0x0) == 0x0) {
		return SpringboardIconTypeFolder;
	}
	return type;
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

bool HomescreenHasApp(CFPropertyListRef homescreen, CFStringRef bundleID, CFDictionaryRef *appInfo) {
	bool foundBundleID = false;
	CFIndex pages = CFArrayGetCount(homescreen);
	for (CFIndex index = 0x0; index < pages; index++) {
		CFArrayRef page = CFArrayGetValueAtIndex(homescreen, index);
		foundBundleID = PageContainsBundleID(page, bundleID, appInfo);
	}
	return foundBundleID;
}

CFMutableDictionaryRef CreateEmptyFolder(CFStringRef name) {
	CFMutableDictionaryRef folder = SDMMD_create_dict();
	CFDictionarySetValue(folder, CFSTR(kDisplayName), name);
	CFMutableArrayRef apps = CFArrayCreateMutable(kCFAllocatorDefault, 0x0, &kCFTypeArrayCallBacks);
	CFDictionarySetValue(folder, CFSTR(kIconLists), apps);
	CFDictionarySetValue(folder, CFSTR(kListType), CFSTR("folder"));
	return folder;
}

CFMutableArrayRef AppendApp(struct SpringboardDeviceInfo *info, CFPropertyListRef homescreen, CFMutableArrayRef screen, CFDictionaryRef item) {
	CFDictionaryRef appInfo = NULL;
	CFStringRef appID = CFDictionaryGetValue(item, CFSTR("Contents"));
	bool appInstalled = HomescreenHasApp(homescreen, appID, &appInfo);
	if (appInstalled && appInfo) {
		CFDictionaryRef appInfoCopy = CFDictionaryCreateCopy(kCFAllocatorDefault, appInfo);
		CFArrayAppendValue(screen, appInfoCopy);
	}
	return screen;
}

CFMutableArrayRef AppendFolder(struct SpringboardDeviceInfo *info, CFPropertyListRef homescreen, CFMutableArrayRef screen, CFDictionaryRef item) {
	CFDictionaryRef folderContents = CFDictionaryGetValue(item, CFSTR("Contents"));
	CFStringRef folderName = CFDictionaryGetValue(folderContents, CFSTR("Name"));
	CFMutableDictionaryRef folder = CreateEmptyFolder(folderName);
	CFMutableArrayRef addApps = (CFMutableArrayRef)CFDictionaryGetValue(folder, CFSTR(kIconLists));
	CFArrayRef folderApps = CFDictionaryGetValue(folderContents, CFSTR("Contents"));
	CFIndex appCount = CFArrayGetCount(folderApps);
	CFIndex folderAppCount = (info->folderColumn*info->folderRow);
	CFIndex appPages = ceil(appCount/folderAppCount);
	CFIndex pageCount = (appPages < info->folderMaxPage ? appPages : info->folderMaxPage);
	for (CFIndex index = 0x0; index < pageCount; index++) {
		CFMutableArrayRef folderPage = CFArrayCreateMutable(kCFAllocatorDefault, 0x0, &kCFTypeArrayCallBacks);
		CFIndex contentsCount = (appPages-index > 1 ? folderAppCount : (appCount%folderAppCount));
		for (CFIndex folderIndex = 0x0; index < contentsCount; folderIndex++) {
			CFDictionaryRef item = CFArrayGetValueAtIndex(folderApps, (index*folderAppCount)+folderIndex);
			folderPage = AppendItem(info, homescreen, folderPage, item);
		}
		addApps = AppendItem(info, homescreen, addApps, item);
	}
	CFArrayAppendValue(screen, folder);
	return screen;
}

CFMutableArrayRef AppendItem(struct SpringboardDeviceInfo *info, CFPropertyListRef homescreen, CFMutableArrayRef screen, CFDictionaryRef item) {
	uint8_t type = ResolveSpringboardItemType(item);
	switch (type) {
		case SpringboardIconTypeInvalid: {
			printf("This item is of an unknown type, skipping...\n");
			break;
		};
		case SpringboardIconTypeApp: {
			screen = AppendApp(info, homescreen, screen, item);
			break;
		};
		case SpringboardIconTypeFolder: {
			screen = AppendFolder(info, homescreen, screen, item);
			break;
		};
		default: {
			break;
		};
	}
	return screen;
}

CFArrayRef CreateSpringboardDock(struct SpringboardDeviceInfo *info, CFPropertyListRef homescreen, CFArrayRef springboardItems) {
	CFMutableArrayRef screen = CFArrayCreateMutable(kCFAllocatorDefault, 0x0, &kCFTypeArrayCallBacks);
	CFIndex itemCount = CFArrayGetCount(springboardItems);
	CFIndex dockCount = (itemCount < info->dockCount ? itemCount : info->dockCount);
	for (CFIndex index = 0x0; index < dockCount; index++) {
		CFDictionaryRef item = CFArrayGetValueAtIndex(springboardItems, index);
		screen = AppendItem(info, homescreen, screen, item);
	}
	return screen;
}

CFArrayRef CreateSpringboardScreen(struct SpringboardDeviceInfo *info, CFPropertyListRef homescreen, CFArrayRef springboardItems) {
	CFMutableArrayRef screen = CFArrayCreateMutable(kCFAllocatorDefault, 0x0, &kCFTypeArrayCallBacks);
	CFIndex itemCount = CFArrayGetCount(springboardItems);
	CFIndex screenTotal = (info->screenColumn*info->screenRow);
	CFIndex screenCount = (itemCount < screenTotal ? itemCount : screenTotal);
	for (CFIndex index = 0x0; index < screenCount; index++) {
		CFDictionaryRef item = CFArrayGetValueAtIndex(springboardItems, index);
		screen = AppendItem(info, homescreen, screen, item);
	}
	return screen;
}

CFDictionaryRef CreateSpringboardItem(CFStringRef type, CFTypeRef value) {
	CFMutableDictionaryRef sbItem = SDMMD_create_dict();
	CFDictionarySetValue(sbItem, CFSTR("Type"), type);
	CFDictionarySetValue(sbItem, CFSTR("Contents"), value);
	return sbItem;
}

CFDictionaryRef CreateSpringboardApp(CFStringRef bundleID) {
	return CreateSpringboardItem(CFSTR("Application"), bundleID);
}

CFDictionaryRef CreateSpringboardFolder(CFStringRef name, CFArrayRef contents) {
	CFMutableDictionaryRef value = SDMMD_create_dict();
	CFDictionarySetValue(value, CFSTR("Name"), name);
	CFDictionarySetValue(value, CFSTR("Contents"), contents);
	return CreateSpringboardItem(CFSTR("Folder"), value);
}

CFPropertyListRef FormatHomescreen(struct SpringboardDeviceInfo *info, CFPropertyListRef homescreen, CFArrayRef dock, CFArrayRef pages) {
	CFMutableArrayRef newFormat = CFArrayCreateMutable(kCFAllocatorDefault, 0x0, &kCFTypeArrayCallBacks);
	
	CFArrayRef newDock = CreateSpringboardDock(info, homescreen, dock);
	CFArrayAppendValue(newFormat, newDock);
	
	CFIndex pageCount = CFArrayGetCount(pages);
	for (CFIndex index = 0x0; index < pageCount; index++) {
		CFArrayRef page = CFArrayGetValueAtIndex(pages, index);
		CFArrayRef screen = CreateSpringboardScreen(info, homescreen, page);
		CFIndex currentPageCount = CFArrayGetCount(newFormat) - 0x1;
		if (currentPageCount < info->screenMaxPage) {
			CFArrayAppendValue(newFormat, screen);
		} else {
			break;
		}
	}
	
	return newFormat;
}

struct SpringboardDeviceInfo* CreateSpringboardInfoFromDevice(SDMMD_AMDeviceRef device) {
	struct SpringboardDeviceInfo *info = calloc(0x1, S(struct SpringboardDeviceInfo));
	
	CFNumberRef dockCount = SDMMD_AMDeviceCopyValue(device, CFSTR(kiTunesMobileDomain), CFSTR(kHomeScreenIconDockMaxCount));
	CFNumberGetValue(dockCount, kCFNumberSInt32Type, &(info->dockCount));
	CFSafeRelease(dockCount);
	
	CFNumberRef screenRow = SDMMD_AMDeviceCopyValue(device, CFSTR(kiTunesMobileDomain), CFSTR(kHomeScreenIconRows));
	CFNumberGetValue(screenRow, kCFNumberSInt32Type, &(info->screenRow));
	CFSafeRelease(screenRow);
	
	CFNumberRef screenColumn = SDMMD_AMDeviceCopyValue(device, CFSTR(kiTunesMobileDomain), CFSTR(kHomeScreenIconColumns));
	CFNumberGetValue(screenColumn, kCFNumberSInt32Type, &(info->screenColumn));
	CFSafeRelease(screenColumn);
	
	CFNumberRef screenMaxPage = SDMMD_AMDeviceCopyValue(device, CFSTR(kiTunesMobileDomain), CFSTR(kHomeScreenMaxPages));
	CFNumberGetValue(screenMaxPage, kCFNumberSInt32Type, &(info->screenMaxPage));
	CFSafeRelease(screenMaxPage);
	
	CFNumberRef folderRow = SDMMD_AMDeviceCopyValue(device, CFSTR(kiTunesMobileDomain), CFSTR(kIconFolderRows));
	CFNumberGetValue(folderRow, kCFNumberSInt32Type, &(info->folderRow));
	CFSafeRelease(folderRow);
	
	CFNumberRef folderColumn = SDMMD_AMDeviceCopyValue(device, CFSTR(kiTunesMobileDomain), CFSTR(kIconFolderColumns));
	CFNumberGetValue(folderColumn, kCFNumberSInt32Type, &(info->folderColumn));
	CFSafeRelease(folderColumn);
	
	CFNumberRef folderMaxPage = SDMMD_AMDeviceCopyValue(device, CFSTR(kiTunesMobileDomain), CFSTR(kIconFolderMaxPages));
	CFNumberGetValue(folderMaxPage, kCFNumberSInt32Type, &(info->folderMaxPage));
	CFSafeRelease(folderMaxPage);

	return info;
}

void SpringboardQuery(char *udid) {
	SDMMD_AMDeviceRef device = FindDeviceFromUDID(udid);
	if (device) {
		//struct SpringboardDeviceInfo *info = CreateSpringboardInfoFromDevice(device);
		
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
		
		/*
		CFMutableDictionaryRef setrequest = SDMMD_create_dict();
		CFDictionarySetValue(setrequest, CFSTR(kCommand), CFSTR(kCommandSetIconState));
		CFDictionarySetValue(setrequest, CFSTR("iconState"), newformat);
		result = SDMMD_ServiceSendMessage(socket, setrequest, kCFPropertyListBinaryFormat_v1_0);
		if (result == kAMDSuccess) {
			CFPropertyListRef response = NULL;
			result = SDMMD_ServiceReceiveMessage(socket, &response);
			if (result == kAMDSuccess && response) {
				PrintCFType(response);
			}
		}
		*/
	}
}

#endif