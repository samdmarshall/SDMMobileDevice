//
//  SDMMD_MobileBackup2.c
//  SDMMobileDevice-Framework
//
//  Created by Sam Marshall on 5/2/14.
//
//

#ifndef SDMMobileDevice_Framework_SDMMD_MobileBackup2_c
#define SDMMobileDevice_Framework_SDMMD_MobileBackup2_c

#include "SDMMD_MobileBackup2.h"
#include "SDMMD_Service.h"

sdmmd_return_t SDMMD_MB2SendFileStream(SDMMD_AMConnectionRef conn, CFStringRef path, CFDataRef file) {
	sdmmd_return_t result = kAMDSuccess;
	
	CFDataRef file_path = CFStringCreateExternalRepresentation(kCFAllocatorDefault, path, kCFStringEncodingUTF8, 0);
	result = SDMMD_ServiceSend(SDMMD_TranslateConnectionToSocket(conn), file_path);
	CFSafeRelease(file_path);
	CheckErrorAndReturn(result);
	
	char data_chunk[1] = { 0x0C };
	CFMutableDataRef data_separator = CFDataCreateMutable(kCFAllocatorDefault, 0);
	CFDataAppendBytes(data_separator, (const UInt8 *)data_chunk, sizeof(uint8_t));
	CFDataAppendBytes(data_separator, CFDataGetBytePtr(file), CFDataGetLength(file));
	result = SDMMD_ServiceSend(SDMMD_TranslateConnectionToSocket(conn), data_separator);
	CFSafeRelease(data_separator);
	CheckErrorAndReturn(result);
	
	char zero_chunk[1] = { 0x0 };
	CFDataRef data_end = CFDataCreate(kCFAllocatorDefault, (const UInt8 *)zero_chunk, sizeof(uint8_t));
	result = SDMMD_ServiceSend(SDMMD_TranslateConnectionToSocket(conn), data_end);
	CFSafeRelease(data_end);
	CheckErrorAndReturn(result);
	
	ExitLabelAndReturn(result);
}

CFMutableArrayRef SDMMD_MB2StatusResponseMessage() {
	CFMutableArrayRef status_response = CFArrayCreateMutable(kCFAllocatorDefault, 0, &kCFTypeArrayCallBacks);
	CFArrayAppendValue(status_response, CFSTR("DLMessageStatusResponse"));
	uint64_t code_value = 0;
	CFNumberRef code = CFNumberCreate(kCFAllocatorDefault, kCFNumberSInt64Type, &code_value);
	CFArrayAppendValue(status_response, code);
	CFSafeRelease(code);
	CFArrayAppendValue(status_response, CFSTR("___EmptyParameterString___"));
	CFMutableDictionaryRef empty_dict = CFDictionaryCreateMutable(NULL, 0, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
	CFArrayAppendValue(status_response, empty_dict);

	return status_response;
}

sdmmd_return_t SDMMD_MB2SendEndStream(SDMMD_AMConnectionRef conn) {
	sdmmd_return_t result = kAMDSuccess;
	
	uint32_t zero_stream = 0x0;
	CFDataRef stream_end = CFDataCreate(kCFAllocatorDefault, (const UInt8 *)&zero_stream, sizeof(uint32_t));
	result = SDMMD_DirectServiceSend(SDMMD_TranslateConnectionToSocket(conn), stream_end);
	CFSafeRelease(stream_end);
	CheckErrorAndReturn(result);
	
	CFMutableArrayRef status_response = SDMMD_MB2StatusResponseMessage();
	result = SDMMD_ServiceSendMessage(SDMMD_TranslateConnectionToSocket(conn), status_response, kCFPropertyListBinaryFormat_v1_0);

	ExitLabelAndReturn(result);
}

sdmmd_return_t SDMMD_MB2SendFile(SDMMD_AMConnectionRef conn, CFStringRef path, CFDataRef file) {
	sdmmd_return_t result = kAMDSuccess;
	result = SDMMD_MB2SendFileStream(conn, path, file);
	CheckErrorAndReturn(result);
	result = SDMMD_MB2SendEndStream(conn);
	CheckErrorAndReturn(result);

	ExitLabelAndReturn(result);
}

sdmmd_return_t SDMMD_MB2SendFiles(SDMMD_AMConnectionRef conn, CFArrayRef paths, CFArrayRef files) {
	sdmmd_return_t result = kAMDSuccess;
	
	if (CFArrayGetCount(paths) == CFArrayGetCount(files)) {
		for (CFIndex index = 0; index < CFArrayGetCount(paths); index++) {
			CFStringRef path = CFArrayGetValueAtIndex(paths, index);
			CFDataRef file = CFArrayGetValueAtIndex(files, index);
			result = SDMMD_MB2SendFileStream(conn, path, file);
			CheckErrorAndReturn(result);
		}
		result = SDMMD_MB2SendEndStream(conn);
	}
	
	ExitLabelAndReturn(result);
}

#endif
