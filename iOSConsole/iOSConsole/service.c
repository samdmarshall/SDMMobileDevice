//
//  service.c
//  iOSConsole
//
//  Created by Sam Marshall on 1/4/14.
//  Copyright (c) 2014 Sam Marshall. All rights reserved.
//

#ifndef iOSConsole_service_c
#define iOSConsole_service_c

#include "service.h"
#include "attach.h"
#include "syslog.h"

void PerformService(char *udid, char *service, ...) {
	if ((udid && strlen(udid) == 0x28) && (service)) {
		uint32_t index = 0x0;
		for (index = 0x0; index < SDM_MD_Service_Count; index++) {
			char *ident = SDMMDServiceIdentifiers[index].identifier;
			char *shorthand = SDMMDServiceIdentifiers[index].shorthand;
			if (strncmp(service, ident, strlen(ident)) == 0x0 || strncmp(service, shorthand, strlen(shorthand)) == 0x0) {
				break;
			}
		}
		switch (index) {
			case SDM_MD_Service_AFC: {
				break;
			};
			case SDM_MD_Service_AFC2: {
				// SDM: drop into console mode
				break;
			};
			case SDM_MD_Service_BACKUP: {
				break;
			};
			case SDM_MD_Service_BACKUP2: {
				break;
			};
			case SDM_MD_Service_CRASH_REPORT_COPY: {
				break;
			};
			case SDM_MD_Service_CRASH_REPORT_COPY_MOB: {
				break;
			};
			case SDM_MD_Service_CRASH_REPORT_COPY_MV: {
				break;
			};
			case SDM_MD_Service_PURPLE_TEST: {
				break;
			};
			case SDM_MD_Service_SYNC: {
				break;
			};
			case SDM_MD_Service_SCREENSHOT: {
				break;
			};
			case SDM_MD_Service_MIS_AGENT: {
				break;
			};
			case SDM_MD_Service_SPRINGBOARD_SERVICES: {
				break;
			};
			case SDM_MD_Service_ATC: {
				break;
			};
			case SDM_MD_Service_PACKETCAPTURE: {
				break;
			};
			case SDM_MD_Service_WEB_INSPECTOR: {
				break;
			};
			case SDM_MD_Service_RASD: {
				break;
			};
			case SDM_MD_Service_THERMAL_MONITOR: {
				break;
			};
			case SDM_MD_Service_AIT_AITD: {
				break;
			};
			case SDM_MD_Service_HPD_MOBILE: {
				break;
			};
			case SDM_MD_Service_SYSLOG_RELAY: {
				Syslog(udid);
				break;
			};
			case SDM_MD_Service_FILE_RELAY: {
				break;
			};
			case SDM_MD_Service_INTEGRITY_RELAY: {
				break;
			};
			case SDM_MD_Service_DIAG_RELAY: {
				break;
			};
			case SDM_MD_Service_SYSTEM_PROFILER: {
				break;
			};
			case SDM_MD_Service_INSTALLATION_PROXY: {
				break;
			};
			case SDM_MD_Service_FACTORY_PROXY: {
				break;
			};
			case SDM_MD_Service_NOTIFICATION_PROXY: {
				break;
			};
			case SDM_MD_Service_HOUSE_ARREST: {
				break;
			};
			case SDM_MD_Service_LOCKDOWN: {
				break;
			};
			case SDM_MD_Service_SOFTWARE_UPDATE: {
				break;
			};
			case SDM_MD_Service_DEBUG_IMAGE_MOUNT: {
				break;
			};
			case SDM_MD_Service_MOBILE_IMAGE_MOUNT: {
				break;
			};
			case SDM_MD_Service_MCINSTALL: {
				break;
			};
			case SDM_MD_Service_MDMSERVICE: {
				break;
			};
			case SDM_MD_Service_HEARTBEAT: {
				break;
			};
			case SDM_MD_Service_ASSERTATION_AGENT: {
				break;
			};
			case SDM_MD_Service_IOSDIAG_RELAY: {
				break;
			};
			case SDM_MD_Service_DEBUG_SERVER: {
				break;
			};
			case SDM_MD_Service_DEBUG_SERVER_SECURE: {
				break;
			};
			case SDM_MD_Service_DEBUG_SERVER_APPLIST: {
				break;
			};
			case SDM_MD_Service_FETCH_SYMBOLS: {
				break;
			};
			case SDM_MD_Service_SIMULATE_LOCATION: {
				break;
			};
			case SDM_MD_Service_RADIOS_TEST_MOBILE: {
				break;
			};
			case SDM_MD_Service_RADIOS_TEST_ROOT: {
				break;
			};
			default: {
				break;
			};
		}
	} else {
		printf("invalid udid or service name\n");
	}
}

#endif
