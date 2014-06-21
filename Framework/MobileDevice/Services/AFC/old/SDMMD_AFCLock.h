/*
 *  SDMMD_AFCLock.h
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

#ifndef _SDM_MD_AFCLOCK_H_
#define _SDM_MD_AFCLOCK_H_

#if 0

#include "SDMMD_Error.h"
#include <stdint.h>
#include <pthread.h>

#pragma mark -
#pragma mark TYPES
#pragma mark -

typedef struct AFCLockClassHeader {
	unsigned char header[16];
} ATR_PACK AFCLockClassHeader; // 0x10

typedef struct AFCLockClassBody {
	pthread_mutex_t mutex_lock;	// 16
	unsigned char padding[44];	// 20
} ATR_PACK AFCLockClassBody; // 0x40

typedef struct afc_lock {
	struct AFCLockClassHeader base;
	struct AFCLockClassBody ivars;
} ATR_PACK afc_lock;

typedef struct afc_lock SDMMD_AFCLockClass;

#define SDMMD_AFCLockRef SDMMD_AFCLockClass*

CFTypeID SDMMD_AFCLockGetTypeID(void);

#pragma mark -
#pragma mark FUNCTIONS
#pragma mark -

void SDMMD_AFCLockRefClassInitialize(void);

SDMMD_AFCLockRef SDMMD_AFCLockCreate();
sdmmd_return_t SDMMD_AFCLockLock(SDMMD_AFCLockRef lock);
sdmmd_return_t SDMMD_AFCLockUnlock(SDMMD_AFCLockRef lock);

#endif
#endif