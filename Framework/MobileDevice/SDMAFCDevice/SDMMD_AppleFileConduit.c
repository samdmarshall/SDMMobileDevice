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
		afc->operationCount = 0;
	}
	return afc;
}

void SDMMD_AFCConnectionRelease(SDMMD_AFCConnectionRef conn) {
	if (conn) {
		SDMMD_AMDServiceConnectionInvalidate(conn->handle);
		dispatch_release(conn->operationQueue);
	}
}

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
	//printf("header sent status: %08x %s\n",result,SDMMD_AMDErrorString(result));
	if (!(op->packet->header.headerLen == op->packet->header.packetLen && op->packet->body_data == NULL)) {
		CFDataRef bodyData = CFDataCreate(kCFAllocatorDefault, (UInt8*)(op->packet->body_data), (uint32_t)op->packet->header.packetLen - (uint32_t)op->packet->header.headerLen);
		result = SDMMD_DirectServiceSend(SDMMD_TranslateConnectionToSocket(conn->handle), bodyData);
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
	SDMMD_AFCPacketHeader *header = (SDMMD_AFCPacketHeader *)CFDataGetBytePtr(headerData);
	
	CFMutableDataRef bodyData = CFDataCreateMutable(kCFAllocatorDefault, (uint32_t)header->packetLen - (uint32_t)header->headerLen);
	char *body = calloc(1, (uint32_t)header->packetLen - sizeof(SDMMD_AFCPacketHeader));
	CFDataAppendBytes(bodyData, (UInt8*)body, (uint32_t)header->packetLen - sizeof(SDMMD_AFCPacketHeader));
	free(body);
	result = SDMMD_DirectServiceReceive(SDMMD_TranslateConnectionToSocket(conn->handle), (CFDataRef*)&bodyData);
	if (bodyData) {
		(*operation)->packet->response_data = (void*)CFDataGetBytePtr(bodyData);
	}
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
		dispatch_release(conn->semaphore);
		conn->operationCount++;
	});
	return result;
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

SDMMD_AFCOperationRef SDMMD_AFCOperationCreateFileRefOpen(CFStringRef path, uint16_t mode, void* *fileRef) {
	SDMMD_AFCOperationRef op = calloc(1, sizeof(struct sdmmd_AFCOperation));
	op->packet = calloc(1, sizeof(struct sdmmd_AFCPacket));
	char *cpath = SDMCFStringGetString(path);
	uint32_t data_length = (uint32_t)strlen(cpath)+1+8;
	op->packet->header_data = calloc(data_length, sizeof(char));
	memcpy(op->packet->header_data, &mode, sizeof(mode));
	memcpy(&(op->packet->header_data[sizeof(mode)]), cpath, strlen(cpath));
	SDMMD_AFCHeaderInit(&(op->packet->header), SDMMD_AFC_Packet_FileRefOpen, data_length, 0, 0);
	free(cpath);
	return op;
}

SDMMD_AFCOperationRef SDMMD_AFCFileDescriptorCreateReadOperation(void* fileRef, uint32_t length) { // _AFCFileDescriptorCreateReadOperation
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

SDMMD_AFCOperationRef SDMMD_AFCFileDescriptorCreateWriteOperation(void* fileRef, CFDataRef data) { // _AFCFileDescriptorCreateWriteOperation
	SDMMD_AFCOperationRef op = calloc(1, sizeof(struct sdmmd_AFCOperation));
	op->packet = calloc(1, sizeof(struct sdmmd_AFCPacket));
	uint32_t data_length = 8;
	uint32_t body_length = (uint32_t)CFDataGetLength(data);
	op->packet->header_data = calloc(data_length, sizeof(char));
	memcpy(op->packet->header_data, fileRef, data_length);
	op->packet->body_data = calloc(body_length, sizeof(char));
	memcpy(op->packet->body_data, CFDataGetBytePtr(data), body_length);
	SDMMD_AFCHeaderInit(&(op->packet->header), SDMMD_AFC_Packet_FileRefWrite, data_length, body_length, 0x0);
	return op;
}

SDMMD_AFCOperationRef SDMMD_AFCFileDescriptorCreateSeekOperation(void* fileRef, uint32_t pos) { // _AFCFileDescriptorCreateSetPositionOperation
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

SDMMD_AFCOperationRef SDMMD_AFCFileDescriptorCreateTellOperation(void* fileRef) { // _AFCFileDescriptorCreateGetPositionOperation
	SDMMD_AFCOperationRef op = calloc(1, sizeof(struct sdmmd_AFCOperation));
	op->packet = calloc(1, sizeof(struct sdmmd_AFCPacket));
	op->packet->header_data = calloc(1, sizeof(uint64_t));
	memcpy(op->packet->header_data, fileRef, sizeof(uint64_t));
	SDMMD_AFCHeaderInit(&(op->packet->header), SDMMD_AFC_Packet_FileRefTell, sizeof(uint64_t), 0, 0);
	return op;
}

SDMMD_AFCOperationRef SDMMD_AFCFileDescriptorCreateCloseOperation(void* fileRef) { // _AFCFileDescriptorCreateCloseOperation
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
	uint32_t path_length = (uint32_t)(strlen(oldPath)+1+(uint32_t)strlen(newPath)+1);
	op->packet->header_data = calloc(1, sizeof(char[path_length]));
	memcpy(&op->packet->header_data, oldPath, strlen(oldPath));
	memcpy(&op->packet->header_data[strlen(oldPath)+1], newPath, strlen(newPath));
	SDMMD_AFCHeaderInit(&(op->packet->header), SDMMD_AFC_Packet_RenamePath, path_length, 0, 0);
	free(oldPath);
	free(newPath);
	return op;
}

SDMMD_AFCOperationRef SDMMD_AFCOperationCreateSetFSBlockSize() { // ___AFCConnectionSetBlockSize
	SDMMD_AFCOperationRef op = calloc(1, sizeof(struct sdmmd_AFCOperation));
	op->packet = calloc(1, sizeof(struct sdmmd_AFCPacket));
	uint32_t data_length = 8;
	op->packet->header_data = calloc(data_length, sizeof(char));
	SDMMD_AFCHeaderInit(&(op->packet->header), SDMMD_AFC_Packet_SetFSBlockSize, data_length, 0, 0);
	return op;
}

SDMMD_AFCOperationRef SDMMD_AFCOperationCreateSetSocketBlockSize() { // ___AFCConnectionSetBlockSize
	SDMMD_AFCOperationRef op = calloc(1, sizeof(struct sdmmd_AFCOperation));
	op->packet = calloc(1, sizeof(struct sdmmd_AFCPacket));
	uint32_t data_length = 8;
	op->packet->header_data = calloc(data_length, sizeof(char));
	SDMMD_AFCHeaderInit(&(op->packet->header), SDMMD_AFC_Packet_SetSocketBlockSize, data_length, 0, 0);
	return op;
}

SDMMD_AFCOperationRef SDMMD_AFCFileDescriptorCreateLockOperation(void* fileRef) {
	SDMMD_AFCOperationRef op = calloc(1, sizeof(struct sdmmd_AFCOperation));
	op->packet = calloc(1, sizeof(struct sdmmd_AFCPacket));
	uint32_t data_length = 16;
	op->packet->header_data = calloc(data_length, sizeof(char));
	SDMMD_AFCHeaderInit(&(op->packet->header), SDMMD_AFC_Packet_FileRefLock, data_length, 0, 0);
	return op;
}

SDMMD_AFCOperationRef SDMMD_AFCOperationCreateLinkPath(uint64_t linkType, CFStringRef target, CFStringRef link) {
	SDMMD_AFCOperationRef op = calloc(1, sizeof(struct sdmmd_AFCOperation));
	op->packet = calloc(1, sizeof(struct sdmmd_AFCPacket));
	char *targetPath = SDMCFStringGetString(target);
	char *linkPath = SDMCFStringGetString(link);
	uint32_t path_length = (uint32_t)(strlen(targetPath)+strlen(linkPath)+2);
	op->packet->header_data = calloc(1, sizeof(char[path_length]));
	memcpy(&op->packet->header_data, targetPath, strlen(targetPath));
	memcpy(&op->packet->header_data[strlen(targetPath)+1], linkPath, strlen(linkPath));
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
SDMMD_AFCOperationRef SDMMD_AFCOperationCreateGetFileHashWithRange(CFStringRef path, void* range) {
	SDMMD_AFCOperationRef op = calloc(1, sizeof(struct sdmmd_AFCOperation));
	op->packet = calloc(1, sizeof(struct sdmmd_AFCPacket));
	char *cpath = SDMCFStringGetString(path);
	uint32_t data_length = (uint32_t)strlen(cpath)+1+0x10;
	op->packet->header_data = calloc(data_length, sizeof(char));
	memcpy(&(op->packet->header_data[0x10]), cpath, strlen(cpath));
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
	*unknown = fileInfo->packet->response_data;
	return kAMDSuccess;
}

#endif
