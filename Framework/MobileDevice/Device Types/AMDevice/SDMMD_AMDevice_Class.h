/*
 *  SDMMD_AMDevice_Class.h
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

#ifndef _SDM_MD_ADMDEVICE_CLASS_H_
#define _SDM_MD_ADMDEVICE_CLASS_H_

#include <CoreFoundation/CoreFoundation.h>
#include "Core.h"
#include "CFRuntime.h"
#include <openssl/ssl.h>

// Please do not access ivars of the AMDevice objects if you can avoid it, they are subject to change.

typedef struct SDMMD_lockdown_conn {
	uint64_t connection;			// 0
	SSL *ssl; 						// 8
	uint64_t *pointer;				// 16
	uint64_t length;				// 24
} ATR_PACK SDMMD_lockdown_conn;

struct AMDeviceClassHeader {
	unsigned char header[16];		// AMDeviceClass CF Header
} __attribute ((packed)) AMDeviceClassHeader; // size 0x10

enum {
	kAMDeviceConnectionTypeWiFi = 0,
	kAMDeviceConnectionTypeUSB = 1,
};
typedef int32_t AMDeviceConnectionType;

struct AMDeviceClassBody {
	int32_t device_id;							// 16
	int32_t location_id;						// 20
	uint16_t product_id;						// 24
	int16_t padding0;							// 26
	int32_t unknown1;							// 28
	CFStringRef unique_device_id;				// 32
	AMDeviceConnectionType connection_type;		// 40 (0 for USB, 1 for WiFi)
	//unsigned char unknown3[4];					// 44
	SDMMD_lockdown_conn *lockdown_conn;			// 48
	CFStringRef session;						// 56 needs to be not zero in AMDeviceSecureStartService  -- connection
	char mutx[4];								// 64 "MUTX" string
	pthread_mutex_t mutex_lock;					// 68
	CFStringRef service_name;					// 128 bonjour service name
	//unsigned int unknown6;						// 132
	int32_t interface_index;					// 136
	int8_t device_active;						// 140
	unsigned char unknown7[3];					// 141
	int32_t unknown8;							// 144
	unsigned char unknown9[4];					// 148
	CFDataRef network_address;					// 152 stores a sockaddr_storage
	//unsigned char unknown10[4];					// 156
	CFDataRef unknown11;						// 160
	//unsigned char unknown12[4];					// 164
} ATR_PACK AMDeviceClassBody; // size 0x98

struct sdmmd_am_device {
	CFRuntimeBase base;
	struct AMDeviceClassBody ivars;
} ATR_PACK sdmmd_am_device;

typedef struct sdmmd_am_device* SDMMD_AMDeviceRef;

void SDMMD_AMDeviceRefClassInitialize();

CFTypeID SDMMD_AMDeviceRefGetTypeID();

/*!
 @function SDMMD_AMDeviceCreateEmpty
 @discussion
 Creating an empty device object, used in conjunction with SDMMD_AMDeviceCreateFromProperties(), returns a SDMMD_AMDeviceRef object.
 */
SDMMD_AMDeviceRef SDMMD_AMDeviceCreateEmpty();

#endif