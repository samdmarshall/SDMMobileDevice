/*
 *  SDMMD_AFC.h
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

#ifndef _SDM_MD_AFC_H_
#define _SDM_MD_AFC_H_

#include <CoreFoundation/CoreFoundation.h>
#include "SDMMD_Error.h"
#include "SDMMD_AFCDevice.h"
#include "SDMMD_AFCIterator.h"
#include "SDMMD_AFCOperation.h"
#include "SDMMD_AFCLock.h"

static char *gAFCPacketTypeNames[39] = {
	"Invalid",
	"Status",
	"Data",
	"ReadDirectory",
	"ReadFile",
	"WriteFile",
	"WritePart",
	"TruncFile",
	"RemovePath",
	"MakeDirectory",
	"GetFileInfo",
	"GetDeviceInfo",
	"WriteFileAtomic",
	"FileRefOpen",
	"FileRefOpenResult",
	"FileRefRead",
	"FileRefWrite",
	"FileRefSeek",
	"FileRefTell",
	"FileRefTellResult",
	"FileRefClose",
	"FileRefSetFileSize",
	"GetConnectionInfo",
	"SetConnectionOptions",
	"RenamePath",
	"SetFSBlockSize",
	"SetSocketBlockSize",
	"FileRefLock",
	"MakeLink",
	"GetFileHash",
	"SetModTime",
	"GetFileHashWithRange",
	"FileRefSetImmutableHint",
	"GetSizeOfPathContents",
	"RemovePathAndContents",
	"DirectoryEnumeratorRefOpen",
	"DirectoryEnumeratorRefOpenResult",
	"DirectoryEnumeratorRefRead",
	"DirectoryEnumeratorRefClose"
};

typedef struct SDMMD_AFCHeader {
	uint64_t header;
	uint64_t a;
	uint64_t b;
	uint64_t c;
	uint64_t d;
} __attribute__ ((packed)) SDMMD_AFCHeader;

#define SDMMD_AFCHeaderRef SDMMD_AFCHeader*

void SDMMD_AFCLog(uint32_t level, const char *format, ...);
void* SDMMD_AFCSetErrorInfoWithArgs(uint32_t level, uint32_t mask, uint32_t code, char *file, uint32_t line, char *call);
sdmmd_return_t SDMMD__AFCSetErrorResult(uint32_t level, uint32_t code, uint32_t line, char *call);

char* SDMMD_AFCStringCopy(char *dest, uint32_t destLength, char *source, uint32_t sourceLength);
char* SDMMD_AFCPacketTypeName(uint32_t packetType);
void* SDMMD_AFCSendStatusExtended(CFTypeRef, void*, uint32_t packetType, CFDictionaryRef ref);
void* SDMMD_AFCSendStatus(CFTypeRef a, void*b, void*c);
void* SDMMD_AFCSendDataPacket(CFTypeRef a, void*b, uint32_t *dataBytePtr, uint32_t dataLength);
void* SDMMD_AFCSendHeader(CFTypeRef a, void*b);
void* SDMMD_AFCReadPacket(CFTypeRef a, CFTypeRef* b, CFTypeRef* c, CFTypeRef* d);
void* SDMMD_AFCReadPacketBody(CFTypeRef a,void*b, CFDataRef* c, uint32_t *readLength);
void* SDMMD_AFCSendPacket(CFTypeRef a,CFTypeRef b,void*c, uint32_t size);
uint32_t SDMMD_AFCHeaderInit(SDMMD_AFCHeaderRef header,void*b,void*c,void*d,void*e);

#endif