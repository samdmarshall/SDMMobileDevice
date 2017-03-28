/*
 *  SDMMD_USBmuxListener_Types.c
 *  SDMMobileDevice
 *
 * Copyright (c) 2014, Samantha Marshall
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
 * 3. Neither the name of Samantha Marshall nor the names of its contributors may be used to endorse or promote products derived from this
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

#include <CoreFoundation/CoreFoundation.h>
#include "SDMMD_USBMuxListener_Types.h"

CFStringRef kSDMMD_USBMuxListenerDeviceAttachedNotification;
CFStringRef kSDMMD_USBMuxListenerDeviceAttachedNotificationFinished;
CFStringRef kSDMMD_USBMuxListenerDeviceDetachedNotification;
CFStringRef kSDMMD_USBMuxListenerDeviceDetachedNotificationFinished;
CFStringRef kSDMMD_USBMuxListenerStoppedListenerNotification;

CFStringRef SDMMD_USBMuxPacketMessage[kSDMMD_USBMuxPacketMessageCount];

void SDMMD_USBMuxListener_Types_Initialize()
{
	kSDMMD_USBMuxListenerDeviceAttachedNotification = CFSTR("SDMMD_USBMuxListenerDeviceAttachedNotification");
	kSDMMD_USBMuxListenerDeviceAttachedNotificationFinished = CFSTR("SDMMD_USBMuxListenerDeviceAttachedNotificationFinished");
	kSDMMD_USBMuxListenerDeviceDetachedNotification = CFSTR("SDMMD_USBMuxListenerDeviceDetachedNotification");
	kSDMMD_USBMuxListenerDeviceDetachedNotificationFinished = CFSTR("SDMMD_USBMuxListenerDeviceDetachedNotificationFinished");
	kSDMMD_USBMuxListenerStoppedListenerNotification = CFSTR("SDMMD_USBMuxListenerStoppedListenerNotification");

	SDMMD_USBMuxPacketMessage[kSDMMD_USBMuxPacketInvalidType] = CFSTR("Invalid");
	SDMMD_USBMuxPacketMessage[kSDMMD_USBMuxPacketConnectType] = CFSTR("Connect");
	SDMMD_USBMuxPacketMessage[kSDMMD_USBMuxPacketListenType] = CFSTR("Listen");
	SDMMD_USBMuxPacketMessage[kSDMMD_USBMuxPacketResultType] = CFSTR("Result");
	SDMMD_USBMuxPacketMessage[kSDMMD_USBMuxPacketAttachType] = CFSTR("Attached");
	SDMMD_USBMuxPacketMessage[kSDMMD_USBMuxPacketDetachType] = CFSTR("Detached");
	SDMMD_USBMuxPacketMessage[kSDMMD_USBMuxPacketLogsType] = CFSTR("Logs");
	SDMMD_USBMuxPacketMessage[kSDMMD_USBMuxPacketListDevicesType] = CFSTR("ListDevices");
	SDMMD_USBMuxPacketMessage[kSDMMD_USBMuxPacketListListenersType] = CFSTR("ListListeners");
	SDMMD_USBMuxPacketMessage[kSDMMD_USBMuxPacketReadBUIDType] = CFSTR("ReadBUID");
	SDMMD_USBMuxPacketMessage[kSDMMD_USBMuxPacketReadPairRecordType] = CFSTR("ReadPairRecord");
	SDMMD_USBMuxPacketMessage[kSDMMD_USBMuxPacketSavePairRecordType] = CFSTR("SavePairRecord");
	SDMMD_USBMuxPacketMessage[kSDMMD_USBMuxPacketDeletePairRecordType] = CFSTR("DeletePairRecord");
}
