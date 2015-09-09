/*
 *  SDMMD_MCP_Class.c
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

#ifndef _SDM_MD_MCP_CLASS_C_
#define _SDM_MD_MCP_CLASS_C_

#include "SDMMD_MCP_Class.h"
#include "SDMMD_MCP_Internal.h"
#include "Core.h"

static void SDMMD_SDMMobileDeviceRefFinalize(CFTypeRef cf)
{
	SDMMobileDeviceRef manager = (SDMMobileDeviceRef)cf;
	CFSafeRelease(manager->ivars.usbmuxd);
	CFSafeRelease(manager->ivars.deviceList);
}

static CFTypeID _kSDMMD_SDMMobileDeviceRefID = _kCFRuntimeNotATypeID;

static CFRuntimeClass _kSDMMD_SDMMobileDeviceRefClass = {0};

void SDMMD_SDMMobileDeviceRefClassInitialize()
{
	_kSDMMD_SDMMobileDeviceRefClass.version = 0;
	_kSDMMD_SDMMobileDeviceRefClass.className = "SDMMobileDeviceRef";
	_kSDMMD_SDMMobileDeviceRefClass.init = NULL;
	_kSDMMD_SDMMobileDeviceRefClass.copy = NULL;
	_kSDMMD_SDMMobileDeviceRefClass.finalize = SDMMD_SDMMobileDeviceRefFinalize;
	_kSDMMD_SDMMobileDeviceRefClass.equal = NULL;
	_kSDMMD_SDMMobileDeviceRefClass.hash = NULL;
	_kSDMMD_SDMMobileDeviceRefClass.copyFormattingDesc = NULL;
	_kSDMMD_SDMMobileDeviceRefClass.copyDebugDesc = NULL;
	_kSDMMD_SDMMobileDeviceRefClass.reclaim = NULL;
	_kSDMMD_SDMMobileDeviceRefID = _CFRuntimeRegisterClass((const CFRuntimeClass *const) & _kSDMMD_SDMMobileDeviceRefClass);
}

SDMMobileDeviceRef SDMMobileDeviceRefCreateEmpty()
{
	uint32_t extra = sizeof(sdm_mobiledevice_body);
	SDMMobileDeviceRef manager = (SDMMobileDeviceRef)_CFRuntimeCreateInstance(kCFAllocatorDefault, _kSDMMD_SDMMobileDeviceRefID, extra, NULL);
	return manager;
}

#endif