/*
 *  SDMMD_Debugger.c
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

#ifndef _SDM_MD_DEBUGGER_C_
#define _SDM_MD_DEBUGGER_C_

#include "SDMMD_Debugger.h"
#include "SDMMD_Functions.h"
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <signal.h>
#include <getopt.h>
#include "Core.h"

#define kChecksumHashLength 0x3

static char *kHexEncodeString = "0123456789ABCDEF";
#define kHexDecode(byte) ((byte >= '0' && byte <= '9') ? (byte - '0') : ( (byte >= 'a' && byte <= 'f') ? (10 + byte - 'a') : ((byte >= 'A' && byte <= 'F') ? (10 + byte - 'A') : byte)))


#define kHexCodeApplyMask(byte) (byte & 0xf)
#define kHexCodeApplyShift(byte) (byte >> 0x4)

#define kHexEncodeFirstByte(byte) kHexEncodeString[kHexDecodeFirstByte(byte)]
#define kHexEncodeSecondByte(byte) kHexEncodeString[(byte & 0x0f)]

#define kHexDecodeFirstByte(byte) (kHexCodeApplyMask(kHexCodeApplyShift(byte)))
#define kHexDecodeSecondByte(byte) (kHexCodeApplyMask(byte))

SDMMD_AMDebugConnectionRef SDMMD_AMDebugConnectionCreateForDevice(SDMMD_AMDeviceRef device) {
	SDMMD_AMDebugConnectionRef dconn = calloc(0x1, sizeof(struct SDMMD_AMDebugConnection));
	if (dconn) {
		dconn->device = SDMMD_AMDeviceCreateCopy(device);
		dconn->connection = NULL;
		dconn->ackEnabled = true;
	}
	return dconn;
}

void SDMMD_AMDebugConnectionClose(SDMMD_AMDebugConnectionRef dconn) {
	CFSafeRelease(dconn->device);
	SDMMD_AMDServiceConnectionInvalidate(dconn->connection);
	Safe(free, dconn->connection);
	Safe(free, dconn);
}

sdmmd_return_t SDMMD_AMDebugConnectionStart(SDMMD_AMDebugConnectionRef dconn) {
	sdmmd_return_t result = SDMMD_AMDeviceConnect(dconn->device);
	if (SDM_MD_CallSuccessful(result)) {
		result = SDMMD_AMDeviceStartSession(dconn->device);
		if (SDM_MD_CallSuccessful(result)) {
			dconn->connection = SDMMD_AMDServiceConnectionCreate(0, NULL, NULL);
			result = SDMMD_AMDeviceStartService(dconn->device, CFSTR(AMSVC_DEBUG_SERVER), NULL, &(dconn->connection));
		}
	}
	return result;
}

sdmmd_return_t SDMMD_AMDebugConnectionStop(SDMMD_AMDebugConnectionRef dconn) {
	sdmmd_return_t result = kAMDSuccess;
	if (SDM_MD_CallSuccessful(result)) {
		SDMMD_AMDServiceConnectionInvalidate(dconn->connection);
		result = SDMMD_AMDeviceStopSession(dconn->device);
		if (SDM_MD_CallSuccessful(result)) {
			result = SDMMD_AMDeviceDisconnect(dconn->device);
		}
	}
	return result;
}

uint32_t GenerateChecksumForData(char *strPtr, uint32_t length) {
	uint32_t checksum = 0;
	for (uint32_t i = 0; i < length; i++) {
		checksum += strPtr[i];
	}
	return checksum;
}

BufferRef SDMMD_EncodeDebuggingString(CFStringRef command) {
	char *commandString = SDMCFStringGetString(command);
	CFIndex encodedLength = ((0x2*strlen(commandString))+kChecksumHashLength+0x1);
	char *encoded = calloc(0x1, S(char)*encodedLength);
	for (CFIndex position = 0x0, index = 0x0; index < strlen(commandString); index++) {
		position = (index*(S(char)*0x2));
		encoded[position] = kHexEncodeFirstByte(commandString[index]);
		encoded[position+1] = kHexEncodeSecondByte(commandString[index]);
	}
	Safe(free,commandString);
	BufferRef commandBuffer = calloc(0x1, S(struct CoreInternalBuffer));
	commandBuffer->data = encoded;
	commandBuffer->length = encodedLength;
	return commandBuffer;
}

void SDMMD_FormatDebuggingCommand(BufferRef buffer, CFStringRef command, bool shouldACK) {
	BufferRef encoded = SDMMD_EncodeDebuggingString(command);
	AppendBufferToBuffer(buffer, encoded);
	BufferRefRelease(encoded);
	char checksumHash[kChecksumHashLength] = { '#', '0', '0' };
	uint32_t encodedCommandLength = (uint32_t)(buffer->length-kChecksumHashLength-0x1);
	if (shouldACK) {
		uint32_t checksum = GenerateChecksumForData(&(buffer->data[0x1]), encodedCommandLength);
		checksumHash[0x1] = kHexEncodeFirstByte(checksum);
		checksumHash[0x2] = kHexEncodeSecondByte(checksum);
	}
	memcpy(&(buffer->data[encodedCommandLength]), checksumHash, kChecksumHashLength);
}

DebuggerCommandRef SDMMD_CreateDebuggingCommand(DebuggerCommandType commandCode, CFStringRef command, CFArrayRef arguments) {
	DebuggerCommandRef debugCommand = calloc(0x1, sizeof(struct DebuggerCommand));
	debugCommand->commandCode = commandCode;
	if (debugCommand->commandCode == kDebugCUSTOMCOMMAND) {
		debugCommand->command = CFStringCreateCopy(kCFAllocatorDefault, command);
	} else {
		debugCommand->command = NULL;
	}
	debugCommand->arguments = CFArrayCreateCopy(kCFAllocatorDefault, arguments);
	return debugCommand;
}

void SDMMD_DebuggingCommandRelease(DebuggerCommandRef command) {
	CFSafeRelease(command->command);
	CFSafeRelease(command->arguments);
	Safe(free,command);
}


void SDMMD_DebuggingLogData(CFDataRef data, CFStringRef prefix) {
    // FIXME: select proper encoding.
    CFStringRef payload = CFStringCreateWithBytes(kCFAllocatorDefault,
                                                  CFDataGetBytePtr(data),
                                                  CFDataGetLength(data),
                                                  kCFStringEncodingUTF8,
                                                  false);
    CFShow(CFStringCreateWithFormat(kCFAllocatorDefault, NULL, CFSTR("<%4ld> %@: %@"),
                                    CFStringGetLength(payload), prefix, payload));
    CFSafeRelease(payload);
}

void SDMMD_DebuggingLogSend(CFDataRef data) {
#if LOG_REMOTE
    SDMMD_DebuggingLogData(data, CFSTR("send packet"));
#endif
}
void SDMMD_DebuggingLogRecv(CFDataRef data) {
#if LOG_REMOTE
    SDMMD_DebuggingLogData(data, CFSTR("read packet"));
#endif
}


/*!
 * Send an acknowledge packet to the debugserver indicating that the received
 * data was understood.
 *
 * @param dconn the debug connection reference.
 * @returns whether or not the send was successful.
 */
bool SDMMD_DebuggingSendAck(SDMMD_AMDebugConnectionRef dconn) {
	sdmmd_return_t result = kAMDSuccess;
	SocketConnection debuggingSocket = SDMMD_TranslateConnectionToSocket(dconn->connection);
    UInt8 * payload = (UInt8*)KnownDebugCommands[kDebugACK].code;
    CFDataRef ack = CFDataCreate(kCFAllocatorDefault, payload, 1);
    assert(ack);
    SDMMD_DebuggingLogSend(ack);
    result = SDMMD_ServiceSend(debuggingSocket, ack);
    CFSafeRelease(ack);
    return SDM_MD_CallSuccessful(result);
}

/*!
 * Read an acknowledge packet from the debugserver. Use this method if ack mode
 * is on. Every successful transmitted packet to the server will result in an
 * ack packet.
 *
 * @param dconn the debug connection reference.
 * @param data pointer. This can be used to inspect the result.
 * @returns whether or not the receive was successful.
 */
bool SDMMD_DebuggingRecvAck(SDMMD_AMDebugConnectionRef dconn, CFMutableDataRef data) {
    sdmmd_return_t result = kAMDSuccess;
    SocketConnection debuggingSocket = SDMMD_TranslateConnectionToSocket(dconn->connection);
    result = SDMMD_DirectServiceReceiveN(debuggingSocket, data, 1);
    SDMMD_DebuggingLogRecv(data);
    return SDM_MD_CallSuccessful(result);
}

sdmmd_return_t SDMMD_DebuggingSend(SDMMD_AMDebugConnectionRef dconn, DebuggerCommandRef command, CFDataRef *response) {
	sdmmd_return_t result = kAMDSuccess;
	SocketConnection debuggingSocket = SDMMD_TranslateConnectionToSocket(dconn->connection);
	BufferRef buffer = CreateBufferRef();
	char *commandPrefix = "$";
	memcpy(buffer->data, commandPrefix, 0x1);
	char *string = 0x0;
	if (command->commandCode == kDebugCUSTOMCOMMAND) {
		string = SDMCFStringGetString(command->command);
	} else {
		unsigned long length = strlen(KnownDebugCommands[command->commandCode].code);
		string = calloc(0x1, S(char)*(length+0x1));
		strlcpy(string, KnownDebugCommands[command->commandCode].code, length+0x1);
	}
	AppendStringToBuffer(buffer, string);
	Safe(free, string);

	CFMutableStringRef entireCommand = CFStringCreateMutable(kCFAllocatorDefault, 0x0);
	CFIndex argumentCount = CFArrayGetCount(command->arguments);
	for (CFIndex index = 0x0; index < argumentCount; index++) {
		CFStringRef argString = CFArrayGetValueAtIndex(command->arguments, index);
		CFStringAppend(entireCommand, argString);
	}
	SDMMD_FormatDebuggingCommand(buffer, entireCommand, dconn->ackEnabled);
	CFSafeRelease(entireCommand);
	CFDataRef sending = CFDataCreate(kCFAllocatorDefault, PtrCast(buffer->data, UInt8*), (CFIndex)(buffer->length)-1);
    SDMMD_DebuggingLogSend(sending);
	result = SDMMD_ServiceSend(debuggingSocket, sending);
	if (SDM_MD_CallSuccessful(result)) {
		result = SDMMD_DebuggingReceive(dconn, response);
		if (SDM_MD_CallSuccessful(result) && command->commandCode == kDebugQStartNoAckMode) {
			dconn->ackEnabled = false;
		}
	}
	BufferRefRelease(buffer);
	CFSafeRelease(sending);
	return result;
}

bool SDMMD_DebuggingReceiveInternalCheck(SocketConnection connection, char *receivedChar) {
	bool didReceiveChar = false;
	CFMutableDataRef receivedData = CFDataCreateMutable(kCFAllocatorDefault, 0x1);
	sdmmd_return_t result = SDMMD_DirectServiceReceive(connection, PtrCast(&receivedData, CFDataRef*));
	char *buffer = calloc(0x1, S(char));
	memcpy(buffer, CFDataGetBytePtr(receivedData), 0x1);
	if (SDM_MD_CallSuccessful(result) && receivedChar[0] != 0x0) {
		didReceiveChar = ((memcmp(buffer, receivedChar, S(char)) == 0x0) ? true : false);
	} else {
		didReceiveChar = false;
	}
	if (!didReceiveChar) {
		memcpy(receivedChar, buffer, S(char));
	}
	Safe(free, buffer);
	return didReceiveChar;
}

bool SDMMD_ValidateChecksumForBuffer(BufferRef buffer) {
	uint32_t checksum = GenerateChecksumForData(&(buffer->data[0x1]), (uint32_t)(buffer->length-kChecksumHashLength-0x1));
	return ((kHexDecode(buffer->data[buffer->length-0x2]) == kHexDecodeFirstByte(checksum)) && (kHexDecode(buffer->data[buffer->length-0x1]) && kHexDecodeSecondByte(checksum)) ? true : false);
}

sdmmd_return_t SDMMD_DebuggingReceive(SDMMD_AMDebugConnectionRef dconn, CFDataRef *response) {
	sdmmd_return_t result = kAMDSuccess;
	bool shouldReceive = true, skipPrefix = false;
	char *commandPrefix = "$";
	BufferRef responseBuffer = CreateBufferRef();
	SocketConnection debuggingSocket = SDMMD_TranslateConnectionToSocket(dconn->connection);
	if (dconn->ackEnabled) {
		char ack[0x1];
		memcpy(ack, (KnownDebugCommands[kDebugACK].code), S(char));
		shouldReceive = SDMMD_DebuggingReceiveInternalCheck(debuggingSocket, ack);
		if (strncmp(ack, commandPrefix, S(char)) == 0x0) {
			shouldReceive = true;
			skipPrefix = true;
			memcpy(responseBuffer->data, commandPrefix, 0x1);
		}
	}
	if (shouldReceive && !skipPrefix) {
		shouldReceive = SDMMD_DebuggingReceiveInternalCheck(debuggingSocket, commandPrefix);
		if (shouldReceive) {
			memcpy(responseBuffer->data, commandPrefix, 0x1);
		}
	}
	if (shouldReceive) {
		uint32_t checksumLength = kChecksumHashLength;
		bool receivingChecksumResponse = false;
		while ((checksumLength > 0)) {
			uint64_t oldSize = IncrementBufferRefBySize(responseBuffer, 0x1);
			char *data = "#";
			receivingChecksumResponse = SDMMD_DebuggingReceiveInternalCheck(debuggingSocket, data);
			if (receivingChecksumResponse) {
				checksumLength--;
			}
			memcpy(&(responseBuffer->data[oldSize]), data, 0x1);
		}
		bool validResponse = SDMMD_ValidateChecksumForBuffer(responseBuffer);
		if (validResponse) {
			*response = CFDataCreate(kCFAllocatorDefault, PtrCast(&(responseBuffer->data[0x1]), UInt8*), (CFIndex)(responseBuffer->length-kChecksumHashLength-0x1));
		} else {
			result = kAMDInvalidResponseError;
		}
		BufferRefRelease(responseBuffer);
	}
	return result;
}

#endif