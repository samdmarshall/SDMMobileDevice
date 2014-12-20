/*
 *  SDMMD_Connection.h
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

#ifndef _SDM_MD_CONNECTION_H_
#define _SDM_MD_CONNECTION_H_

#include "SDMMD_Connection_Class.h"
#include "SDMMD_Error.h"
#include "SDMMD_AMDevice.h"

#pragma mark -
#pragma mark FUNCTIONS
#pragma mark -

sdmmd_return_t SDMMD_perform_command(SDMMD_AMConnectionRef conn, CFStringRef command, uint64_t code, CallBack callback, uint32_t argsCount, void* paramStart, ...);

sdmmd_return_t SDMMD_AMDeviceStartService(SDMMD_AMDeviceRef device, CFStringRef service, CFDictionaryRef options, SDMMD_AMConnectionRef *connection);
sdmmd_return_t SDMMD_AMDeviceSecureStartService(SDMMD_AMDeviceRef device, CFStringRef service, CFDictionaryRef options, SDMMD_AMConnectionRef *connection);

void SDMMD_AMDServiceConnectionSetServiceName(SDMMD_AMConnectionRef *connection, CFStringRef service);
void SDMMD_AMDServiceConnectionSetDevice(SDMMD_AMConnectionRef *connection, SDMMD_AMDeviceRef device);
CF_RETURNS_NOT_RETAINED SDMMD_AMDeviceRef SDMMD_AMDServiceConnectionGetDevice(SDMMD_AMConnectionRef connection);

uint32_t SDMMD_AMDServiceConnectionGetSocket(SDMMD_AMConnectionRef connection);

sdmmd_return_t SDMMD_AMDeviceSecureStartSessionedService(SDMMD_AMDeviceRef device, CFStringRef service, SDMMD_AMConnectionRef *connection);

#endif