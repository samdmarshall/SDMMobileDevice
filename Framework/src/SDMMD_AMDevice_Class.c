/*
 *  SDMMD_AMDevice_Class.c
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

#ifndef _SDM_MD_ADMDEVICE_CLASS_C_
#define _SDM_MD_ADMDEVICE_CLASS_C_

#include "SDMMD_AMDevice_Class.h"
#include "SDMMD_AMDevice_Internal.h"
#include "Core.h"

#pragma clang diagnostic ignored "-Wdeprecated-declarations"

static Boolean SDMMD_AMDeviceRefEqual(CFTypeRef cf1, CFTypeRef cf2)
{
	SDMMD_AMDeviceRef device1 = (SDMMD_AMDeviceRef)cf1;
	SDMMD_AMDeviceRef device2 = (SDMMD_AMDeviceRef)cf2;
	Boolean result = (device1->ivars.device_id == device2->ivars.device_id);
	if (!result) {
		// evaluate for usb vs wifi
		result = (CFStringCompare(device1->ivars.unique_device_id, device2->ivars.unique_device_id, 0) == kCFCompareEqualTo);
	}
	return result;
}

static CFStringRef SDMMD_AMDeviceRefCopyFormattingDesc(CFTypeRef cf, CFDictionaryRef formatOpts)
{
	SDMMD_AMDeviceRef device = (SDMMD_AMDeviceRef)cf;
	return CFStringCreateWithFormat(CFGetAllocator(device), NULL, CFSTR("<SDMMD_AMDeviceRef %p>{device = %d}"), device, device->ivars.device_id);
}

static CFStringRef SDMMD_AMDeviceRefCopyDebugDesc(CFTypeRef cf)
{
	SDMMD_AMDeviceRef device = (SDMMD_AMDeviceRef)cf;
	return CFStringCreateWithFormat(CFGetAllocator(device), NULL, CFSTR("<SDMMD_AMDeviceRef %p>{device = %d}"), device, device->ivars.device_id);
}

static void SDMMD_AMDeviceRefFinalize(CFTypeRef cf)
{
	SDMMD_AMDeviceRef device = (SDMMD_AMDeviceRef)cf;
	CFSafeRelease(device->ivars.unique_device_id);

	if (device->ivars.lockdown_conn) {
		Safe(close, (uint32_t)device->ivars.lockdown_conn->connection);
		Safe(SSL_free, device->ivars.lockdown_conn->ssl);
		Safe(free, device->ivars.lockdown_conn);
	}
	CFSafeRelease(device->ivars.session);
	CFSafeRelease(device->ivars.service_name);
	CFSafeRelease(device->ivars.network_address);
	CFSafeRelease(device->ivars.unknownData);
}

static CFTypeID _kSDMMD_AMDeviceRefID = _kCFRuntimeNotATypeID;

static CFRuntimeClass _kSDMMD_AMDeviceRefClass = {0};

void SDMMD_AMDeviceRefClassInitialize(void)
{
	_kSDMMD_AMDeviceRefClass.version = 0;
	_kSDMMD_AMDeviceRefClass.className = "SDMMD_AMDeviceRef";
	_kSDMMD_AMDeviceRefClass.init = NULL;
	_kSDMMD_AMDeviceRefClass.copy = NULL;
	_kSDMMD_AMDeviceRefClass.finalize = SDMMD_AMDeviceRefFinalize;
	_kSDMMD_AMDeviceRefClass.equal = SDMMD_AMDeviceRefEqual;
	_kSDMMD_AMDeviceRefClass.hash = NULL;
	_kSDMMD_AMDeviceRefClass.copyFormattingDesc = SDMMD_AMDeviceRefCopyFormattingDesc;
	_kSDMMD_AMDeviceRefClass.copyDebugDesc = SDMMD_AMDeviceRefCopyDebugDesc;
	_kSDMMD_AMDeviceRefClass.reclaim = NULL;
	_kSDMMD_AMDeviceRefID = _CFRuntimeRegisterClass((const CFRuntimeClass *const) & _kSDMMD_AMDeviceRefClass);
}

CFTypeID SDMMD_AMDeviceRefGetTypeID(void)
{
	return _kSDMMD_AMDeviceRefID;
}

SDMMD_AMDeviceRef SDMMD_AMDeviceCreateEmpty()
{
	uint32_t extra = sizeof(AMDeviceClassBody);
	SDMMD_AMDeviceRef device = (SDMMD_AMDeviceRef)_CFRuntimeCreateInstance(kCFAllocatorDefault, _kSDMMD_AMDeviceRefID, extra, NULL);
	return device;
}

#endif