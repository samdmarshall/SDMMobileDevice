/*
 *  SDMMD_AFCDevice.h
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

#ifndef _SDM_MD_AFCDEVICE_H_
#define _SDM_MD_AFCDEVICE_H_

#if 0

#include <CoreFoundation/CoreFoundation.h>
#include "SDMMD_Error.h"
#include "SDMMD_AFCLock.h"
#include "SDMMD_AFCCondition.h"

#pragma mark -
#pragma mark TYPES
#pragma mark -

typedef struct AFCConnectionClassHeader {
	unsigned char header[16];
} ATR_PACK AFCConnectionClassHeader; // 0x10

typedef struct AFCConnectionClassBody {
	int32_t state;							// 16 set to 0x1
	SDMMD_AFCConditionRef cond0;			// 24
	int8_t is_valid1;						// 32
	SDMMD_AFCConditionRef cond_signal;		// 40
	CFSocketRef socket;						// 48
	int32_t handle;							// 56
	int8_t connection_active;				// 60
	int8_t dispose_secure_on_invalid;		// 61
	unsigned char padding0[2];				// 62		
	int32_t statusPtr;						// 64
	int8_t fatal_error;						// 68
	int32_t last_error;						// 72 last error
	int32_t operation_count;				// 80 on sending operation is incremented
	int32_t fs_block_size;					// 88
	int32_t socket_block_size;				// 96
	int32_t io_timeout;						// 104 set to 0x3c
	int32_t h;								// 112 set to 0x1
	int32_t context;						// 120
	int32_t i;								// 124
	int32_t j;								// 128
	int32_t k;								// 132
	SDMMD_AFCLockRef lock0;					// 136
	SDMMD_AFCLockRef lock1;					// 144
	CFMutableArrayRef operation_enqueue;	// 152
	CFMutableArrayRef operation_dequeue;	// 160
	CFMutableSetRef file_descriptors;		// 168
	CFMutableSetRef directory_enumerator; 	// 176
	CFRunLoopSourceRef source;				// 184
	CFRunLoopRef runloop;					// 200
	int32_t secure_context;					// 208
	void *callback;							// 224
	SDMMD_AFCLockRef lock2;					// 248
	int32_t queue;							// 1256
	CFTypeRef fileDescriptors0;				// 1260
	CFTypeRef fileDescriptors1;				// 1264
} ATR_PACK AFCConnectionClassBody; // 0x4f0

typedef struct afc_connection {
	struct AFCConnectionClassHeader base;
	struct AFCConnectionClassBody ivars;
} ATR_PACK afc_connection;

typedef struct afc_connection SDMMD_AFCConnectionClass;

#define SDMMD_AFCConnectionRef SDMMD_AFCConnectionClass*

CFTypeID SDMMD_AFCConnectionGetTypeID(void);

#pragma mark -
#pragma mark FUNCTIONS
#pragma mark -

void SDMMD_AFCConnectionRefClassInitialize(void);

SDMMD_AFCConnectionRef SDMMD_AFCConnectionCreate(void*a, uint32_t sock,void*c,void*d,void*e);

#endif

#endif