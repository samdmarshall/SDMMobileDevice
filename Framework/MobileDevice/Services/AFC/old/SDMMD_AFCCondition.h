/*
 *  SDMMD_AFCCondition.h
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

#ifndef _SDM_MD_AFCCONDITION_H_
#define _SDM_MD_AFCCONDITION_H_

#include "SDMMD_Error.h"
#include <pthread.h>

#pragma mark -
#pragma mark TYPES
#pragma mark -

typedef struct AFCConditionClassHeader {
	unsigned char header[16];
} ATR_PACK AFCConditionClassHeader; // 0x10

typedef struct AFCConditionClassBody {
	pthread_mutex_t mutex_lock;	// 16
	pthread_cond_t mutex_cond; 	// 80
	bool signaled; 				// 128
} ATR_PACK AFCConditionClassBody; // 0x78

typedef struct afc_condition {
	struct AFCConditionClassHeader base;
	struct AFCConditionClassBody ivars;
} ATR_PACK afc_condition;

typedef struct afc_condition SDMMD_AFCConditionClass;

#define SDMMD_AFCConditionRef SDMMD_AFCConditionClass*

CFTypeID SDMMD_AFCConditionGetTypeID(void);

#pragma mark -
#pragma mark FUNCTIONS
#pragma mark -

void SDMMD_AFCConditionRefClassInitialize(void);


SDMMD_AFCConditionRef SDMMD_AFCConditionCreate();
bool SDMMD_AFCConditionIsSignaled(SDMMD_AFCConditionRef cond);
sdmmd_return_t SDMMD_AFCConditionSignal(SDMMD_AFCConditionRef cond);
sdmmd_return_t SDMMD_AFCConditionWait(SDMMD_AFCConditionRef cond);

#endif