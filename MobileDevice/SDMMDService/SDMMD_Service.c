/*
 *  SDMMD_Service.c
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

#ifndef _SDM_MD_SERVICE_C_
#define _SDM_MD_SERVICE_C_

#include "SDMMD_Service.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/select.h>

int32_t CheckIfExpectingResponse(SocketConnection handle, uint32_t timeout) {
	fd_set fds;
	int32_t returnValue = -1;
	struct timeval to;
	struct timeval *pto;

	FD_ZERO(&fds);
	if (handle.isSSL) {
		FD_SET(handle.socket.ssl, &fds);
	} else {
		FD_SET(handle.socket.conn, &fds);
	}

	if (timeout > 0) {
		to.tv_sec = (time_t) (timeout / 1000);
		to.tv_usec = (time_t) ((timeout - (to.tv_sec * 1000)) * 1000);
		pto = &to;
	} else {
		pto = NULL;
	}
	if (handle.isSSL) {
		returnValue = select(handle.socket.ssl + 1, &fds, NULL, NULL, pto);
	} else {
		returnValue = select(handle.socket.conn + 1, &fds, NULL, NULL, pto);
	}
	return returnValue;
}


sdmmd_return_t SDMMD_ServiceSend(SocketConnection handle, CFDataRef data) {
	uint32_t msgLen = (data ? CFDataGetLength(data) : 0);
	if (msgLen) {
	    uint32_t size = (handle.isSSL ? msgLen : htonl((uint32_t)msgLen));
		uint32_t result;
		if (handle.isSSL) {
			result = SSL_write(handle.socket.ssl, &size, sizeof(size));
		} else {
			result = send(handle.socket.conn, &size, sizeof(size), 0);
		}
		if (result == sizeof(size)) {
			if (handle.isSSL) {
				result = SSL_write(handle.socket.ssl, &size, msgLen);
			} else {
				result = send(handle.socket.conn, CFDataGetBytePtr(data), msgLen, 0);
			}
			if (result == msgLen) {
				return (result == msgLen ? MDERR_OK : MDERR_QUERY_FAILED);
			}
		}
		return MDERR_QUERY_FAILED;
	}
	return MDERR_OK;
}

sdmmd_return_t SDMMD_ServiceReceive(SocketConnection handle, CFDataRef *data) {
	uint32_t size = (data && *data ? (uint32_t)CFDataGetLength(*data) : 0);
	if (size) {
		if (CheckIfExpectingResponse(handle, 1000)) {
			unsigned char *buffer = malloc(size);
			uint32_t remainder = size;
			while (remainder) {
				size_t recieved;
				if (handle.isSSL) {
					recieved = SSL_read(handle.socket.ssl, &buffer[size-remainder], remainder);
				} else {
					recieved = recv(handle.socket.conn, &buffer[size-remainder], remainder, 0);
				}
				if (!recieved)
					break;
				remainder -= recieved;
			}
			*data = CFDataCreate(kCFAllocatorDefault, buffer, size);
			free(buffer);
		}
		return MDERR_OK;
	}
	return MDERR_OK;
}

sdmmd_return_t SDMMD_ServiceSendMessage(SocketConnection handle, CFPropertyListRef data) {
	return ((data) ? SDMMD_ServiceSend(handle, data) : MDERR_DICT_NOT_LOADED);
}

sdmmd_return_t SDMMD_ServiceReceiveMessage(SocketConnection handle, CFPropertyListRef *data) {
	CFDataRef dataBuffer = NULL;
	if (SDM_MD_CallSuccessful(SDMMD_ServiceReceive(handle, &dataBuffer))) {
		*data = CFPropertyListCreateWithData(0, dataBuffer, kCFPropertyListImmutable, NULL, NULL);
		return MDERR_OK;
	} else {
		return MDERR_QUERY_FAILED;
	}
}

#endif