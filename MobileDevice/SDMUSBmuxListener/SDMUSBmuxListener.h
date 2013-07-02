/*
 *  SDMUSBmuxListener.h
 *  SDM_MD_Demo
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

#ifndef _SDMUSBMUXLISTENER_H_
#define _SDMUSBMUXLISTENER_H_

#import <CoreFoundation/CoreFoundation.h>

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

struct USBmuxListenerClass {
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
} USBmuxListenerClass;

typedef struct USBmuxListenerClass SDM_USBMuxListenerClass;

#define SDMUSBMuxListenerRef SDM_USBMuxListenerClass*

typedef enum SDMUSBMuxPacketMessageType {
	kSDMUSBMuxPacketInvalidType = 0x0,
	kSDMUSBMuxPacketConnectType = 0x1,
	kSDMUSBMuxPacketListenType = 0x2,
	kSDMUSBMuxPacketResultType = 0x3,
	kSDMUSBMuxPacketAttachType = 0x4,
	kSDMUSBMuxPacketDetachType = 0x5,
	kSDMUSBMuxPacketLogsType = 0x6,
	kSDMUSBMuxPacketListDevicesType = 0x7,
	kSDMUSBMuxPacketListListenersType = 0x8
} SDMUSBMuxPacketMessageType;

#define kKnownSDMUSBMuxPacketMessageType 0x9

static CFStringRef SDMUSBMuxPacketMessage[kKnownSDMUSBMuxPacketMessageType] = {
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

typedef enum SDMUSBMuxResultCodeType {
	SDMUSBMuxResult_OK = 0x0,
	SDMUSBMuxResult_BadCommand = 0x1,
	SDMUSBMuxResult_BadDevice = 0x2,
	SDMUSBMuxResult_ConnectionRefused = 0x3,
	SDMUSBMuxResult_Unknown0 = 0x4,
	SDMUSBMuxResult_Unknown1 = 0x5,
	SDMUSBMuxResult_BadVersion = 0x6,
	SDMUSBMuxResult_Unknown2 = 0x7
} SDMUSBMuxResultCodeType;

#pragma mark -
#pragma mark Functions
#pragma mark -

SDMUSBMuxListenerRef SDMUSBMuxCreate();
void SDMUSBMuxStartListener(SDMUSBMuxListenerRef *listener);
void SDMUSBMuxListenerSend(SDMUSBMuxListenerRef listener, struct USBMuxPacket *packet);
void SDMUSBMuxListenerReceive(SDMUSBMuxListenerRef listener, struct USBMuxPacket *packet);
struct USBMuxPacket * SDMUSBMuxCreatePacketType(SDMUSBMuxPacketMessageType type, CFDictionaryRef payload);

void USBMuxPacketRelease(struct USBMuxPacket *packet);
#endif