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
#include "SDMMD_AppleFileConduit.h"
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
		if (SDM_MD_CallSuccessful(result)) {
			result = SDMMD_AMDeviceStartSession(device);
			if (SDM_MD_CallSuccessful(result)) {
				SDMMD_AMConnectionRef copyConn = NULL;
				result = SDMMD_AMDeviceSecureStartService(device, CFSTR(AMSVC_AFC), NULL, &copyConn);
				if (SDM_MD_CallSuccessful(result)) {
					SDMMD_AFCConnectionRef copyAFCConn = SDMMD_AFCConnectionCreate(copyConn);
					SDMMD_AFCOperationRef makeStaging = SDMMD_AFCOperationCreateMakeDirectory(CFSTR("PublicStaging"));
					result = SDMMD_AFCProcessOperation(copyAFCConn, &makeStaging);
					if (SDM_MD_CallSuccessful(result)) {
						// SDM copy file AFC
						char *pathString = SDMCFStringGetString(path);
						result = SDMMD_AMDeviceCopyFile(NULL, NULL, NULL, copyAFCConn, pathString,"PublicStaging/staging.dimage");
						//SDMMD_AMDeviceRemoteCopyFile(NULL, NULL, NULL, copyAFCConn, pathString,"PublicStaging/staging.dimage");
						Safe(free, pathString);
					}
				}
			}
		}
		else {
			
		}
	}
	return result;
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
		if (SDM_MD_CallSuccessful(result)) {
			CFPropertyListRef response = NULL;
			result = SDMMD_ServiceReceiveMessage(socket, &response);
			PrintCFDictionary(response);
		}
	}
	return result;
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
		CFStringRef os_version = SDMMD_AMDeviceCopyValue(device, NULL, CFSTR(kProductVersion));
		CFStringRef build_version = SDMMD_AMDeviceCopyValue(device, NULL, CFSTR(kBuildVersion));
		SDMMD_AMDeviceStopSession(device);
		SDMMD_AMDeviceDisconnect(device);
		if (os_version && build_version) {
			// Standardize the ProductVersion string to 2 number components (e.g "7.0.6" becomes "7.0", "9 becomes 9.0")
			CFArrayRef osVersionComponents = CFStringCreateArrayBySeparatingStrings(kCFAllocatorDefault, os_version, CFSTR("."));
			CFMutableStringRef truncatedOSVersion = CFStringCreateMutable(kCFAllocatorDefault, 0);
			if (CFArrayGetCount(osVersionComponents) > 0) {
				CFStringAppend(truncatedOSVersion, CFArrayGetValueAtIndex(osVersionComponents, 0));
				CFStringAppend(truncatedOSVersion, CFSTR("."));
				if (CFArrayGetCount(osVersionComponents) > 1) {
					CFStringAppend(truncatedOSVersion, CFArrayGetValueAtIndex(osVersionComponents, 1));
				}
				else {
					CFStringAppend(truncatedOSVersion, CFSTR("0"));
				}
			}
			CFSafeRelease(osVersionComponents);
			CFSafeRelease(os_version);
			os_version = truncatedOSVersion;
			
			CFStringRef sdk_path = /*CFSTR("/Users/sam");*/ SDMMD_CopyDeviceSupportPathFromXCRUN();
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
			CFSafeRelease(signature_data);
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

sdmmd_return_t SDMMD_AMDeviceMountImage(SDMMD_AMDeviceRef device, CFStringRef path, CFDictionaryRef dict, CallBack handle, void* unknown) {
	sdmmd_return_t result = kAMDMissingOptionsError;
	if (dict) {
		SDMMD_AMConnectionRef connection = NULL;
		CFTypeRef digest = NULL;
		CFTypeRef imageType = CFDictionaryGetValue(dict, CFSTR("ImageType"));
		if (imageType) {
			CFTypeRef signature = CFDictionaryGetValue(dict, CFSTR("ImageSignature"));
			char *cpath = calloc(1, sizeof(char[1024]));
			Boolean pathCopy = CFStringGetCString(path, cpath, 1024, kCFStringEncodingUTF8);
			if (pathCopy) {
				unsigned char *sumdigest = calloc(1, sizeof(unsigned char[32]));
				result = SDMMD_AMDeviceDigestFile(cpath, 0, PtrCast(&sumdigest, unsigned char**));
				if (result) {
					SDMMD_AMDeviceRef deviceCopy = SDMMD_AMDeviceCreateCopy(device);
					result = kAMDUndefinedError;
					if (deviceCopy) {
						result = SDMMD_AMDeviceSecureStartSessionedService(device, CFSTR(AMSVC_MOBILE_IMAGE_MOUNT), &connection);
						if (result == 0) {
							SDMMD_fire_callback(handle, (int)unknown, CFSTR("LookingUpImage"));
							if (connection) {
								CFMutableDictionaryRef commandDict = SDMMD_create_dict();
								if (commandDict) {
									CFDictionarySetValue(commandDict, CFSTR("Command"), CFSTR("LookupImage"));
									CFDictionarySetValue(commandDict, CFSTR("ImageType"), imageType);
									result = SDMMD_ServiceSendMessage(SDMMD_TranslateConnectionToSocket(connection), commandDict, kCFPropertyListXMLFormat_v1_0);
									if (result == 0) {
										CFDictionaryRef response;
										result = SDMMD_ServiceReceiveMessage(SDMMD_TranslateConnectionToSocket(connection), (CFPropertyListRef*)&response);
										if (result == 0) {
											if (response) {
												CFTypeRef error = CFDictionaryGetValue(response, CFSTR("Error"));
												if (error) {
													CFShow(response);
													// convert error
													result = SDMMD__ConvertServiceError(error);
												}
												else {
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
											}
											else {
												result = kAMDReadError;
											}
										}
									}
								}
								else {
									result = kAMDNoResourcesError;
								}
							}
							else {
								result = kAMDInvalidArgumentError;
							}
							if (!digest) {
								bool supported = SDMMD_device_os_is_at_least(device, CFSTR("3.0"));
								bool mounted = false;
								if (supported && result != kAMDSuccess) {
									SDMMD_fire_callback(handle, (int)unknown, CFSTR("CopyingImage"));
									result = SDMMD_copy_image(device, path);
									if (result != kAMDSuccess) {
										SDMMD_fire_callback(handle, (int)unknown, CFSTR("StreamingImage"));
										char fspath[0x400] = {0};
										Boolean fsRep = CFStringGetFileSystemRepresentation(path, fspath, 0x400);
										if (fsRep) {
											struct stat fileStat;
											lstat(fspath, &fileStat);
											CFNumberRef size = CFNumberCreate(kCFAllocatorDefault, 0xb, &fileStat.st_size);
											CFMutableDictionaryRef streamDict = SDMMD_create_dict();
											CFDictionarySetValue(streamDict, CFSTR("Command"), CFSTR("ReceiveBytes"));
											CFDictionarySetValue(streamDict, CFSTR("ImageType"), imageType);
											CFDictionarySetValue(streamDict, CFSTR("ImageSize"), size);
											result = SDMMD_ServiceSendMessage(SDMMD_TranslateConnectionToSocket(connection), streamDict, kCFPropertyListXMLFormat_v1_0);
											if (result == 0) {
												CFDictionaryRef response;
												result = SDMMD_ServiceReceiveMessage(SDMMD_TranslateConnectionToSocket(connection), (CFPropertyListRef*)&response);
												if (result == 0) {
													if (response) {
														CFTypeRef error = CFDictionaryGetValue(response, CFSTR("Error"));
														if (error) {
															// convert error
															result = SDMMD_ImageMounterErrorConvert(error);
														}
														else {
															CFTypeRef status = CFDictionaryGetValue(response, CFSTR("Status"));
															if (status) {
																if (CFStringCompare(status, CFSTR("ReceiveBytesAck"), 0) == 0) {
																	// block code
																	CFShow(response);
#if 0
																	 uint32_t (^block)(uint32_t, uint32_t, CFTypeRef) = ^(uint32_t size, uint32_t code, CFTypeRef thing){return code;};
																	 result = SDMMD_read_file(fspath, 0x10000, block);
																	 if (result == 0) {
																	 	CFDictionaryRef getStatus;
																	 	result = SDMMD_ServiceReceiveMessage(SDMMD_TranslateConnectionToSocket(connection), (CFPropertyListRef*)&getStatus);
																	 	if (result == 0) {
																	 		CFTypeRef error = CFDictionaryGetValue(getStatus, CFSTR("Error"));
																	 		if (error) {
																	 			// convert error
																				result = SDMMD_ImageMounterErrorConvert(error);
																	 		}
																			else {
																	 			CFTypeRef streamStatus = CFDictionaryGetValue(getStatus, CFSTR("Status"));
																				if (streamStatus) {
																					if (CFStringCompare(streamStatus, CFSTR("Complete"), 0x0) == 0) {
																	 					result = 0x0;
																					}
																				}
																	 		}
																	 	}
																	 }
#endif
																}
															}
															else {
																result = kAMDUndefinedError;
															}
														}
													}
													else {
														result = kAMDReadError;
													}
												}
											}
											CFSafeRelease(size);
										}
										else {
											result = kAMDNoResourcesError;
										}
									}
									if (!mounted) {
										SDMMD_fire_callback(handle, (int)unknown, CFSTR("MountingImage"));
										CFMutableDictionaryRef mountDict = SDMMD_create_dict();
										if (mountDict) {
											CFDictionarySetValue(mountDict, CFSTR("Command"), CFSTR("MountImage"));
											CFDictionarySetValue(mountDict, CFSTR("ImageType"), imageType);
											CFDictionarySetValue(mountDict, CFSTR("ImagePath"), CFSTR("/var/mobile/Media/PublicStaging/staging.dimage"));
											if (signature) {
												CFDictionarySetValue(mountDict, CFSTR("ImageSignature"), signature);
											}
											result = SDMMD_ServiceSendMessage(SDMMD_TranslateConnectionToSocket(connection), mountDict, kCFPropertyListXMLFormat_v1_0);
											if (result == 0) {
												CFDictionaryRef response;
												result = SDMMD_ServiceReceiveMessage(SDMMD_TranslateConnectionToSocket(connection), (CFPropertyListRef*)&response);
												if (result == 0) {
													if (response) {
														CFTypeRef error = CFDictionaryGetValue(response, CFSTR("Error"));
														if (error) {
															// convert error
															result = SDMMD_ImageMounterErrorConvert(error);
														}
														else {
															CFTypeRef status = CFDictionaryGetValue(response, CFSTR("Status"));
															if (status) {
																if (CFEqual(status, CFSTR("Complete"))) {
																	mounted = true;
																}
																else {
																	result = kAMDMissingDigestError;
																}
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
											}
										}
									}
								}
								if (mounted) {
									result = SDMMD__hangup_with_image_mounter_service(connection);
									// insert error code
								}
							}
						}
					}
					CFSafeRelease(deviceCopy);
				}
				else {
					printf("%s: Could not digest %s\n",__FUNCTION__,cpath);
					result = kAMDDigestFailedError;
				}
				Safe(free, sumdigest);
			}
			else {
				result = kAMDInvalidArgumentError;
			}
			Safe(free, cpath);
		}
		else {
			result = kAMDMissingImageTypeError;
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
	memcpy(buffer->data, commandPrefix, 1);
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
	CFMutableDataRef receivedData = CFDataCreateMutable(kCFAllocatorDefault, 1);
	sdmmd_return_t result = SDMMD_DirectServiceReceive(connection, PtrCast(&receivedData, CFDataRef*));
	char *buffer = calloc(1, S(char));
	memcpy(buffer, CFDataGetBytePtr(receivedData), 1);
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
			memcpy(responseBuffer->data, commandPrefix, 1);
		}
	}
	if (shouldReceive && !skipPrefix) {
		shouldReceive = SDMMD_DebuggingReceiveInternalCheck(debuggingSocket, commandPrefix);
		if (shouldReceive) {
			memcpy(responseBuffer->data, commandPrefix, 1);
		}
	}
	if (shouldReceive) {
		uint32_t checksumLength = kChecksumHashLength;
		bool receivingChecksumResponse = false;
		while ((checksumLength > 0)) {
			uint64_t oldSize = IncrementBufferRefBySize(responseBuffer, 1);
			char *data = "#";
			receivingChecksumResponse = SDMMD_DebuggingReceiveInternalCheck(debuggingSocket, data);
			if (receivingChecksumResponse) {
				checksumLength--;
			}
			memcpy(&(responseBuffer->data[oldSize]), data, 1);
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