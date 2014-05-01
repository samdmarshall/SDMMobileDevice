/*
 *  SDMMD_AFCOperation.h
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

#ifndef _SDM_MD_AFCOPERATION_H_
#define _SDM_MD_AFCOPERATION_H_

#if 0

#include "SDMMD_AFCLock.h"
#include "SDMMD_AFCCondition.h"

#pragma mark -
#pragma mark TYPES
#pragma mark -

typedef struct AFCOperationClassHeader {
	unsigned char header[16];
} ATR_PACK AFCOperationClassHeader; // 0x10

typedef struct AFCOperationClassBody {
	void *context;							// 16
	int32_t state;							// 24
	int32_t a;								// 40
											// 48
											// 64
	void *packet_data;						// 72
	int32_t data_size;						// 80
	CFTypeRef d;							// 88 CFType
	CFTypeRef c;							// 96
	uint64_t b;								// 104 0x80
											// 112
	void *result;							// 240
	SDMMD_AFCLockRef lock0;					// 248
	SDMMD_AFCConditionRef condition; 		// 256 AFCConditionCreate();
} ATR_PACK AFCOperationClassBody; // 0xf8

typedef struct afc_operation {
	struct AFCOperationClassHeader base;
	struct AFCOperationClassBody ivars;
} ATR_PACK afc_operation;

typedef struct afc_operation SDMMD_AFCOperationClass;

#define SDMMD_AFCOperationRef SDMMD_AFCOperationClass*

#pragma mark -
#pragma mark FUNCTIONS
#pragma mark -

#endif
#endif