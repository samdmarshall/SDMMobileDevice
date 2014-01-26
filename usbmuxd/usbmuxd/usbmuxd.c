//
//  usbmuxd.c
//  usbmuxd
//
//  Created by Sam Marshall on 1/25/14.
//  Copyright (c) 2014 Sam Marshall. All rights reserved.
//

#ifndef usbmuxd_usbmuxd_c
#define usbmuxd_usbmuxd_c

#include "usbmuxd.h"
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <Security/Security.h>

static char *sdm_usbmuxd_path = "/tmp/sdm_usbmuxd";

USBMuxAgentRef MuxAgent;

USBMuxAgentRef USBMuxAgentCreate() {
	USBMuxAgentRef agent = (USBMuxAgentRef)calloc(1, sizeof(struct USBMuxAgentClass));
	agent->socket = 0x0;
	agent->isActive = false;
	agent->socketQueue = dispatch_queue_create("com.samdmarshall.sdm_usbmux.socketQueue", NULL);
	return agent;
}

uint32_t SDM_USBMux_SocketCreate() {
	unlink(sdm_usbmuxd_path);
	struct sockaddr_un local;
	uint32_t sock = socket(AF_UNIX, SOCK_STREAM, 0x0);
	if (sock != 0xff) {
		local.sun_family = AF_UNIX;
		strcpy(local.sun_path, sdm_usbmuxd_path);
		unlink(local.sun_path);
		socklen_t len = (socklen_t)strlen(local.sun_path) + sizeof(local.sun_family);
		if (bind(sock, (struct sockaddr *)&local, len) == -1) {
			perror("bind");
			exit(1);
		}
		
		if (listen(sock, 0x5) == -1) {
			perror("listen");
			exit(1);
		}

	}
	return sock;
}

int acquireTaskForPortRight() {
	AuthorizationRef authorization;
	OSStatus status = AuthorizationCreate(NULL, kAuthorizationEmptyEnvironment, kAuthorizationFlagDefaults, &authorization);
	if (status != 0x0) {
		printf("Error creating authorization reference\n");
		return 0xffffffff;
	}
	AuthorizationItem systemRight = { kAuthorizationRightExecute, 0x0, 0x0, 0x0 };
	AuthorizationItem taskRight = { "system.privilege.taskport", 0x0, 0x0, 0x0 };
	AuthorizationItem items[] = { systemRight, taskRight };
	AuthorizationRights rights = { 0x2, items };
	AuthorizationFlags flags = kAuthorizationFlagInteractionAllowed | kAuthorizationFlagExtendRights | kAuthorizationFlagPreAuthorize;
	
	status = AuthorizationCopyRights(authorization, &rights, NULL, flags, NULL);
	if (status != 0x0) {
		printf("Error authorizing current process with right to call task_for_pid()\n");
		return 0xffffffff;
	}
	return 0x0;
}

void StartMux() {
	int status = acquireTaskForPortRight();
	if (status == 0x0) {
		setuid(0x0);
		MuxAgent = USBMuxAgentCreate();
		if (MuxAgent) {
			MuxAgent->socket = SDM_USBMux_SocketCreate();
			MuxAgent->socketSource = dispatch_source_create(DISPATCH_SOURCE_TYPE_READ, MuxAgent->socket, 0x0, MuxAgent->socketQueue);
			dispatch_source_set_event_handler((*MuxAgent)->socketSource, ^{
				
			});
			dispatch_source_set_cancel_handler((*MuxAgent)->socketSource, ^{
				printf("socketSourceEventCancelHandler: source canceled\n");
			});
			dispatch_resume(MuxAgent->socketSource);
			
			//sub_d8b5();
			
			CFRunLoopRun();
		}
	}
}

#endif