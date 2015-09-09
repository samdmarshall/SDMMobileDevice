//
//  SDMMD_FileRelay.c
//  SDMMobileDevice-Framework
//
//  Created by Samantha Marshall on 8/22/14.
//
//

#ifndef SDMMobileDevice_Framework_SDMMD_FileRelay_c
#define SDMMobileDevice_Framework_SDMMD_FileRelay_c

#include "SDMMD_FileRelay.h"
#include "SDMMD_Service.h"
#include "SDMMD_Connection.h"
#include "SDMMD_Service.h"
#include "SDMMD_Connection_Internal.h"
#include "CFTypeAddition.h"

sdmmd_return_t SDMMD_AMDeviceRelayFile(SDMMD_AMDeviceRef device, CFStringRef source, CFDictionaryRef options)
{
	sdmmd_return_t result = kAMDInvalidArgumentError;
	SDMMD_AMConnectionRef connection = NULL;

	if (device == NULL || CFGetTypeID(source) != CFStringGetTypeID() || CFGetTypeID(options) != CFDictionaryGetTypeID()) {
		CheckErrorAndReturn(result);
	}

	CFStringRef type = CFDictionaryGetValue(options, CFSTR("RelayType"));
	CFTypeRef location = CFDictionaryGetValue(options, CFSTR("RelayLocation"));

	if (type == NULL) {
		CheckErrorAndReturn(result);
	}
	else {
		bool correct_format = false;

		if (CFStringCompare(type, CFSTR("RelayTypeFileDescriptor"), 0) == 0 && CFGetTypeID(location) == CFNumberGetTypeID()) {
			correct_format = true;
		}

		if (CFStringCompare(type, CFSTR("RelayTypeData"), 0) == 0 && CFGetTypeID(location) == CFDataGetTypeID()) {
			correct_format = true;
		}

		if (correct_format) {
			CFArrayRef source_list = CFArrayCreate(kCFAllocatorDefault, (const void **)&source, 1, &kCFTypeArrayCallBacks);
			CFStringRef key[] = {CFSTR("Sources")};
			CFArrayRef value[] = {source_list};
			CFDictionaryRef message = CFDictionaryCreate(kCFAllocatorDefault, (const void **)&key, (const void **)&value, 1, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);

			result = SDMMD_AMDeviceConnect(device);
			CheckErrorAndReturn(result);

			result = SDMMD_AMDeviceStartSession(device);
			CheckErrorAndReturn(result);

			result = SDMMD_AMDeviceSecureStartService(device, CFSTR(AMSVC_FILE_RELAY), NULL, &connection);
			CheckErrorAndReturn(result);

			SocketConnection socket = SDMMD_TranslateConnectionToSocket(connection);
			result = SDMMD_ServiceSendMessage(socket, message, kCFPropertyListXMLFormat_v1_0);
			CheckErrorAndReturn(result);

			CFPropertyListRef response = NULL;
			result = SDMMD_ServiceReceiveMessage(socket, &response);
			CheckErrorAndReturn(result);

			result = SDMMD__ErrorHandler(SDMMD__ConvertServiceError, response);
			CheckErrorAndReturn(result);

			// SDM - for whatever reason it seems the current code only has it copy over 1MB of data and then writes that locally.
			//		This doesn't seem to be used directly by anything in the library.

			CFSafeRelease(connection);

			result = SDMMD_AMDeviceStopSession(device);
			CheckErrorAndReturn(result);

			result = SDMMD_AMDeviceDisconnect(device);
			CheckErrorAndReturn(result);
		}
	}

	ExitLabelAndReturn(result);
}

#endif
