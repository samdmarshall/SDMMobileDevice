/*
 *  SDMMD_AMDevice_Class.h
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

#ifndef _SDM_MD_ADMDEVICE_CLASS_H_
#define _SDM_MD_ADMDEVICE_CLASS_H_

#include <CoreFoundation/CoreFoundation.h>

// Please do not access ivars of the AMDevice objects if you can avoid it, they are subject to change.

typedef struct SDMMD_lockdown_conn_internal SDMMD_lockdown_conn;

typedef struct sdmmd_am_device* SDMMD_AMDeviceRef;

void SDMMD_AMDeviceRefClassInitialize();

CFTypeID SDMMD_AMDeviceRefGetTypeID();

/*!
 @function SDMMD_AMDeviceCreateEmpty
 @discussion
 Creating an empty device object, used in conjunction with SDMMD_AMDeviceCreateFromProperties(), returns a SDMMD_AMDeviceRef object.
 */
CF_RETURNS_RETAINED SDMMD_AMDeviceRef SDMMD_AMDeviceCreateEmpty();

#endif