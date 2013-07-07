/*
 *  SDMMDService.c
 *  SDM_MD_Demo
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
#ifndef _SDMMDSERVICE_C_
#define _SDMMDSERVICE_C_

#include "SDMMDService.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/select.h>

int32_t CheckIfExpectingResponse(uint32_t handle, uint32_t timeout) {
	fd_set fds;
	int32_t returnValue = -1;
	struct timeval to;
	struct timeval *pto;

	FD_ZERO(&fds);
	FD_SET(handle, &fds);

	if (timeout > 0) {
		to.tv_sec = (time_t) (timeout / 1000);
		to.tv_usec = (time_t) ((timeout - (to.tv_sec * 1000)) * 1000);
		pto = &to;
	} else {
		pto = NULL;
	}
	returnValue = select(handle + 1, &fds, NULL, NULL, pto);
	
	return returnValue;
}


sdmmd_return_t SDMMDServiceSend(uint32_t handle, CFDataRef data) {
	uint32_t msgLen = (data ? CFDataGetLength(data) : 0);
	if (msgLen) {
	    uint32_t size = htonl((uint32_t)msgLen);
		uint32_t result = send(handle, &size, sizeof(size), 0);
		if (result == sizeof(size)) {
			result = send(handle, CFDataGetBytePtr(data), msgLen, 0);
			if (result == msgLen) {
				return (result == msgLen ? MDERR_OK : MDERR_QUERY_FAILED);
			}
		}
		return MDERR_QUERY_FAILED;
	}
	return MDERR_OK;
}

sdmmd_return_t SDMMDServiceReceive(uint32_t handle, CFDataRef *data) {
	uint32_t size = (data && *data ? (uint32_t)CFDataGetLength(*data) : 0);
	if (size) {
		if (CheckIfExpectingResponse(handle, 1000)) {
			unsigned char *buffer = malloc(size);
			uint32_t remainder = size;
			while (remainder) {
				size_t recieved = recv(handle, &buffer[size-remainder], remainder, 0);
				if (!recieved)
					break;
				remainder -= recieved;
			}
			*data = CFDataCreate(kCFAllocatorDefault, buffer, size);
			free(buffer);
		}
		return MDERR_OK;
			/*size_t rc = recv(handle, &size, size, 0);
			if (rc == (uint32_t)CFDataGetLength(*data)) {
				size = ntohl(size);
				if (size) {
					unsigned char *buffer = malloc(rc);
					size_t remainder = rc;
					while (remainder) {
						size_t recieved = recv(handle, &buffer[size-remainder], remainder, 0);
						remainder -= recieved;
					}
					CFDataRef dataBuffer = CFDataCreateWithBytesNoCopy(0, buffer, size, kCFAllocatorDefault);
					*data = CFDataCreateCopy(kCFAllocatorDefault, dataBuffer);
					free(buffer);
					CFRelease(dataBuffer);
				}
				return MDERR_OK;
			} else {
				return MDERR_QUERY_FAILED;
			}
		}*/
	}
	return MDERR_OK;
}

sdmmd_return_t SDMMDServiceSendMessage(uint32_t handle, CFPropertyListRef data) {
	return ((data) ? SDMMDServiceSend(handle, data) : MDERR_DICT_NOT_LOADED);
}

sdmmd_return_t SDMMDServiceReceiveMessage(uint32_t handle, CFPropertyListRef *data) {
	CFDataRef dataBuffer = NULL;
	if (SDM_MD_CallSuccessful(SDMMDServiceReceive(handle, &dataBuffer))) {
		*data = CFPropertyListCreateWithData(0, dataBuffer, kCFPropertyListImmutable, NULL, NULL);
		return MDERR_OK;
	} else {
		return MDERR_QUERY_FAILED;
	}
}

#endif