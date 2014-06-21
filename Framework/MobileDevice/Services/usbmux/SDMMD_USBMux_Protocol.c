/*
 *  SDMMD_USBMux_Protocol.c
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

#ifndef _SDM_MD_USBMUX_PROTOCOL_C_
#define _SDM_MD_USBMUX_PROTOCOL_C_

#include "SDMMD_USBMux_Protocol.h"
#include "SDMMD_USBMuxListener.h"
#include <sys/socket.h>

void SDMMD_USBMuxSend(uint32_t sock, struct USBMuxPacket *packet);
void SDMMD_USBMuxReceive(uint32_t sock, struct USBMuxPacket *packet);

void SDMMD_USBMuxListenerSend(SDMMD_USBMuxListenerRef listener, struct USBMuxPacket **packet) {
	listener->ivars.semaphore = dispatch_semaphore_create(0x0);
	SDMMD_USBMuxSend(listener->ivars.socket, *packet);
	dispatch_semaphore_wait(listener->ivars.semaphore, (*packet)->timeout);
	
	CFMutableArrayRef updateWithRemove = CFArrayCreateMutableCopy(kCFAllocatorDefault, 0x0, listener->ivars.responses);
	struct USBMuxPacket *responsePacket = NULL;
	uint32_t removeCounter = 0x0;
	for (uint32_t i = 0x0; i < CFArrayGetCount(listener->ivars.responses); i++) {
		struct USBMuxPacket *response = (struct USBMuxPacket *)CFArrayGetValueAtIndex(listener->ivars.responses, i);
		if ((*packet)->body.tag == response->body.tag) {
			responsePacket = response;
			CFArrayRemoveValueAtIndex(updateWithRemove, i-removeCounter);
			removeCounter++;
		}
	}
	CFSafeRelease(listener->ivars.responses);
	listener->ivars.responses = CFArrayCreateMutableCopy(kCFAllocatorDefault, 0x0, updateWithRemove);
	CFSafeRelease(updateWithRemove);
	USBMuxPacketRelease(*packet);
	if (!responsePacket) {
		responsePacket = (struct USBMuxPacket *)calloc(0x1, sizeof(struct USBMuxPacket));
	}
	*packet = responsePacket;
	dispatch_release(listener->ivars.semaphore);
}

void SDMMD_USBMuxSend(uint32_t sock, struct USBMuxPacket *packet) {
	CFDataRef xmlData = CFPropertyListCreateXMLData(kCFAllocatorDefault, packet->payload);
	char *buffer = (char *)CFDataGetBytePtr(xmlData);
	ssize_t result = send(sock, &packet->body, sizeof(struct USBMuxPacketBody), 0x0);
	if (result == sizeof(struct USBMuxPacketBody)) {
		if (packet->body.length > result) {
			ssize_t payloadSize = packet->body.length - result;
			ssize_t remainder = payloadSize;
			while (remainder) {
				result = send(sock, &buffer[payloadSize-remainder], sizeof(char), 0x0);
				if (result != sizeof(char)) {
					break;
				}
				remainder -= result;
			}
		}
	}
	CFSafeRelease(xmlData);
}

void SDMMD_USBMuxListenerReceive(SDMMD_USBMuxListenerRef listener, struct USBMuxPacket *packet) {
	SDMMD_USBMuxReceive(listener->ivars.socket, packet);
}

void SDMMD_USBMuxReceive(uint32_t sock, struct USBMuxPacket *packet) {
	ssize_t result = recv(sock, &packet->body, sizeof(struct USBMuxPacketBody), 0x0);
	if (result == sizeof(struct USBMuxPacketBody)) {
		ssize_t payloadSize = packet->body.length - result;
		if (payloadSize) {
			char *buffer = calloc(0x1, payloadSize);
			ssize_t remainder = payloadSize;
			while (remainder) {
				result = recv(sock, &buffer[payloadSize-remainder], sizeof(char), 0x0);
				if (result != sizeof(char)) {
					break;
				}
				remainder -= result;
			}
			CFDataRef xmlData = CFDataCreate(kCFAllocatorDefault, (UInt8 *)buffer, payloadSize);
			packet->payload = CFPropertyListCreateFromXMLData(kCFAllocatorDefault, xmlData, kCFPropertyListImmutable, NULL);
			Safe(free,buffer);
			CFSafeRelease(xmlData);
		}
	}
}

#endif