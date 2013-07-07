/*
 *  SDMMD_USBmuxListener.h
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

#ifndef _SDM_MD_USBMUXLISTENER_H_
#define _SDM_MD_USBMUXLISTENER_H_

#include <CoreFoundation/CoreFoundation.h>

#pragma mark -
#pragma mark TYPES
#pragma mark -

typedef struct USBMuxPacketBody {
	uint32_t length;
	uint32_t reserved;
	uint32_t type;
	uint32_t tag;
} __attribute__ ((packed)) USBMuxPacketBody;

typedef struct USBMuxPacket {
	dispatch_time_t timeout;
	struct USBMuxPacketBody body;
	CFPropertyListRef payload;
} __attribute__ ((packed)) USBMuxPacket;

typedef void (*callbackFunction)(void *, struct USBMuxPacket *);

struct USBMuxListenerClass {
	uint32_t socket;
	bool isActive;
	dispatch_queue_t socketQueue;
	dispatch_source_t socketSource;
	dispatch_semaphore_t semaphore;
	callbackFunction responseCallback;
	callbackFunction attachedCallback;
	callbackFunction detachedCallback;
	callbackFunction logsCallback;
	callbackFunction deviceListCallback;
	callbackFunction listenerListCallback;
	CFMutableArrayRef responses;
} USBMuxListenerClass;

typedef struct USBMuxListenerClass SDM_USBMuxListenerClass;

#define SDMMD_USBMuxListenerRef SDM_USBMuxListenerClass*

typedef enum SDMMD_USBMuxPacketMessageType {
	kSDMMD_USBMuxPacketInvalidType = 0x0,
	kSDMMD_USBMuxPacketConnectType = 0x1,
	kSDMMD_USBMuxPacketListenType = 0x2,
	kSDMMD_USBMuxPacketResultType = 0x3,
	kSDMMD_USBMuxPacketAttachType = 0x4,
	kSDMMD_USBMuxPacketDetachType = 0x5,
	kSDMMD_USBMuxPacketLogsType = 0x6,
	kSDMMD_USBMuxPacketListDevicesType = 0x7,
	kSDMMD_USBMuxPacketListListenersType = 0x8
} SDMMD_USBMuxPacketMessageType;

#define kKnownSDMMD_USBMuxPacketMessageType 0x9

static CFStringRef SDMMD_USBMuxPacketMessage[kKnownSDMMD_USBMuxPacketMessageType] = {
	CFSTR("Invalid"),
	CFSTR("Connect"),
	CFSTR("Listen"),
	CFSTR("Result"),
	CFSTR("Attached"),
	CFSTR("Detached"),
	CFSTR("Logs"),
	CFSTR("ListDevices"),
	CFSTR("ListListeners")
};

typedef enum SDMMD_USBMuxResultCodeType {
	SDMMD_USBMuxResult_OK = 0x0,
	SDMMD_USBMuxResult_BadCommand = 0x1,
	SDMMD_USBMuxResult_BadDevice = 0x2,
	SDMMD_USBMuxResult_ConnectionRefused = 0x3,
	SDMMD_USBMuxResult_Unknown0 = 0x4,
	SDMMD_USBMuxResult_Unknown1 = 0x5,
	SDMMD_USBMuxResult_BadVersion = 0x6,
	SDMMD_USBMuxResult_Unknown2 = 0x7
} SDMMD_USBMuxResultCodeType;

#pragma mark -
#pragma mark Functions
#pragma mark -

SDMMD_USBMuxListenerRef SDMMD_USBMuxCreate();
void SDMMD_USBMuxClose(SDMMD_USBMuxListenerRef listener);
void SDMMD_USBMuxStartListener(SDMMD_USBMuxListenerRef *listener);
void SDMMD_USBMuxListenerSend(SDMMD_USBMuxListenerRef listener, struct USBMuxPacket *packet);
void SDMMD_USBMuxListenerReceive(SDMMD_USBMuxListenerRef listener, struct USBMuxPacket *packet);

struct USBMuxPacket * SDMMD_USBMuxCreatePacketType(SDMMD_USBMuxPacketMessageType type, CFDictionaryRef payload);
void USBMuxPacketRelease(struct USBMuxPacket *packet);

#endif