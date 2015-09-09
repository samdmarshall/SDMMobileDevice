/*
 *  SDMMD_AFCConnection_Class.c
 *  SDMMobileDevice
 *
 * Copyright (c) 2014, Samantha Marshall
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
 * 3. Neither the name of Samantha Marshall nor the names of its contributors may be used to endorse or promote products derived from this
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

#ifndef _SDM_MD_AFCCONNECTION_CLASS_C_
#define _SDM_MD_AFCCONNECTION_CLASS_C_

#include "SDMMD_AFCConnection_Class.h"
#include "SDMMD_Connection.h"
#include "SDMMD_Connection_Internal.h"
#include "SDMMD_AFCConnection_Internal.h"
#include "Core.h"

static Boolean SDMMD_AFCConnectionRefEqual(CFTypeRef cf1, CFTypeRef cf2)
{
	SDMMD_AFCConnectionRef connection1 = (SDMMD_AFCConnectionRef)cf1;
	SDMMD_AFCConnectionRef connection2 = (SDMMD_AFCConnectionRef)cf2;

	return (connection1->ivars.handle->ivars.socket == connection2->ivars.handle->ivars.socket);
}

static CFStringRef SDMMD_AFCConnectionRefCopyFormattingDesc(CFTypeRef cf, CFDictionaryRef formatOpts)
{
	SDMMD_AFCConnectionRef connection = (SDMMD_AFCConnectionRef)cf;
	return CFStringCreateWithFormat(CFGetAllocator(connection), NULL, CFSTR("<SDMMD_AFCConnectionRef %p>{socket = %d}"), connection, connection->ivars.handle->ivars.socket);
}

static CFStringRef SDMMD_AFCConnectionRefCopyDebugDesc(CFTypeRef cf)
{
	SDMMD_AFCConnectionRef connection = (SDMMD_AFCConnectionRef)cf;
	return CFStringCreateWithFormat(CFGetAllocator(connection), NULL, CFSTR("<SDMMD_AFCConnectionRef %p>{socket = %d}"), connection, connection->ivars.handle->ivars.socket);
}

static void SDMMD_AFCConnectionRefFinalize(CFTypeRef cf)
{
	SDMMD_AFCConnectionRef connection = (SDMMD_AFCConnectionRef)cf;
	CFSafeRelease(connection->ivars.handle);
	Safe(dispatch_release, connection->ivars.operationQueue);
}

static CFTypeID _kSDMMD_AFCConnectionRefID = _kCFRuntimeNotATypeID;

static CFRuntimeClass _kSDMMD_AFCConnectionRefClass = {0};

void SDMMD_AFCConnectionRefClassInitialize(void)
{
	_kSDMMD_AFCConnectionRefClass.version = 0;
	_kSDMMD_AFCConnectionRefClass.className = "SDMMD_AFCConnectionRef";
	_kSDMMD_AFCConnectionRefClass.init = NULL;
	_kSDMMD_AFCConnectionRefClass.copy = NULL;
	_kSDMMD_AFCConnectionRefClass.finalize = SDMMD_AFCConnectionRefFinalize;
	_kSDMMD_AFCConnectionRefClass.equal = SDMMD_AFCConnectionRefEqual;
	_kSDMMD_AFCConnectionRefClass.hash = NULL;
	_kSDMMD_AFCConnectionRefClass.copyFormattingDesc = SDMMD_AFCConnectionRefCopyFormattingDesc;
	_kSDMMD_AFCConnectionRefClass.copyDebugDesc = SDMMD_AFCConnectionRefCopyDebugDesc;
	_kSDMMD_AFCConnectionRefClass.reclaim = NULL;
	_kSDMMD_AFCConnectionRefID = _CFRuntimeRegisterClass((const CFRuntimeClass *const) & _kSDMMD_AFCConnectionRefClass);
}

CFTypeID SDMMD_AFCConnectionRefGetTypeID(void)
{
	return _kSDMMD_AFCConnectionRefID;
}

SDMMD_AFCConnectionRef SDMMD_AFCConnectionCreateEmpty()
{
	uint32_t extra = sizeof(sdmmd_AFCConnectionClassBody);
	SDMMD_AFCConnectionRef connection = (SDMMD_AFCConnectionRef)_CFRuntimeCreateInstance(kCFAllocatorDefault, _kSDMMD_AFCConnectionRefID, extra, NULL);
	return connection;
}

#endif