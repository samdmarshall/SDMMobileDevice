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

// Ignore OS X SSL deprecation warnings
#pragma clang diagnostic ignored "-Wdeprecated-declarations"

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
	if (!handle.isSSL) {
		FD_SET(handle.socket.conn, &fds);
	}

	if (timeout > 0) {
		to.tv_sec = (time_t) (timeout / 1000);
		to.tv_usec = (time_t) ((timeout - (to.tv_sec * 1000)) * 1000);
		pto = &to;
	} else {
		pto = NULL;
	}
	if (!handle.isSSL) {
		returnValue = select(handle.socket.conn + 1, &fds, 0x0, 0x0, pto);
	} else {
		returnValue = 0;
	}
	return returnValue;
}


sdmmd_return_t SDMMD_ServiceSend(SocketConnection handle, CFDataRef data) {
	uint32_t msgLen = (data ? CFDataGetLength(data) : 0);
	if (msgLen) {
	    msgLen = htonl((uint32_t)msgLen);
		uint32_t result;
		if (handle.isSSL) {
			result = SSL_write(handle.socket.ssl, &msgLen, sizeof(uint32_t));
		} else {
			result = send(handle.socket.conn, &msgLen, sizeof(uint32_t), 0);
		}
		if (result == sizeof(uint32_t)) {
			msgLen = ntohl(msgLen);
			if (handle.isSSL) {
				result = SSL_write(handle.socket.ssl, CFDataGetBytePtr(data), msgLen);
			} else {
				result = send(handle.socket.conn, CFDataGetBytePtr(data), msgLen, 0);
			}
			if (result == msgLen) {
				return (result == msgLen ? kAMDSuccess : kAMDInvalidResponseError);
			}
		}
		return kAMDNotConnectedError;
	}
	return kAMDSuccess;
}

sdmmd_return_t SDMMD_DirectServiceSend(SocketConnection handle, CFDataRef data) {
	uint32_t msgLen = (data ? CFDataGetLength(data) : 0);
	if (msgLen) {
		uint32_t result;
		if (handle.isSSL) {
			result = SSL_write(handle.socket.ssl, CFDataGetBytePtr(data), msgLen);
		} else {
			result = send(handle.socket.conn, CFDataGetBytePtr(data), msgLen, 0);
		}
		printf("send length: %i\n",result);
		if (result == msgLen) {
			return (result == msgLen ? kAMDSuccess : kAMDNotConnectedError);
		}
		return kAMDNotConnectedError;
	}
	return kAMDSuccess;
}

sdmmd_return_t SDMMD_ServiceReceive(SocketConnection handle, CFDataRef *data) {
	size_t received;
	uint32_t length = 0;
	if (handle.isSSL == true || CheckIfExpectingResponse(handle, 10000)) {
		if (handle.isSSL) {
			received = SSL_read(handle.socket.ssl, &length, 0x4);
		} else {
			received = recv(handle.socket.conn, &length, 0x4, 0);
		}
		length = ntohl(length);
		if (sizeof(length) == 0x4) {
			unsigned char *buffer = calloc(0x1, length);
			uint32_t remainder = length;
			while (remainder) {
				if (handle.isSSL) {
					received = SSL_read(handle.socket.ssl, &buffer[length-remainder], remainder);
				} else {
					received = recv(handle.socket.conn, &buffer[length-remainder], remainder, 0);
				}
				if (!received)
					break;
				remainder -= received;
			}
			*data = CFDataCreate(kCFAllocatorDefault, buffer, length);
			free(buffer);
		}
	}
	return kAMDSuccess;
}

sdmmd_return_t SDMMD_DirectServiceReceive(SocketConnection handle, CFDataRef *data) {
	uint32_t size = (data && *data ? (uint32_t)CFDataGetLength(*data) : 0);
	if (size) {
		if (handle.isSSL == true || CheckIfExpectingResponse(handle, 1000)) {
			unsigned char *buffer = calloc(1, size);
			uint32_t remainder = size;
			size_t received;
			while (remainder) {
				if (handle.isSSL) {
					received = SSL_read(handle.socket.ssl, &buffer[size-remainder], remainder);
				} else {
					received = recv(handle.socket.conn, &buffer[size-remainder], remainder, 0);
				}
				printf("recieved: %llu\n",(uint64_t)received);
				if (!received)
					break;
				remainder -= received;
			}
			*data = CFDataCreate(kCFAllocatorDefault, buffer, size);
			CFShow(*data);
			free(buffer);
		}
		return kAMDSuccess;
	}
	return kAMDSuccess;
}

sdmmd_return_t SDMMD_ServiceSendMessage(SocketConnection handle, CFPropertyListRef data, CFPropertyListFormat format) {
	CFErrorRef error;
	CFDataRef xmlData = CFPropertyListCreateData(kCFAllocatorDefault, data, format, 0, &error);
	sdmmd_return_t result = ((data) ? SDMMD_ServiceSend(handle, xmlData) : kAMDInvalidArgumentError);
	if (xmlData)
		CFRelease(xmlData);
	return result;
}

sdmmd_return_t SDMMD_ServiceReceiveMessage(SocketConnection handle, CFPropertyListRef *data) {
	CFDataRef dataBuffer = NULL;
	if (SDM_MD_CallSuccessful(SDMMD_ServiceReceive(handle, &dataBuffer))) {
		if (dataBuffer && CFDataGetLength(dataBuffer)) {
			*data = CFPropertyListCreateWithData(0, dataBuffer, kCFPropertyListImmutable, NULL, NULL);
		} else {
			*data = CFDictionaryCreateMutable(kCFAllocatorDefault, 0x0, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
		}
		return kAMDSuccess;
	} else {
		return kAMDNotConnectedError;
	}
}

sdmmd_return_t SDMMD_ServiceSendStream(SocketConnection handle, CFPropertyListRef data, CFPropertyListFormat format) {
	CFStringRef errStr;
	CFWriteStreamRef write = CFWriteStreamCreateWithAllocatedBuffers(kCFAllocatorDefault, kCFAllocatorDefault);
	CFWriteStreamOpen(write);
	uint32_t length = CFPropertyListWriteToStream(data, write, format, &errStr);
	CFDataRef xmlData = CFWriteStreamCopyProperty(write, kCFStreamPropertyDataWritten);
	sdmmd_return_t result = kAMDInvalidArgumentError;
	if (length == CFDataGetLength(xmlData)) {
		result = ((data) ? SDMMD_ServiceSend(handle, xmlData) : kAMDInvalidArgumentError);
	}
	if (xmlData)
		CFRelease(xmlData);
	CFWriteStreamClose(write);
	if (write)
		CFRelease(write);
	return result;
}

sdmmd_return_t SDMMD_ServiceReceiveStream(SocketConnection handle, CFPropertyListRef *data) {
	CFDataRef dataBuffer = NULL;
	if (SDM_MD_CallSuccessful(SDMMD_ServiceReceive(handle, &dataBuffer))) {
		if (dataBuffer && CFDataGetLength(dataBuffer)) {
			CFReadStreamRef read = CFReadStreamCreateWithBytesNoCopy(kCFAllocatorDefault, CFDataGetBytePtr(dataBuffer), CFDataGetLength(dataBuffer), kCFAllocatorNull);
			CFReadStreamOpen(read);
			*data = CFPropertyListCreateWithStream(kCFAllocatorDefault, read, CFDataGetLength(dataBuffer), 0x2, 0, NULL);
			CFReadStreamClose(read);
			if (read)
				CFRelease(read);
		}
		return kAMDSuccess;
	} else {
		return kAMDNotConnectedError;
	}
}

SocketConnection SDMMD_TranslateConnectionToSocket(SDMMD_AMConnectionRef connection) {
	SocketConnection sock;
	if (connection->ivars.ssl) {
		sock = (SocketConnection){true, {.ssl = connection->ivars.ssl}};
	} else {
		sock = (SocketConnection){false, {.conn = connection->ivars.socket}};
	}
	return sock;
}

#endif