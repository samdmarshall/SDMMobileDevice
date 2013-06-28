/*
 *  SDMMDService.h
 *  SDM_MD_Demo
 *
 *  Created by sam on 5/24/13.
 *  Copyright 2013 Sam Marshall. All rights reserved.
 *
 */

#ifndef _SDMMDSERVICE_H_
#define _SDMMDSERVICE_H_

#import <CoreFoundation/CoreFoundation.h>
#import "SDMMDError.h"

#pragma mark -
#pragma mark Service List
#pragma mark -

#define AMSVC_AFC					"com.apple.afc"
#define AMSVC_AFC2					"com.apple.afc2"
#define AMSVC_BACKUP				"com.apple.mobilebackup"
#define AMSVC_CRASH_REPORT_COPY		"com.apple.crashreportcopy"
#define AMSVC_DEBUG_IMAGE_MOUNT		"com.apple.mobile.debug_image_mount"
#define AMSVC_NOTIFICATION_PROXY	"com.apple.mobile.notification_proxy"
#define AMSVC_PURPLE_TEST			"com.apple.purpletestr"
#define AMSVC_SOFTWARE_UPDATE		"com.apple.mobile.software_update"
#define AMSVC_SYNC					"com.apple.mobilesync"
#define AMSVC_SCREENSHOT			"com.apple.screenshotr"
#define AMSVC_SYSLOG_RELAY			"com.apple.syslog_relay"
#define AMSVC_SYSTEM_PROFILER		"com.apple.mobile.system_profiler"
#define AMSVC_SPRINGBOARD_SERVICES	"com.apple.springboardservices"
#define AMSVC_INSTALLATION_PROXY	"com.apple.mobile.installation_proxy"
#define AMSVC_HOUSE_ARREST			"com.apple.mobile.house_arrest"

#define AMSVC_MIS_AGENT				"com.apple.misagent"

#define AMSVC_FILE_RELAY			"com.apple.mobile.file_relay"
#define AMSVC_FETCH_SYMBOLS			"com.apple.dt.fetchsymbols"
#define AMSVC_SIMULATE_LOCATION		"com.apple.dt.simulatelocation"
#define AMSVC_DEBUG_SERVER			"com.apple.debugserver"
#define AMSVC_DEBUG_SERVER_SECURE	"com.apple.debugserver.secure"
#define AMSVC_DEBUG_SERVER_APPLIST	"com.apple.debugserver.applist"


#pragma mark -
#pragma mark Springboard Service Access
#pragma mark -

#define kCommand								"command"
#pragma mark *Commands
#define kCommandGetIconState					"getIconState"
#define kCommandSetIconState					"setIconState"
#define kCommandGetIconPNGData					"getIconPNGData"
#define kCommandGetHomeScreenWallpaperPNGData	"getHomeScreenWallpaperPNGData"
#define kCommandGetInterfaceOrientation			"getInterfaceOrientation"
#pragma mark *FormatVersion
#define kFormatVersion							"formatVersion" // use "2" as the key

#pragma mark *Item Properties
#define kPropertyIconState						"iconState"
#define kPropertyPNGData						"pngData"
#define kPropertyBundleID						"bundleId"
#define kPropertyInterfaceOrientation			"interfaceOrientation"


#define kListType								"listType"
#define kDisplayName							"displayName"
#define kIconLists								"iconLists"



#pragma mark -
#pragma mark Service Command Functions
#pragma mark -

sdmmd_return_t SDMMDServiceSend(uint32_t handle, CFDataRef data);
sdmmd_return_t SDMMDServiceReceive(uint32_t handle, CFDataRef *data);
sdmmd_return_t SDMMDServiceSendMessage(uint32_t handle, CFPropertyListRef data);
sdmmd_return_t SDMMDServiceReceiveMessage(uint32_t handle, CFPropertyListRef *data);

#endif