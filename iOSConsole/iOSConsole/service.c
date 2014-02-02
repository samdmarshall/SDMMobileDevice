//
//  service.c
//  iOSConsole
//
//  Created by Sam Marshall on 1/4/14.
//  Copyright (c) 2014 Sam Marshall. All rights reserved.
//

#ifndef iOSConsole_service_c
#define iOSConsole_service_c

#include <sys/socket.h>
#include <sys/un.h>

#include "service.h"
#include "attach.h"
#include "syslog.h"
#include "springboard.h"
#include "Core.h"

void ServiceSocketCallback(CFSocketRef s, CFSocketCallBackType callbackType, CFDataRef address, const void *data, void *info) {
    CFSocketNativeHandle socket = (CFSocketNativeHandle)(*((CFSocketNativeHandle *)data));
	printf("callback: %lu\n",callbackType);
    struct msghdr message;
    struct iovec iov[0x1];
    struct cmsghdr *control_message = NULL;
    char ctrl_buf[CMSG_SPACE(sizeof(int))];
    char dummy_data[0x1];
	
    memset(&message, 0x0, sizeof(struct msghdr));
    memset(ctrl_buf, 0x0, CMSG_SPACE(sizeof(int)));
	
    dummy_data[0] = ' ';
    iov[0x0].iov_base = dummy_data;
    iov[0x0].iov_len = sizeof(dummy_data);
	
    message.msg_name = NULL;
    message.msg_namelen = 0x0;
    message.msg_iov = iov;
    message.msg_iovlen = 0x1;
    message.msg_controllen = CMSG_SPACE(sizeof(int));
    message.msg_control = ctrl_buf;
	
    control_message = CMSG_FIRSTHDR(&message);
    control_message->cmsg_level = SOL_SOCKET;
    control_message->cmsg_type = SCM_RIGHTS;
    control_message->cmsg_len = CMSG_LEN(sizeof(int));
	
    *((int *) CMSG_DATA(control_message)) = PtrCast(info,int);
	
    sendmsg(socket, &message, 0x0);
    //CFSocketInvalidate(s);
    //CFSafeRelease(s);
}

void CreateLocalSocket(char *udid, struct SDM_MD_Service_Identifiers service) {
	SDMMD_AMDeviceRef device = FindDeviceFromUDID(udid);
	SDMMD_AMConnectionRef serviceCon = AttachToDeviceAndService(device, service.identifier);
	if (serviceCon) {
		char *socketPath = calloc(0x1, sizeof(char)*0x400);
		strlcat(socketPath, "/tmp/sdm_", 0xa);
		strlcat(&(socketPath[0x9]), service.shorthand, 0xa+strlen(service.shorthand));
		printf("Creating socket at %s\n",socketPath);
		
		SocketConnection serviceSocket = SDMMD_TranslateConnectionToSocket(serviceCon);
		CFSocketContext context;
		if (serviceSocket.isSSL) {
			context = (CFSocketContext){ 0x0, serviceSocket.socket.ssl, NULL, NULL, NULL };
		} else {
			context = (CFSocketContext){ 0x0, &serviceSocket.socket.conn, NULL, NULL, NULL };
		}
		CFSocketRef serviceSock = CFSocketCreate(NULL, AF_UNIX, 0x0, 0x0, kCFSocketAcceptCallBack, &ServiceSocketCallback, &context);
		
		int yes = 0x1;
		setsockopt(CFSocketGetNative(serviceSock), SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
		
		struct sockaddr_un address;
		memset(&address, 0x0, sizeof(address));
		address.sun_family = AF_UNIX;
		strcpy(address.sun_path, socketPath);
		address.sun_len = SUN_LEN(&address);
		CFDataRef address_data = CFDataCreate(NULL, (const UInt8 *)&address, sizeof(address));

		unlink(socketPath);
		
		CFSocketSetAddress(serviceSock, address_data);
		CFSafeRelease(address_data);
		CFRunLoopSourceRef socketSource = CFSocketCreateRunLoopSource(NULL, serviceSock, 0);
		CFRunLoopAddSource(CFRunLoopGetMain(), socketSource, kCFRunLoopCommonModes);
		CFSafeRelease(socketSource);
		Safe(free,socketPath);
		CFRunLoopRun();
	}
}

void PerformService(char *udid, char *service, ...) {
	if ((udid && strlen(udid) == 0x28) && (service)) {
		uint32_t index = 0x0;
		for (index = 0x0; index < SDM_MD_Service_Count; index++) {
			char *ident = SDMMDServiceIdentifiers[index].identifier;
			char *shorthand = SDMMDServiceIdentifiers[index].shorthand;
			if ((strncmp(service, ident, strlen(ident)) == 0x0 && strlen(service) == strlen(ident)) || (strncmp(service, shorthand, strlen(shorthand)) == 0x0 && strlen(service) == strlen(shorthand))) {
				break;
			}
		}
		switch (index) {
			case SDM_MD_Service_SYSLOG_RELAY: {
				Syslog(udid);
				break;
			};
			/*
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
			*/
			case SDM_MD_Service_SPRINGBOARD_SERVICES: {
				SpringboardQuery(udid);
				break;
			};
			/*
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
			*/
			default: {
				if (index < SDM_MD_Service_Count) {
					CreateLocalSocket(udid, SDMMDServiceIdentifiers[index]);
				}
				break;
			};
		}
	} else {
		printf("invalid udid or service name\n");
	}
}

#endif
