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
	sdmmd_return_t result = kAMDSuccess;
	CFMutableDataRef headerData = CFDataCreateMutable(kCFAllocatorDefault, (CFIndex)op->packet->header.packetLen);//(kCFAllocatorDefault, (UInt8*)&op->packet->header, sizeof(SDMMD_AFCPacketHeader));
	CFDataAppendBytes(headerData, (UInt8*)&(op->packet->header), sizeof(SDMMD_AFCPacketHeader));
	if (op->packet->header.headerLen > sizeof(SDMMD_AFCPacketHeader)) {
		CFDataAppendBytes(headerData, (UInt8*)(op->packet->data), (uint32_t)op->packet->header.packetLen - sizeof(SDMMD_AFCPacketHeader));
	}
	result = SDMMD_DirectServiceSend(SDMMD_TranslateConnectionToSocket(conn->handle), headerData);
	printf("header sent status: %08x %s\n",result,SDMMD_AMDErrorString(result));
	if (!(op->packet->header.headerLen == op->packet->header.packetLen && op->packet->data == NULL)) {
		CFDataRef bodyData = CFDataCreate(kCFAllocatorDefault, (UInt8*)(op->packet->data), (uint32_t)op->packet->header.packetLen - sizeof(SDMMD_AFCPacketHeader));
		result = SDMMD_DirectServiceSend(SDMMD_TranslateConnectionToSocket(conn->handle), bodyData);
		printf("body sent status: %08x %s\n",result,SDMMD_AMDErrorString(result));
	}
	return result;
}

sdmmd_return_t SDMMD_AFCReceiveOperation(SDMMD_AFCConnectionRef conn, SDMMD_AFCOperationRef *op) {
	sdmmd_return_t result = kAMDSuccess;
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
	if (bodyData) {
		packet->body_data = (void*)CFDataGetBytePtr(bodyData);
	}
	SDMMD_AFCOperationRef response = calloc(1, sizeof(struct sdmmd_AFCOperation));
	response->packet = packet;
	response->timeout = 0;
	*op = response;
	return result;
}

sdmmd_return_t SDMMD_AFCProcessOperation(SDMMD_AFCConnectionRef conn, SDMMD_AFCOperationRef op, SDMMD_AFCOperationRef *response) {
	__block sdmmd_return_t result = kAMDSuccess;
	__block SDMMD_AFCOperationRef blockReply;
	dispatch_sync(conn->operationQueue, ^{
		conn->semaphore = dispatch_semaphore_create(0x0);
		op->packet->header.pid = k64BitMask; //conn->operationCount;
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
	return CFDataCreateWithBytesNoCopy(kCFAllocatorDefault, (UInt8*)(op->packet->data), (uint32_t)op->packet->header.packetLen-(uint32_t)op->packet->header.headerLen, kCFAllocatorDefault);
}


#endif
