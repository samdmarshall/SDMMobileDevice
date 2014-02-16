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

static char *sdm_usbmuxd_path = "/var/run/sdm_usbmuxd";

USBMuxAgentRef MuxAgent;

USBMuxAgentRef USBMuxAgentCreate() {
	USBMuxAgentRef agent = (USBMuxAgentRef)calloc(1, sizeof(struct USBMuxAgentClass));
	agent->socket = -1;
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

void USBMuxSend(uint32_t sock, struct USBMuxPacket *packet) {
	CFDataRef xmlData = CFPropertyListCreateXMLData(kCFAllocatorDefault, packet->payload);
	char *buffer = (char *)CFDataGetBytePtr(xmlData);
	ssize_t result = send(sock, &packet->body, sizeof(struct USBMuxPacketBody), 0x0);
	if (result == sizeof(struct USBMuxPacketBody)) {
		if (packet->body.length > result) {
			ssize_t payloadSize = packet->body.length - result;
			ssize_t remainder = payloadSize;
			while (remainder) {
				result = send(sock, &buffer[payloadSize-remainder], sizeof(char), 0x0);
				if (result != sizeof(char))
					break;
				remainder -= result;
			}
		}
	}
	CFSafeRelease(xmlData);
}

void USBMuxReceive(uint32_t sock, struct USBMuxPacket *packet) {
	ssize_t result = recv(sock, &packet->body, sizeof(struct USBMuxPacketBody), 0x0);
	if (result == sizeof(struct USBMuxPacketBody)) {
		ssize_t payloadSize = packet->body.length - result;
		if (payloadSize) {
			char *buffer = calloc(0x1, payloadSize);
			ssize_t remainder = payloadSize;
			while (remainder) {
				result = recv(sock, &buffer[payloadSize-remainder], sizeof(char), 0x0);
				if (result != sizeof(char))
					break;
				remainder -= result;
			}
			CFDataRef xmlData = CFDataCreate(kCFAllocatorDefault, (UInt8 *)buffer, payloadSize);
			packet->payload = CFPropertyListCreateFromXMLData(kCFAllocatorDefault, xmlData, kCFPropertyListImmutable, NULL);
			Safe(free,buffer);
			CFSafeRelease(xmlData);
		}
	}
}

void StartMux() {
	MuxAgent = USBMuxAgentCreate();
	if (MuxAgent) {
		MuxAgent->socket = SDM_USBMux_SocketCreate();
		MuxAgent->socketSource = dispatch_source_create(DISPATCH_SOURCE_TYPE_READ, MuxAgent->socket, 0x0, MuxAgent->socketQueue);
		dispatch_source_set_event_handler(MuxAgent->socketSource, ^{
			struct USBMuxPacket *packet = (struct USBMuxPacket *)calloc(0x1, sizeof(struct USBMuxPacket));
			USBMuxReceive(MuxAgent->socket, packet);
			if (CFPropertyListIsValid(packet->payload, kCFPropertyListXMLFormat_v1_0)) {
				if (CFDictionaryContainsKey(packet->payload, CFSTR("MessageType"))) {
					CFStringRef type = CFDictionaryGetValue(packet->payload, CFSTR("MessageType"));
					if (CFStringCompare(type, USBMuxPacketMessage[kUSBMuxPacketResultType], 0x0) == 0x0) {
						
					} else if (CFStringCompare(type, USBMuxPacketMessage[kUSBMuxPacketAttachType], 0x0) == 0x0) {

					} else if (CFStringCompare(type, USBMuxPacketMessage[kUSBMuxPacketDetachType], 0x0) == 0x0) {
						
					}
				} else {
					if (CFDictionaryContainsKey(packet->payload, CFSTR("Logs"))) {
						
					} else if (CFDictionaryContainsKey(packet->payload, CFSTR("DeviceList"))) {
						
					} else if (CFDictionaryContainsKey(packet->payload, CFSTR("ListenerList"))) {
						
					} else {
						
					}
				}
			} else {
                printf("socketSourceEventHandler: failed to decodeCFPropertyList from packet payload\n");
            }
		});
		dispatch_source_set_cancel_handler(MuxAgent->socketSource, ^{
			printf("socketSourceEventCancelHandler: source canceled\n");
		});
		dispatch_resume(MuxAgent->socketSource);
		
		//sub_d8b5();
		
		CFRunLoopRun();
	}
}

#endif