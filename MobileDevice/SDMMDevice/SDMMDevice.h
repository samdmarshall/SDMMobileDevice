/*
 *  SDMMDevice.h
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

#ifndef _SDMMDevice_H_
#define _SDMMDevice_H_

#import <CoreFoundation/CoreFoundation.h>
#import "CFRuntime.h"
#import "SDMMDError.h"
#import <dlfcn.h>

#pragma mark -
#pragma mark TYPES
#pragma mark -

/* structs */

/*typedef struct __CFRuntimeClass {	// Version 0 struct
	CFIndex version; // 0x0
    const char *className; // 0x000bc0d2
    void (*init)(CFTypeRef cf); // NULL
    CFTypeRef (*copy)(CFAllocatorRef allocator, CFTypeRef cf); // NULL
    void (*dealloc)(CFTypeRef cf); // 0x0006fdd4
    Boolean (*equal)(CFTypeRef cf1, CFTypeRef cf2); // 0x0006fe85
    CFHashCode (*hash)(CFTypeRef cf); // NULL
    CFStringRef (*copyFormattingDesc)(CFTypeRef cf, CFDictionaryRef formatOptions);	// 0x0006ff0d
    CFStringRef (*copyDebugDesc)(CFTypeRef cf);	// 0x0006ff17
    void (*reclaim)(CFTypeRef cf); // NULL
} CFRuntimeClass;*/

CFRuntimeClass* SDM_AMDeviceRuntimeClassCreate();

#define SDM_AMDeviceRuntimeClass SDM_AMDeviceRuntimeClassCreate()


typedef struct AMDeviceClassHeader {
	unsigned char header[16];		// AMDeviceClass CF Header 
} __attribute ((packed)) AMDeviceClassHeader; // size 0x10

typedef struct AMDeviceClassBody {
	int32_t device_id;				// 16
	int32_t location_id;			// 20
	int16_t product_id;				// 24
	int16_t padding0;				// 26
	int32_t unknown1;				// 28
	CFStringRef unique_device_id;	// 32
	int32_t unknown2;				// 36
	int32_t connection_type;		// 40 (1 for USB, 2 for WiFi)
	unsigned char unknown3[4];		// 44
	int32_t lockdown_conn;			// 48
	unsigned char unknown4[4];		// 52
	int32_t not_zero;				// 56 needs to be not zero in AMDeviceSecureStartService 
	int32_t padding1;				// 60
	pthread_mutex_t mutex_lock;		// 64
	unsigned char unknown5[60];		// 68
	CFStringRef service_name;		// 128 bonjour service name
	unsigned int unknown6;			// 132
	int32_t interface_index;		// 136
	int8_t device_active;			// 140
	unsigned char unknown7[3];		// 141
	int32_t unknown8;				// 144
	unsigned char unknown9[4];		// 148
	CFDataRef network_address;		// 152 stores a sockaddr_storage
	unsigned char unknown10[4];		// 156
	CFDataRef unknown11;			// 160 
	unsigned char unknown12[4];		// 164
} __attribute__ ((packed)) AMDeviceClassBody; // size 0x98

typedef struct sdm_am_device {
	CFRuntimeBase base;
	struct AMDeviceClassBody ivars;
} __attribute__ ((packed)) sdm_am_device;

/* Classes */
typedef struct sdm_am_device SDM_AMDeviceClass;

#define SDM_AMDeviceRef SDM_AMDeviceClass*

#pragma mark -
#pragma mark FUNCTIONS
#pragma mark -


SDM_AMDeviceRef SDM_AMDeviceCreateEmpty();
SDM_AMDeviceRef SDM_AMDeviceCreateFromProperties(CFDictionaryRef dictionary);

bool SDM_AMDeviceIsAttached(SDM_AMDeviceRef device);


sdmmd_return_t AMDeviceActivate(SDM_AMDeviceRef device, CFMutableDictionaryRef);
sdmmd_return_t AMDeviceDeactivate(SDM_AMDeviceRef device);

sdmmd_return_t AMDeviceConnect(SDM_AMDeviceRef device);
sdmmd_return_t AMDeviceDisconnect(SDM_AMDeviceRef device);
bool AMDeviceIsPaired(SDM_AMDeviceRef device);
sdmmd_return_t AMDeviceValidatePairing(SDM_AMDeviceRef device);
sdmmd_return_t AMDeviceStartSession(SDM_AMDeviceRef device);
sdmmd_return_t AMDeviceStopSession(SDM_AMDeviceRef device);
sdmmd_return_t AMDeviceStartService(SDM_AMDeviceRef device, CFStringRef service_name, uint32_t *handle, unsigned int *unknown);

CFStringRef AMDeviceCopyDeviceIdentifier(SDM_AMDeviceRef device);
uint32_t AMDeviceGetConnectionID(SDM_AMDeviceRef device);
uint16_t AMDeviceUSBProductID(SDM_AMDeviceRef device);

CFArrayRef SDM_AMDCreateDeviceList();
sdmmd_return_t SDM_AMDeviceConnectByAddressAndPort(SDM_AMDeviceRef device);
SDM_AMDeviceRef SDM_AMDeviceCreateCopy(SDM_AMDeviceRef device);
SDM_AMDeviceRef AMDeviceCreateFromProperties(CFDictionaryRef dict);

sdmmd_sim_return_t SDMMDSIMStatusCode(SDM_AMDeviceRef device);
sdmmd_activation_return_t SDMMDActivationStatus(SDM_AMDeviceRef device);

#endif