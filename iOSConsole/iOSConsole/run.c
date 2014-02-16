//
//  run.c
//  iOSConsole
//
//  Created by Sam Marshall on 1/4/14.
//  Copyright (c) 2014 Sam Marshall. All rights reserved.
//

#ifndef iOSConsole_run_c
#define iOSConsole_run_c

#include "run.h"
#include "apps.h"
#include "attach.h"
#include <CoreFoundation/CoreFoundation.h>
#include "SDMMobileDevice.h"


void RunAppOnDeviceWithIdentifier(char *udid, char* identifier, int argc, char ** argv) {
	SDMMD_AMDeviceRef device = FindDeviceFromUDID(udid);
	if (device) {
		sdmmd_return_t result = SDMMD_AMDeviceConnect(device);
		if (SDM_MD_CallSuccessful(result)) {
			result = SDMMD_AMDeviceStartSession(device);
			if (SDM_MD_CallSuccessful(result)) {
				CFDictionaryRef response;
				CFArrayRef lookupValues = SDMMD_ApplicationLookupDictionary();
				CFMutableDictionaryRef optionsDict = SDMMD_create_dict();
				CFDictionarySetValue(optionsDict, CFSTR("ReturnAttributes"), lookupValues);
				
				result = SDMMD_AMDeviceLookupApplications(device, optionsDict, &response);
				if (SDM_MD_CallSuccessful(result)) {
					CFStringRef bundleIdentifier = CFStringCreateWithCString(kCFAllocatorDefault, identifier, kCFStringEncodingUTF8);
					CFDictionaryRef details = NULL;
					if (CFDictionaryContainsKey(response, bundleIdentifier)) {
						details = CFDictionaryGetValue(response, bundleIdentifier);
					}
					CFSafeRelease(bundleIdentifier);
					if (details) {
						SDMMD_AMDeviceStopSession(device);
						SDMMD_AMDeviceDisconnect(device);
						SDMMD_AMDebugConnectionRef dconn = SDMMD_AMDebugConnectionCreateForDevice(device);
						sdmmd_return_t result = SDMMD_AMDebugConnectionStart(dconn);
						bool launchSuccess = false;
						if (SDM_MD_CallSuccessful(result)) {
							CFDataRef response;
							// setting max packet size
							CFMutableArrayRef maxPacketArgs = CFArrayCreateMutable(kCFAllocatorDefault, 0x0, &kCFTypeArrayCallBacks);
							CFArrayAppendValue(maxPacketArgs, CFSTR("1024"));
							DebuggerCommandRef maxPacket = SDMMD_CreateDebuggingCommand(kDebugQSetMaxPacketSize, NULL, maxPacketArgs);
							CFSafeRelease(maxPacketArgs);
							
							CFDataRef maxPacketResponse = NULL;
							result = SDMMD_DebuggingSend(dconn, maxPacket, &maxPacketResponse);
							CFSafeRelease(maxPacketResponse);
							SDMMD_DebuggingCommandRelease(maxPacket);
							result = SDMMD_DebuggingReceive(dconn, &response);
//							CFSafeRelease(response);
							
							// setting the working directory
							CFStringRef path = CFDictionaryGetValue(details, CFSTR("Path"));
							CFStringRef container = CFDictionaryGetValue(details, CFSTR("Container"));
							if (!container) {
								CFURLRef pathURL = CFURLCreateWithString(kCFAllocatorDefault, path, NULL);
								CFURLRef containerURL = CFURLCreateCopyDeletingLastPathComponent(kCFAllocatorDefault, pathURL);
								container = CFURLGetString(containerURL);
								CFSafeRelease(pathURL);
							}
							CFMutableArrayRef containerPathArgs = CFArrayCreateMutable(kCFAllocatorDefault, 0x0, &kCFTypeArrayCallBacks);
							CFArrayAppendValue(containerPathArgs, container);
							DebuggerCommandRef containerPath = SDMMD_CreateDebuggingCommand(kDebugQSetWorkingDir, NULL, containerPathArgs);
							CFSafeRelease(containerPathArgs);
							
							CFDataRef containerPathResponse = NULL;
							result = SDMMD_DebuggingSend(dconn, containerPath, &containerPathResponse);
							CFSafeRelease(containerPathResponse);
							SDMMD_DebuggingCommandRelease(containerPath);
							result = SDMMD_DebuggingReceive(dconn, &response);
//							CFSafeRelease(response);

							// setting launch args
							CFMutableStringRef commandFormat = CFStringCreateMutable(kCFAllocatorDefault, 0);
							CFStringRef encodedPath = SDMMD_CreateDoubleByteString(CFStringGetCStringPtr(path, kCFStringEncodingUTF8), CFStringGetLength(path));
							CFStringAppendFormat(commandFormat, NULL, CFSTR("A%d,0,%@"), (uint32_t)CFStringGetLength(path)*0x2, encodedPath);

							/* accumulate any additional arguments in commandFormat */
							for(int i = 0; i < argc; i++) {
								CFStringRef encodedArg = SDMMD_CreateDoubleByteString(argv[i], strlen(argv[i]));
								CFStringAppendFormat(commandFormat, NULL, CFSTR(",%d,%d,%@"),
													 (uint32_t)CFStringGetLength(encodedArg), i+1, encodedArg);
								CFSafeRelease(encodedArg);
							}
							CFMutableArrayRef setLaunchArgsArgs = CFArrayCreateMutable(kCFAllocatorDefault, 0x0, &kCFTypeArrayCallBacks);
							DebuggerCommandRef setLaunchArgs = SDMMD_CreateDebuggingCommand(kDebugCUSTOMCOMMAND, commandFormat, setLaunchArgsArgs);
							CFSafeRelease(setLaunchArgsArgs);
							CFSafeRelease(commandFormat);
							
							CFDataRef setLaunchArgsResponse = NULL;
							result = SDMMD_DebuggingSend(dconn, setLaunchArgs, &setLaunchArgsResponse);
							CFSafeRelease(setLaunchArgsResponse);
							SDMMD_DebuggingCommandRelease(setLaunchArgs);
							result = SDMMD_DebuggingReceive(dconn, &response);
							// setting thread to attach
							CFMutableArrayRef setThreadArgs = CFArrayCreateMutable(kCFAllocatorDefault, 0x0, &kCFTypeArrayCallBacks);
							CFArrayAppendValue(setThreadArgs, CFSTR(""));
							DebuggerCommandRef setThread = SDMMD_CreateDebuggingCommand(kDebugCUSTOMCOMMAND, CFSTR("Hc0"), setThreadArgs);
							CFSafeRelease(setThreadArgs);
							
							CFDataRef setThreadResponse = NULL;
							result = SDMMD_DebuggingSend(dconn, setThread, &setThreadResponse);
							CFSafeRelease(setThreadResponse);
							SDMMD_DebuggingCommandRelease(setThread);
							result = SDMMD_DebuggingReceive(dconn, &response);

							// setting continue with execution
							CFMutableArrayRef contArgs = CFArrayCreateMutable(kCFAllocatorDefault, 0x0, &kCFTypeArrayCallBacks);
							CFArrayAppendValue(contArgs, CFSTR(""));
							DebuggerCommandRef cont = SDMMD_CreateDebuggingCommand(kDebugc, NULL, contArgs);
							CFSafeRelease(contArgs);
							
							CFDataRef contResponse = NULL;
							result = SDMMD_DebuggingSend(dconn, cont, &contResponse);
							CFSafeRelease(contResponse);
							SDMMD_DebuggingCommandRelease(cont);

							launchSuccess = true;

						}
						if (launchSuccess) {
							CFMutableArrayRef contArgs = CFArrayCreateMutable(kCFAllocatorDefault, 0x0, &kCFTypeArrayCallBacks);
							CFArrayAppendValue(contArgs, CFSTR("13"));
							DebuggerCommandRef cont = SDMMD_CreateDebuggingCommand(kDebugC, NULL, contArgs);
							CFSafeRelease(contArgs);
							while(true) {
								usleep(500);
								CFDataRef contResponse = NULL;
								CFDataRef response = NULL;
								result = SDMMD_DebuggingSend(dconn, cont, &contResponse);
								CFSafeRelease(contResponse);
								result = SDMMD_DebuggingReceive(dconn, &response);
								HandleResult((sdmmd_debug_return_t){.result = result, .data = response});
								/* W Packet signals Exited. */
								if (response && *CFDataGetBytePtr(response) == 'W') {
									break;
								}
							}
						}
					}
				}
			}
		}
	}
}

#endif