/*
 *  SDMMD_AFCOperation_Internal.h
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

#ifndef SDMMobileDevice_Framework_SDMMD_AFCOperation_Internal_h
#define SDMMobileDevice_Framework_SDMMD_AFCOperation_Internal_h

#include <CoreFoundation/CoreFoundation.h>
#include <SDMMobileDevice/CFRuntime.h>

typedef struct SDMMD_AFCPacketHeader {
	uint64_t signature;
	uint64_t packetLen;
	uint64_t headerLen;
	uint64_t pid;
	uint64_t type;
} __attribute__ ((packed)) SDMMD_AFCPacketHeader;

struct sdmmd_AFCPacket {
	SDMMD_AFCPacketHeader header;
	void* header_data;
	void* body_data;
	CFTypeRef response;
} sdmmd_AFCPacket;

struct sdmmd_AFCOperationBody {
	struct sdmmd_AFCPacket *packet;
	dispatch_time_t timeout;
} sdmmd_AFCOperationBody;

struct sdmmd_AFCOperation {
	CFRuntimeBase base;
	struct sdmmd_AFCOperationBody ivars;
} __attribute__ ((packed)) sdmmd_AFCOperation;

#endif
