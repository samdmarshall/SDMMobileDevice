/*
 *  SDMMD_AppleFileConduit.c
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

#ifndef SDMMobileDevice_Framework_SDMMD_AppleFileConduit_c
#define SDMMobileDevice_Framework_SDMMD_AppleFileConduit_c

#include "SDMMD_AppleFileConduit.h"
#include "SDMMD_AMDevice_Internal.h"
#include "SDMMD_Connection_Internal.h"
#include "SDMMD_AFCConnection_Internal.h"
#include "SDMMD_AFCOperation_Internal.h"
#include "SDMMD_Functions.h"
#include <string.h>
#include <sys/types.h>
#include "Core.h"
#include <dirent.h>

#define kAFCMaxTransferSize 4194304

void SDMMD_AFCHeaderInit(SDMMD_AFCPacketHeader *header, uint32_t command, uint32_t size, uint32_t data, uint32_t pack_num);

SDMMD_AFCConnectionRef SDMMD_AFCConnectionCreate(SDMMD_AMConnectionRef conn)
{
	SDMMD_AFCConnectionRef afc = SDMMD_AFCConnectionCreateEmpty();
	if (afc != NULL) {
		afc->ivars.handle = conn;
		char *udidString = SDMCFStringGetString((conn->ivars.device)->ivars.unique_device_id);
		CFStringRef date_string = SDMCreateCurrentDateString();
		char *dateString = SDMCFStringGetString(date_string);
		CFSafeRelease(date_string);
		CFStringRef name = CFStringCreateWithFormat(kCFAllocatorDefault, NULL, CFSTR("%s.%s.%s"), "com.samdmarshall.sdmmobiledevice.afc", udidString, dateString);
		Safe(free, udidString);
		Safe(free, dateString);
		char *queueName = SDMCFStringGetString(name);
		afc->ivars.operationQueue = dispatch_queue_create(queueName, NULL);
		Safe(free, queueName);
		CFSafeRelease(name);
		afc->ivars.operationCount = 0;
	}
	return afc;
}

#pragma mark -

void SDMMD_AFCHeaderInit(SDMMD_AFCPacketHeader *header, uint32_t command, uint32_t header_size, uint32_t body_size, uint32_t pack_num)
{
	header->signature = 0x4141504c36414643; // 'APPL6AFC'
	header->headerLen = header_size + sizeof(struct SDMMD_AFCPacketHeader);
	header->packetLen = body_size + header->headerLen;
	header->type = command;
	if (pack_num) {
		header->pid = pack_num;
	}
	else {
		header->pid = k64BitMask; // but not always? this is really weird
	}
}

sdmmd_return_t SDMMD_AFCSendOperation(SDMMD_AFCConnectionRef conn, SDMMD_AFCOperationRef op);
sdmmd_return_t SDMMD_AFCReceiveOperation(SDMMD_AFCConnectionRef conn, SDMMD_AFCOperationRef *op);

#if 0
sdmmd_return_t SDMMD_AFCConnectionPerformOperation(SDMMD_AFCConnectionRef conn, SDMMD_AFCOperationRef op) {
	__block sdmmd_return_t result = kAMDSuccess;
	dispatch_sync(conn->operationQueue, ^{
		//printf("Packet: %s\n",SDMMD_gAFCPacketTypeNames[op->ivars.packet->header.type]);
		op->ivars.packet->header.pid = conn->operationCount;
		result = SDMMD_AFCSendOperation(conn, op);
		SDMMD_AFCOperationRef response;
		result = SDMMD_AFCReceiveOperation(conn, &response);
		//printf("Response: %08x\n",result);
	});
	conn->operationCount++;
	return result;
}
#endif

sdmmd_return_t SDMMD_AFCSendOperation(SDMMD_AFCConnectionRef conn, SDMMD_AFCOperationRef op)
{
	sdmmd_return_t result = kAMDSuccess;
	CFMutableDataRef headerData = CFDataCreateMutable(kCFAllocatorDefault, (CFIndex)op->ivars.packet->header.headerLen); //(kCFAllocatorDefault, (UInt8*)&op->ivars.packet->header, sizeof(SDMMD_AFCPacketHeader));
	CFDataAppendBytes(headerData, (UInt8 *)&(op->ivars.packet->header), sizeof(SDMMD_AFCPacketHeader));
	if (op->ivars.packet->header.headerLen > sizeof(SDMMD_AFCPacketHeader)) {
		CFDataAppendBytes(headerData, (UInt8 *)(op->ivars.packet->header_data), (uint32_t)op->ivars.packet->header.headerLen - sizeof(SDMMD_AFCPacketHeader));
	}
	result = SDMMD_DirectServiceSend(SDMMD_TranslateConnectionToSocket(conn->ivars.handle), headerData);
	CFSafeRelease(headerData);
	//printf("header sent status: %08x %s\n",result,SDMMD_AMDErrorString(result));
	if (!(op->ivars.packet->header.headerLen == op->ivars.packet->header.packetLen && op->ivars.packet->body_data == NULL)) {
		CFDataRef bodyData = CFDataCreate(kCFAllocatorDefault, (UInt8 *)(op->ivars.packet->body_data), (uint32_t)op->ivars.packet->header.packetLen - (uint32_t)op->ivars.packet->header.headerLen);
		result = SDMMD_DirectServiceSend(SDMMD_TranslateConnectionToSocket(conn->ivars.handle), bodyData);
		CFSafeRelease(bodyData);
		//printf("body sent status: %08x %s\n",result,SDMMD_AMDErrorString(result));
	}
	return result;
}

sdmmd_return_t SDMMD_AFCReceiveOperation(SDMMD_AFCConnectionRef conn, SDMMD_AFCOperationRef *operation)
{
	sdmmd_return_t result = kAMDSuccess;
	CFMutableDataRef headerData = CFDataCreateMutable(kCFAllocatorDefault, sizeof(SDMMD_AFCPacketHeader));
	SDMMD_AFCPacketHeader *zeros = calloc(1, sizeof(SDMMD_AFCPacketHeader));
	CFDataAppendBytes(headerData, (UInt8 *)zeros, sizeof(SDMMD_AFCPacketHeader));
	free(zeros);

	result = SDMMD_DirectServiceReceive(SDMMD_TranslateConnectionToSocket(conn->ivars.handle), &headerData);
	if (result == kAMDSuccess) {
		SDMMD_AFCPacketHeader *header = (SDMMD_AFCPacketHeader *)CFDataGetBytePtr(headerData);

		if (header->packetLen > sizeof(struct SDMMD_AFCPacketHeader)) {
			uint32_t body_length = (uint32_t)header->packetLen - (uint32_t)sizeof(struct SDMMD_AFCPacketHeader);
			CFMutableDataRef bodyData = CFDataCreateMutable(kCFAllocatorDefault, body_length);
			char *body = calloc(body_length, sizeof(char));
			CFDataAppendBytes(bodyData, (UInt8 *)body, body_length);
			free(body);
			result = SDMMD_DirectServiceReceive(SDMMD_TranslateConnectionToSocket(conn->ivars.handle), &bodyData);
			if (bodyData) {
				(*operation)->ivars.packet->response = bodyData;
			}
		}
	}
	CFSafeRelease(headerData);

	return result;
}

sdmmd_return_t SDMMD_AFCProcessOperation(SDMMD_AFCConnectionRef conn, SDMMD_AFCOperationRef *operation)
{
	__block sdmmd_return_t result = kAMDSuccess;
	dispatch_sync(conn->ivars.operationQueue, ^{
		conn->ivars.semaphore = dispatch_semaphore_create(0);
		(*operation)->ivars.packet->header.pid = k64BitMask; //conn->operationCount;
		result = SDMMD_AFCSendOperation(conn, *operation);
		dispatch_semaphore_wait(conn->ivars.semaphore, (*operation)->ivars.timeout);
		CheckErrorAndReturn(result);
		
		result = SDMMD_AFCReceiveOperation(conn, operation);
		CheckErrorAndReturn(result);
		
		CFDataRef response_data = (*operation)->ivars.packet->response;
		bool validResponse = (response_data != NULL);
		
		switch ((*operation)->ivars.packet->header.type) {
			case SDMMD_AFC_Packet_Status: {
				break;
			}
			case SDMMD_AFC_Packet_Data: {
				break;
			}
			case SDMMD_AFC_Packet_ReadDirectory: {
				if (validResponse) {
					(*operation)->ivars.packet->response = SDMMD_ConvertResponseArray(response_data);
				}
				else {
					result = kAMDReadError;
				}
				break;
			}
			case SDMMD_AFC_Packet_ReadFile: {
				break;
			}
			case SDMMD_AFC_Packet_WriteFile: {
				break;
			}
			case SDMMD_AFC_Packet_WritePart: {
				break;
			}
			case SDMMD_AFC_Packet_TruncFile: {
				break;
			}
			case SDMMD_AFC_Packet_RemovePath: {
				break;
			}
			case SDMMD_AFC_Packet_MakeDirectory: {
				break;
			}
			case SDMMD_AFC_Packet_GetFileInfo: {
				bool should_parse = false;
				if (validResponse) {
					CFIndex data_length = CFDataGetLength(response_data);
					if (data_length == sizeof(uint64_t)) {
						uint64_t response;
						memcpy(&response, CFDataGetBytePtr(response_data), data_length);
						if (response != 8 && response != 4) {
							// this file can be accessed.
							should_parse = true;
						}
					}
					else {
						should_parse = true;
					}
				}
				
				if (should_parse) {
					(*operation)->ivars.packet->response = SDMMD_ConvertResponseDictionary(response_data);
				}
				else {
					result = kAMDReadError;
				}
				break;
			}
			case SDMMD_AFC_Packet_GetDeviceInfo: {
				if (validResponse) {
					(*operation)->ivars.packet->response = SDMMD_ConvertResponseDictionary(response_data);
				}
				else {
					result = kAMDReadError;
				}
				break;
			}
			case SDMMD_AFC_Packet_WriteFileAtomic: {
				break;
			}
			case SDMMD_AFC_Packet_FileRefOpen: {
				break;
			}
			case SDMMD_AFC_Packet_FileRefOpenResult: {
				break;
			}
			case SDMMD_AFC_Packet_FileRefRead: {
				break;
			}
			case SDMMD_AFC_Packet_FileRefWrite: {
				break;
			}
			case SDMMD_AFC_Packet_FileRefSeek: {
				break;
			}
			case SDMMD_AFC_Packet_FileRefTell: {
				break;
			}
			case SDMMD_AFC_Packet_FileRefTellResult: {
				break;
			}
			case SDMMD_AFC_Packet_FileRefClose: {
				break;
			}
			case SDMMD_AFC_Packet_FileRefSetFileSize: {
				break;
			}
			case SDMMD_AFC_Packet_GetConnectionInfo: {
				if (validResponse) {
					(*operation)->ivars.packet->response = SDMMD_ConvertResponseDictionary(response_data);
				}
				else {
					result = kAMDReadError;
				}
				break;
			}
			case SDMMD_AFC_Packet_SetConnectionOptions: {
				break;
			}
			case SDMMD_AFC_Packet_RenamePath: {
				break;
			}
			case SDMMD_AFC_Packet_SetFSBlockSize: {
				break;
			}
			case SDMMD_AFC_Packet_SetSocketBlockSize: {
				break;
			}
			case SDMMD_AFC_Packet_FileRefLock: {
				break;
			}
			case SDMMD_AFC_Packet_MakeLink: {
				break;
			}
			case SDMMD_AFC_Packet_GetFileHash: {
				break;
			}
			case SDMMD_AFC_Packet_SetModTime: {
				break;
			}
			case SDMMD_AFC_Packet_GetFileHashWithRange: {
				break;
			}
			case SDMMD_AFC_Packet_FileRefSetImmutableHint: {
				break;
			}
			case SDMMD_AFC_Packet_GetSizeOfPathContents: {
				break;
			}
			case SDMMD_AFC_Packet_RemovePathAndContents: {
				break;
			}
			case SDMMD_AFC_Packet_DirectoryEnumeratorRefOpen: {
				break;
			}
			case SDMMD_AFC_Packet_DirectoryEnumeratorRefOpenResult: {
				break;
			}
			case SDMMD_AFC_Packet_DirectoryEnumeratorRefRead: {
				break;
			}
			case SDMMD_AFC_Packet_DirectoryEnumeratorRefClose: {
				break;
			}
			default: {
				break;
			}
		}
		
	ExitLabel:
		
		dispatch_release(conn->ivars.semaphore);
		conn->ivars.operationCount++;
	});
	return result;
}

CF_RETURNS_RETAINED CFStringRef SDMMD_ConvertResponseString(CFDataRef CF_RELEASES_ARGUMENT response_data)
{
	return CFStringCreateWithBytes(kCFAllocatorDefault, CFDataGetBytePtr(response_data), CFDataGetLength(response_data), kCFStringEncodingUTF8, false);
}

CFArrayRef SDMMD_ConvertResponseArray(CFDataRef CF_RELEASES_ARGUMENT response_data)
{
	CFMutableArrayRef response = CFArrayCreateMutable(kCFAllocatorDefault, 0, &kCFTypeArrayCallBacks);
	if (response_data != NULL) {
		char *offset = (char *)CFDataGetBytePtr(response_data);
		uint32_t length = (uint32_t)CFDataGetLength(response_data);
		uint32_t counter = 0;
		while (counter < length) {
			uint32_t str_length = (uint32_t)strlen(offset);
			CFStringRef value = CFStringCreateWithBytes(kCFAllocatorDefault, (UInt8 *)offset, str_length, kCFStringEncodingUTF8, false);
			str_length++;
			counter += str_length;
			offset = PtrAdd(offset, str_length);

			CFArrayAppendValue(response, value);
			CFSafeRelease(value);
		}
	}
	return response;
}

CFDictionaryRef SDMMD_ConvertResponseDictionary(CFDataRef CF_RELEASES_ARGUMENT response_data)
{
	CFMutableDictionaryRef response = CFDictionaryCreateMutable(kCFAllocatorDefault, 0, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
	char *offset = (char *)CFDataGetBytePtr(response_data);
	uint32_t length = (uint32_t)CFDataGetLength(response_data);
	uint32_t counter = 0;
	while (counter < length) {
		uint32_t key_str_length = (uint32_t)strlen(offset);
		CFStringRef key = CFStringCreateWithBytes(kCFAllocatorDefault, (UInt8 *)offset, key_str_length, kCFStringEncodingUTF8, false);
		key_str_length++;
		counter += key_str_length;
		offset = PtrAdd(offset, key_str_length);

		uint32_t value_str_length = (uint32_t)strlen(offset);
		CFStringRef value = CFStringCreateWithBytes(kCFAllocatorDefault, (UInt8 *)offset, value_str_length, kCFStringEncodingUTF8, false);
		value_str_length++;
		counter += value_str_length;
		offset = PtrAdd(offset, value_str_length);

		CFDictionaryAddValue(response, key, value);
		CFSafeRelease(key);
		CFSafeRelease(value);
	}
	return response;
}

CFMutableDataRef SDMMD___AFCCreateAFCDataWithDictionary(CFDictionaryRef dict)
{
	CFMutableDataRef data = CFDataCreateMutable(kCFAllocatorDefault, 0);
	if (data) {
		CFDictionaryApplyFunction(dict, SDMMD___ConvertDictEntry, data);
	}
	return data;
}

#pragma mark -
#pragma mark Operations

SDMMD_AFCOperationRef SDMMD_AFCOperationCreateReadDirectory(CFStringRef path)
{ // _AFCOperationCreateReadDirectory
	SDMMD_AFCOperationRef op = SDMMD_AFCOperationCreateEmpty();
	op->ivars.packet = calloc(1, sizeof(struct sdmmd_AFCPacket));
	char *cpath = SDMCFStringGetString(path);
	uint32_t data_length = (uint32_t)strlen(cpath) + 1;
	op->ivars.packet->header_data = calloc(data_length, sizeof(char));
	memcpy(op->ivars.packet->header_data, cpath, data_length);
	SDMMD_AFCHeaderInit(&(op->ivars.packet->header), SDMMD_AFC_Packet_ReadDirectory, data_length, 0, 0);
	Safe(free, cpath);
	return op;
}

SDMMD_AFCOperationRef SDMMD_AFCOperationCreateRemovePath(CFStringRef path)
{
	SDMMD_AFCOperationRef op = SDMMD_AFCOperationCreateEmpty();
	op->ivars.packet = calloc(1, sizeof(struct sdmmd_AFCPacket));
	char *cpath = SDMCFStringGetString(path);
	uint32_t data_length = (uint32_t)strlen(cpath) + 1;
	op->ivars.packet->header_data = calloc(data_length, sizeof(char));
	memcpy(op->ivars.packet->header_data, cpath, data_length);
	SDMMD_AFCHeaderInit(&(op->ivars.packet->header), SDMMD_AFC_Packet_RemovePath, data_length, 0, 0);
	Safe(free, cpath);
	return op;
}

SDMMD_AFCOperationRef SDMMD_AFCOperationCreateMakeDirectory(CFStringRef path)
{
	SDMMD_AFCOperationRef op = SDMMD_AFCOperationCreateEmpty();
	op->ivars.packet = calloc(1, sizeof(struct sdmmd_AFCPacket));
	char *cpath = SDMCFStringGetString(path);
	uint32_t data_length = (uint32_t)strlen(cpath) + 1;
	op->ivars.packet->header_data = calloc(data_length, sizeof(char));
	memcpy(op->ivars.packet->header_data, cpath, data_length);
	SDMMD_AFCHeaderInit(&(op->ivars.packet->header), SDMMD_AFC_Packet_MakeDirectory, data_length, 0, 0);
	Safe(free, cpath);
	return op;
}

SDMMD_AFCOperationRef SDMMD_AFCOperationCreateGetFileInfo(CFStringRef path)
{
	SDMMD_AFCOperationRef op = SDMMD_AFCOperationCreateEmpty();
	op->ivars.packet = calloc(1, sizeof(struct sdmmd_AFCPacket));
	char *cpath = SDMCFStringGetString(path);
	uint32_t data_length = (uint32_t)strlen(cpath) + 1;
	op->ivars.packet->header_data = calloc(data_length, sizeof(char));
	memcpy(op->ivars.packet->header_data, cpath, data_length);
	SDMMD_AFCHeaderInit(&(op->ivars.packet->header), SDMMD_AFC_Packet_GetFileInfo, data_length, 0, 0);
	Safe(free, cpath);
	return op;
}

SDMMD_AFCOperationRef SDMMD_AFCOperationCreateGetDeviceInfo()
{
	SDMMD_AFCOperationRef op = SDMMD_AFCOperationCreateEmpty();
	op->ivars.packet = calloc(1, sizeof(struct sdmmd_AFCPacket));
	SDMMD_AFCHeaderInit(&(op->ivars.packet->header), SDMMD_AFC_Packet_GetDeviceInfo, 0, 0, 0);
	op->ivars.timeout = dispatch_time(DISPATCH_TIME_NOW, 0);
	return op;
}

SDMMD_AFCOperationRef SDMMD_AFCOperationCreateFileRefOpen(CFStringRef path, uint64_t mode)
{
	SDMMD_AFCOperationRef op = SDMMD_AFCOperationCreateEmpty();
	op->ivars.packet = calloc(1, sizeof(struct sdmmd_AFCPacket));
	char *cpath = SDMCFStringGetString(path);
	uint32_t data_length = (uint32_t)strlen(cpath) + 1 + sizeof(uint64_t);
	op->ivars.packet->header_data = calloc(data_length, sizeof(char));
	mode = htole64(mode);
	memcpy(op->ivars.packet->header_data, &mode, sizeof(uint64_t));
	memcpy(&(op->ivars.packet->header_data[sizeof(uint64_t)]), cpath, strlen(cpath));
	SDMMD_AFCHeaderInit(&(op->ivars.packet->header), SDMMD_AFC_Packet_FileRefOpen, data_length, 0, 0);
	Safe(free, cpath);
	return op;
}

SDMMD_AFCOperationRef SDMMD_AFCFileDescriptorCreateReadOperation(uint64_t fileRef, uint64_t length)
{ // _AFCFileDescriptorCreateReadOperation
	SDMMD_AFCOperationRef op = SDMMD_AFCOperationCreateEmpty();
	op->ivars.packet = calloc(1, sizeof(struct sdmmd_AFCPacket));
	uint32_t data_length = sizeof(uint64_t) * 2;
	uint64_t converted_length = htole64(length); // This maxes out at 4MB, if the length is higher than that this needs to be run multiple times
	op->ivars.packet->header_data = calloc(2, sizeof(uint64_t));
	memcpy(&(op->ivars.packet->header_data[0]), &fileRef, sizeof(uint64_t));
	memcpy(&(op->ivars.packet->header_data[sizeof(uint64_t)]), &converted_length, sizeof(uint64_t));
	SDMMD_AFCHeaderInit(&(op->ivars.packet->header), SDMMD_AFC_Packet_FileRefRead, data_length, 0, 0);
	return op;
}

SDMMD_AFCOperationRef SDMMD_AFCFileDescriptorCreateWriteOperation(uint64_t fileRef, CFDataRef data)
{ // _AFCFileDescriptorCreateWriteOperation
	SDMMD_AFCOperationRef op = SDMMD_AFCOperationCreateEmpty();
	op->ivars.packet = calloc(1, sizeof(struct sdmmd_AFCPacket));
	uint32_t data_length = sizeof(uint64_t);
	uint32_t body_length = (uint32_t)CFDataGetLength(data);
	op->ivars.packet->header_data = calloc(1, sizeof(uint64_t));
	memcpy(op->ivars.packet->header_data, &fileRef, data_length);
	op->ivars.packet->body_data = calloc(body_length, sizeof(char));
	memcpy(op->ivars.packet->body_data, CFDataGetBytePtr(data), body_length);
	SDMMD_AFCHeaderInit(&(op->ivars.packet->header), SDMMD_AFC_Packet_FileRefWrite, data_length, body_length, 0);
	return op;
}

SDMMD_AFCOperationRef SDMMD_AFCFileDescriptorCreateWriteAtPositionOperation(uint64_t fileRef, CFDataRef data, uint64_t pos)
{ // _AFCFileDescriptorCreateWriteOperation
	SDMMD_AFCOperationRef op = SDMMD_AFCOperationCreateEmpty();
	op->ivars.packet = calloc(1, sizeof(struct sdmmd_AFCPacket));
	uint32_t data_length = sizeof(uint64_t) * 2;
	uint32_t body_length = (uint32_t)CFDataGetLength(data);
	op->ivars.packet->header_data = calloc(1, sizeof(uint64_t));
	memcpy(op->ivars.packet->header_data, &fileRef, data_length);
	op->ivars.packet->body_data = calloc(body_length, sizeof(char));
	memcpy(op->ivars.packet->body_data, CFDataGetBytePtr(data), body_length);
	memcpy(&(op->ivars.packet->header_data[sizeof(uint64_t)]), &pos, sizeof(uint64_t));
	SDMMD_AFCHeaderInit(&(op->ivars.packet->header), SDMMD_AFC_Packet_WriteAtPosition, data_length, body_length, 0);
	return op;
}

SDMMD_AFCOperationRef SDMMD_AFCFileDescriptorCreateSeekOperation(uint64_t fileRef, uint32_t pos)
{ // _AFCFileDescriptorCreateSetPositionOperation
	SDMMD_AFCOperationRef op = SDMMD_AFCOperationCreateEmpty();
	op->ivars.packet = calloc(1, sizeof(struct sdmmd_AFCPacket));
	op->ivars.packet->header_data = calloc(3, sizeof(uint64_t));
	uint64_t converted_pos = htole64(pos);
	memcpy(&(op->ivars.packet->header_data[0]), &fileRef, sizeof(uint64_t));
	memset(&(op->ivars.packet->header_data[sizeof(uint64_t)]), 0, sizeof(uint64_t));
	memcpy(&(op->ivars.packet->header_data[sizeof(uint64_t) * 2]), &converted_pos, sizeof(uint64_t));
	SDMMD_AFCHeaderInit(&(op->ivars.packet->header), SDMMD_AFC_Packet_FileRefSeek, sizeof(uint64_t) * 3, 0, 0);
	return op;
}

SDMMD_AFCOperationRef SDMMD_AFCFileDescriptorCreateTellOperation(uint64_t fileRef)
{ // _AFCFileDescriptorCreateGetPositionOperation
	SDMMD_AFCOperationRef op = SDMMD_AFCOperationCreateEmpty();
	op->ivars.packet = calloc(1, sizeof(struct sdmmd_AFCPacket));
	op->ivars.packet->header_data = calloc(1, sizeof(uint64_t));
	memcpy(op->ivars.packet->header_data, &fileRef, sizeof(uint64_t));
	SDMMD_AFCHeaderInit(&(op->ivars.packet->header), SDMMD_AFC_Packet_FileRefTell, sizeof(uint64_t), 0, 0);
	return op;
}

SDMMD_AFCOperationRef SDMMD_AFCFileDescriptorCreateCloseOperation(uint64_t fileRef)
{ // _AFCFileDescriptorCreateCloseOperation
	SDMMD_AFCOperationRef op = SDMMD_AFCOperationCreateEmpty();
	op->ivars.packet = calloc(1, sizeof(struct sdmmd_AFCPacket));
	uint32_t data_length = sizeof(uint64_t);
	op->ivars.packet->header_data = calloc(1, sizeof(uint64_t));
	memcpy(op->ivars.packet->header_data, &fileRef, sizeof(uint64_t));
	SDMMD_AFCHeaderInit(&(op->ivars.packet->header), SDMMD_AFC_Packet_FileRefClose, data_length, 0, 0);
	return op;
}

// rdi, rsi, rdx
SDMMD_AFCOperationRef SDMMD_AFCFileDescriptorCreateSetFileSizeOperation(uint64_t fileRef, uint64_t file_size)
{ // _AFCFileDescriptorCreateSetSizeOperation
	SDMMD_AFCOperationRef op = SDMMD_AFCOperationCreateEmpty();
	op->ivars.packet = calloc(1, sizeof(struct sdmmd_AFCPacket));
	op->ivars.packet->header_data = calloc(2, sizeof(uint64_t));
	memcpy(&(op->ivars.packet->header_data[0]), &fileRef, sizeof(uint64_t));
	memcpy(&(op->ivars.packet->header_data[sizeof(uint64_t)]), &file_size, sizeof(uint64_t));
	SDMMD_AFCHeaderInit(&(op->ivars.packet->header), SDMMD_AFC_Packet_FileRefSetFileSize, sizeof(uint64_t) * 2, 0, 0);
	return op;
}

SDMMD_AFCOperationRef SDMMD_AFCOperationCreateGetConnectionInfo()
{ // _AFCOperationCreateGetConnectionInfo
	SDMMD_AFCOperationRef op = SDMMD_AFCOperationCreateEmpty();
	op->ivars.packet = calloc(1, sizeof(struct sdmmd_AFCPacket));
	SDMMD_AFCHeaderInit(&(op->ivars.packet->header), SDMMD_AFC_Packet_GetConnectionInfo, 0, 0, 0);
	return op;
}

SDMMD_AFCOperationRef SDMMD_AFCOperationCreateSetConnectionOptions(CFDictionaryRef options)
{ // _AFCOperationCreateSetConnectionOptions
	SDMMD_AFCOperationRef op = SDMMD_AFCOperationCreateEmpty();
	op->ivars.packet = calloc(1, sizeof(struct sdmmd_AFCPacket));
	CFDataRef options_data = SDMMD___AFCCreateAFCDataWithDictionary(options);
	CFIndex data_length = CFDataGetLength(options_data);
	op->ivars.packet->body_data = calloc(data_length, sizeof(char));
	memcpy(op->ivars.packet->body_data, CFDataGetBytePtr(options_data), data_length);
	SDMMD_AFCHeaderInit(&(op->ivars.packet->header), SDMMD_AFC_Packet_SetConnectionOptions, 0, (uint32_t)data_length, 0);
	CFSafeRelease(options_data);
	return op;
}

SDMMD_AFCOperationRef SDMMD_AFCOperationCreateRenamePath(CFStringRef old, CFStringRef new)
{
	SDMMD_AFCOperationRef op = SDMMD_AFCOperationCreateEmpty();
	op->ivars.packet = calloc(1, sizeof(struct sdmmd_AFCPacket));
	char *oldPath = SDMCFStringGetString(old);
	char *newPath = SDMCFStringGetString(new);

	uint32_t old_length = (uint32_t)strlen(oldPath) + 1;
	uint32_t new_length = (uint32_t)strlen(newPath) + 1;
	uint32_t total_length = old_length + new_length;
	op->ivars.packet->header_data = calloc(total_length, sizeof(char));
	memcpy(&(op->ivars.packet->header_data[0]), oldPath, old_length - 1);
	memcpy(&(op->ivars.packet->header_data[old_length]), newPath, new_length);
	SDMMD_AFCHeaderInit(&(op->ivars.packet->header), SDMMD_AFC_Packet_RenamePath, total_length, 0, 0);
	free(oldPath);
	free(newPath);
	return op;
}

SDMMD_AFCOperationRef SDMMD_AFCOperationCreateSetFSBlockSize(uint64_t fs_block_size)
{ // ___AFCConnectionSetBlockSize
	SDMMD_AFCOperationRef op = SDMMD_AFCOperationCreateEmpty();
	op->ivars.packet = calloc(1, sizeof(struct sdmmd_AFCPacket));
	uint32_t data_length = sizeof(uint64_t);
	op->ivars.packet->header_data = calloc(1, sizeof(uint64_t));
	memcpy(op->ivars.packet->header_data, &fs_block_size, sizeof(uint64_t));
	SDMMD_AFCHeaderInit(&(op->ivars.packet->header), SDMMD_AFC_Packet_SetFSBlockSize, data_length, 0, 0);
	return op;
}

SDMMD_AFCOperationRef SDMMD_AFCOperationCreateSetSocketBlockSize(uint64_t socket_block_size)
{ // ___AFCConnectionSetBlockSize
	SDMMD_AFCOperationRef op = SDMMD_AFCOperationCreateEmpty();
	op->ivars.packet = calloc(1, sizeof(struct sdmmd_AFCPacket));
	uint32_t data_length = sizeof(uint64_t);
	op->ivars.packet->header_data = calloc(1, sizeof(uint64_t));
	memcpy(op->ivars.packet->header_data, &socket_block_size, sizeof(uint64_t));
	SDMMD_AFCHeaderInit(&(op->ivars.packet->header), SDMMD_AFC_Packet_SetSocketBlockSize, data_length, 0, 0);
	return op;
}

SDMMD_AFCOperationRef SDMMD_AFCFileDescriptorCreateLockOperation(uint64_t fileRef)
{
	SDMMD_AFCOperationRef op = SDMMD_AFCOperationCreateEmpty();
	op->ivars.packet = calloc(1, sizeof(struct sdmmd_AFCPacket));
	uint32_t data_length = sizeof(uint64_t) * 2;
	op->ivars.packet->header_data = calloc(2, sizeof(uint64_t));
	SDMMD_AFCHeaderInit(&(op->ivars.packet->header), SDMMD_AFC_Packet_FileRefLock, data_length, 0, 0);
	return op;
}

SDMMD_AFCOperationRef SDMMD_AFCOperationCreateLinkPath(uint64_t linkType, CFStringRef target, CFStringRef link)
{
	SDMMD_AFCOperationRef op = SDMMD_AFCOperationCreateEmpty();
	op->ivars.packet = calloc(1, sizeof(struct sdmmd_AFCPacket));
	char *targetPath = SDMCFStringGetString(target);
	char *linkPath = SDMCFStringGetString(link);
	uint32_t target_length = (uint32_t)strlen(targetPath) + 1;
	uint32_t link_length = (uint32_t)strlen(linkPath) + 1;
	uint32_t path_length = target_length + link_length;
	op->ivars.packet->header_data = calloc(1, sizeof(char[path_length]));
	memcpy(&(op->ivars.packet->header_data[0]), targetPath, target_length - 1);
	memcpy(&(op->ivars.packet->header_data[target_length]), linkPath, link_length);
	SDMMD_AFCHeaderInit(&(op->ivars.packet->header), SDMMD_AFC_Packet_MakeLink, path_length, 0, 0);
	free(targetPath);
	free(linkPath);
	return op;
}

SDMMD_AFCOperationRef SDMMD_AFCOperationCreateGetFileHash(CFStringRef path)
{
	SDMMD_AFCOperationRef op = SDMMD_AFCOperationCreateEmpty();
	op->ivars.packet = calloc(1, sizeof(struct sdmmd_AFCPacket));
	char *cpath = SDMCFStringGetString(path);
	uint32_t data_length = (uint32_t)strlen(cpath) + 1;
	op->ivars.packet->header_data = calloc(data_length, sizeof(char));
	memcpy(op->ivars.packet->header_data, cpath, data_length);
	SDMMD_AFCHeaderInit(&(op->ivars.packet->header), SDMMD_AFC_Packet_GetFileHash, data_length, 0, 0);
	free(cpath);
	return op;
}

SDMMD_AFCOperationRef SDMMD_AFCOperationCreateSetModTime(CFStringRef ref)
{
	SDMMD_AFCOperationRef op = SDMMD_AFCOperationCreateEmpty();
	op->ivars.packet = calloc(1, sizeof(struct sdmmd_AFCPacket));
	char *cref = SDMCFStringGetString(ref);
	uint32_t data_length = (uint32_t)strlen(cref) + 1 + 8;
	op->ivars.packet->header_data = calloc(data_length, sizeof(char));
	memcpy(op->ivars.packet->header_data, cref, strlen(cref));
	SDMMD_AFCHeaderInit(&(op->ivars.packet->header), SDMMD_AFC_Packet_SetModTime, data_length, 0, 0);
	free(cref);
	return op;
}

SDMMD_AFCOperationRef SDMMD_AFCOperationCreateGetFileHashWithRange(CFStringRef path, CFRange range)
{
	SDMMD_AFCOperationRef op = SDMMD_AFCOperationCreateEmpty();
	op->ivars.packet = calloc(1, sizeof(struct sdmmd_AFCPacket));
	char *cpath = SDMCFStringGetString(path);
	uint32_t data_length = (uint32_t)strlen(cpath) + 1 + sizeof(CFRange);
	op->ivars.packet->header_data = calloc(data_length, sizeof(char));
	memcpy(op->ivars.packet->header_data, &range, sizeof(CFRange));
	memcpy(&(op->ivars.packet->header_data[sizeof(CFRange)]), cpath, strlen(cpath));
	SDMMD_AFCHeaderInit(&(op->ivars.packet->header), SDMMD_AFC_Packet_GetFileHashWithRange, data_length, 0, 0);
	free(cpath);
	return op;
}

SDMMD_AFCOperationRef SDMMD_AFCFileDescriptorCreateSetImmutableHintOperation(uint64_t fileRef, uint64_t immutable_hint)
{
	SDMMD_AFCOperationRef op = SDMMD_AFCOperationCreateEmpty();
	op->ivars.packet = calloc(1, sizeof(struct sdmmd_AFCPacket));
	uint32_t data_length = sizeof(uint64_t) * 2;
	op->ivars.packet->header_data = calloc(2, sizeof(uint64_t));
	immutable_hint = (immutable_hint != 0 ? 0xff : 0);
	memcpy(&(op->ivars.packet->header_data[0]), &fileRef, sizeof(uint64_t));
	memcpy(&(op->ivars.packet->header_data[sizeof(uint64_t)]), &immutable_hint, sizeof(uint64_t));
	SDMMD_AFCHeaderInit(&(op->ivars.packet->header), SDMMD_AFC_Packet_FileRefSetImmutableHint, data_length, 0, 0);
	return op;
}

SDMMD_AFCOperationRef SDMMD_AFCOperationCreateGetSizeOfPathContents(CFStringRef path)
{
	SDMMD_AFCOperationRef op = SDMMD_AFCOperationCreateEmpty();
	op->ivars.packet = calloc(1, sizeof(struct sdmmd_AFCPacket));
	char *cref = SDMCFStringGetString(path);
	uint32_t data_length = (uint32_t)strlen(cref) + 1;
	op->ivars.packet->header_data = calloc(data_length, sizeof(char));
	memcpy(op->ivars.packet->header_data, cref, strlen(cref));
	SDMMD_AFCHeaderInit(&(op->ivars.packet->header), SDMMD_AFC_Packet_GetSizeOfPathContents, data_length, 0, 0);
	Safe(free, cref);
	return op;
}

SDMMD_AFCOperationRef SDMMD_AFCOperationCreateRemovePathAndContents(CFStringRef path)
{
	SDMMD_AFCOperationRef op = SDMMD_AFCOperationCreateEmpty();
	op->ivars.packet = calloc(1, sizeof(struct sdmmd_AFCPacket));
	char *cref = SDMCFStringGetString(path);
	uint32_t data_length = (uint32_t)strlen(cref) + 1;
	op->ivars.packet->header_data = calloc(data_length, sizeof(char));
	memcpy(op->ivars.packet->header_data, cref, strlen(cref));
	SDMMD_AFCHeaderInit(&(op->ivars.packet->header), SDMMD_AFC_Packet_RemovePathAndContents, data_length, 0, 0);
	Safe(free, cref);
	return op;
}

#pragma mark -
#pragma mark Alias Operations

sdmmd_return_t SDMMD_AMDeviceCopy(SDMMD_AFCConnectionRef conn, char *local, char *remote)
{
	sdmmd_return_t result = kAMDSuccess;
	bool is_dir = false;
	bool does_exist = FileExistsAtPathIsDir(local, &is_dir);
	if (does_exist) {
		if (is_dir) {
			CFStringRef new_dir = CFStringCreateWithCString(kCFAllocatorDefault, remote, kCFStringEncodingUTF8);
			SDMMD_AFCOperationRef make_dir = SDMMD_AFCOperationCreateMakeDirectory(new_dir);
			result = SDMMD_AFCProcessOperation(conn, &make_dir);
			CFSafeRelease(new_dir);
			CheckErrorAndReturn(result);

			struct dirent *ent;
			DIR *dir = opendir(local);
			while ((ent = readdir(dir)) != NULL) {
				char *file_name = ent->d_name;
				if (!((strncmp(file_name, ".", sizeof(char[1])) == 0 && ent->d_namlen == 1) || (strncmp(file_name, "..", sizeof(char[2])) == 0 && ent->d_namlen == 2))) {
					uint32_t local_length = (uint32_t)strlen(local) + 1 + (uint32_t)strlen(file_name) + 1;
					char *new_local = calloc(local_length, sizeof(char));
					snprintf(new_local, local_length, "%s/%s", local, file_name);

					uint32_t remote_length = (uint32_t)strlen(remote) + 1 + (uint32_t)strlen(file_name) + 1;
					char *new_remote = calloc(remote_length, sizeof(char));
					snprintf(new_remote, remote_length, "%s/%s", remote, file_name);

					result = SDMMD_AMDeviceCopy(conn, new_local, new_remote);
					Safe(free, new_local);
					Safe(free, new_remote);
				}
			}
			closedir(dir);
		}
		else {
			result = SDMMD_AMDeviceCopyFile(NULL, NULL, NULL, conn, local, remote);
		}
	}
	else {
		result = kAMDNotFoundError;
	}
	ExitLabelAndReturn(result);
}

sdmmd_return_t SDMMD_AMDeviceCopyFile(CallBack callback, void *thing2, void *thing3, SDMMD_AFCConnectionRef conn, char *local, char *remote)
{
	sdmmd_return_t result = kAMDSuccess;
	CFDataRef local_file = CFDataCreateFromFilePath(local);
	if (local_file) {
		uint32_t packets = (uint32_t)((CFDataGetLength(local_file) + kAFCMaxTransferSize - 1) / kAFCMaxTransferSize);
		CFStringRef send_remote_path = CFStringCreateWithBytes(kCFAllocatorDefault, (const UInt8 *)remote, strlen(remote), kCFStringEncodingUTF8, false);
		SDMMD_AFCOperationRef file_create = SDMMD_AFCOperationCreateFileRefOpen(send_remote_path, 2);
		result = SDMMD_AFCProcessOperation(conn, &file_create);
		CFSafeRelease(send_remote_path);
		CheckErrorAndReturn(result);

		uint64_t file_descriptor;
		memcpy(&file_descriptor, CFDataGetBytePtr(file_create->ivars.packet->response), sizeof(uint64_t));

		uint64_t offset = 0;
		uint64_t remainder = 0;
		uint32_t percent_calc = 30;
		bool should_stop = false;
		for (uint32_t index = 0; index < packets; index++) {
			offset = kAFCMaxTransferSize * index;
			remainder = (CFDataGetLength(local_file) - offset);
			remainder = (remainder > kAFCMaxTransferSize ? kAFCMaxTransferSize : remainder);
			CFRange current_read = CFRangeMake((CFIndex)offset, (CFIndex)remainder);
			CFDataRef write_data = CFDataCreateFromSubrangeOfData(local_file, current_read);
			SDMMD_AFCOperationRef write_op = SDMMD_AFCFileDescriptorCreateWriteOperation(file_descriptor, write_data);
			result = SDMMD_AFCProcessOperation(conn, &write_op);
			if (SDM_MD_CallSuccessful(result)) {
				if (callback != NULL) {
					CFMutableDictionaryRef status = SDMMD_create_dict();
					CFDictionarySetValue(status, CFSTR("Status"), CFSTR("CopyingFile"));
					double test = (double)CFDataGetLength(local_file);
					percent_calc += (floor(((double)remainder / test) * 100.0) * 0.59);
					CFNumberRef percent = CFNumberCreate(kCFAllocatorDefault, kCFNumberSInt32Type, &percent_calc);
					CFDictionarySetValue(status, CFSTR("PercentComplete"), percent);
					CFStringRef local_path = CFStringCreateWithBytes(kCFAllocatorDefault, (const UInt8 *)local, strlen(local), kCFStringEncodingUTF8, false);
					CFDictionarySetValue(status, CFSTR("LocalPath"), local_path);
					CFStringRef remote_path = CFStringCreateWithBytes(kCFAllocatorDefault, (const UInt8 *)remote, strlen(remote), kCFStringEncodingUTF8, false);
					CFDictionarySetValue(status, CFSTR("RemotePath"), remote_path);
					callback(status, thing3);
					CFSafeRelease(status);
					CFSafeRelease(local_path);
					CFSafeRelease(remote_path);
					CFSafeRelease(percent);
				}
			}
			else {
				should_stop = true;
			}
			CFSafeRelease(write_op);
			CFSafeRelease(write_data);

			if (should_stop) {
				break;
			}
		}
		SDMMD_AFCOperationRef file_close = SDMMD_AFCFileDescriptorCreateCloseOperation(file_descriptor);
		SDMMD_AFCProcessOperation(conn, &file_close);
		CFSafeRelease(file_close);

		CFSafeRelease(file_create);
	}
	else {
		result = kAMDNotFoundError;
	}

ExitLabel:
	CFSafeRelease(local_file);
	return result;
}

sdmmd_return_t SDMMD_AMDeviceRemoteCopyFile(CallBack callback, void *thing2, void *thing3, SDMMD_AFCConnectionRef conn, char *local, char *remote)
{
	sdmmd_return_t result = kAMDSuccess;
	CFDataRef local_file = CFDataCreateFromFilePath(local);
	if (local_file != NULL) {
		uint32_t packets = (uint32_t)((CFDataGetLength(local_file) + kAFCMaxTransferSize - 1) / kAFCMaxTransferSize);
		CFStringRef remote_path = CFStringCreateWithBytes(kCFAllocatorDefault, (const UInt8 *)remote, strlen(remote), kCFStringEncodingUTF8, false);
		SDMMD_AFCOperationRef file_create = SDMMD_AFCOperationCreateFileRefOpen(remote_path, 2);
		result = SDMMD_AFCProcessOperation(conn, &file_create);
		CFSafeRelease(remote_path);
		CheckErrorAndReturn(result);

		uint64_t file_descriptor;
		memcpy(&file_descriptor, CFDataGetBytePtr(file_create->ivars.packet->response), sizeof(uint64_t));

		mode_t fileMode = (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
		int local_fd = open(local, O_CREAT | O_RDWR, fileMode);

		uint64_t offset = 0;
		uint64_t remainder = 0;
		for (uint32_t index = 0; index < packets; index++) {
			offset = kAFCMaxTransferSize * index;
			remainder = (CFDataGetLength(local_file) - offset);
			remainder = (remainder > kAFCMaxTransferSize ? kAFCMaxTransferSize : remainder);
			SDMMD_AFCOperationRef write_op = SDMMD_AFCFileDescriptorCreateReadOperation(file_descriptor, remainder);
			result = SDMMD_AFCProcessOperation(conn, &write_op);
			if (SDM_MD_CallSuccessful(result)) {
				// probably fire a callback?
				CFTypeRef data = SDMMD_AFCOperationGetPacketResponse(write_op);
				write(local_fd, CFDataGetBytePtr(data), CFDataGetLength(data));
			}
			else {
				break;
			}
			CFSafeRelease(write_op);
		}
		SDMMD_AFCOperationRef file_close = SDMMD_AFCFileDescriptorCreateCloseOperation(file_descriptor);
		SDMMD_AFCProcessOperation(conn, &file_close);
		CFSafeRelease(file_close);

		close(local_fd);

		CFSafeRelease(file_create);
	}
	else {
		result = kAMDNotFoundError;
	}

ExitLabel:
	CFSafeRelease(local_file);
	return result;
}

sdmmd_return_t SDMMD_check_can_touch(SDMMD_AFCConnectionRef conn, CFDataRef *unknown)
{
	SDMMD_AFCOperationRef fileInfo = SDMMD_AFCOperationCreateGetFileInfo((CFStringRef)*unknown);
	SDMMD_AFCProcessOperation(conn, &fileInfo);
	*unknown = CFRetain(fileInfo->ivars.packet->response);
	CFSafeRelease(fileInfo);
	return kAMDSuccess; // lol
}

#endif
