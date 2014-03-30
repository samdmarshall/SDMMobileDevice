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

void RunAppOnDeviceWithIdentifier(char *udid, char* identifier) {
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
							
							// setting max packet size
							CFMutableArrayRef maxPacketArgs = CFArrayCreateMutable(kCFAllocatorDefault, 0x0, &kCFTypeArrayCallBacks);
							CFArrayAppendValue(maxPacketArgs, CFSTR("1024"));
							DebuggerCommandRef maxPacket = SDMMD_CreateDebuggingCommand(kDebugQSetMaxPacketSize, NULL, maxPacketArgs);
							CFSafeRelease(maxPacketArgs);
							
							CFDataRef maxPacketResponse = NULL;
							result = SDMMD_DebuggingSend(dconn, maxPacket, &maxPacketResponse);
							CFSafeRelease(maxPacketResponse);
							SDMMD_DebuggingCommandRelease(maxPacket);
							
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
							
							// setting launch args
							CFStringRef commandFormat = CFStringCreateWithFormat(kCFAllocatorDefault, NULL, CFSTR("A%d,0,"), (uint32_t)CFStringGetLength(path)*0x2);

							CFMutableArrayRef setLaunchArgsArgs = CFArrayCreateMutable(kCFAllocatorDefault, 0x0, &kCFTypeArrayCallBacks);
							CFArrayAppendValue(setLaunchArgsArgs, path);
							DebuggerCommandRef setLaunchArgs = SDMMD_CreateDebuggingCommand(kDebugCUSTOMCOMMAND, commandFormat, setLaunchArgsArgs);
							CFSafeRelease(setLaunchArgsArgs);
							CFSafeRelease(commandFormat);
							
							CFDataRef setLaunchArgsResponse = NULL;
							result = SDMMD_DebuggingSend(dconn, setLaunchArgs, &setLaunchArgsResponse);
							CFSafeRelease(setLaunchArgsResponse);
							SDMMD_DebuggingCommandRelease(setLaunchArgs);
							
							// setting thread to attach
							CFMutableArrayRef setThreadArgs = CFArrayCreateMutable(kCFAllocatorDefault, 0x0, &kCFTypeArrayCallBacks);
							CFArrayAppendValue(setThreadArgs, CFSTR(""));
							DebuggerCommandRef setThread = SDMMD_CreateDebuggingCommand(kDebugCUSTOMCOMMAND, CFSTR("Hc0"), setThreadArgs);
							CFSafeRelease(setThreadArgs);
							
							CFDataRef setThreadResponse = NULL;
							result = SDMMD_DebuggingSend(dconn, setThread, &setThreadResponse);
							CFSafeRelease(setThreadResponse);
							SDMMD_DebuggingCommandRelease(setThread);
							
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
							CFRunLoopRun();
						}
						/*
						sdmmd_return_t result = SDMMD_StartDebuggingSessionOnDevice(device, &connection);
						if (SDM_MD_CallSuccessful(result)) {
							bool launchSuccess = false;
							CFStringRef path = CFDictionaryGetValue(details, CFSTR("Path"));
							CFStringRef encodedPath = SDMMD_EncodeDebuggingString(path);
							CFStringRef container = CFDictionaryGetValue(details, CFSTR("Container"));
							if (!container) {
								CFURLRef pathURL = CFURLCreateWithString(kCFAllocatorDefault, path, NULL);
								CFURLRef containerURL = CFURLCreateCopyDeletingLastPathComponent(kCFAllocatorDefault, pathURL);
								container = CFURLGetString(containerURL);
								CFSafeRelease(pathURL);
								//CFSafeRelease(containerURL);
							}
							if (container) {
								CFStringRef containerPath = SDMMD_EncodeDebuggingString(container);
								sdmmd_debug_return_t dresult;
								CFStringRef maxPacket = SDMMD_EncodeDebuggingString(CFSTR("1024"));
								dresult = SDMMD_DebuggingSend(connection, KnownDebugCommands[kDebugQSetMaxPacketSize], maxPacket);
								CFSafeRelease(maxPacket);
								dresult = SDMMD_DebuggingSend(connection, KnownDebugCommands[kDebugQSetWorkingDir], containerPath);
								CFSafeRelease(containerPath);
								CFStringRef commandFormat = CFStringCreateWithFormat(kCFAllocatorDefault, NULL, CFSTR("%d,0,%s"), (uint32_t)CFStringGetLength(encodedPath), CFStringGetCStringPtr(encodedPath, kCFStringEncodingUTF8));
								dresult = SDMMD_DebuggingSend(connection, KnownDebugCommands[kDebugA], commandFormat);
								CFSafeRelease(commandFormat);
								dresult = SDMMD_DebuggingSend(connection, KnownDebugCommands[kDebugH], CFSTR("c0"));
								dresult = SDMMD_DebuggingSend(connection, KnownDebugCommands[kDebugc], CFSTR(""));
								launchSuccess = true;
							}
							CFSafeRelease(encodedPath);
							if (launchSuccess) {
								CFRunLoopRun();
							}
						}
						*/
					}
				}
			}
		}
	}
}

#endif