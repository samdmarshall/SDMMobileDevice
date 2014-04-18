/*
 *  SDMMD_AFCCondition.c
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

#ifndef _SDM_MD_AFCCONDITION_C_
#define _SDM_MD_AFCCONDITION_C_

#include "SDMMD_AFCCondition.h"
#include "CFRuntime.h"

static CFTypeID _kSDMMD_AFCConditionRefID = _kCFRuntimeNotATypeID;

static CFRuntimeClass _kSDMMD_AFCConditionRefClass = {0};

void SDMMD_AFCConditionRefClassInitialize(void) {
    _kSDMMD_AFCConditionRefClass.version = 0;
    _kSDMMD_AFCConditionRefClass.className = "SDMMD_AFCConditionRef";
    _kSDMMD_AFCConditionRefClass.init = NULL;
    _kSDMMD_AFCConditionRefClass.copy = NULL;
    _kSDMMD_AFCConditionRefClass.finalize = NULL;
    _kSDMMD_AFCConditionRefClass.equal = NULL;
    _kSDMMD_AFCConditionRefClass.hash = NULL;
    _kSDMMD_AFCConditionRefClass.copyFormattingDesc = NULL;
    _kSDMMD_AFCConditionRefClass.copyDebugDesc = NULL;
	_kSDMMD_AFCConditionRefClass.reclaim = NULL;
    _kSDMMD_AFCConditionRefID = _CFRuntimeRegisterClass((const CFRuntimeClass * const)&_kSDMMD_AFCConditionRefClass);
}

CFTypeID SDMMD_AFCConditionGetTypeID(void) {
    return _kSDMMD_AFCConditionRefID;
}

SDMMD_AFCConditionRef SDMMD_AFCConditionCreate() {
	uint32_t extra = sizeof(AFCConditionClassBody);
	SDMMD_AFCConditionRef cond = calloc(0x1, sizeof(SDMMD_AFCConditionClass));
	cond = (SDMMD_AFCConditionRef)_CFRuntimeCreateInstance(kCFAllocatorDefault, _kSDMMD_AFCConditionRefID, extra, NULL);
	if (cond) {
		pthread_mutex_init(&cond->ivars.mutex_lock,0x0);
		pthread_cond_init(&cond->ivars.mutex_cond,0x0);
		cond->ivars.signaled = false;
	}
	return cond;
}

bool SDMMD_AFCConditionIsSignaled(SDMMD_AFCConditionRef cond) {
	return cond->ivars.signaled;
}

sdmmd_return_t SDMMD_AFCConditionSignal(SDMMD_AFCConditionRef cond) {
	sdmmd_return_t result = pthread_mutex_lock(&cond->ivars.mutex_lock);
	if (result == 0) {
		cond->ivars.signaled = true;
		result = pthread_cond_broadcast(&cond->ivars.mutex_cond);
		pthread_mutex_unlock(&cond->ivars.mutex_lock);
	}
	return result;
}

sdmmd_return_t SDMMD_AFCConditionWait(SDMMD_AFCConditionRef cond) {
	sdmmd_return_t result = pthread_mutex_lock(&cond->ivars.mutex_lock);
	if (result == 0) {
		if (cond->ivars.signaled) {
			struct timespec waittime = {0x0, 0x000000008e0004c0};
			pthread_cond_timedwait(&cond->ivars.mutex_cond,&cond->ivars.mutex_lock,&waittime);
		}
		pthread_mutex_unlock(&cond->ivars.mutex_lock);
	}
	return result;
	
}

#endif