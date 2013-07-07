/*
 *  SDMMD_AMDevice.h
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

#ifndef _SDM_MD_ADMDEVICE_H_
#define _SDM_MD_ADMDEVICE_H_

#include <CoreFoundation/CoreFoundation.h>
#include <openssl/ssl.h>
#include "SDMMD_Error.h"
#include "SDMMD_Keys.h"
#include "SDMMD_MRecoveryModeDevice.h"
#include "SDMMD_MRUSBDevice.h"
#include "SDMMD_MDFUModeDevice.h"
#include "SDMMD_MRestoreModeDevice.h"
#include "SDMMD_MRestorableDevice.h"

typedef struct SDMMD_lockdown_conn {
	uint64_t connection;			// 0
	SSL *ssl; 						// 8
	uint64_t unknown0;				// 16
	uint64_t unknown1;				// 24
} __attribute__ ((packed)) SDMMD_lockdown_conn;

typedef struct AMDeviceClassHeader {
	unsigned char header[16];		// AMDeviceClass CF Header 
} __attribute ((packed)) AMDeviceClassHeader; // size 0x10

typedef struct AMDeviceClassBody {
	int32_t device_id;					// 16
	int32_t location_id;				// 20
	uint16_t product_id;				// 24
	int16_t padding0;					// 26
	int32_t unknown1;					// 28
	CFStringRef unique_device_id;		// 32
	int32_t unknown2;					// 36
	int32_t connection_type;			// 40 (1 for USB, 2 for WiFi)
	unsigned char unknown3[4];			// 44
	SDMMD_lockdown_conn *lockdown_conn; // 48
	unsigned char unknown4[4];			// 52
	int32_t session;					// 56 needs to be not zero in AMDeviceSecureStartService  -- connection
	int32_t padding1;					// 60
	pthread_mutex_t mutex_lock;			// 64
	unsigned char unknown5[60];			// 68
	CFStringRef service_name;			// 128 bonjour service name
	unsigned int unknown6;				// 132
	int32_t interface_index;			// 136
	int8_t device_active;				// 140
	unsigned char unknown7[3];			// 141
	int32_t unknown8;					// 144
	unsigned char unknown9[4];			// 148
	CFDataRef network_address;			// 152 stores a sockaddr_storage
	unsigned char unknown10[4];			// 156
	CFDataRef unknown11;				// 160 
	unsigned char unknown12[4];			// 164
} __attribute__ ((packed)) AMDeviceClassBody; // size 0x98

typedef struct sdmmd_am_device {
	struct AMDeviceClassHeader base;
	struct AMDeviceClassBody ivars;
} __attribute__ ((packed)) sdmmd_am_device;

/* Classes */
typedef struct sdmmd_am_device SDM_AMDeviceClass;

#define SDMMD_AMDeviceRef SDM_AMDeviceClass*

#pragma mark -

void* SDMMD_AMDeviceActivate(SDMMD_AMDeviceRef device, CFDictionaryRef options);
void* SDMMD_AMDeviceDeactivate(SDMMD_AMDeviceRef device);

sdmmd_return_t SDMMD_AMDeviceConnect(SDMMD_AMDeviceRef device);
sdmmd_return_t SDMMD_AMDeviceDisconnect(SDMMD_AMDeviceRef device);

bool SDMMD_AMDeviceIsValid(SDMMD_AMDeviceRef device);

bool SDMMD_AMDeviceIsPaired(SDMMD_AMDeviceRef device);

sdmmd_return_t SDMMD_AMDeviceStartSession(SDMMD_AMDeviceRef device);
sdmmd_return_t SDMMD_AMDeviceStopSession(SDMMD_AMDeviceRef device);

sdmmd_return_t SDMMD_AMDeviceStartService(SDMMD_AMDeviceRef device, CFStringRef service_name, CFDictionaryRef options, uint32_t *handle);

uint32_t SDMMD_AMDeviceUSBDeviceID(SDMMD_AMDeviceRef device);
uint32_t SDMMD_AMDeviceUSBLocationID(SDMMD_AMDeviceRef device);
uint16_t SDMMD_AMDeviceUSBProductID(SDMMD_AMDeviceRef device);
uint32_t SDMMD_AMDeviceGetConnectionID(SDMMD_AMDeviceRef device);

CFTypeRef SDMMD_DeviceCopyValue(SDMMD_AMDeviceRef device, CFStringRef domain, CFStringRef key);

SDMMD_AMDeviceRef SDMMD_AMDeviceCreateFromProperties(CFDictionaryRef dict);

#endif