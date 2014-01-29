//
//  usbmuxd.h
//  usbmuxd
//
//  Created by Sam Marshall on 1/25/14.
//  Copyright (c) 2014 Sam Marshall. All rights reserved.
//

#ifndef usbmuxd_usbmuxd_h
#define usbmuxd_usbmuxd_h

#include <CoreFoundation/CoreFoundation.h>
#include "Core.h"

struct USBMuxPacketBody {
	uint32_t length;
	uint32_t reserved;
	uint32_t type;
	uint32_t tag;
} ATR_PACK USBMuxPacketBody;

struct USBMuxPacket {
	dispatch_time_t timeout;
	struct USBMuxPacketBody body;
	CFPropertyListRef payload;
} ATR_PACK USBMuxPacket;

typedef void (*callbackFunction)(void *, struct USBMuxPacket *);

struct USBMuxAgentClass {
	uint32_t socket;
	bool isActive;
	dispatch_queue_t socketQueue;
	dispatch_source_t socketSource;
	dispatch_semaphore_t semaphore;
} USBMuxAgentClass;

typedef enum USBMuxPacketMessageType {
	kUSBMuxPacketInvalidType = 0x0,
	kUSBMuxPacketConnectType = 0x1,
	kUSBMuxPacketListenType = 0x2,
	kUSBMuxPacketResultType = 0x3,
	kUSBMuxPacketAttachType = 0x4,
	kUSBMuxPacketDetachType = 0x5,
	kUSBMuxPacketLogsType = 0x6,
	kUSBMuxPacketListDevicesType = 0x7,
	kUSBMuxPacketListListenersType = 0x8,
	kUSBMuxPacketReadBUIDType,
	kUSBMuxPacketReadPairRecordType,
	kUSBMuxPacketSavePairRecordType,
	kUSBMuxPacketDeletePairRecordType,
	kUSBMuxPacketMessageCount
} USBMuxPacketMessageType;

#define kKnownUSBMuxPacketMessageType kUSBMuxPacketMessageCount

static CFStringRef USBMuxPacketMessage[kUSBMuxPacketMessageCount] = {
	CFSTR("Invalid"),
	CFSTR("Connect"),
	CFSTR("Listen"),
	CFSTR("Result"),
	CFSTR("Attached"),
	CFSTR("Detached"),
	CFSTR("Logs"),
	CFSTR("ListDevices"),
	CFSTR("ListListeners"),
	CFSTR("ReadBUID"),
	CFSTR("ReadPairRecord"),
	CFSTR("SavePairRecord"),
	CFSTR("DeletePairRecord")
};

typedef enum USBMuxResultCodeType {
	USBMuxResult_OK = 0x0,
	USBMuxResult_BadCommand = 0x2d,
	USBMuxResult_BadDevice = 0x6,
	USBMuxResult_ConnectionRefused = 0x3d,
	USBMuxResult_Unknown0 = 0xffffffff,
	USBMuxResult_BadMessage = 0x16,
	USBMuxResult_BadVersion = 0x49,
	USBMuxResult_Unknown2 = 0x4b
} USBMuxResultCodeType;

typedef struct USBMuxAgentClass* USBMuxAgentRef;

void StartMux();

#endif
