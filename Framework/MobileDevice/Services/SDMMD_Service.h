/*
 *  SDMMD_Service.h
 *  SDMMobileDevice
 *
 * Copyright (c) 2014, Sam Marshall
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the 
 * following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer 
 * 		in the documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of Sam Marshall nor the names of its contributors may be used to endorse or promote products derived from this
 * 		software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef _SDM_MD_SERVICE_H_
#define _SDM_MD_SERVICE_H_

#include <CoreFoundation/CoreFoundation.h>
#include "SDMMD_Error.h"
#include "SDMMD_Connection.h"



typedef struct SocketConnection_Internal SocketConnection;

#pragma mark -
#pragma mark Service List
#pragma mark -

#define AMSVC_AFC					"com.apple.afc"
#define AMSVC_AFC2					"com.apple.afc2" // hi
#define AMSVC_BACKUP				"com.apple.mobilebackup"
#define AMSVC_BACKUP2				"com.apple.mobilebackup2"
#define AMSVC_CRASH_REPORT_COPY		"com.apple.crashreportcopy" // nope
#define AMSVC_CRASH_REPORT_COPY_MOB	"com.apple.crashreportcopymobile"
#define AMSVC_CRASH_REPORT_COPY_MV	"com.apple.crashreportmover"
#define AMSVC_PURPLE_TEST			"com.apple.purpletestr" // expects
#define AMSVC_SYNC					"com.apple.mobilesync"
#define AMSVC_SCREENSHOT			"com.apple.screenshotr"
#define AMSVC_MIS_AGENT				"com.apple.misagent"
#define AMSVC_SPRINGBOARD_SERVICES	"com.apple.springboardservices"
#define AMSVC_ATC					"com.apple.atc" // expects
#define AMSVC_ATC2					"com.apple.atc2"
#define AMSVC_PACKETCAPTURE			"com.apple.pcapd"
#define AMSVC_WEB_INSPECTOR			"com.apple.webinspector"
#define AMSVC_RASD					"com.apple.rasd" // expects

#define AMSVC_THERMAL_MONITOR		"com.apple.thermalmonitor.thermtgraphrelay" // expects

#define AMSVC_AIT_AITD				"com.apple.ait.aitd" // accessibility response

#define AMSVC_HPD_MOBILE			"com.apple.hpd.mobile" // expects

#define AMSVC_SYSLOG_RELAY			"com.apple.syslog_relay"
#define AMSVC_FILE_RELAY			"com.apple.mobile.file_relay"
#define AMSVC_INTEGRITY_RELAY		"com.apple.mobile.integrity_relay"
#define AMSVC_DIAG_RELAY			"com.apple.mobile.diagnostics_relay"

#define AMSVC_MOBILE_DEBUG			"com.apple.mobile.debug"
#define AMSVC_SYSTEM_PROFILER		"com.apple.mobile.system_profiler" // invalid
#define AMSVC_INSTALLATION_PROXY	"com.apple.mobile.installation_proxy"
#define AMSVC_FACTORY_PROXY			"com.apple.mobile.factory_proxy"
#define AMSVC_NOTIFICATION_PROXY	"com.apple.mobile.notification_proxy"
#define AMSVC_INSEC_NOTIF_PROXY		"com.apple.mobile.insecure_notification_proxy"
#define AMSVC_HOUSE_ARREST			"com.apple.mobile.house_arrest"
#define AMSVC_LOCKDOWN				"com.apple.mobile.lockdown" // invalid
#define AMSVC_SOFTWARE_UPDATE		"com.apple.mobile.software_update" // invalid
#define AMSVC_DEBUG_IMAGE_MOUNT		"com.apple.mobile.debug_image_mount"
#define AMSVC_MOBILE_IMAGE_MOUNT	"com.apple.mobile.mobile_image_mounter"
#define AMSVC_MCINSTALL				"com.apple.mobile.MCInstall"
#define AMSVC_MDMSERVICE			"com.apple.mobile.MDMService"
#define AMSVC_HEARTBEAT				"com.apple.mobile.heartbeat" // expects
#define AMSVC_ASSERTATION_AGENT		"com.apple.mobile.assertion_agent" // expects

#define AMSVC_IOSDIAG_RELAY			"com.apple.iosdiagnostics.relay" // expects

#define AMSVC_DEBUG_SERVER			"com.apple.debugserver"
#define AMSVC_DEBUG_SERVER_SECURE	"com.apple.debugserver.secure" // used for apple tv?
#define AMSVC_DEBUG_SERVER_APPLIST	"com.apple.debugserver.applist"
#define AMSVC_FETCH_SYMBOLS			"com.apple.dt.fetchsymbols" // expects
#define AMSVC_SIMULATE_LOCATION		"com.apple.dt.simulatelocation"

#define AMSVC_RADIOS_TEST_MOBILE	"com.apple.radios.wirelesstester.mobile" // expects
#define AMSVC_RADIOS_TEST_ROOT		"com.apple.radios.wirelesstester.root" // expects

#define AMSVC_STREAM_ZIP			"com.apple.streaming_zip_conduit"

#define AMSVC_PREBOARD				"com.apple.preboardservice"

struct SDM_MD_Service_Identifiers {
	char *identifier;
	char *shorthand;
};

enum SDM_MD_Service_Enum {
	SDM_MD_Service_AFC,
	SDM_MD_Service_AFC2,
	SDM_MD_Service_BACKUP,
	SDM_MD_Service_BACKUP2,
	SDM_MD_Service_CRASH_REPORT_COPY,
	SDM_MD_Service_CRASH_REPORT_COPY_MOB,
	SDM_MD_Service_CRASH_REPORT_COPY_MV,
	SDM_MD_Service_PURPLE_TEST,
	SDM_MD_Service_SYNC,
	SDM_MD_Service_SCREENSHOT,
	SDM_MD_Service_MIS_AGENT,
	SDM_MD_Service_SPRINGBOARD_SERVICES,
	SDM_MD_Service_ATC,
	SDM_MD_Service_PACKETCAPTURE,
	SDM_MD_Service_WEB_INSPECTOR,
	SDM_MD_Service_RASD,
	SDM_MD_Service_THERMAL_MONITOR,
	SDM_MD_Service_AIT_AITD,
	SDM_MD_Service_HPD_MOBILE,
	SDM_MD_Service_SYSLOG_RELAY,
	SDM_MD_Service_FILE_RELAY,
	SDM_MD_Service_INTEGRITY_RELAY,
	SDM_MD_Service_DIAG_RELAY,
	SDM_MD_Service_MOBILE_DEBUG,
	SDM_MD_Service_SYSTEM_PROFILER,
	SDM_MD_Service_INSTALLATION_PROXY,
	SDM_MD_Service_FACTORY_PROXY,
	SDM_MD_Service_NOTIFICATION_PROXY,
	SDM_MD_Service_HOUSE_ARREST,
	SDM_MD_Service_LOCKDOWN,
	SDM_MD_Service_SOFTWARE_UPDATE,
	SDM_MD_Service_DEBUG_IMAGE_MOUNT,
	SDM_MD_Service_MOBILE_IMAGE_MOUNT,
	SDM_MD_Service_MCINSTALL,
	SDM_MD_Service_MDMSERVICE,
	SDM_MD_Service_HEARTBEAT,
	SDM_MD_Service_ASSERTATION_AGENT,
	SDM_MD_Service_IOSDIAG_RELAY,
	SDM_MD_Service_DEBUG_SERVER,
	SDM_MD_Service_DEBUG_SERVER_SECURE,
	SDM_MD_Service_DEBUG_SERVER_APPLIST,
	SDM_MD_Service_FETCH_SYMBOLS,
	SDM_MD_Service_SIMULATE_LOCATION,
	SDM_MD_Service_RADIOS_TEST_MOBILE,
	SDM_MD_Service_RADIOS_TEST_ROOT,
	SDM_MD_Service_Count
};

__attribute__ ((unused)) static struct SDM_MD_Service_Identifiers SDMMDServiceIdentifiers[SDM_MD_Service_Count] = {
	{AMSVC_AFC, "afc"},
	{AMSVC_AFC2, "afc2"},
	{AMSVC_BACKUP, "backup"},
	{AMSVC_BACKUP2, "backup2"},
	{AMSVC_CRASH_REPORT_COPY, "crashr"},
	{AMSVC_CRASH_REPORT_COPY_MOB, "mcrashr"},
	{AMSVC_CRASH_REPORT_COPY_MV, "crashrmove"},
	{AMSVC_PURPLE_TEST, "purple"},
	{AMSVC_SYNC, "sync"},
	{AMSVC_SCREENSHOT, "screenshot"},
	{AMSVC_MIS_AGENT, "misagent"},
	{AMSVC_SPRINGBOARD_SERVICES, "sb"},
	{AMSVC_ATC, "atc"},
	{AMSVC_PACKETCAPTURE, "pcap"},
	{AMSVC_WEB_INSPECTOR, "web"},
	{AMSVC_RASD, "rasd"},
	{AMSVC_THERMAL_MONITOR, "thermal"},
	{AMSVC_AIT_AITD, "ait"},
	{AMSVC_HPD_MOBILE, "hpd"},
	{AMSVC_SYSLOG_RELAY, "syslog"},
	{AMSVC_FILE_RELAY, "file"},
	{AMSVC_INTEGRITY_RELAY, "integ"},
	{AMSVC_DIAG_RELAY, "diag"},
	{AMSVC_MOBILE_DEBUG, "mobdebug"},
	{AMSVC_SYSTEM_PROFILER, "sysprof"},
	{AMSVC_INSTALLATION_PROXY, "install"},
	{AMSVC_FACTORY_PROXY, "factory"},
	{AMSVC_NOTIFICATION_PROXY, "notif"},
	{AMSVC_HOUSE_ARREST, "house"},
	{AMSVC_LOCKDOWN, "lockdown"},
	{AMSVC_SOFTWARE_UPDATE, "update"},
	{AMSVC_DEBUG_IMAGE_MOUNT, "dimage"},
	{AMSVC_MOBILE_IMAGE_MOUNT, "mimage"},
	{AMSVC_MCINSTALL, "mcinstall"},
	{AMSVC_MDMSERVICE, "mdm"},
	{AMSVC_HEARTBEAT, "heartbeat"},
	{AMSVC_ASSERTATION_AGENT, "assert"},
	{AMSVC_IOSDIAG_RELAY, "iosdiag"},
	{AMSVC_DEBUG_SERVER, "debug"},
	{AMSVC_DEBUG_SERVER_SECURE, "debugsec"},
	{AMSVC_DEBUG_SERVER_APPLIST, "debugapp"},
	{AMSVC_FETCH_SYMBOLS, "fetchsym"},
	{AMSVC_SIMULATE_LOCATION, "simloc"},
	{AMSVC_RADIOS_TEST_MOBILE, "mradiotest"},
	{AMSVC_RADIOS_TEST_ROOT, "rradiotest"},
};

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

/*
 Results (Array) [
 	[0] Dock (Array) [
 		[0] App (Dictionary) {
			displayIdentifier (String)
 			displayName (String)
 			iconModDate (String)
 			bundleVersion (String)
			bundleIdentifier (String)
		}
 	]
 	[1] Page 1 (Array)
 		[0] Page 1 (Array) [
 			(see page format above)
		]
 		...
		[n] Folder (Dictionary) {
			displayName (String)
 			iconLists (Array) [
 				[0] Page 1 (Array) [
 					(see page format above)
 				]
 			]
 			listType (String)
		}
 	[2] Page 2 (Array) [
 		(see page format above)
 	]
 	[3] ...
 ]
*/

#pragma mark -
#pragma mark Service Command Functions
#pragma mark -

sdmmd_return_t SDMMD_DirectServiceSend(SocketConnection handle, CFDataRef data);
sdmmd_return_t SDMMD_DirectServiceReceive(SocketConnection handle, CFDataRef *data);

sdmmd_return_t SDMMD_ServiceSend(SocketConnection handle, CFDataRef data);
sdmmd_return_t SDMMD_ServiceReceive(SocketConnection handle, CFDataRef *data);

sdmmd_return_t SDMMD_ServiceSendMessage(SocketConnection handle, CFPropertyListRef data, CFPropertyListFormat format);
sdmmd_return_t SDMMD_ServiceReceiveMessage(SocketConnection handle, CFPropertyListRef *data);

sdmmd_return_t SDMMD_ServiceSendStream(SocketConnection handle, CFPropertyListRef data, CFPropertyListFormat format);
sdmmd_return_t SDMMD_ServiceReceiveStream(SocketConnection handle, CFPropertyListRef *data);

SocketConnection SDMMD_TranslateConnectionToSocket(SDMMD_AMConnectionRef connection);

#endif