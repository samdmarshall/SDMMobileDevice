/*
 *  SDMMD_AFC.h
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

#ifndef _SDM_MD_AFC_H_
#define _SDM_MD_AFC_H_

#if 0

#include <CoreFoundation/CoreFoundation.h>
#include "SDMMD_Error.h"
#include "SDMMD_Service.h"
//#include "SDMMD_AFCDevice.h"
//#include "SDMMD_AFCIterator.h"
//#include "SDMMD_AFCOperation.h"
//#include "SDMMD_AFCLock.h"
//#include "SDMMD_AFCCondition.h"

#pragma mark -
#pragma mark TYPES
#pragma mark -

struct sdmmd_AFCConnectionClass {
	SDMMD_AMConnectionRef handle;
	__unsafe_unretained dispatch_queue_t operationQueue;
	__unsafe_unretained dispatch_semaphore_t semaphore;
	uint64_t operationCount;
} sdmmd_AFCConnectionClass;

typedef struct sdmmd_AFCConnectionClass* SDMMD_AFCConnectionRef;

typedef struct SDMMD_AFCPacketHeader {
	uint64_t signature;
	uint64_t packetLen;
	uint64_t headerLen;
	uint64_t pid;
	uint64_t type;
} ATR_PACK SDMMD_AFCPacketHeader;

struct sdmmd_AFCPacket {
	SDMMD_AFCPacketHeader header;
	char* data;
} sdmmd_AFCPacket;

struct sdmmd_AFCOperation {
	struct sdmmd_AFCPacket *packet;
	dispatch_time_t timeout;
} sdmmd_AFCOperation;

typedef struct sdmmd_AFCOperation* SDMMD_AFCOperationRef;

enum SDMMD_AFC_Packet_Type {
	SDMMD_AFC_Packet_Invalid = 0,
	SDMMD_AFC_Packet_Status,
	SDMMD_AFC_Packet_Data,
	SDMMD_AFC_Packet_ReadDirectory = 3,
	SDMMD_AFC_Packet_ReadFile,
	SDMMD_AFC_Packet_WriteFile,
	SDMMD_AFC_Packet_WritePart,
	SDMMD_AFC_Packet_TruncFile,
	SDMMD_AFC_Packet_RemovePath = 8,
	SDMMD_AFC_Packet_MakeDirectory = 9,
	SDMMD_AFC_Packet_GetFileInfo = 10,
	SDMMD_AFC_Packet_GetDeviceInfo = 11,
	SDMMD_AFC_Packet_WriteFileAtomic = 12,
	SDMMD_AFC_Packet_FileRefOpen = 13,
	SDMMD_AFC_Packet_FileRefOpenResult = 14,
	SDMMD_AFC_Packet_FileRefRead = 15,
	SDMMD_AFC_Packet_FileRefWrite = 16,
	SDMMD_AFC_Packet_FileRefSeek,
	SDMMD_AFC_Packet_FileRefTell,
	SDMMD_AFC_Packet_FileRefTellResult,
	SDMMD_AFC_Packet_FileRefClose,
	SDMMD_AFC_Packet_FileRefSetFileSize,
	SDMMD_AFC_Packet_GetConnectionInfo = 22,
	SDMMD_AFC_Packet_SetConnectionOptions,
	SDMMD_AFC_Packet_RenamePath = 24,
	SDMMD_AFC_Packet_SetFSBlockSize,
	SDMMD_AFC_Packet_SetSocketBlockSize,
	SDMMD_AFC_Packet_FileRefLock = 27,
	SDMMD_AFC_Packet_MakeLink = 28,
	SDMMD_AFC_Packet_GetFileHash = 29,
	SDMMD_AFC_Packet_SetModTime = 30,
	SDMMD_AFC_Packet_GetFileHashWithRange,
	SDMMD_AFC_Packet_FileRefSetImmutableHint,
	SDMMD_AFC_Packet_GetSizeOfPathContents,
	SDMMD_AFC_Packet_RemovePathAndContents,
	SDMMD_AFC_Packet_DirectoryEnumeratorRefOpen,
	SDMMD_AFC_Packet_DirectoryEnumeratorRefOpenResult,
	SDMMD_AFC_Packet_DirectoryEnumeratorRefRead,
	SDMMD_AFC_Packet_DirectoryEnumeratorRefClose,
	SDMMD_AFC_Packet_Count
};

static char* SDMMD_gAFCPacketTypeNames[SDMMD_AFC_Packet_Count] = {
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

#pragma mark -
#pragma mark Functions
#pragma mark -

static inline char*  SDMMD_GetPacketTypeName(uint32_t type) {
	return SDMMD_gAFCPacketTypeNames[type];
}

CFDataRef SDMMD_GetDataResponseFromOperation(SDMMD_AFCOperationRef op);

SDMMD_AFCConnectionRef SDMMD_AFCConnectionCreate(SDMMD_AMConnectionRef conn);
void SDMMD_AFCConnectionRelease(SDMMD_AFCConnectionRef);

SDMMD_AFCOperationRef SDMMD_AFCOperationCreateReadDirectory(CFStringRef path);

SDMMD_AFCOperationRef SDMMD_AFCOperationCreateRemovePath(CFStringRef path);
SDMMD_AFCOperationRef SDMMD_AFCOperationCreateMakeDirectory(CFStringRef path);
SDMMD_AFCOperationRef SDMMD_AFCOperationCreateGetFileInfo(CFStringRef path);
SDMMD_AFCOperationRef SDMMD_AFCOperationCreateGetConnectionInfo();
SDMMD_AFCOperationRef SDMMD_AFCOperationCreateGetDeviceInfo();
SDMMD_AFCOperationRef SDMMD_AFCOperationCreateOpenFile(CFStringRef path, uint16_t mode, void* *fileRef);

SDMMD_AFCOperationRef SDMMD_AFCOperationCreateRenamePath(CFStringRef old, CFStringRef new);

SDMMD_AFCOperationRef SDMMD_AFCOperationCreateLinkPath(uint64_t linkType, CFStringRef target, CFStringRef link);
SDMMD_AFCOperationRef SDMMD_AFCOperationCreateGetFileHash(CFStringRef path);
SDMMD_AFCOperationRef SDMMD_AFCOperationCreateSetModTime(CFStringRef ref);

sdmmd_return_t SDMMD_AFCConnectionPerformOperation(SDMMD_AFCConnectionRef conn, SDMMD_AFCOperationRef op);

sdmmd_return_t SDMMD_AFCProcessOperation(SDMMD_AFCConnectionRef conn, SDMMD_AFCOperationRef op, SDMMD_AFCOperationRef *response);

sdmmd_return_t SDMMD_AMDeviceCopyFile(void *thing, void *thing2, void *thing3, SDMMD_AFCConnectionRef conn, char *local, char *remote);

/*void SDMMD_AFCLog(uint32_t level, const char *format, ...);
sdmmd_return_t SDMMD_AFCSetErrorInfoWithArgs(uint32_t level, uint32_t mask, uint32_t code, char *file, uint32_t line, char *call);
sdmmd_return_t SDMMD__AFCSetErrorResult(uint32_t level, uint32_t code, uint32_t line, char *call);

char* SDMMD_AFCStringCopy(char *dest, uint32_t destLength, char *source, uint32_t sourceLength);
char* SDMMD_AFCPacketTypeName(uint32_t packetType);
sdmmd_return_t SDMMD_AFCSendStatusExtended(SDMMD_AFCConnectionRef afcConn, void*, uint32_t packetType, CFDictionaryRef ref);
sdmmd_return_t SDMMD_AFCSendStatus(SDMMD_AFCConnectionRef afcConn, void*b, void*c);
sdmmd_return_t SDMMD_AFCSendDataPacket(SDMMD_AFCConnectionRef afcConn, void*b, uint32_t *dataBytePtr, uint32_t dataLength);
sdmmd_return_t SDMMD_AFCSendHeader(SDMMD_AFCConnectionRef afcConn, void*b);
sdmmd_return_t SDMMD_AFCReadPacket(SDMMD_AFCConnectionRef afcConn, CFTypeRef* b, CFTypeRef* c, CFTypeRef* d);
sdmmd_return_t SDMMD_AFCReadPacketBody(CFTypeRef a,void*b, CFDataRef* c, uint32_t *readLength);
sdmmd_return_t SDMMD_AFCSendPacket(SDMMD_AFCConnectionRef afcConn, CFTypeRef b, void* c, uint32_t size);*/

#endif

#endif