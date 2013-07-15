/*
 *  SDMMD_Service.h
 *  SDMMobileDevice
 *
 *  Copyright (c) 2013, Sam Marshall
 *  All rights reserved.
 * 
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 *  1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
 *  3. All advertising materials mentioning features or use of this software must display the following acknowledgement:
 *  	This product includes software developed by the Sam Marshall.
 *  4. Neither the name of the Sam Marshall nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
 * 
 *  THIS SOFTWARE IS PROVIDED BY Sam Marshall ''AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL Sam Marshall BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 */

#ifndef _SDM_MD_SERVICE_H_
#define _SDM_MD_SERVICE_H_

#include <CoreFoundation/CoreFoundation.h>
#include "SDMMD_Error.h"
#include "SDMMD_Connection.h"

typedef struct SocketConnection {
	bool isSSL;
	union {
		SSL *ssl;
		uint32_t conn;
	} socket;
} SocketConnection;

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

#define AMSVC_LOCKDOWN				"com.apple.mobile.lockdown"

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

sdmmd_return_t SDMMD_ServiceSend(SocketConnection handle, CFDataRef data);
sdmmd_return_t SDMMD_ServiceReceive(SocketConnection handle, CFDataRef *data);
sdmmd_return_t SDMMD_ServiceSendMessage(SocketConnection handle, CFPropertyListRef data, CFPropertyListFormat format);
sdmmd_return_t SDMMD_ServiceReceiveMessage(SocketConnection handle, CFPropertyListRef *data);

SocketConnection SDMMD_TranslateConnectionToSocket(SDMMD_AMConnectionRef connection);

#endif