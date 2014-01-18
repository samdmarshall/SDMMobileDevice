/*
 *  SDMMD_AFC.c
 *  SDMMobileDevice
 *
 * Copyright (c) 2013, Sam Marshall
* All rights reserved.
* 
* Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
* 
* 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
* 
* 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
* 
* 3. Neither the name of Sam Marshall nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
* 
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 */

#ifndef _SDM_MD_AFC_C_
#define _SDM_MD_AFC_C_

#include "SDMMD_Functions.h"
#include "SDMMD_AFC.h"
#include <string.h>
#include "Core.h"

sdmmd_return_t SDMMD_check_can_touch(SDMMD_AFCConnectionRef conn, CFDataRef *unknown) {
	SDMMD_AFCOperationRef fileInfo = SDMMD_AFCOperationCreateGetFileInfo((CFStringRef)*unknown);
	SDMMD_AFCOperationRef reply; 
	SDMMD_AFCProcessOperation(conn, fileInfo, &reply);
	*unknown = SDMMD_GetDataResponseFromOperation(reply);
	return kAMDSuccess;
}

SDMMD_AFCConnectionRef SDMMD_AFCConnectionCreate(SDMMD_AMConnectionRef conn) {
	SDMMD_AFCConnectionRef afc = calloc(1, sizeof(struct sdmmd_AFCConnectionClass));
	afc->handle = conn;
	CFStringRef name = CFStringCreateWithFormat(kCFAllocatorDefault, NULL, CFSTR("%s.%s.%s"), "com.samdmarshall.sdmmobiledevice.afc", SDMCFStringGetString((conn->ivars.device)->ivars.unique_device_id), SDMCFStringGetString(SDMGetCurrentDateString()));
	afc->operationQueue = dispatch_queue_create(SDMCFStringGetString(name), NULL);
	afc->operationCount = 0;
	return afc;
}

void SDMMD_AFCConnectionRelease(SDMMD_AFCConnectionRef conn) {
	
}

void SDMMD_AFCHeaderInit(SDMMD_AFCPacketHeader *header, uint32_t command, uint32_t size, uint32_t data, uint32_t pack_num) {
	header->signature = 0x4141504c36414643;
	header->headerLen = size;
	header->packetLen = data+size;
	header->type = command;
	if (pack_num) {
		header->pid = pack_num;
	} else {
		header->pid = 0xffffffff;
	}
}

SDMMD_AFCOperationRef SDMMD_AFCOperationCreateReadDirectory(CFStringRef path) {
	SDMMD_AFCOperationRef op = calloc(1, sizeof(struct sdmmd_AFCOperation));
	op->packet = calloc(1, sizeof(struct sdmmd_AFCPacket));
	char *cpath = SDMCFStringGetString(path);
	op->packet->data = calloc(1, strlen(cpath)+1);
	memcpy(op->packet->data, cpath, strlen(cpath));
	SDMMD_AFCHeaderInit(&op->packet->header, 0x3, (uint32_t)strlen(cpath)+0x29, 0x0, 0x0);
	free(cpath);
	return op;
}

/*
SDMMD_AFCOperationRef SDMMD
	SDMMD_AFCOperationRef op = calloc(1, sizeof(struct sdmmd_AFCOperation));
	op->packet = calloc(1, sizeof(struct sdmmd_AFCPacket));
	SDMMD_AFCHeaderInit(&op->packet->header, , , 0x0, 0x0);
	return op;
}
*/

SDMMD_AFCOperationRef SDMMD_AFCOperationCreateRemovePath(CFStringRef path) {
	SDMMD_AFCOperationRef op = calloc(1, sizeof(struct sdmmd_AFCOperation));
	op->packet = calloc(1, sizeof(struct sdmmd_AFCPacket));
	char *cpath = SDMCFStringGetString(path);
	op->packet->data = calloc(1, strlen(cpath)+1);
	memcpy(op->packet->data, cpath, strlen(cpath));
	SDMMD_AFCHeaderInit(&op->packet->header, 0x8, (uint32_t)strlen(cpath)+0x29, 0x0, 0x0);
	free(cpath);
	return op;
}

SDMMD_AFCOperationRef SDMMD_AFCOperationCreateMakeDirectory(CFStringRef path) {
	SDMMD_AFCOperationRef op = calloc(1, sizeof(struct sdmmd_AFCOperation));
	op->packet = calloc(1, sizeof(struct sdmmd_AFCPacket));
	char *cpath = SDMCFStringGetString(path);
	op->packet->data = calloc(1, strlen(cpath)+1);
	memcpy(op->packet->data, cpath, strlen(cpath));
	SDMMD_AFCHeaderInit(&op->packet->header, 0x9, (uint32_t)strlen(cpath)+0x29, 0x0, 0x0);
	free(cpath);
	return op;
}

SDMMD_AFCOperationRef SDMMD_AFCOperationCreateGetFileInfo(CFStringRef path) {
	SDMMD_AFCOperationRef op = calloc(1, sizeof(struct sdmmd_AFCOperation));
	op->packet = calloc(1, sizeof(struct sdmmd_AFCPacket));
	char *cpath = SDMCFStringGetString(path);
	op->packet->data = calloc(1, strlen(cpath)+1);
	memcpy(op->packet->data, cpath, strlen(cpath));
	SDMMD_AFCHeaderInit(&op->packet->header, 0xa, (uint32_t)strlen(cpath)+0x29, 0x0, 0x0);
	free(cpath);
	return op;
}

SDMMD_AFCOperationRef SDMMD_AFCOperationCreateGetDeviceInfo() {
	SDMMD_AFCOperationRef op = calloc(1, sizeof(struct sdmmd_AFCOperation));
	op->packet = calloc(1, sizeof(struct sdmmd_AFCPacket));
	SDMMD_AFCHeaderInit(&op->packet->header, 0xb, 0x28, 0x0, 0x0);
	op->timeout = dispatch_time(DISPATCH_TIME_NOW, 0);
	return op;
}

SDMMD_AFCOperationRef SDMMD_AFCOperationCreateOpenFile(CFStringRef path, uint16_t mode, void* *fileRef) {
	SDMMD_AFCOperationRef op = calloc(1, sizeof(struct sdmmd_AFCOperation));
	op->packet = calloc(1, sizeof(struct sdmmd_AFCPacket));
	char *cpath = SDMCFStringGetString(path);
	op->packet->data = calloc(1, strlen(cpath)+2);
	op->packet->data = (*(void**)&(mode));
	memcpy(&op->packet->data[2], cpath, strlen(cpath));
	SDMMD_AFCHeaderInit(&op->packet->header, 0xd, (uint32_t)strlen(cpath)+0x31, 0x0, 0x0);
	free(cpath);
	return op;
}

/*
SDMMD_AFCOperationRef SDMMD
	SDMMD_AFCOperationRef op = calloc(1, sizeof(struct sdmmd_AFCOperation));
	op->packet = calloc(1, sizeof(struct sdmmd_AFCPacket));
	SDMMD_AFCHeaderInit(&op->packet->header, , , 0x0, 0x0);
	return op;
}
*/

SDMMD_AFCOperationRef SDMMD_AFCFileDescriptorCreateReadOperation(void* fileRef) {
	SDMMD_AFCOperationRef op = calloc(1, sizeof(struct sdmmd_AFCOperation));
	op->packet = calloc(1, sizeof(struct sdmmd_AFCPacket));
	SDMMD_AFCHeaderInit(&op->packet->header, 0xf, 0x38, 0x0, 0x0);
	return op;
}

SDMMD_AFCOperationRef SDMMD_AFCFileDescriptorCreateWriteOperation(void* thing, CFDataRef data, void* thing2) {
	SDMMD_AFCOperationRef op = calloc(1, sizeof(struct sdmmd_AFCOperation));
	op->packet = calloc(1, sizeof(struct sdmmd_AFCPacket));
	SDMMD_AFCHeaderInit(&op->packet->header, 0x10, 0x30, (uint32_t)CFDataGetLength(data), 0x0);
	return op;
}

SDMMD_AFCOperationRef SDMMD_AFCOperationCreateRenamePath(CFStringRef old, CFStringRef new) {
	SDMMD_AFCOperationRef op = calloc(1, sizeof(struct sdmmd_AFCOperation));
	op->packet = calloc(1, sizeof(struct sdmmd_AFCPacket));
	char *oldPath = SDMCFStringGetString(old);
	char *newPath = SDMCFStringGetString(new);
	op->packet->data = calloc(1, strlen(oldPath)+strlen(newPath)+2);
	memcpy(&op->packet->data, oldPath, strlen(oldPath));
	memcpy(&op->packet->data[strlen(oldPath)+1], newPath, strlen(newPath));
	SDMMD_AFCHeaderInit(&op->packet->header, 0x18, (uint32_t)strlen(oldPath)+(uint32_t)strlen(newPath)+0x30, 0x0, 0x0);
	free(oldPath);
	free(newPath);
	return op;
}

SDMMD_AFCOperationRef SDMMD_AFCFileDescriptorCreateLockOperation(void* fileRef) {
	SDMMD_AFCOperationRef op = calloc(1, sizeof(struct sdmmd_AFCOperation));
	op->packet = calloc(1, sizeof(struct sdmmd_AFCPacket));
	SDMMD_AFCHeaderInit(&op->packet->header, 0x1b, 0x38, 0x0, 0x0);
	return op;
}

SDMMD_AFCOperationRef SDMMD_AFCOperationCreateLinkPath(uint64_t linkType, CFStringRef target, CFStringRef link) {
	SDMMD_AFCOperationRef op = calloc(1, sizeof(struct sdmmd_AFCOperation));
	op->packet = calloc(1, sizeof(struct sdmmd_AFCPacket));
	char *targetPath = SDMCFStringGetString(target);
	char *linkPath = SDMCFStringGetString(link);
	op->packet->data = calloc(1, strlen(targetPath)+strlen(linkPath)+2);
	memcpy(&op->packet->data, targetPath, strlen(targetPath));
	memcpy(&op->packet->data[strlen(targetPath)+1], linkPath, strlen(linkPath));
	SDMMD_AFCHeaderInit(&op->packet->header, 0x1c, (uint32_t)strlen(targetPath)+(uint32_t)strlen(linkPath)+0x30, 0x0, 0x0);
	free(targetPath);
	free(linkPath);
	return op;
}

SDMMD_AFCOperationRef SDMMD_AFCOperationCreateGetFileHash(CFStringRef path) {
	SDMMD_AFCOperationRef op = calloc(1, sizeof(struct sdmmd_AFCOperation));
	op->packet = calloc(1, sizeof(struct sdmmd_AFCPacket));
	char *cpath = SDMCFStringGetString(path);
	memcpy(op->packet->data, cpath, strlen(cpath));
	SDMMD_AFCHeaderInit(&op->packet->header, 0x1d, (uint32_t)strlen(cpath)+0x29, 0x0, 0x0);
	free(cpath);
	return op;
}

SDMMD_AFCOperationRef SDMMD_AFCOperationCreateSetModTime(CFStringRef ref) {
	SDMMD_AFCOperationRef op = calloc(1, sizeof(struct sdmmd_AFCOperation));
	op->packet = calloc(1, sizeof(struct sdmmd_AFCPacket));
	char *cref = SDMCFStringGetString(ref);
	memcpy(op->packet->data, cref, strlen(cref));
	SDMMD_AFCHeaderInit(&op->packet->header, 0x1e, (uint32_t)strlen(cref)+0x31, 0x0, 0x0);
	free(cref);
	return op;
}

sdmmd_return_t SDMMD_AFCSendOperation(SDMMD_AFCConnectionRef conn, SDMMD_AFCOperationRef op);
sdmmd_return_t SDMMD_AFCReceiveOperation(SDMMD_AFCConnectionRef conn, SDMMD_AFCOperationRef *op);

sdmmd_return_t SDMMD_AFCConnectionPerformOperation(SDMMD_AFCConnectionRef conn, SDMMD_AFCOperationRef op) {
	__block sdmmd_return_t result = 0x0;
	dispatch_sync(conn->operationQueue, ^{
		printf("Packet: %s\n",SDMMD_gAFCPacketTypeNames[op->packet->header.type]);
		op->packet->header.pid = conn->operationCount;
		result = SDMMD_AFCSendOperation(conn, op);
		SDMMD_AFCOperationRef response;
		result = SDMMD_AFCReceiveOperation(conn, &response);
		printf("Response: %08x\n",result);
		PrintCFType(response->packet->data);
	});
	conn->operationCount++;
	return result;
}

sdmmd_return_t SDMMD_AFCSendOperation(SDMMD_AFCConnectionRef conn, SDMMD_AFCOperationRef op) {
	sdmmd_return_t result = 0x0;
	CFDataRef headerData = CFDataCreate(kCFAllocatorDefault, (UInt8*)&op->packet->header, sizeof(SDMMD_AFCPacketHeader));
	result = SDMMD_DirectServiceSend(SDMMD_TranslateConnectionToSocket(conn->handle), headerData);
	printf("header sent status: %08x\n",result);
	CFDataRef bodyData = CFDataCreate(kCFAllocatorDefault, (UInt8*)&op->packet->data, (uint32_t)op->packet->header.packetLen - sizeof(SDMMD_AFCPacketHeader));
	result = SDMMD_DirectServiceSend(SDMMD_TranslateConnectionToSocket(conn->handle), bodyData);
	printf("body sent status: %08x\n",result);
	return result;
}

sdmmd_return_t SDMMD_AFCReceiveOperation(SDMMD_AFCConnectionRef conn, SDMMD_AFCOperationRef *op) {
	sdmmd_return_t result = 0x0;
	CFMutableDataRef headerData = CFDataCreateMutable(kCFAllocatorDefault, sizeof(SDMMD_AFCPacketHeader));
	SDMMD_AFCPacketHeader *zeros = calloc(1, sizeof(SDMMD_AFCPacketHeader));
	CFDataAppendBytes(headerData, (UInt8*)zeros, sizeof(SDMMD_AFCPacketHeader));
	free(zeros);
	result = SDMMD_DirectServiceReceive(SDMMD_TranslateConnectionToSocket(conn->handle), (CFDataRef*)&headerData);
	SDMMD_AFCPacketHeader *header = (SDMMD_AFCPacketHeader *)CFDataGetBytePtr(headerData);
	
	CFMutableDataRef bodyData = CFDataCreateMutable(kCFAllocatorDefault, (uint32_t)header->packetLen - sizeof(SDMMD_AFCPacketHeader));
	char *body = calloc(1, (uint32_t)header->packetLen - sizeof(SDMMD_AFCPacketHeader));
	CFDataAppendBytes(bodyData, (UInt8*)body, (uint32_t)header->packetLen - sizeof(SDMMD_AFCPacketHeader));
	free(body);
	result = SDMMD_DirectServiceReceive(SDMMD_TranslateConnectionToSocket(conn->handle), (CFDataRef*)&bodyData);
	struct sdmmd_AFCPacket *packet = calloc(1, sizeof(struct sdmmd_AFCPacket));
	packet->header = *header;
	if (bodyData)
		packet->data = (void*)CFDataGetBytePtr(bodyData);
	SDMMD_AFCOperationRef response = calloc(1, sizeof(struct sdmmd_AFCOperation));
	response->packet = packet;
	response->timeout = 0;
	*op = response;
	return result;
}

sdmmd_return_t SDMMD_AFCProcessOperation(SDMMD_AFCConnectionRef conn, SDMMD_AFCOperationRef op, SDMMD_AFCOperationRef *response) {
	__block sdmmd_return_t result = 0x0;
	__block SDMMD_AFCOperationRef blockReply;
	dispatch_sync(conn->operationQueue, ^{
		conn->semaphore = dispatch_semaphore_create(0x0);
		op->packet->header.pid = conn->operationCount;
		result = SDMMD_AFCSendOperation(conn, op);
		dispatch_semaphore_wait(conn->semaphore, op->timeout);
		SDMMD_AFCReceiveOperation(conn, &blockReply);
		dispatch_release(conn->semaphore);
		conn->operationCount++;
	});
	*response = blockReply;
	return result;
}

CFDataRef SDMMD_GetDataResponseFromOperation(SDMMD_AFCOperationRef op) {
	return CFDataCreateWithBytesNoCopy(kCFAllocatorDefault, (UInt8*)op->packet->data, (uint32_t)op->packet->header.packetLen-(uint32_t)op->packet->header.headerLen, kCFAllocatorDefault);
}


sdmmd_return_t SDMMD_AMDeviceCopyFile(void *thing, void *thing2, void *thing3, SDMMD_AFCConnectionRef conn, char *local, char *remote) {
	return kAMDSuccess;
}

/*CFMutableDataRef SDMMD___AFCCreateAFCDataWithDictionary(CFDictionaryRef dict) {
	CFMutableDataRef data = CFDataCreateMutable(kCFAllocatorDefault, kCFAllocatorDefault);
	if (data) {
		CFDictionaryApplyFunction(dict, SDMMD___ConvertDictEntry, data);
	}
	return data;
}

void SDMMD_AFCLog(uint32_t level, const char *format, ...) {
	va_list args;
	va_start(args, format);
	vprintf(format, args);
	va_end(args);
}

sdmmd_return_t SDMMD_AFCSetErrorInfoWithArgs(uint32_t level, uint32_t mask, uint32_t code, char *file, uint32_t line, char *call) {
	sdmmd_return_t result = 0x0;
	CFTypeRef err;
	SDMMD_AFCErrorInfoCreateWithArgs(err, mask, code, file, line, call);
	if (err) {
		
	}
	return result;
}

sdmmd_return_t SDMMD__AFCSetErrorResult(uint32_t level, uint32_t code, uint32_t line, char *call) {
	sdmmd_return_t result = 0x0;
	SDMMD_AFCLog(0x5, "Setting error result %d, %d, %s, %d\n", 0xffffffff, code, __FILE__, line);
	result = SDMMD_AFCSetErrorInfoWithArgs(level, 0xffffffff, code, __FILE__, line, call);
	return result;
}

char* SDMMD_AFCStringCopy(char *dest, uint32_t destLength, char *source, uint32_t sourceLength) {
	if (sourceLength) {
		if (sourceLength < destLength) {
			sourceLength = destLength;
		}
		strncpy(dest, source, sourceLength);
	} else {
		strlcpy(dest, source, destLength);
	}
	return dest;
}

char* SDMMD_AFCPacketTypeName(uint32_t packetType) {
	char *name = "Unknown";
	if (packetType != 0x0 && packetType <= 0x28) {
		name = gAFCPacketTypeNames[packetType];
	}
	return name;
}

sdmmd_return_t SDMMD_AFCSendStatusExtended(SDMMD_AFCConnectionRef afcConn, void*b, uint32_t packetType, CFDictionaryRef ref) {
	sdmmd_return_t result = 0x0;
	SDMMD_AFCLog(0x5, "Writing status packet %d info %p\\n",packetType,ref);
	if (afcConn && packetType >= 0x15) {
		SDMMD_AFCLog(0x5, "Oh no!");
	}
	if (afcConn->ivars.e == 0x0) {
		uint32_t length = 0x30;
		CFDataRef afcData = NULL;
		if (ref != 0x0 && (afcConn->ivars.j & 1) != 0x0) {
			afcData = SDMMD___AFCCreateAFCDataWithDictionary(ref);
			length += CFDataGetLength(afcData);
		}
		uint64_t header = 0x4141504c36414643;
		uint32_t data24 = 0x30;
		uint32_t data40 = 0x1;
		uint32_t packetMask = packetType & 0x1fff;
		uint32_t data32 = 0x0;
		if (b != 0x0) {
			data32 = (b+0x18);
		} else {
			data32 = 0xffffffff;
		}
		result = SDMMD_AFCSendHeader(afcConn, &header);
		if (afcData != 0x0) {
			if (result == 0x0) {
				result = SDMMD_AFCSendData(afcConn, CFDataGetBytePtr(afcData), length);
			}
		}
		CFRelease(afcData);
	} else {
		result = afcConn->ivars.statusPtr;	
	}
	return result;
}

sdmmd_return_t SDMMD_AFCSendStatus(SDMMD_AFCConnectionRef afcConn, void*b, void*c) {
	CFTypeRef arg = SDMMD_AFCCopyAndClearLastErrorInfo();
	sdmmd_return_t result = SDMMD_AFCSendStatusExtended(afcConn, b, c, arg);
	if (arg) {
		CFRelease(arg);
	}
	return result;
}

sdmmd_return_t SDMMD_AFCSendDataPacket(SDMMD_AFCConnectionRef afcConn, void*b, uint32_t *dataBytePtr, uint32_t dataLength) {
	sdmmd_return_t result = 0x0;
	SDMMD_AFCLog(0x5, "Writing data packet with data length %u\n",dataLength);
	if (afcConn->ivars.e != 0x0) {
		result = afcConn->ivars.statusPtr;
	} else {
		uint64_t header = 0x4141504c36414643;
		uint32_t packetLength = dataLength + 0x28;
		uint32_t data32 = 0x0;
		if (b != 0x0) {
			data32 = ((char*)b+0x18);
		} else {
			data32 = 0xffffffff;
		}
		result = SDMMD_AFCSendHeader(afcConn, &header);
		if (result == 0x0) {
			result = SDMMD_AFCSendData(afcConn, dataBytePtr, dataLength);
		}
	}
	return result;
}

sdmmd_return_t SDMMD_AFCSendHeader(SDMMD_AFCConnectionRef afcConn, void*b) {
	sdmmd_return_t result = 0x0;
	if (afcConn->ivars.e != 0x0) {
		result = afcConn->ivars.statusPtr;
	} else {
		SDMMD_AFCLogPacketInfo(0x4, "AFCSendHeader", b);
		result = SDMMD_AFCSendData(afcConn, b, 0x10);
	}
	return result;
}

sdmmd_return_t SDMMD_AFCReadPacket(SDMMD_AFCConnectionRef afcConn, CFTypeRef* b, CFTypeRef* c, CFTypeRef* d) {
	sdmmd_return_t result = 0xe800400b;
	CFTypeRef packetHeader = NULL;
	CFTypeRef packetBody0 = 0x0;
	uint32_t packetBody1 = 0x0;
	SDMMD_AFCLockLock(afcConn->ivars.lock1);
	uint32_t packetHeaderUnknown = 0x0;
	if (afcConn->ivars.state) {
		result = SDMMD_AFCReadPacketHeader(afcConn, &packetHeaderUnknown, 0x80, packetHeader);
		if (result == 0x0) {
			result = SDMMD_AFCReadPacketBody(afcConn, packetHeader, &packetBody0, &packetBody1);
			if (result == 0x0) {
				*b = packetHeader;
				*c = packetBody0;
				*d = packetBody1;
				result = 0x0;
			} else {
				if (packetBody0 != 0x0) {
					CFTypeRef allocRef = CFGetAllocator(afcConn);
					CFAllocatorDeallocate(allocRef, packetBody0);
				}
			}
		} else {
			if (packetHeader != 0x0) {
				CFTypeRef allocRef = CFGetAllocator(afcConn);
				CFAllocatorDeallocate(allocRef, packetHeader);
			}
		}
	}
	SDMMD_AFCLockUnlock(afcConn->ivars.lock1);
	return result;
}

sdmmd_return_t SDMMD_AFCReadPacketBody(CFTypeRef a,void*b, CFDataRef* c, uint32_t *readLength) {
	sdmmd_return_t result = 0x0;
	if ((a+0x44) == 0x0) {
		uint32_t dataLength = (b+0x8);
		CFDataRef data = NULL;
		if (dataLength-((uint32_t)(char*)b+0x10) != 0x0) {
			CFTypeRef allocRef = CFGetAllocator(a);
			data = CFAllocatorAllocate(allocRef, dataLength, 0x0);
			if (result == 0x0) {
				result = SDMMD__AFCSetErrorResult(0x0, 0xe8004003, __LINE__, "CFAllocatorAllocate");
			}
			result = SDMMD_AFCReadData(a, data, dataLength);
			if (result == 0x0) {
				
			} else {
				result = (a+0x40);
			}
		}
		*c = data;
		*readLength = dataLength;
	} else {
		result = (a+0x40);
	}
	return result;
}

sdmmd_return_t SDMMD_AFCSendPacket(SDMMD_AFCConnectionRef afcConn, CFTypeRef b, void* c, uint32_t size) {
	sdmmd_return_t result = 0xe800400b;
	SDMMD_AFCLockLock(afcConn->ivars.lock1);
	if (afcConn->ivars.state) {
		if (c == 0x0)
			result = 0xe8004007;
		if (((char*)b+0x10) + size > 0x2000) {
			result = SDMMD_AFCSendHeader(afcConn, b);
			if (result == 0x0) {
				result = SDMMD_AFCSendData(afcConn, c, size);
			}
		} else {
			char *data = malloc(0x2000);
			if (data) {
				memcpy(data, c, size);
				SDMMD_AFCLogPacketInfo(0x4, "AFCSendHeader", b);
				result = SDMMD_AFCSendData(afcConn, data, size);
				free(data);
			} else {
				result = SDMMD_AFCSendHeader(afcConn, b);
				if (result == 0x0) {
					result = SDMMD_AFCSendData(afcConn, c, size);
				}
			}
		}
	}
	SDMMD_AFCLockUnlock(afcConn->ivars.lock1);
	return result;
}

uint32_t SDMMD_AFCHeaderInit(SDMMD_AFCHeaderRef header, uint32_t command, uint32_t size, uint32_t data, uint32_t unknown) {
	header->signature = 0x4141504c36414643;
	header->headerLen = size;
	header->packetLen = data+size;
	header->type = command;
	if (unknown) {
		header->pid = unknown + 0x18;
	} else {
		header->pid = 0xffffffff;
	}
	return header->signature;
}

sdmmd_return_t SDMMD_AFCValidateHeader(SDMMD_AFCHeaderRef header, uint32_t command, void*b, void*c, uint64_t d) {
	if (header->signature != 0x434641364c504141) {
		if (d != 0x4141504c36414643) {
			return SDMMD___AFCSetErrorResult();
		} else {
			
		}
	} else {
		
	}
}*/

#endif