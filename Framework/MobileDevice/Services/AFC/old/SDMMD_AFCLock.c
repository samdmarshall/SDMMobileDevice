/*
 *  SDMMD_AFCLock.c
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

#ifndef _SDM_MD_AFCLOCK_C_
#define _SDM_MD_AFCLOCK_C_

#if 0

#include "SDMMD_AFCLock.h"
#include "CFRuntime.h"
#include "SDMMD_Functions.h"

static CFTypeID _kSDMMD_AFCLockRefID = _kCFRuntimeNotATypeID;

static CFRuntimeClass _kSDMMD_AFCLockRefClass = {0};

void SDMMD_AFCLockRefClassInitialize(void) {
    _kSDMMD_AFCLockRefClass.version = 0;
    _kSDMMD_AFCLockRefClass.className = "SDMMD_AFCLockRef";
    _kSDMMD_AFCLockRefClass.init = NULL;
    _kSDMMD_AFCLockRefClass.copy = NULL;
    _kSDMMD_AFCLockRefClass.finalize = NULL;
    _kSDMMD_AFCLockRefClass.equal = NULL;
    _kSDMMD_AFCLockRefClass.hash = NULL;
    _kSDMMD_AFCLockRefClass.copyFormattingDesc = NULL;
    _kSDMMD_AFCLockRefClass.copyDebugDesc = NULL;
	_kSDMMD_AFCLockRefClass.reclaim = NULL;
    _kSDMMD_AFCLockRefID = _CFRuntimeRegisterClass((const CFRuntimeClass * const)&_kSDMMD_AFCLockRefClass);
}

CFTypeID SDMMD_AFCLockGetTypeID(void) {
    return _kSDMMD_AFCLockRefID;
}

SDMMD_AFCLockRef SDMMD_AFCLockCreate() {
	uint32_t extra = sizeof(AFCLockClassBody);
	SDMMD_AFCLockRef lock = calloc(0x1, sizeof(SDMMD_AFCLockClass));
	lock = (SDMMD_AFCLockRef)_CFRuntimeCreateInstance(kCFAllocatorDefault, _kSDMMD_AFCLockRefID, extra, NULL);
	if (lock) {
		sdmmd_mutex_init(lock->ivars.mutex_lock);
	}
	return lock;
}

sdmmd_return_t SDMMD_AFCLockLock(SDMMD_AFCLockRef lock) {
	sdmmd_return_t result = 0xe8004017;
	if (lock) {
		result = pthread_mutex_lock(&(lock->ivars.mutex_lock));
	} else {
		result = 0xe8004007;
	}
	return result;
}

sdmmd_return_t SDMMD_AFCLockUnlock(SDMMD_AFCLockRef lock) {
	sdmmd_return_t result = 0xe8004017;
	if (lock) {
		result = pthread_mutex_unlock(&(lock->ivars.mutex_lock));
	} else {
		result = 0xe8004007;
	}
	return result;
}

#endif

#endif