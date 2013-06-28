/*
 *  SDMUSBmuxListener.h
 *  SDM_MD_Demo
 *
 *  Created by sam on 6/10/13.
 *  Copyright 2013 Sam Marshall. All rights reserved.
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