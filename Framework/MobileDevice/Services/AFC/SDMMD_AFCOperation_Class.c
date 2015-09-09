/*
 *  SDMMD_AFCOperation_Class.c
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

#ifndef _SDM_MD_AFCOPERATION_CLASS_C_
#define _SDM_MD_AFCOPERATION_CLASS_C_

#include "SDMMD_AFCOperation_Class.h"
#include "SDMMD_AFC_Types.h"
#include "SDMMD_AFCOperation_Internal.h"
#include "Core.h"

static Boolean SDMMD_AFCOperationRefEqual(CFTypeRef cf1, CFTypeRef cf2)
{
	SDMMD_AFCOperationRef operation1 = (SDMMD_AFCOperationRef)cf1;
	SDMMD_AFCOperationRef operation2 = (SDMMD_AFCOperationRef)cf2;

	return (operation1->ivars.packet == operation2->ivars.packet);
}

static CFStringRef SDMMD_AFCOperationRefCopyFormattingDesc(CFTypeRef cf, CFDictionaryRef formatOpts)
{
	SDMMD_AFCOperationRef operation = (SDMMD_AFCOperationRef)cf;
	return CFStringCreateWithFormat(CFGetAllocator(operation), NULL, CFSTR("<SDMMD_AFCOperationRef %p>{PacketType = %s}"), operation, SDMMD_gAFCPacketTypeNames[operation->ivars.packet->header.type]);
}

static CFStringRef SDMMD_AFCOperationRefCopyDebugDesc(CFTypeRef cf)
{
	SDMMD_AFCOperationRef operation = (SDMMD_AFCOperationRef)cf;
	return CFStringCreateWithFormat(CFGetAllocator(operation), NULL, CFSTR("<SDMMD_AFCOperationRef %p>{PacketType = %s}"), operation, SDMMD_gAFCPacketTypeNames[operation->ivars.packet->header.type]);
}

static void SDMMD_AFCOperationRefFinalize(CFTypeRef cf)
{
	SDMMD_AFCOperationRef operation = (SDMMD_AFCOperationRef)cf;
	if (operation->ivars.packet) {
		if (operation->ivars.packet->header_data) {
			free(operation->ivars.packet->header_data);
		}

		if (operation->ivars.packet->body_data) {
			free(operation->ivars.packet->body_data);
		}

		CFSafeRelease(operation->ivars.packet->response);

		free(operation->ivars.packet);
	}
}

static CFTypeID _kSDMMD_AFCOperationRefID = _kCFRuntimeNotATypeID;

static CFRuntimeClass _kSDMMD_AFCOperationRefClass = {0};

void SDMMD_AFCOperationRefClassInitialize(void)
{
	_kSDMMD_AFCOperationRefClass.version = 0;
	_kSDMMD_AFCOperationRefClass.className = "SDMMD_AFCOperationRef";
	_kSDMMD_AFCOperationRefClass.init = NULL;
	_kSDMMD_AFCOperationRefClass.copy = NULL;
	_kSDMMD_AFCOperationRefClass.finalize = SDMMD_AFCOperationRefFinalize;
	_kSDMMD_AFCOperationRefClass.equal = SDMMD_AFCOperationRefEqual;
	_kSDMMD_AFCOperationRefClass.hash = NULL;
	_kSDMMD_AFCOperationRefClass.copyFormattingDesc = SDMMD_AFCOperationRefCopyFormattingDesc;
	_kSDMMD_AFCOperationRefClass.copyDebugDesc = SDMMD_AFCOperationRefCopyDebugDesc;
	_kSDMMD_AFCOperationRefClass.reclaim = NULL;
	_kSDMMD_AFCOperationRefID = _CFRuntimeRegisterClass((const CFRuntimeClass *const) & _kSDMMD_AFCOperationRefClass);
}

CFTypeID SDMMD_AFCOperationRefGetTypeID(void)
{
	return _kSDMMD_AFCOperationRefID;
}

SDMMD_AFCOperationRef SDMMD_AFCOperationCreateEmpty()
{
	uint32_t extra = sizeof(sdmmd_AFCOperationBody);
	SDMMD_AFCOperationRef operation = (SDMMD_AFCOperationRef)_CFRuntimeCreateInstance(kCFAllocatorDefault, _kSDMMD_AFCOperationRefID, extra, NULL);
	return operation;
}

CFTypeRef SDMMD_AFCOperationGetPacketResponse(SDMMD_AFCOperationRef operation)
{
	return operation->ivars.packet->response;
}

#endif