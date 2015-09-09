/*
 *  SDMMD_USBmuxListener_Class.c
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

#ifndef _SDM_MD_USBMUXLISTENER_CLASS_C_
#define _SDM_MD_USBMUXLISTENER_CLASS_C_

#include "SDMMD_USBmuxListener_Class.h"
#include "SDMMD_USBMuxListener_Internal.h"
#include "Core.h"

static Boolean SDMMD_USBMuxListenerRefEqual(CFTypeRef cf1, CFTypeRef cf2)
{
	SDMMD_USBMuxListenerRef listener1 = (SDMMD_USBMuxListenerRef)cf1;
	SDMMD_USBMuxListenerRef listener2 = (SDMMD_USBMuxListenerRef)cf2;

	return (listener1->ivars.socket == listener2->ivars.socket);
}

static CFStringRef SDMMD_USBMuxListenerRefCopyFormattingDesc(CFTypeRef cf, CFDictionaryRef formatOpts)
{
	SDMMD_USBMuxListenerRef listener = (SDMMD_USBMuxListenerRef)cf;
	return CFStringCreateWithFormat(CFGetAllocator(listener), NULL, CFSTR("<SDMMD_USBMuxListenerRef %p>{socket = %d}"), listener, listener->ivars.socket);
}

static CFStringRef SDMMD_USBMuxListenerRefCopyDebugDesc(CFTypeRef cf)
{
	SDMMD_USBMuxListenerRef listener = (SDMMD_USBMuxListenerRef)cf;
	return CFStringCreateWithFormat(CFGetAllocator(listener), NULL, CFSTR("<SDMMD_USBMuxListenerRef %p>{socket = %d}"), listener, listener->ivars.socket);
}

static void SDMMD_USBMuxListenerRefFinalize(CFTypeRef cf)
{
	SDMMD_USBMuxListenerRef listener = (SDMMD_USBMuxListenerRef)cf;
	listener->ivars.isActive = false;
	CFSafeRelease(listener->ivars.responses);
	Safe(close, listener->ivars.socket);
	Safe(dispatch_release, listener->ivars.socketQueue);
	listener->ivars.responseCallback = NULL;
	listener->ivars.attachedCallback = NULL;
	listener->ivars.detachedCallback = NULL;
	listener->ivars.logsCallback = NULL;
	listener->ivars.deviceListCallback = NULL;
	listener->ivars.listenerListCallback = NULL;
	listener->ivars.unknownCallback = NULL;
	dispatch_async(dispatch_get_main_queue(), ^{
		CFNotificationCenterPostNotification(CFNotificationCenterGetLocalCenter(), kSDMMD_USBMuxListenerStoppedListenerNotification, NULL, NULL, true);
	});
}

static CFTypeID _kSDMMD_USBMuxListenerRefID = _kCFRuntimeNotATypeID;

static CFRuntimeClass _kSDMMD_USBMuxListenerRefClass = {0};

void SDMMD_USBMuxListenerRefClassInitialize(void)
{
	_kSDMMD_USBMuxListenerRefClass.version = 0;
	_kSDMMD_USBMuxListenerRefClass.className = "SDMMD_USBMuxListenerRef";
	_kSDMMD_USBMuxListenerRefClass.init = NULL;
	_kSDMMD_USBMuxListenerRefClass.copy = NULL;
	_kSDMMD_USBMuxListenerRefClass.finalize = SDMMD_USBMuxListenerRefFinalize;
	_kSDMMD_USBMuxListenerRefClass.equal = SDMMD_USBMuxListenerRefEqual;
	_kSDMMD_USBMuxListenerRefClass.hash = NULL;
	_kSDMMD_USBMuxListenerRefClass.copyFormattingDesc = SDMMD_USBMuxListenerRefCopyFormattingDesc;
	_kSDMMD_USBMuxListenerRefClass.copyDebugDesc = SDMMD_USBMuxListenerRefCopyDebugDesc;
	_kSDMMD_USBMuxListenerRefClass.reclaim = NULL;
	_kSDMMD_USBMuxListenerRefID = _CFRuntimeRegisterClass((const CFRuntimeClass *const) & _kSDMMD_USBMuxListenerRefClass);
}

CFTypeID SDMMD_USBMuxListenerRefGetTypeID(void)
{
	return _kSDMMD_USBMuxListenerRefID;
}

SDMMD_USBMuxListenerRef SDMMD_USBMuxListenerCreateEmpty()
{
	uint32_t extra = sizeof(USBMuxListenerClassBody);
	SDMMD_USBMuxListenerRef device = (SDMMD_USBMuxListenerRef)_CFRuntimeCreateInstance(kCFAllocatorDefault, _kSDMMD_USBMuxListenerRefID, extra, NULL);
	return device;
}

#endif