/*
 *  SDMMD_Connection.c
 *  SDMMobileDevice
 *
 *  Copyright (c) 2013, Sam Marshall
 *  All rights reserved.
 * 
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 *  1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
 *  3. All advertising materials mentioning features or use of this software must display the following acknowledgement:
 *  	This product includes software developed by the Sam Marshall.
 *  4. Neither the name of the Sam Marshall nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
 * 
 *  THIS SOFTWARE IS PROVIDED BY Sam Marshall ''AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL Sam Marshall BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 */

#ifndef _SDM_MD_CONNECTION_C_
#define _SDM_MD_CONNECTION_C_

#include "SDMMD_Connection.h"

sdmmd_return_t SDMMD_perform_command(CFSocketRef socket, CFStringRef command, void* unknown, void* callback, void* unknown1, ...) {
	sdmmd_return_t result = 0x0;
	CFMutableDictionaryRef dict = SDMMD_create_dict();
	if (dict) {
		CFDictionarySetValue(dict, CFSTR("Command"), command);
		va_list args;
		va_start(args, unknown1);
		CFTypeRef key, value;
		while (key = va_arg(args)) {
			value = va_arg(args);
			CFDictionarySetValue(dict, key, value);
		}
		va_end(args);
		result = SDMMD_ServiceSendMessage(CFSocketGetNative(socket), dict);
		if (result == 0) {
			CFDictionaryRef response;
			SDMMD_ServiceReceiveMessage(CFSocketGetNative(socket), &response);
			CFTypeRef error = CFDictionaryGetValue(response, CFSTR("Error"));
			if (error) {
				result = SDMMD__ConvertServiceError(error);
				printf("call_and_response: GOT AN ERROR 0x%08x.\n",result);
			} else {
				CFTypeRef status = CFDictionaryGetValue(response, CFSTR("Status"));
				if (status) {
					if (CFStringCompare(status, CFSTR("Complete"), 0) == 0) {
						CFTypeRef responseValue = CFDictionaryGetValue(response, CFSTR("LookupResult"));
					}
				}
			}
			
		}
	} else {
		result = 0xe8000001;
	}
	return result;
}

SDMMD_AMConnectionRef SDMMD__CreateTemporaryServConn(uint32_t socket, void* unknown) {
	SDMMD_AMConnectionRef handle = NULL;
	CFDictionaryRef dict = CFDictionaryCreate(kCFAllocatorDefault, CFSTR("CloseOnInvalidate"), kCFBooleanFalse, 0x1, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
	if (dict) {
		handle = SDMMD_AMDServiceConnectionCreate(socket, unknown, dict);
		CFRelease(dict);
	}
	return handle;
}

SDMMD_AMConnectionRef SDMMD_AMDServiceConnectionCreate(uint32_t socket, SDMMD_AMDeviceRef device, CFDictionaryRef dict) {
	SDMMD_AMConnectionRef handle = calloc(0x1, sizeof(SDMMD_AMConnectionClass));
	handle->ivars.socket = socket;
	handle->ivars.secure_context = device;
	handle->ivars.one0 = 0x1;
	handle->ivars.one1 = 0x1;
	if (dict) {
		CFTypeRef value = CFDictionaryGetValue(dict, CFSTR("CloseOnInvalidate"));
		if (CFEqual(value, kCFBooleanFalse))
			handle->ivars.one0 = 0x0;
	}
	return handle;
}

sdmmd_return_t SDMMD_AMDServiceConnectionInvalidate(SDMMD_AMConnectionRef connection) {
	
}


#endif

