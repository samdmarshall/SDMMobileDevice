//
//  main.c
//  iOSConsole
//
//  Created by Sam Marshall on 1/1/14.
//  Copyright (c) 2014 Sam Marshall. All rights reserved.
//

#include <CoreFoundation/CoreFoundation.h>
#include <notify.h>

#include "SDMMobileDevice.h"
#include "Core.h"

static char *kDebugType = "<Debug>";
static char *kNoticeType = "<Notice>";
static char *kErrorType = "<Error>";

static char *listArg = "--list";
static char *attachArg = "--attach";

static char *updateLogNotifyName = "notify.com.samdmarshall.mobile.syslog.updatelog";

#define SysLogBufferSize 0x80

static CFMutableDataRef syslogBuffer;
static uint32_t bufferCounter = 0x1;
static UInt8 newlineBytes[0x3] = {0xa, 0x0, 0x0};

static dispatch_queue_t operatingQueue;
static dispatch_queue_t updatelogQueue;

void ListConnectedDevices() {
	CFArrayRef devices = SDMMD_AMDCreateDeviceList();
	CFIndex numberOfDevices = CFArrayGetCount(devices);
	
	if (numberOfDevices) {
		// return type (uint32_t) corresponds with known return codes (SDMMD_Error.h)
		sdmmd_return_t result;
		
		uint32_t index;
		// Iterating over connected devices
		for (index = 0; index < numberOfDevices; index++) {
			
			// getting the device object from the array of connected devices
			SDMMD_AMDeviceRef device = (SDMMD_AMDeviceRef)CFArrayGetValueAtIndex(devices, index);
			
			// attempting to connect to the device
			result = SDMMD_AMDeviceConnect(device);
			if (SDM_MD_CallSuccessful(result)) {
				CFTypeRef deviceName = SDMMD_AMDeviceCopyValue(device, NULL, CFSTR(kDeviceName));
				CFTypeRef deviceUDID = SDMMD_AMDeviceCopyValue(device, NULL, CFSTR(kUniqueDeviceID));
				printf("%i) %s : %s\n",index+0x1,CFStringGetCStringPtr(deviceName,kCFStringEncodingMacRoman),CFStringGetCStringPtr(deviceUDID,kCFStringEncodingMacRoman));
				CFRelease(deviceName);
				CFRelease(deviceUDID);
				SDMMD_AMDeviceDisconnect(device);
			}
		}
	}
	CFRelease(devices);
}

void AttachToDevice(char *udid) {
	dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0x0), ^{
		dispatch_sync(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0x0), ^{
			CFArrayRef devices = SDMMD_AMDCreateDeviceList();
			CFIndex numberOfDevices = CFArrayGetCount(devices);
			
			if (numberOfDevices) {
				// return type (uint32_t) corresponds with known return codes (SDMMD_Error.h)
				sdmmd_return_t result;
				bool foundDevice = false;
				char *deviceId;
				uint32_t index;
				// Iterating over connected devices
				for (index = 0; index < numberOfDevices; index++) {
					
					// getting the device object from the array of connected devices
					SDMMD_AMDeviceRef device = (SDMMD_AMDeviceRef)CFArrayGetValueAtIndex(devices, index);
					
					// attempting to connect to the device
					result = SDMMD_AMDeviceConnect(device);
					if (SDM_MD_CallSuccessful(result)) {
						CFTypeRef deviceUDID = SDMMD_AMDeviceCopyValue(device, NULL, CFSTR(kUniqueDeviceID));
						deviceId = (char*)CFStringGetCStringPtr(deviceUDID,kCFStringEncodingMacRoman);
						CFRelease(deviceUDID);
						if (strncmp(udid, deviceId, strlen(deviceId)) == 0x0) {
							foundDevice = true;
							break;
						}
						SDMMD_AMDeviceDisconnect(device);
					}
				}
				if (foundDevice) {
					SDMMD_AMConnectionRef syslog;
					SDMMD_AMDeviceRef device = (SDMMD_AMDeviceRef)CFArrayGetValueAtIndex(devices, index);
					result = SDMMD_AMDeviceStartSession(device);
					if (SDM_MD_CallSuccessful(result)) {
						result = SDMMD_AMDeviceStartService(device, CFSTR(AMSVC_SYSLOG_RELAY), NULL, &syslog);
						if (SDM_MD_CallSuccessful(result)) {
							CFTypeRef deviceName = SDMMD_AMDeviceCopyValue(device, NULL, CFSTR(kDeviceName));
							char *name = (char*)CFStringGetCStringPtr(deviceName,kCFStringEncodingMacRoman);
							if (!name) {
								name = "unnamed device";
							}
							printf("Connected to %s, loading syslog...\n",name);
							CFRelease(deviceName);
						}
						syslogBuffer = CFDataCreateMutable(kCFAllocatorDefault, 0x0);
						// header to syslog
						unsigned char syslogHeaderBuffer[SysLogBufferSize*0x3];
						CFDataRef syslogHeader = CFDataCreate(kCFAllocatorDefault, syslogHeaderBuffer, SysLogBufferSize*0x3);
						result = SDMMD_DirectServiceReceive(SDMMD_TranslateConnectionToSocket(syslog), (CFDataRef*)&syslogHeader);
						while (SDM_MD_CallSuccessful(result)) {
							if (SDMMD_AMDeviceIsValid(device)) {
								unsigned char syslogRelayBuffer[SysLogBufferSize];
								CFDataRef syslogData = CFDataCreate(kCFAllocatorDefault, syslogRelayBuffer, SysLogBufferSize);
								result = SDMMD_DirectServiceReceive(SDMMD_TranslateConnectionToSocket(syslog), (CFDataRef*)&syslogData);
								dispatch_sync(operatingQueue, ^{
									CFDataAppendBytes(syslogBuffer, CFDataGetBytePtr(syslogData), SysLogBufferSize);
									notify_post(updateLogNotifyName);
								});
								CFRelease(syslogData);
							} else {
								break;
							}
						}
						printf("\n\nLost Connection with Device\n");
					}
				} else {
					printf("Could not find device with that UDID\n");
				}
			}
		});
	});
}

void PrintSysLog() {
	int token;
	CFURLRef filePath = CFURLCreateWithFileSystemPath(kCFAllocatorDefault, CFSTR("/Users/sam/Desktop/sys.log"),kCFURLPOSIXPathStyle,false);
	__block CFWriteStreamRef syslogFile = CFWriteStreamCreateWithFile(kCFAllocatorDefault,filePath);
	Boolean streamStatus = CFWriteStreamOpen(syslogFile);
	if (streamStatus) {
		uint32_t status = notify_register_dispatch(updateLogNotifyName, &token, updatelogQueue, ^(int token){
			__block CFDataRef logData;
			dispatch_sync(operatingQueue, ^{
				CFDataRef newlineData = CFDataCreate(kCFAllocatorDefault, (UInt8*)&newlineBytes, 0x3);
				logData = CFDataCreateCopy(kCFAllocatorDefault, syslogBuffer);
				CFRange lineRange;
				Boolean foundRange = false;
				__block CFIndex startingIndex = 0x0;
				__block CFIndex indexLength = 0x0;
				while (!foundRange) {
					lineRange = CFDataFind(logData, newlineData, CFRangeMake(startingIndex, SysLogBufferSize*bufferCounter), kCFDataSearchBackwards);
					if (lineRange.location != 0) {
						foundRange = true;
					} else {
						startingIndex += lineRange.length;
					}
				}
				if (lineRange.location == kCFNotFound) {
					bufferCounter++;
				} else {
					UInt8 *lineBuffer = calloc(0x1, 0x1000); // 4096 is the max line length for syslog
					CFDataGetBytes(logData, CFRangeMake(startingIndex+0x3, lineRange.location), lineBuffer);
					// lineBuffer now contains the whole line.
					dispatch_sync(dispatch_get_main_queue(), ^{
						char month[0x4], process[0x80], type[0x80], message[0x1000], pidstr[0x80],name[0x100];
						int day, hour, minute, second, pid, count;
						bool hasName = (lineBuffer[0x10] == ' ' ? false : true);
						if (hasName) {
							count = sscanf((char*)lineBuffer, "%3s %2i %2i:%2i:%2i %s %[^\[][%[^]]] %[^:]: %4096[^\0]s",month,&day,&hour,&minute,&second,name,process,pidstr,type,message);
						} else {
							count = sscanf((char*)lineBuffer, "%3s %2i %2i:%2i:%2i  %[^\[][%[^]]] %[^:]: %4096[^\0]s",month,&day,&hour,&minute,&second,process,pidstr,type,message);
						}
						pid = atoi(pidstr);
						if (count == 0x9 + hasName) {
							LogArg(COLOR_NRM,"%3s %2i %02i:%02i:%02i",month,day,hour,minute,second);
							LogArg(COLOR_NRM," ");
							LogArg(COLOR_GRN,"%s",process);
							LogArg(COLOR_NRM," ");
							LogArg(COLOR_NRM,"[");
							LogArg(COLOR_BLU,"%i",pid);
							LogArg(COLOR_NRM,"]");
							LogArg(COLOR_NRM," ");
							if (strncmp(type, kDebugType, strlen(kDebugType)) == 0x0) {
								LogArg(COLOR_MAG,"%s",type);
							} else if (strncmp(type, kNoticeType, strlen(kNoticeType)) == 0x0) {
								LogArg(COLOR_CYN,"%s",type);
							} else if (strncmp(type, kErrorType, strlen(kErrorType)) == 0x0) {
								LogArg(COLOR_RED,"%s",type);
							} else {
								LogArg(COLOR_YEL,"%s",type);
							}
							LogArg(COLOR_NRM,": ");
#pragma clang push // SDM: we should probably remove this...
#pragma clang diagnostic ignored "-Wformat-security"
							LogArg(COLOR_NRM,message); // SDM: PFFFFTTT, we know what we are doing...
#pragma clang pop
							fflush(NULL);
							indexLength = lineRange.location;
						} else if (count == 0x1) {
							startingIndex = 0x0;
							indexLength = 0x36;
						} else if (count == kCFNotFound) {
							startingIndex = 0x0;
							indexLength = 0x1;
						}
					});
					CFWriteStreamWrite(syslogFile, lineBuffer, indexLength);
					CFDataDeleteBytes(syslogBuffer, CFRangeMake(startingIndex, indexLength));
					bufferCounter = 0x1;
				}
			});
			CFRelease(logData);
		});
		if (status == KERN_SUCCESS) {
			CFRunLoopRun();
			CFWriteStreamClose(syslogFile);
		} else {
			printf("Failed to register callback to parse syslog.");
		}
	} else {
		printf("Failed to open syslog file to write to.");
	}
}

int main(int argc, const char * argv[]) {
	operatingQueue = dispatch_queue_create("com.samdmarshall.mobile.syslog.operations", DISPATCH_QUEUE_SERIAL);
	updatelogQueue = dispatch_queue_create("com.samdmarshall.mobile.syslog.updatelog", DISPATCH_QUEUE_SERIAL);
	SDMMobileDevice;
	bool attachDevice = false;
	bool listDevice = false;
	switch (argc) {
		case 0x1: {
			printf("%s : list attached devices\n",listArg);
			printf("%s [UDID] : attach to the syslog of requested device\n",attachArg);
			break;
		}
		case 0x2:
		case 0x3: {
			if (strncmp(argv[0x1], listArg, strlen(listArg)) == 0x0) {
				listDevice = true;
			}
			if (strncmp(argv[0x1], attachArg, strlen(attachArg)) == 0x0 && argc == 0x3) {
				attachDevice = true;
			}
			break;
		}
		default:
			break;
	}
	if (listDevice) {
		ListConnectedDevices();
	}
	if (attachDevice) {
		AttachToDevice((char*)argv[0x2]);
		PrintSysLog();
	}
	printf(" exiting...\n");
    return 0;
}

