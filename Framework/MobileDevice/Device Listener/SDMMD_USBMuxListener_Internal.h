/*
 *  SDMMD_USBMuxListener_Internal.h
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

#ifndef SDMMobileDevice_Framework_SDMMD_USBMuxListener_Internal_h
#define SDMMobileDevice_Framework_SDMMD_USBMuxListener_Internal_h

#include <CoreFoundation/CoreFoundation.h>
#include <SDMMobileDevice/SDMMD_USBMuxListener_Types.h>
#include <SDMMobileDevice/CFRuntime.h>

typedef void (*callbackFunction)(void *, struct USBMuxPacket *);

struct USBMuxListenerClassBody {
	uint32_t socket;
	bool isActive;
	__unsafe_unretained dispatch_queue_t socketQueue;
	__unsafe_unretained dispatch_source_t socketSource;
	__unsafe_unretained dispatch_semaphore_t semaphore;
	callbackFunction responseCallback;
	callbackFunction attachedCallback;
	callbackFunction detachedCallback;
	callbackFunction logsCallback;
	callbackFunction deviceListCallback;
	callbackFunction listenerListCallback;
	callbackFunction unknownCallback;
	CFMutableArrayRef responses;
} USBMuxListenerClassBody;

struct USBMuxListenerClass {
	CFRuntimeBase base;
	struct USBMuxListenerClassBody ivars;
} USBMuxListenerClass;

#endif
