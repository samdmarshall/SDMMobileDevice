/*
 *  SDMMD_MCP.c
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

#ifndef _SDM_MD_MCP_C_
#define _SDM_MD_MCP_C_

#include "SDMMD_Initialize.h"
#include "SDMMD_MCP.h"
#include "SDMMD_Functions.h"
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/crypto.h>
#include "SDMMD_SSL_Functions.h"
#include "SDMMD_MCP_Internal.h"

static SDMMobileDeviceRef controller = NULL;
static dispatch_once_t once;

SDMMobileDeviceRef InitializeSDMMobileDevice()
{
	dispatch_once(&once, ^{
		InitializeSDMMobileDeviceClasses();
		if (controller == NULL) {
			controller = SDMMobileDeviceRefCreateEmpty();
			controller->ivars.deviceList = CFArrayCreate(kCFAllocatorDefault, NULL, 0, &kCFTypeArrayCallBacks);
			controller->ivars.usbmuxd = SDMMD_USBMuxCreate();
			SDMMD_USBMuxStartListener(&controller->ivars.usbmuxd);
			SSL_library_init();
			ERR_load_crypto_strings();
			SSL_load_error_strings();
			controller->ivars.peer_certificate_data_index = SDMMD_lockssl_init();
		}
	});
	return controller;
}

#endif
