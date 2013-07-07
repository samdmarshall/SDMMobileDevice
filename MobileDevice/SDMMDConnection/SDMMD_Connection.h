/*
 *  SDMMD_Connection.h
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

#ifndef _SDM_MD_CONNECTION_H_
#define _SDM_MD_CONNECTION_H_

#include <CoreFoundation/CoreFoundation.h>
#include "SDMMD_Error.h"
#include "SDMMD_AMDevice.h"

typedef struct AMConnectionClassHeader {
	unsigned char header[16];		// AMConnectionClass CF Header 
} __attribute ((packed)) AMConnectionClassHeader; // size 0x10

typedef struct AMConnectionClassBody {
	uint32_t socket;			// 16
	unsigned char unknown0[4];	// 20
	int32_t secure_context;		// 24
	unsigned char unknown1[4];	// 28
	int8_t one0;				// 32
	int8_t one1;				// 33
	unsigned char unknown2[2];	// 34
	int32_t device_id; 			// 36
	unsigned char unknown3[112];// 40
} __attribute__ ((packed)) AMConnectionClassBody; // size 0x98

typedef struct am_connection {
	struct AMConnectionClassHeader head;
	struct AMConnectionClassBody body;
} __attribute__ ((packed)) am_connection;

/* Classes */
typedef struct am_connection SDMMD_AMConnectionClass;

#define SDMMD_AMConnectionRef SDMMD_AMConnectionClass*

#pragma mark -
#pragma mark FUNCTIONS
#pragma mark -
SDMMD_AMConnectionRef SDMMD_AMDServiceConnectionCreate(CFAllocatorRef allocator, SDMMD_AMDeviceRef device, CFDictionaryRef dict);
sdmmd_return_t SDMMD_AMDServiceConnectionInvalidate(SDMMD_AMConnectionRef connection);

#endif