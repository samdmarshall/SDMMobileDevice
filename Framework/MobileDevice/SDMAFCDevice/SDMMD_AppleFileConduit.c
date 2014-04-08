//
//  SDMMD_AppleFileConduit.c
//  SDMMobileDevice-Framework
//
//  Created by Sam Marshall on 4/5/14.
//
//

#ifndef SDMMobileDevice_Framework_SDMMD_AppleFileConduit_c
#define SDMMobileDevice_Framework_SDMMD_AppleFileConduit_c

#include "SDMMD_AppleFileConduit.h"
#include "SDMMD_Functions.h"
#include <string.h>
#include <sys/types.h>
#include "Core.h"

void SDMMD_AFCHeaderInit(SDMMD_AFCPacketHeader *header, uint32_t command, uint32_t size, uint32_t data, uint32_t pack_num);

SDMMD_AFCConnectionRef SDMMD_AFCConnectionCreate(SDMMD_AMConnectionRef conn) {
	SDMMD_AFCConnectionRef afc = calloc(1, sizeof(struct sdmmd_AFCConnectionClass));
	if (afc != NULL) {
		afc->handle = conn;
		char *udidString = SDMCFStringGetString((conn->ivars.device)->ivars.unique_device_id);
		CFStringRef date_string = SDMGetCurrentDateString();
		char *dateString = SDMCFStringGetString(date_string);
		CFSafeRelease(date_string);
		CFStringRef name = CFStringCreateWithFormat(kCFAllocatorDefault, NULL, CFSTR("%s.%s.%s"), "com.samdmarshall.sdmmobiledevice.afc", udidString, dateString);
		Safe(free, udidString);
		Safe(free, dateString);
		char *queueName = SDMCFStringGetString(name);
		afc->operationQueue = dispatch_queue_create(queueName, NULL);
		Safe(free, queueName);
		CFSafeRelease(name);
		afc->operationCount = 0;
	}
	return afc;
}

void SDMMD_AFCOperationRelease(SDMMD_AFCOperationRef operation) {
	if (operation) {
		if (operation->packet) {
			if (operation->packet->header_data) {
				free(operation->packet->header_data);
			}
			
			if (operation->packet->body_data) {
				free(operation->packet->body_data);
			}
			
			CFSafeRelease(operation->packet->response);
			
			free(operation->packet);
		}
		free(operation);
	}
}

void SDMMD_AFCConnectionRelease(SDMMD_AFCConnectionRef conn) {
	if (conn) {
		SDMMD_AMDServiceConnectionInvalidate(conn->handle);
		dispatch_release(conn->operationQueue);
	}
}

#pragma mark -

void SDMMD_AFCHeaderInit(SDMMD_AFCPacketHeader *header, uint32_t command, uint32_t header_size, uint32_t body_size, uint32_t pack_num) {
	header->signature = 0x4141504c36414643; // 'APPL6AFC'
	header->headerLen = header_size + sizeof(struct SDMMD_AFCPacketHeader);
	header->packetLen = body_size + header->headerLen;
	header->type = command;
	if (pack_num) {
		header->pid = pack_num;
	} else {
		header->pid = k64BitMask; // but not always? this is really weird
	}
}

sdmmd_return_t SDMMD_AFCSendOperation(SDMMD_AFCConnectionRef conn, SDMMD_AFCOperationRef op);
sdmmd_return_t SDMMD_AFCReceiveOperation(SDMMD_AFCConnectionRef conn, SDMMD_AFCOperationRef *op);

sdmmd_return_t SDMMD_AFCConnectionPerformOperation(SDMMD_AFCConnectionRef conn, SDMMD_AFCOperationRef op) {
	__block sdmmd_return_t result = kAMDSuccess;
	dispatch_sync(conn->operationQueue, ^{
		//printf("Packet: %s\n",SDMMD_gAFCPacketTypeNames[op->packet->header.type]);
		op->packet->header.pid = conn->operationCount;
		result = SDMMD_AFCSendOperation(conn, op);
		SDMMD_AFCOperationRef response;
		result = SDMMD_AFCReceiveOperation(conn, &response);
		//printf("Response: %08x\n",result);
	});
	conn->operationCount++;
	return result;
}

sdmmd_return_t SDMMD_AFCSendOperation(SDMMD_AFCConnectionRef conn, SDMMD_AFCOperationRef op) {
	sdmmd_return_t result = kAMDSuccess;
	CFMutableDataRef headerData = CFDataCreateMutable(kCFAllocatorDefault, (CFIndex)op->packet->header.headerLen);//(kCFAllocatorDefault, (UInt8*)&op->packet->header, sizeof(SDMMD_AFCPacketHeader));
	CFDataAppendBytes(headerData, (UInt8*)&(op->packet->header), sizeof(SDMMD_AFCPacketHeader));
	if (op->packet->header.headerLen > sizeof(SDMMD_AFCPacketHeader)) {
		CFDataAppendBytes(headerData, (UInt8*)(op->packet->header_data), (uint32_t)op->packet->header.headerLen - sizeof(SDMMD_AFCPacketHeader));
	}
	result = SDMMD_DirectServiceSend(SDMMD_TranslateConnectionToSocket(conn->handle), headerData);
	CFSafeRelease(headerData);
	//printf("header sent status: %08x %s\n",result,SDMMD_AMDErrorString(result));
	if (!(op->packet->header.headerLen == op->packet->header.packetLen && op->packet->body_data == NULL)) {
		CFDataRef bodyData = CFDataCreate(kCFAllocatorDefault, (UInt8*)(op->packet->body_data), (uint32_t)op->packet->header.packetLen - (uint32_t)op->packet->header.headerLen);
		result = SDMMD_DirectServiceSend(SDMMD_TranslateConnectionToSocket(conn->handle), bodyData);
		CFSafeRelease(bodyData);
		//printf("body sent status: %08x %s\n",result,SDMMD_AMDErrorString(result));
	}
	return result;
}

sdmmd_return_t SDMMD_AFCReceiveOperation(SDMMD_AFCConnectionRef conn, SDMMD_AFCOperationRef *operation) {
	sdmmd_return_t result = kAMDSuccess;
	CFMutableDataRef headerData = CFDataCreateMutable(kCFAllocatorDefault, sizeof(SDMMD_AFCPacketHeader));
	SDMMD_AFCPacketHeader *zeros = calloc(1, sizeof(SDMMD_AFCPacketHeader));
	CFDataAppendBytes(headerData, (UInt8*)zeros, sizeof(SDMMD_AFCPacketHeader));
	free(zeros);
	
	result = SDMMD_DirectServiceReceive(SDMMD_TranslateConnectionToSocket(conn->handle), (CFDataRef*)&headerData);
	if (result == kAMDSuccess) {
		SDMMD_AFCPacketHeader *header = (SDMMD_AFCPacketHeader *)CFDataGetBytePtr(headerData);
		
		CFMutableDataRef bodyData = CFDataCreateMutable(kCFAllocatorDefault, (uint32_t)header->packetLen - (uint32_t)header->headerLen);
		uint32_t body_length = (uint32_t)header->packetLen - (uint32_t)header->headerLen;
		char *body = calloc(body_length, sizeof(char));
		CFDataAppendBytes(bodyData, (UInt8*)body, body_length);
		free(body);
		result = SDMMD_DirectServiceReceive(SDMMD_TranslateConnectionToSocket(conn->handle), (CFDataRef*)&bodyData);
		if (bodyData) {
			(*operation)->packet->response = bodyData;
		}
	}
	CFSafeRelease(headerData);
	
	return result;
}

sdmmd_return_t SDMMD_AFCProcessOperation(SDMMD_AFCConnectionRef conn, SDMMD_AFCOperationRef *operation) {
	__block sdmmd_return_t result = kAMDSuccess;
	dispatch_sync(conn->operationQueue, ^{
		conn->semaphore = dispatch_semaphore_create(0x0);
		(*operation)->packet->header.pid = k64BitMask; //conn->operationCount;
		result = SDMMD_AFCSendOperation(conn, *operation);
		dispatch_semaphore_wait(conn->semaphore, (*operation)->timeout);
		SDMMD_AFCReceiveOperation(conn, operation);
		switch ((*operation)->packet->header.type) {
			case SDMMD_AFC_Packet_Status: {
				break;
			}
			case SDMMD_AFC_Packet_Data: {
				break;
			}
			case SDMMD_AFC_Packet_ReadDirectory: {
				(*operation)->packet->response = SDMMD_ConvertResponseArray((*operation)->packet->response);
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
				(*operation)->packet->response = SDMMD_ConvertResponseDictionary((*operation)->packet->response);
				break;
			}
			case SDMMD_AFC_Packet_GetDeviceInfo: {
				(*operation)->packet->response = SDMMD_ConvertResponseDictionary((*operation)->packet->response);
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
				(*operation)->packet->response = SDMMD_ConvertResponseDictionary((*operation)->packet->response);
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
		
		dispatch_release(conn->semaphore);
		conn->operationCount++;
	});
	return result;
}

CF_RETURNS_RETAINED CFStringRef SDMMD_ConvertResponseString(CFDataRef CF_RELEASES_ARGUMENT response_data) {
	return CFStringCreateWithBytes(kCFAllocatorDefault, CFDataGetBytePtr(response_data), CFDataGetLength(response_data), kCFStringEncodingUTF8, false);
}

CF_RETURNS_RETAINED CFArrayRef SDMMD_ConvertResponseArray(CFDataRef CF_RELEASES_ARGUMENT response_data) {
	CFMutableArrayRef response = CFArrayCreateMutable(kCFAllocatorDefault, 0, &kCFTypeArrayCallBacks);
	char *offset = (char *)CFDataGetBytePtr(response_data);
	uint32_t length = (uint32_t)CFDataGetLength(response_data);
	uint32_t counter = 0;
	while (counter < length) {
		uint32_t str_length = (uint32_t)strlen(offset);
		CFStringRef value = CFStringCreateWithBytes(kCFAllocatorDefault, (UInt8*)offset, str_length, kCFStringEncodingUTF8, false);
		str_length++;
		counter += str_length;
		offset = PtrAdd(offset, str_length);
		
		CFArrayAppendValue(response, value);
		CFSafeRelease(value);
	}
	return response;
}

CF_RETURNS_RETAINED CFDictionaryRef SDMMD_ConvertResponseDictionary(CFDataRef CF_RELEASES_ARGUMENT response_data) {
	CFMutableDictionaryRef response = CFDictionaryCreateMutable(kCFAllocatorDefault, 0, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
	char *offset = (char *)CFDataGetBytePtr(response_data);
	uint32_t length = (uint32_t)CFDataGetLength(response_data);
	uint32_t counter = 0;
	while (counter < length) {
		uint32_t key_str_length = (uint32_t)strlen(offset);
		CFStringRef key = CFStringCreateWithBytes(kCFAllocatorDefault, (UInt8*)offset, key_str_length, kCFStringEncodingUTF8, false);
		key_str_length++;
		counter += key_str_length;
		offset = PtrAdd(offset, key_str_length);
		
		uint32_t value_str_length = (uint32_t)strlen(offset);
		CFStringRef value = CFStringCreateWithBytes(kCFAllocatorDefault, (UInt8*)offset, value_str_length, kCFStringEncodingUTF8, false);
		value_str_length++;
		counter += value_str_length;
		offset = PtrAdd(offset, value_str_length);
		
		CFDictionaryAddValue(response, key, value);
		CFSafeRelease(key);
		CFSafeRelease(value);
	}
	return response;
}

#pragma mark -
#pragma mark Operations

SDMMD_AFCOperationRef SDMMD_AFCOperationCreateReadDirectory(CFStringRef path) { // _AFCOperationCreateReadDirectory
	SDMMD_AFCOperationRef op = calloc(1, sizeof(struct sdmmd_AFCOperation));
	op->packet = calloc(1, sizeof(struct sdmmd_AFCPacket));
	char *cpath = SDMCFStringGetString(path);
	uint32_t data_length = (uint32_t)strlen(cpath)+1;
	op->packet->header_data = calloc(data_length, sizeof(char));
	memcpy(op->packet->header_data, cpath, data_length);
	SDMMD_AFCHeaderInit(&(op->packet->header), SDMMD_AFC_Packet_ReadDirectory, data_length, 0, 0);
	free(cpath);
	return op;
}

SDMMD_AFCOperationRef SDMMD_AFCOperationCreateRemovePath(CFStringRef path) {
	SDMMD_AFCOperationRef op = calloc(1, sizeof(struct sdmmd_AFCOperation));
	op->packet = calloc(1, sizeof(struct sdmmd_AFCPacket));
	char *cpath = SDMCFStringGetString(path);
	uint32_t data_length = (uint32_t)strlen(cpath)+1;
	op->packet->header_data = calloc(data_length, sizeof(char));
	memcpy(op->packet->header_data, cpath, data_length);
	SDMMD_AFCHeaderInit(&(op->packet->header), SDMMD_AFC_Packet_RemovePath, data_length, 0, 0);
	free(cpath);
	return op;
}

SDMMD_AFCOperationRef SDMMD_AFCOperationCreateMakeDirectory(CFStringRef path) {
	SDMMD_AFCOperationRef op = calloc(1, sizeof(struct sdmmd_AFCOperation));
	op->packet = calloc(1, sizeof(struct sdmmd_AFCPacket));
	char *cpath = SDMCFStringGetString(path);
	uint32_t data_length = (uint32_t)strlen(cpath)+1;
	op->packet->header_data = calloc(data_length, sizeof(char));
	memcpy(op->packet->header_data, cpath, data_length);
	SDMMD_AFCHeaderInit(&(op->packet->header), SDMMD_AFC_Packet_MakeDirectory, data_length, 0, 0);
	free(cpath);
	return op;
}

SDMMD_AFCOperationRef SDMMD_AFCOperationCreateGetFileInfo(CFStringRef path) {
	SDMMD_AFCOperationRef op = calloc(1, sizeof(struct sdmmd_AFCOperation));
	op->packet = calloc(1, sizeof(struct sdmmd_AFCPacket));
	char *cpath = SDMCFStringGetString(path);
	uint32_t data_length = (uint32_t)strlen(cpath)+1;
	op->packet->header_data = calloc(data_length, sizeof(char));
	memcpy(op->packet->header_data, cpath, data_length);
	SDMMD_AFCHeaderInit(&(op->packet->header), SDMMD_AFC_Packet_GetFileInfo, data_length, 0, 0);
	free(cpath);
	return op;
}

SDMMD_AFCOperationRef SDMMD_AFCOperationCreateGetDeviceInfo() {
	SDMMD_AFCOperationRef op = calloc(1, sizeof(struct sdmmd_AFCOperation));
	op->packet = calloc(1, sizeof(struct sdmmd_AFCPacket));
	SDMMD_AFCHeaderInit(&(op->packet->header), SDMMD_AFC_Packet_GetDeviceInfo, 0, 0, 0);
	op->timeout = dispatch_time(DISPATCH_TIME_NOW, 0);
	return op;
}

SDMMD_AFCOperationRef SDMMD_AFCOperationCreateFileRefOpen(CFStringRef path, uint64_t mode, Pointer *fileRef) {
	SDMMD_AFCOperationRef op = calloc(1, sizeof(struct sdmmd_AFCOperation));
	op->packet = calloc(1, sizeof(struct sdmmd_AFCPacket));
	char *cpath = SDMCFStringGetString(path);
	uint32_t data_length = (uint32_t)strlen(cpath)+1+sizeof(uint64_t);
	op->packet->header_data = calloc(data_length, sizeof(char));
	memcpy(op->packet->header_data, &mode, sizeof(uint64_t));
	memcpy(&(op->packet->header_data[sizeof(uint64_t)]), cpath, strlen(cpath));
	SDMMD_AFCHeaderInit(&(op->packet->header), SDMMD_AFC_Packet_FileRefOpen, data_length, 0, 0);
	free(cpath);
	return op;
}

SDMMD_AFCOperationRef SDMMD_AFCFileDescriptorCreateReadOperation(Pointer fileRef, uint32_t length) { // _AFCFileDescriptorCreateReadOperation
	SDMMD_AFCOperationRef op = calloc(1, sizeof(struct sdmmd_AFCOperation));
	op->packet = calloc(1, sizeof(struct sdmmd_AFCPacket));
	uint32_t data_length = sizeof(uint64_t)*2;
	uint64_t converted_length = htole64(length);
	op->packet->header_data = calloc(2, sizeof(uint64_t));
	memcpy(&(op->packet->header_data[0]), fileRef, sizeof(uint64_t));
	memcpy(&(op->packet->header_data[sizeof(uint64_t)]), &converted_length, sizeof(uint64_t));
	SDMMD_AFCHeaderInit(&(op->packet->header), SDMMD_AFC_Packet_FileRefRead, data_length, 0, 0x0);
	return op;
}

SDMMD_AFCOperationRef SDMMD_AFCFileDescriptorCreateWriteOperation(Pointer fileRef, CFDataRef data) { // _AFCFileDescriptorCreateWriteOperation
	SDMMD_AFCOperationRef op = calloc(1, sizeof(struct sdmmd_AFCOperation));
	op->packet = calloc(1, sizeof(struct sdmmd_AFCPacket));
	uint32_t data_length = sizeof(uint64_t);
	uint32_t body_length = (uint32_t)CFDataGetLength(data);
	op->packet->header_data = calloc(1, sizeof(uint64_t));
	memcpy(op->packet->header_data, fileRef, data_length);
	op->packet->body_data = calloc(body_length, sizeof(char));
	memcpy(op->packet->body_data, CFDataGetBytePtr(data), body_length);
	SDMMD_AFCHeaderInit(&(op->packet->header), SDMMD_AFC_Packet_FileRefWrite, data_length, body_length, 0x0);
	return op;
}

SDMMD_AFCOperationRef SDMMD_AFCFileDescriptorCreateSeekOperation(Pointer fileRef, uint32_t pos) { // _AFCFileDescriptorCreateSetPositionOperation
	SDMMD_AFCOperationRef op = calloc(1, sizeof(struct sdmmd_AFCOperation));
	op->packet = calloc(1, sizeof(struct sdmmd_AFCPacket));
	op->packet->header_data = calloc(3, sizeof(uint64_t));
	uint64_t converted_pos = htole64(pos);
	memcpy(&(op->packet->header_data[0]), fileRef, sizeof(uint64_t));
	memset(&(op->packet->header_data[sizeof(uint64_t)]), 0, sizeof(uint64_t));
	memcpy(&(op->packet->header_data[sizeof(uint64_t)*2]), &converted_pos, sizeof(uint64_t));
	SDMMD_AFCHeaderInit(&(op->packet->header), SDMMD_AFC_Packet_FileRefSeek, sizeof(uint64_t)*3, 0, 0);
	return op;
}

SDMMD_AFCOperationRef SDMMD_AFCFileDescriptorCreateTellOperation(Pointer fileRef) { // _AFCFileDescriptorCreateGetPositionOperation
	SDMMD_AFCOperationRef op = calloc(1, sizeof(struct sdmmd_AFCOperation));
	op->packet = calloc(1, sizeof(struct sdmmd_AFCPacket));
	op->packet->header_data = calloc(1, sizeof(uint64_t));
	memcpy(op->packet->header_data, fileRef, sizeof(uint64_t));
	SDMMD_AFCHeaderInit(&(op->packet->header), SDMMD_AFC_Packet_FileRefTell, sizeof(uint64_t), 0, 0);
	return op;
}

SDMMD_AFCOperationRef SDMMD_AFCFileDescriptorCreateCloseOperation(Pointer fileRef) { // _AFCFileDescriptorCreateCloseOperation
	SDMMD_AFCOperationRef op = calloc(1, sizeof(struct sdmmd_AFCOperation));
	op->packet = calloc(1, sizeof(struct sdmmd_AFCPacket));
	uint32_t data_length = sizeof(uint64_t);
	op->packet->header_data = calloc(1, sizeof(uint64_t));
	memcpy(op->packet->header_data, fileRef, sizeof(uint64_t));
	SDMMD_AFCHeaderInit(&(op->packet->header), SDMMD_AFC_Packet_FileRefClose, data_length, 0, 0);
	return op;
}

// rdi, rsi, rdx
SDMMD_AFCOperationRef SDMMD_AFCFileDescriptorCreateSetFileSizeOperation() { // _AFCFileDescriptorCreateSetSizeOperation
	SDMMD_AFCOperationRef op = calloc(1, sizeof(struct sdmmd_AFCOperation));
	op->packet = calloc(1, sizeof(struct sdmmd_AFCPacket));
	SDMMD_AFCHeaderInit(&(op->packet->header), SDMMD_AFC_Packet_FileRefSetFileSize, 0, 0, 0);
	return op;
}

SDMMD_AFCOperationRef SDMMD_AFCOperationCreateGetConnectionInfo() { // _AFCOperationCreateGetConnectionInfo
	SDMMD_AFCOperationRef op = calloc(1, sizeof(struct sdmmd_AFCOperation));
	op->packet = calloc(1, sizeof(struct sdmmd_AFCPacket));
	SDMMD_AFCHeaderInit(&(op->packet->header), SDMMD_AFC_Packet_GetConnectionInfo, 0, 0, 0);
	return op;
}


// rdi, rsi, rdx
SDMMD_AFCOperationRef SDMMD_AFCOperationCreateSetConnectionOptions() { // _AFCOperationCreateSetConnectionOptions
	SDMMD_AFCOperationRef op = calloc(1, sizeof(struct sdmmd_AFCOperation));
	op->packet = calloc(1, sizeof(struct sdmmd_AFCPacket));
	SDMMD_AFCHeaderInit(&(op->packet->header), SDMMD_AFC_Packet_SetConnectionOptions, 0, 0, 0);
	return op;
}

SDMMD_AFCOperationRef SDMMD_AFCOperationCreateRenamePath(CFStringRef old, CFStringRef new) {
	SDMMD_AFCOperationRef op = calloc(1, sizeof(struct sdmmd_AFCOperation));
	op->packet = calloc(1, sizeof(struct sdmmd_AFCPacket));
	char *oldPath = SDMCFStringGetString(old);
	char *newPath = SDMCFStringGetString(new);
	
	uint32_t old_length = (uint32_t)strlen(oldPath)+1;
	uint32_t new_length = (uint32_t)strlen(newPath)+1;
	uint32_t total_length = old_length + new_length;
	op->packet->header_data = calloc(total_length, sizeof(char));
	memcpy(&op->packet->header_data, oldPath, old_length);
	memcpy(&op->packet->header_data[old_length], newPath, new_length);
	SDMMD_AFCHeaderInit(&(op->packet->header), SDMMD_AFC_Packet_RenamePath, total_length, 0, 0);
	free(oldPath);
	free(newPath);
	return op;
}

SDMMD_AFCOperationRef SDMMD_AFCOperationCreateSetFSBlockSize() { // ___AFCConnectionSetBlockSize
	SDMMD_AFCOperationRef op = calloc(1, sizeof(struct sdmmd_AFCOperation));
	op->packet = calloc(1, sizeof(struct sdmmd_AFCPacket));
	uint32_t data_length = sizeof(uint64_t);
	op->packet->header_data = calloc(1, sizeof(uint64_t));
	SDMMD_AFCHeaderInit(&(op->packet->header), SDMMD_AFC_Packet_SetFSBlockSize, data_length, 0, 0);
	return op;
}

SDMMD_AFCOperationRef SDMMD_AFCOperationCreateSetSocketBlockSize() { // ___AFCConnectionSetBlockSize
	SDMMD_AFCOperationRef op = calloc(1, sizeof(struct sdmmd_AFCOperation));
	op->packet = calloc(1, sizeof(struct sdmmd_AFCPacket));
	uint32_t data_length = sizeof(uint64_t);
	op->packet->header_data = calloc(1, sizeof(uint64_t));
	SDMMD_AFCHeaderInit(&(op->packet->header), SDMMD_AFC_Packet_SetSocketBlockSize, data_length, 0, 0);
	return op;
}

SDMMD_AFCOperationRef SDMMD_AFCFileDescriptorCreateLockOperation(void* fileRef) {
	SDMMD_AFCOperationRef op = calloc(1, sizeof(struct sdmmd_AFCOperation));
	op->packet = calloc(1, sizeof(struct sdmmd_AFCPacket));
	uint32_t data_length = sizeof(uint64_t)*2;
	op->packet->header_data = calloc(2, sizeof(uint64_t));
	SDMMD_AFCHeaderInit(&(op->packet->header), SDMMD_AFC_Packet_FileRefLock, data_length, 0, 0);
	return op;
}

SDMMD_AFCOperationRef SDMMD_AFCOperationCreateLinkPath(uint64_t linkType, CFStringRef target, CFStringRef link) {
	SDMMD_AFCOperationRef op = calloc(1, sizeof(struct sdmmd_AFCOperation));
	op->packet = calloc(1, sizeof(struct sdmmd_AFCPacket));
	char *targetPath = SDMCFStringGetString(target);
	char *linkPath = SDMCFStringGetString(link);
	uint32_t target_length = (uint32_t)strlen(targetPath)+1;
	uint32_t link_length = (uint32_t)strlen(linkPath)+1;
	uint32_t path_length = target_length + link_length;
	op->packet->header_data = calloc(1, sizeof(char[path_length]));
	memcpy(&op->packet->header_data, targetPath, target_length);
	memcpy(&op->packet->header_data[target_length], linkPath, link_length);
	SDMMD_AFCHeaderInit(&(op->packet->header), SDMMD_AFC_Packet_MakeLink, path_length, 0, 0);
	free(targetPath);
	free(linkPath);
	return op;
}

SDMMD_AFCOperationRef SDMMD_AFCOperationCreateGetFileHash(CFStringRef path) {
	SDMMD_AFCOperationRef op = calloc(1, sizeof(struct sdmmd_AFCOperation));
	op->packet = calloc(1, sizeof(struct sdmmd_AFCPacket));
	char *cpath = SDMCFStringGetString(path);
	uint32_t data_length = (uint32_t)strlen(cpath)+1;
	op->packet->header_data = calloc(data_length, sizeof(char));
	memcpy(op->packet->header_data, cpath, data_length);
	SDMMD_AFCHeaderInit(&(op->packet->header), SDMMD_AFC_Packet_GetFileHash, data_length, 0, 0);
	free(cpath);
	return op;
}

SDMMD_AFCOperationRef SDMMD_AFCOperationCreateSetModTime(CFStringRef ref) {
	SDMMD_AFCOperationRef op = calloc(1, sizeof(struct sdmmd_AFCOperation));
	op->packet = calloc(1, sizeof(struct sdmmd_AFCPacket));
	char *cref = SDMCFStringGetString(ref);
	uint32_t data_length = (uint32_t)strlen(cref)+1+8;
	op->packet->header_data = calloc(data_length, sizeof(char));
	memcpy(op->packet->header_data, cref, strlen(cref));
	SDMMD_AFCHeaderInit(&(op->packet->header), SDMMD_AFC_Packet_SetModTime, data_length, 0, 0);
	free(cref);
	return op;
}

SDMMD_AFCOperationRef SDMMD_AFCOperationCreateGetFileHashWithRange(CFStringRef path, Range range) {
	SDMMD_AFCOperationRef op = calloc(1, sizeof(struct sdmmd_AFCOperation));
	op->packet = calloc(1, sizeof(struct sdmmd_AFCPacket));
	char *cpath = SDMCFStringGetString(path);
	uint32_t data_length = (uint32_t)strlen(cpath)+1+sizeof(Range);
	op->packet->header_data = calloc(data_length, sizeof(char));
	memcpy(op->packet->header_data, &range, sizeof(Range));
	memcpy(&(op->packet->header_data[sizeof(Range)]), cpath, strlen(cpath));
	SDMMD_AFCHeaderInit(&(op->packet->header), SDMMD_AFC_Packet_GetFileHashWithRange, data_length, 0, 0);
	free(cpath);
	return op;
}

SDMMD_AFCOperationRef SDMMD_AFCFileDescriptorCreateSetImmutableHintOperation() {
	SDMMD_AFCOperationRef op = calloc(1, sizeof(struct sdmmd_AFCOperation));
	op->packet = calloc(1, sizeof(struct sdmmd_AFCPacket));
	SDMMD_AFCHeaderInit(&(op->packet->header), SDMMD_AFC_Packet_FileRefSetImmutableHint, 0, 0, 0);
	return op;
}

SDMMD_AFCOperationRef SDMMD_AFCOperationCreateGetSizeOfPathContents() {
	SDMMD_AFCOperationRef op = calloc(1, sizeof(struct sdmmd_AFCOperation));
	op->packet = calloc(1, sizeof(struct sdmmd_AFCPacket));
	SDMMD_AFCHeaderInit(&(op->packet->header), SDMMD_AFC_Packet_GetSizeOfPathContents, 0, 0, 0);
	return op;
}

SDMMD_AFCOperationRef SDMMD_AFCOperationCreateRemovePathAndContents() {
	SDMMD_AFCOperationRef op = calloc(1, sizeof(struct sdmmd_AFCOperation));
	op->packet = calloc(1, sizeof(struct sdmmd_AFCPacket));
	SDMMD_AFCHeaderInit(&(op->packet->header), SDMMD_AFC_Packet_RemovePathAndContents, 0, 0, 0);
	return op;
}

#pragma mark -
#pragma mark Alias Operations

sdmmd_return_t SDMMD_AMDeviceCopyFile(void *thing, void *thing2, void *thing3, SDMMD_AFCConnectionRef conn, char *local, char *remote) {
	return kAMDSuccess;
}

sdmmd_return_t SDMMD_check_can_touch(SDMMD_AFCConnectionRef conn, CFDataRef *unknown) {
	SDMMD_AFCOperationRef fileInfo = SDMMD_AFCOperationCreateGetFileInfo((CFStringRef)*unknown);
	SDMMD_AFCProcessOperation(conn, &fileInfo);
	*unknown = fileInfo->packet->response;
	return kAMDSuccess;
}

#endif
