/*
 *  SDMMD_USBmuxListener_Types.h
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

#ifndef _SDM_MD_USBMUXLISTENER_TYPES_H_
#define _SDM_MD_USBMUXLISTENER_TYPES_H_

#include <CoreFoundation/CoreFoundation.h>

// Initialize static CFString key arrays
void SDMMD_USBMuxListener_Types_Initialize(void);

extern CFStringRef kSDMMD_USBMuxListenerDeviceAttachedNotification;
extern CFStringRef kSDMMD_USBMuxListenerDeviceAttachedNotificationFinished;
extern CFStringRef kSDMMD_USBMuxListenerDeviceDetachedNotification;
extern CFStringRef kSDMMD_USBMuxListenerDeviceDetachedNotificationFinished;
extern CFStringRef kSDMMD_USBMuxListenerStoppedListenerNotification;

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

typedef enum SDMMD_USBMuxPacketMessageType {
	kSDMMD_USBMuxPacketInvalidType = 0x0,
	kSDMMD_USBMuxPacketConnectType = 0x1,
	kSDMMD_USBMuxPacketListenType = 0x2,
	kSDMMD_USBMuxPacketResultType = 0x3,
	kSDMMD_USBMuxPacketAttachType = 0x4,
	kSDMMD_USBMuxPacketDetachType = 0x5,
	kSDMMD_USBMuxPacketLogsType = 0x6,
	kSDMMD_USBMuxPacketListDevicesType = 0x7,
	kSDMMD_USBMuxPacketListListenersType = 0x8,
	kSDMMD_USBMuxPacketReadBUIDType,
	kSDMMD_USBMuxPacketReadPairRecordType,
	kSDMMD_USBMuxPacketSavePairRecordType,
	kSDMMD_USBMuxPacketDeletePairRecordType,
	kSDMMD_USBMuxPacketMessageCount
} SDMMD_USBMuxPacketMessageType;

#define kKnownSDMMD_USBMuxPacketMessageType kSDMMD_USBMuxPacketMessageCount

extern CFStringRef SDMMD_USBMuxPacketMessage[kSDMMD_USBMuxPacketMessageCount];

typedef enum SDMMD_USBMuxResultCodeType {
	SDMMD_USBMuxResult_OK = 0x0,
	SDMMD_USBMuxResult_BadCommand = 0x2d,
	SDMMD_USBMuxResult_BadDevice = 0x6,
	SDMMD_USBMuxResult_ConnectionRefused = 0x3d,
	SDMMD_USBMuxResult_Unknown0 = -1,
	SDMMD_USBMuxResult_BadMessage = 0x16,
	SDMMD_USBMuxResult_BadVersion = 0x49,
	SDMMD_USBMuxResult_Unknown2 = 0x4b
} SDMMD_USBMuxResultCodeType;

#endif