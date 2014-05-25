/*
 *  SDMMD_Debugger.c
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

#ifndef _SDM_MD_DEBUGGER_C_
#define _SDM_MD_DEBUGGER_C_

#include "SDMMD_Debugger.h"
#include "SDMMD_Functions.h"
#include "SDMMD_AppleFileConduit.h"
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <signal.h>
#include <getopt.h>
#include "Core.h"

#define kChecksumHashLength 0x3

#define kDeveloperImageStreamSize 0x400000

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
	CheckErrorAndReturn(result);
	
	result = SDMMD_AMDeviceStartSession(dconn->device);
	CheckErrorAndReturn(result);
	
	dconn->connection = SDMMD_AMDServiceConnectionCreate(0, NULL, NULL);
	result = SDMMD_AMDeviceStartService(dconn->device, CFSTR(AMSVC_DEBUG_SERVER), NULL, &(dconn->connection));
		
	result = SDMMD_AMDeviceStopSession(dconn->device);
	CheckErrorAndReturn(result);
	
	result = SDMMD_AMDeviceDisconnect(dconn->device);
	CheckErrorAndReturn(result);

	
	ExitLabelAndReturn(result);
}

sdmmd_return_t SDMMD_AMDebugConnectionStop(SDMMD_AMDebugConnectionRef dconn) {
	sdmmd_return_t result = kAMDSuccess;
	SDMMD_AMDServiceConnectionInvalidate(dconn->connection);
	
	result = SDMMD_AMDeviceStopSession(dconn->device);
	CheckErrorAndReturn(result);
	
	result = SDMMD_AMDeviceDisconnect(dconn->device);
	CheckErrorAndReturn(result);
	
	ExitLabelAndReturn(result);
}

bool SDMMD_device_os_is_at_least(SDMMD_AMDeviceRef device, CFStringRef version) {
	bool result = false;
	if (device) {
		SDMMD_AMDeviceConnect(device);
		CFStringRef prodVers = SDMMD_AMDeviceCopyValue(device, NULL, CFSTR(kProductVersion));
		if (prodVers) {
			result = (CFStringCompare(prodVers, version, kCFCompareNumerically) != kCFCompareLessThan ? true : false);
		}
		CFSafeRelease(prodVers);
	}
	return result;
}


sdmmd_return_t SDMMD_copy_image(SDMMD_AMDeviceRef device, CFStringRef path) {
	sdmmd_return_t result = kAMDUndefinedError;
	if (device) {
		result = kAMDSuccess;//SDMMD_AMDeviceConnect(device);
		CheckErrorAndReturn(result);
		
		result = SDMMD_AMDeviceStartSession(device);
		CheckErrorAndReturn(result);
		
		SDMMD_AMConnectionRef copyConn = NULL;
		result = SDMMD_AMDeviceSecureStartService(device, CFSTR(AMSVC_AFC), NULL, &copyConn);
		CheckErrorAndReturn(result);
		
		SDMMD_AFCConnectionRef copyAFCConn = SDMMD_AFCConnectionCreate(copyConn);
		SDMMD_AFCOperationRef makeStaging = SDMMD_AFCOperationCreateMakeDirectory(CFSTR("PublicStaging"));
		result = SDMMD_AFCProcessOperation(copyAFCConn, &makeStaging);
		CheckErrorAndReturn(result);
		
		// SDM copy file AFC
		char *pathString = SDMCFStringGetString(path);
		result = SDMMD_AMDeviceCopyFile(SDMMD_Default_AFC_CopyFile_Callback, NULL, NULL, copyAFCConn, pathString, "PublicStaging/staging.dimage");
		Safe(free, pathString);
		
		SDMMD_AFCOperationRelease(makeStaging);
		SDMMD_AFCConnectionRelease(copyAFCConn);
		
		SDMMD_AMDServiceConnectionInvalidate(copyConn);
	}
	
	ExitLabelAndReturn(result);
}

sdmmd_return_t SDMMD__hangup_with_image_mounter_service(SDMMD_AMConnectionRef connection) {
	sdmmd_return_t result = kAMDNoResourcesError;
	CFMutableDictionaryRef dict = SDMMD_create_dict();
	if (dict) {
		CFDictionarySetValue(dict, CFSTR("Command"), CFSTR("Hangup"));
	}
	if (connection) {
		SocketConnection socket = SDMMD_TranslateConnectionToSocket(connection);
		result = SDMMD_ServiceSendMessage(socket, dict, kCFPropertyListXMLFormat_v1_0);
		CheckErrorAndReturn(result);
		
		CFPropertyListRef response = NULL;
		result = SDMMD_ServiceReceiveMessage(socket, &response);
		PrintCFDictionary(response);
	}
	ExitLabelAndReturn(result);
}

CFStringRef SDMMD_CopyDeviceSupportPathFromXCRUN() {
    FILE *output_pipe;
    char *xcrun_command = "xcrun -sdk iphoneos --show-sdk-platform-path";

	output_pipe = (FILE *)popen(xcrun_command, "r");
    if (output_pipe == NULL) {
		perror("Error encountered while opening pipe");
	}
	char *path = calloc(1, sizeof(char[1024]));
    fgets(path, sizeof(char[1024]), output_pipe);
    pclose(output_pipe);
	strtok(path, "\n");
	CFStringRef xcrun_path = CFStringCreateWithCString(NULL, path, kCFStringEncodingUTF8);
	Safe(free,path);
    return xcrun_path;
}

CFStringRef SDMMD_PathToDeviceSupport(SDMMD_AMDeviceRef device) {
	CFStringRef dev_support_path = NULL;
	if (device) {
		SDMMD_AMDeviceConnect(device);
		SDMMD_AMDeviceStartSession(device);
		CFStringRef full_os_version = SDMMD_AMDeviceCopyValue(device, NULL, CFSTR(kProductVersion));
		CFStringRef os_version = CFStringCreateWithSubstring(kCFAllocatorDefault, full_os_version, CFRangeMake(0, 3));
		CFSafeRelease(full_os_version);
		CFStringRef build_version = SDMMD_AMDeviceCopyValue(device, NULL, CFSTR(kBuildVersion));
		SDMMD_AMDeviceStopSession(device);
		SDMMD_AMDeviceDisconnect(device);
		if (os_version && build_version) {
			CFStringRef sdk_path = SDMMD_CopyDeviceSupportPathFromXCRUN();
			CFStringRef device_support = CFStringCreateWithFormat(kCFAllocatorDefault, NULL, CFSTR("%@/DeviceSupport/%@"), sdk_path, os_version);
			char *device_support_cstr = SDMCFStringGetString(device_support);
			bool isDir = false;
			bool file_exists = FileExistsAtPathIsDir(device_support_cstr, &isDir);
			if (file_exists && isDir) {
				dev_support_path = CFStringCreateCopy(kCFAllocatorDefault, device_support);
			}
			else {
				CFStringRef device_support_build = CFStringCreateWithFormat(kCFAllocatorDefault, NULL, CFSTR("%@ (%@)"), device_support, build_version);
				char *device_support_build_cstr = SDMCFStringGetString(device_support_build);
				file_exists = FileExistsAtPathIsDir(device_support_build_cstr, &isDir);
				if (file_exists && isDir) {
					dev_support_path = CFStringCreateCopy(kCFAllocatorDefault, device_support_build);
				}
				Safe(free, device_support_build_cstr);
				CFSafeRelease(device_support_build);
			}
			Safe(free, device_support_cstr);
			CFSafeRelease(sdk_path);
			CFSafeRelease(device_support);
		}
		CFSafeRelease(os_version);
		CFSafeRelease(build_version);
	}
	return dev_support_path;
}

CFStringRef SDMMD_PathToDeviceSupportDiskImage(CFStringRef device_support) {
	CFStringRef image_path = device_support;
	if (image_path) {
		image_path = CFStringCreateWithFormat(kCFAllocatorDefault, NULL, CFSTR("%@/DeveloperDiskImage.dmg"),device_support);
	}
	return image_path;
}

CFDictionaryRef SDMMD_CreateImageDictionary(CFStringRef device_support_image) {
	CFMutableDictionaryRef dict = CFDictionaryCreateMutable(kCFAllocatorDefault, 0, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
	if (dict) {
		CFStringRef signature_path = CFStringCreateWithFormat(kCFAllocatorDefault, NULL, CFSTR("%@.signature"),device_support_image);
		char *signature_path_cstr = SDMCFStringGetString(signature_path);
		if (FileExistsAtPath(signature_path_cstr)) {
			CFDataRef signature_data = CFDataCreateFromFilePath(signature_path_cstr);
			CFDictionarySetValue(dict, CFSTR("ImageType"), CFSTR("Developer"));
			CFDictionarySetValue(dict, CFSTR("ImageSignature"), signature_data);
		}
		Safe(free, signature_path_cstr);
		CFSafeRelease(signature_path);
	}
	return dict;
}

sdmmd_return_t SDMMD_AMDeviceMountDeveloperImage(SDMMD_AMDeviceRef device) {
	sdmmd_return_t result = kAMDMissingOptionsError;
	if (device) {
		CFStringRef device_support = SDMMD_PathToDeviceSupport(device);
		CFStringRef image_path = SDMMD_PathToDeviceSupportDiskImage(device_support);
		CFSafeRelease(device_support);
		CFDictionaryRef options = SDMMD_CreateImageDictionary(image_path);
		result = SDMMD_AMDeviceMountImage(device, image_path, options, SDMMD_Default_mount_callback, NULL);
		CFSafeRelease(image_path);
		CFSafeRelease(options);
	}
	return result;
}

sdmmd_return_t SDMMD_stream_image(SDMMD_AMConnectionRef connection, CFStringRef path, CFStringRef image_type) {
	sdmmd_return_t result = kAMDSuccess;
	char fspath[0x400] = {0};
	Boolean fsRep = CFStringGetFileSystemRepresentation(path, fspath, 0x400);
	if (fsRep) {
		struct stat fileStat;
		lstat(fspath, &fileStat);
		CFNumberRef size = CFNumberCreate(kCFAllocatorDefault, 0xb, &fileStat.st_size);
		CFMutableDictionaryRef streamDict = SDMMD_create_dict();
		CFDictionarySetValue(streamDict, CFSTR("Command"), CFSTR("ReceiveBytes"));
		CFDictionarySetValue(streamDict, CFSTR("ImageType"), image_type);
		CFDictionarySetValue(streamDict, CFSTR("ImageSize"), size);
		result = SDMMD_ServiceSendMessage(SDMMD_TranslateConnectionToSocket(connection), streamDict, kCFPropertyListXMLFormat_v1_0);
		CFSafeRelease(size);
		
		CheckErrorAndReturn(result);
		
		CFDictionaryRef response;
		result = SDMMD_ServiceReceiveMessage(SDMMD_TranslateConnectionToSocket(connection), (CFPropertyListRef*)&response);
		
		CheckErrorAndReturn(result);
		
		if (response) {
			result = SDMMD__ErrorHandler(SDMMD_ImageMounterErrorConvert, response);
			
			CheckErrorAndReturn(result);
			
			CFTypeRef status = CFDictionaryGetValue(response, CFSTR("Status"));
			if (status) {
				if (CFStringCompare(status, CFSTR("ReceiveBytesAck"), 0) == 0) {
					// block code
					CFDataRef image_file = CFDataCreateFromPath(path);
					uint64_t offset = 0;
					uint64_t remainder = 0;
					while (offset < fileStat.st_size) {
						remainder = (fileStat.st_size - offset);
						remainder = (remainder > kDeveloperImageStreamSize ? kDeveloperImageStreamSize : remainder);
						CFRange current_read = CFRangeMake((CFIndex)offset, (CFIndex)remainder);
						CFDataRef image_stream = CFDataCreateFromSubrangeOfData(image_file, current_read);
						result = SDMMD_DirectServiceSend(SDMMD_TranslateConnectionToSocket(connection), image_stream);
						CheckErrorAndReturn(result);
						offset += remainder;
						CFSafeRelease(image_stream);
					}
					CFDictionaryRef getStatus;
					result = SDMMD_ServiceReceiveMessage(SDMMD_TranslateConnectionToSocket(connection), (CFPropertyListRef*)&getStatus);
					
					if (result == 0) {
						result = SDMMD__ErrorHandler(SDMMD_ImageMounterErrorConvert, response);
						
						CheckErrorAndReturn(result);
						
						CFTypeRef streamStatus = CFDictionaryGetValue(getStatus, CFSTR("Status"));
						if (streamStatus) {
							if (CFStringCompare(streamStatus, CFSTR("Complete"), 0x0) == 0) {
								result = kAMDSuccess;
							}
						}
					}
				}
			}
			else {
				result = kAMDUndefinedError;
			}
		}
		else {
			result = kAMDReadError;
		}
		CFSafeRelease(size);
	}
	else {
		result = kAMDNoResourcesError;
	}
	
	ExitLabelAndReturn(result);
}

sdmmd_return_t SDMMD_mount_image(SDMMD_AMConnectionRef connection, CFStringRef image_type, CFDataRef signature, bool *mounted) {
	sdmmd_return_t result = kAMDSuccess;
	CFMutableDictionaryRef mountDict = SDMMD_create_dict();
	if (mountDict) {
		CFDictionarySetValue(mountDict, CFSTR("Command"), CFSTR("MountImage"));
		CFDictionarySetValue(mountDict, CFSTR("ImageType"), image_type);
		CFDictionarySetValue(mountDict, CFSTR("ImagePath"), CFSTR("/var/mobile/Media/PublicStaging/staging.dimage"));
		if (signature) {
			CFDictionarySetValue(mountDict, CFSTR("ImageSignature"), signature);
		}
		result = SDMMD_ServiceSendMessage(SDMMD_TranslateConnectionToSocket(connection), mountDict, kCFPropertyListXMLFormat_v1_0);
		CheckErrorAndReturn(result);
		
		CFDictionaryRef response;
		result = SDMMD_ServiceReceiveMessage(SDMMD_TranslateConnectionToSocket(connection), (CFPropertyListRef*)&response);
		CheckErrorAndReturn(result);
		
		if (response) {
			result = SDMMD__ErrorHandler(SDMMD_ImageMounterErrorConvert, response);
					
			CheckErrorAndReturn(result);
					
			CFTypeRef status = CFDictionaryGetValue(response, CFSTR("Status"));
			if (status) {
				if (CFEqual(status, CFSTR("Complete"))) {
					*mounted = true;
				}
				else {
					result = kAMDMissingDigestError;
				}
			}
		}
		else {
			result = kAMDReadError;
		}
	}
	else {
		result = kAMDReadError;
	}

	ExitLabelAndReturn(result);
}

sdmmd_return_t SDMMD_AMDeviceMountImage(SDMMD_AMDeviceRef device, CFStringRef path, CFDictionaryRef dict, CallBack handle, void* unknown) {
	sdmmd_return_t result = kAMDSuccess;
	if (dict) {
		SDMMD_AMConnectionRef connection = NULL;
		CFTypeRef digest = NULL;
		CFTypeRef signature = CFDictionaryGetValue(dict, CFSTR("ImageSignature"));
		CFStringRef image_type = CFDictionaryGetValue(dict, CFSTR("ImageType"));
		if (image_type) {
			unsigned char sum_digest[HASH_LENGTH];
			result = SDMMD_AMDeviceDigestFile(path, PtrCast(&sum_digest, unsigned char **));
			CheckErrorAndReturn(result);
			
			SDMMD_AMDeviceRef device_copy = SDMMD_AMDeviceCreateCopy(device);
			if (SDMMD_AMDeviceIsValid(device_copy)) {
				result = SDMMD_AMDeviceSecureStartSessionedService(device, CFSTR(AMSVC_MOBILE_IMAGE_MOUNT), &connection);
				CheckErrorAndReturn(result);
				
				SDMMD_fire_callback(handle, unknown, CFSTR("LookingUpImage"));
				if (connection) {
					CFMutableDictionaryRef commandDict = SDMMD_create_dict();
					if (commandDict) {
						CFDictionarySetValue(commandDict, CFSTR("Command"), CFSTR("LookupImage"));
						CFDictionarySetValue(commandDict, CFSTR("ImageType"), image_type);
					}
					else {
						result = kAMDNoResourcesError;
					}
					
					CheckErrorAndReturn(result);
					
					result = SDMMD_ServiceSendMessage(SDMMD_TranslateConnectionToSocket(connection), commandDict, kCFPropertyListXMLFormat_v1_0);
					
					CheckErrorAndReturn(result);
					
					CFDictionaryRef response;
					result = SDMMD_ServiceReceiveMessage(SDMMD_TranslateConnectionToSocket(connection), (CFPropertyListRef*)&response);
					
					CheckErrorAndReturn(result);
					
					if (response) {
						result = SDMMD__ErrorHandler(SDMMD__ConvertServiceError, response);
						
						CheckErrorAndReturn(result);
						
						CFTypeRef image = CFDictionaryGetValue(response, CFSTR("ImagePresent"));
						if (image) {
							if (CFEqual(image, kCFBooleanTrue)) {
								digest = CFDictionaryGetValue(response, CFSTR("ImageDigest"));
							}
							else {
								result = kAMDMissingDigestError;
							}
						}
					}
					
					if (!digest) {
						bool use_stream = SDMMD_device_os_is_at_least(device, CFSTR("7.0"));
						if (use_stream) {
							SDMMD_fire_callback(handle, unknown, CFSTR("StreamingImage"));
							result = SDMMD_stream_image(connection, path, image_type);
						}
						else {
							SDMMD_fire_callback(handle, unknown, CFSTR("CopyingImage"));
							result = SDMMD_copy_image(device, path);
						}
					}
					
					CheckErrorAndReturn(result);
					
					bool mounted = false;
					SDMMD_fire_callback(handle, unknown, CFSTR("MountingImage"));
					result = SDMMD_mount_image(connection, image_type, signature, &mounted);
					
					if (mounted) {
						result = SDMMD__hangup_with_image_mounter_service(connection);
					}
					
				}
				SDMMD_AMDServiceConnectionInvalidate(connection);
				CFSafeRelease(device_copy);
				CheckErrorAndReturn(result);
			}
			else {
				result = kAMDUndefinedError;
			}
		}
		else {
			result = kAMDMissingImageTypeError;
		}
	}
	else {
		result = kAMDMissingOptionsError;
	}
	
	ExitLabelAndReturn(result);
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
	CFIndex encodedLength = ((2*strlen(commandString))+kChecksumHashLength+1);
	char *encoded = calloc(1, S(char)*encodedLength);
	for (CFIndex position = 0, index = 0; index < strlen(commandString); index++) {
		position = (index*(S(char)*2));
		encoded[position] = kHexEncodeFirstByte(commandString[index]);
		encoded[position+1] = kHexEncodeSecondByte(commandString[index]);
	}
	Safe(free,commandString);
	BufferRef commandBuffer = calloc(1, S(struct CoreInternalBuffer));
	commandBuffer->data = encoded;
	commandBuffer->length = encodedLength;
	return commandBuffer;
}

void SDMMD_FormatDebuggingCommand(BufferRef buffer, CFStringRef command, bool shouldACK) {
	BufferRef encoded = SDMMD_EncodeDebuggingString(command);
	AppendBufferToBuffer(buffer, encoded);
	BufferRefRelease(encoded);
	char checksumHash[kChecksumHashLength] = { '#', '0', '0' };
	uint32_t encodedCommandLength = (uint32_t)(buffer->length-kChecksumHashLength-1);
	if (shouldACK) {
		uint32_t checksum = GenerateChecksumForData(&(buffer->data[1]), encodedCommandLength);
		checksumHash[1] = kHexEncodeFirstByte(checksum);
		checksumHash[2] = kHexEncodeSecondByte(checksum);
	}
	memcpy(&(buffer->data[encodedCommandLength]), checksumHash, kChecksumHashLength);
}

DebuggerCommandRef SDMMD_CreateDebuggingCommand(DebuggerCommandType commandCode, CFStringRef command, CFArrayRef arguments) {
	DebuggerCommandRef debugCommand = calloc(1, sizeof(struct DebuggerCommand));
	debugCommand->commandCode = commandCode;
	if (debugCommand->commandCode == kDebugCUSTOMCOMMAND) {
		debugCommand->command = CFStringCreateCopy(kCFAllocatorDefault, command);
	}
	else {
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

sdmmd_return_t SDMMD_DebuggingSend(SDMMD_AMDebugConnectionRef dconn, DebuggerCommandRef command, CFDataRef *response) {
	sdmmd_return_t result = kAMDSuccess;
	SocketConnection debuggingSocket = SDMMD_TranslateConnectionToSocket(dconn->connection);
	BufferRef buffer = CreateBufferRef();
	char *commandPrefix = "$";
	memcpy(buffer->data, commandPrefix, sizeof(char));
	char *string = NULL;
	if (command->commandCode == kDebugCUSTOMCOMMAND) {
		string = SDMCFStringGetString(command->command);
	}
	else {
		unsigned long length = strlen(KnownDebugCommands[command->commandCode].code);
		string = calloc(1, S(char)*(length+1));
		strlcpy(string, KnownDebugCommands[command->commandCode].code, length+1);
	}
	AppendStringToBuffer(buffer, string);
	Safe(free, string);

	CFMutableStringRef entireCommand = CFStringCreateMutable(kCFAllocatorDefault, 0);
	CFIndex argumentCount = CFArrayGetCount(command->arguments);
	for (CFIndex index = 0; index < argumentCount; index++) {
		CFStringRef argString = CFArrayGetValueAtIndex(command->arguments, index);
		CFStringAppend(entireCommand, argString);
	}
	SDMMD_FormatDebuggingCommand(buffer, entireCommand, dconn->ackEnabled);
	CFSafeRelease(entireCommand);
	CFDataRef sending = CFDataCreate(kCFAllocatorDefault, PtrCast(buffer->data, UInt8*), (CFIndex)(buffer->length));
	result = SDMMD_ServiceSend(debuggingSocket, sending);
	CheckErrorAndReturn(result);
	
	result = SDMMD_DebuggingReceive(dconn, response);
	if (SDM_MD_CallSuccessful(result) && command->commandCode == kDebugQStartNoAckMode) {
		dconn->ackEnabled = false;
	}
	
	BufferRefRelease(buffer);
	CFSafeRelease(sending);
	
	ExitLabelAndReturn(result);
}

bool SDMMD_DebuggingReceiveInternalCheck(SocketConnection connection, char *receivedChar) {
	bool didReceiveChar = false;
	CFMutableDataRef receivedData = CFDataCreateMutable(kCFAllocatorDefault, 1);
	sdmmd_return_t result = SDMMD_DirectServiceReceive(connection, PtrCast(&receivedData, CFDataRef*));
	char *buffer = calloc(1, S(char));
	memcpy(buffer, CFDataGetBytePtr(receivedData), sizeof(char));
	if (SDM_MD_CallSuccessful(result) && receivedChar[0] != 0) {
		didReceiveChar = ((memcmp(buffer, receivedChar, S(char)) == 0) ? true : false);
	}
	else {
		didReceiveChar = false;
	}
	if (!didReceiveChar) {
		memcpy(receivedChar, buffer, S(char));
	}
	Safe(free, buffer);
	return didReceiveChar;
}

bool SDMMD_ValidateChecksumForBuffer(BufferRef buffer) {
	uint32_t checksum = GenerateChecksumForData(&(buffer->data[1]), (uint32_t)(buffer->length-kChecksumHashLength-1));
	return ((kHexDecode(buffer->data[buffer->length-2]) == kHexDecodeFirstByte(checksum)) && (kHexDecode(buffer->data[buffer->length-1]) && kHexDecodeSecondByte(checksum)) ? true : false);
}

sdmmd_return_t SDMMD_DebuggingReceive(SDMMD_AMDebugConnectionRef dconn, CFDataRef *response) {
	sdmmd_return_t result = kAMDSuccess;
	bool shouldReceive = true, skipPrefix = false;
	char *commandPrefix = "$";
	BufferRef responseBuffer = CreateBufferRef();
	SocketConnection debuggingSocket = SDMMD_TranslateConnectionToSocket(dconn->connection);
	if (dconn->ackEnabled) {
		char ack[1];
		memcpy(ack, (KnownDebugCommands[kDebugACK].code), S(char));
		shouldReceive = SDMMD_DebuggingReceiveInternalCheck(debuggingSocket, ack);
		if (strncmp(ack, commandPrefix, S(char)) == 0) {
			shouldReceive = true;
			skipPrefix = true;
			memcpy(responseBuffer->data, commandPrefix, sizeof(char));
		}
	}
	if (shouldReceive && !skipPrefix) {
		shouldReceive = SDMMD_DebuggingReceiveInternalCheck(debuggingSocket, commandPrefix);
		if (shouldReceive) {
			memcpy(responseBuffer->data, commandPrefix, sizeof(char));
		}
	}
	if (shouldReceive) {
		uint32_t checksumLength = kChecksumHashLength;
		bool receivingChecksumResponse = false;
		while ((checksumLength > 0)) {
			uint64_t oldSize = IncrementBufferRefBySize(responseBuffer, sizeof(char));
			char *data = "#";
			receivingChecksumResponse = SDMMD_DebuggingReceiveInternalCheck(debuggingSocket, data);
			if (receivingChecksumResponse) {
				checksumLength--;
			}
			memcpy(&(responseBuffer->data[oldSize]), data, sizeof(char));
		}
		bool validResponse = SDMMD_ValidateChecksumForBuffer(responseBuffer);
		if (validResponse) {
			*response = CFDataCreate(kCFAllocatorDefault, PtrCast(&(responseBuffer->data[1]), UInt8*), (CFIndex)(responseBuffer->length-kChecksumHashLength-0x1));
		}
		else {
			result = kAMDInvalidResponseError;
		}
		BufferRefRelease(responseBuffer);
	}
	return result;
}

#endif