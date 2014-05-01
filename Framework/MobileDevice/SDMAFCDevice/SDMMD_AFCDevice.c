/*
 *  SDMMD_AFCDevice.c
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

#ifndef _SDM_MD_AFCDEVICE_C_
#define _SDM_MD_AFCDEVICE_C_

#if 0

#include "SDMMD_AFCDevice.h"
#include "CFRuntime.h"
#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>

static CFTypeID _kSDMMD_AFCConnectionRefID = _kCFRuntimeNotATypeID;

static CFRuntimeClass _kSDMMD_AFCConnectionRefClass = {0};

SDMMD_AFCConnectionRef SDMMD_AFCConnectionCreate(void*a, uint32_t sock,void*c,void*d,void*e) {
	uint32_t extra = sizeof(AFCConnectionClassBody);
	SDMMD_AFCConnectionRef connection = calloc(0x1, sizeof(SDMMD_AFCConnectionClass));
	connection = (SDMMD_AFCConnectionRef)_CFRuntimeCreateInstance(kCFAllocatorDefault, _kSDMMD_AFCConnectionRefID, extra, NULL);
	if (connection) {
		connection->ivars.state = 0x1;
		connection->ivars.cond0 = SDMMD_AFCConditionCreate();
		connection->ivars.cond_signal = SDMMD_AFCConditionCreate();
		connection->ivars.handle = sock;
		connection->ivars.connection_active = (c ? true : false);
		connection->ivars.statusPtr = 0x0;
		connection->ivars.e = false;
		connection->ivars.operation_count = 0x0;
		connection->ivars.fs_block_size = 0x100000;
		connection->ivars.socket_block_size = 0x100000;
		connection->ivars.io_timeout = 0x3c;
		connection->ivars.h = 1;
		connection->ivars.context = e;
		connection->ivars.lock0 = SDMMD_AFCLockCreate();
		connection->ivars.lock1 = SDMMD_AFCLockCreate();
		connection->ivars.operation_enqueue = CFArrayCreateMutable(kCFAllocatorDefault, 0x0, &kCFTypeArrayCallBacks);
		connection->ivars.operation_dequeue = CFArrayCreateMutable(kCFAllocatorDefault, 0x0, &kCFTypeArrayCallBacks);
		connection->ivars.secure_context = d;
		connection->ivars.queue = kqueue();
	}
	return connection;
}

void SDMMD_AFCConnectionRefClassInitialize(void) {
    _kSDMMD_AFCConnectionRefClass.version = 0;
    _kSDMMD_AFCConnectionRefClass.className = "SDMMD_AFCConnectionRef";
    _kSDMMD_AFCConnectionRefClass.init = SDMMD_AFCConnectionCreate;
    _kSDMMD_AFCConnectionRefClass.copy = NULL;
    _kSDMMD_AFCConnectionRefClass.finalize = NULL;
    _kSDMMD_AFCConnectionRefClass.equal = NULL;
    _kSDMMD_AFCConnectionRefClass.hash = NULL;
    _kSDMMD_AFCConnectionRefClass.copyFormattingDesc = NULL;
    _kSDMMD_AFCConnectionRefClass.copyDebugDesc = NULL;
	_kSDMMD_AFCConnectionRefClass.reclaim = NULL;
    _kSDMMD_AFCConnectionRefID = _CFRuntimeRegisterClass((const CFRuntimeClass * const)&_kSDMMD_AFCConnectionRefClass);
}

CFTypeID SDMMD_AFCConnectionGetTypeID(void) {
    return _kSDMMD_AFCConnectionRefID;
}

#endif
#endif