//
//  syslog.c
//  iOSConsole
//
//  Created by Sam Marshall on 1/4/14.
//  Copyright (c) 2014 Sam Marshall. All rights reserved.
//

#ifndef iOSConsole_syslog_c
#define iOSConsole_syslog_c

#include "syslog.h"
#include "attach.h"

#include <CoreFoundation/CoreFoundation.h>
#include <notify.h>

#include "SDMMobileDevice.h"
#include "Core.h"

static char *kDebugType = "<Debug>";
static char *kWarningType = "<Warning>";
static char *kErrorType = "<Error>";

static char *updateLogNotifyName = "notify.com.samdmarshall.mobile.syslog.updatelog";
static char *exitNotifyName = "notify.com.samdmarshall.iOSConsole.exit";

#define SysLogHeaderSize 0x80
#define SysLogBufferSize 0x10

static CFMutableDataRef syslogBuffer;

#define kNewlineBytesiOS6Up 0x3
#define kNewlineBytesiOS5 0x2
static uint8_t newlineBytesLength = 0x0;
static UInt8 newlineBytes[0x3] = {0xa, 0x0, 0x0};

static dispatch_queue_t operatingQueue;
static dispatch_queue_t updatelogQueue;

void Syslog(char *udid) {
	operatingQueue = dispatch_queue_create("com.samdmarshall.mobile.syslog.operations", DISPATCH_QUEUE_SERIAL);
	updatelogQueue = dispatch_queue_create("com.samdmarshall.mobile.syslog.updatelog", DISPATCH_QUEUE_SERIAL);
	AttachToSyslog(udid);
	PrintSysLog();
}

void EnableExtendedLogging(SDMMD_AMDeviceRef device) {
	sdmmd_return_t result = SDMMD_AMDeviceConnect(device);
	if (SDM_MD_CallSuccessful(result)) {
		result = SDMMD_AMDeviceStartSession(device);
		if (SDM_MD_CallSuccessful(result)) {
			CFTypeRef value = SDMMD_AMDeviceCopyValue(device, CFSTR(AMSVC_MOBILE_DEBUG), CFSTR(kEnableLockdownExtendedLogging));
			if (CFGetTypeID(value) == CFBooleanGetTypeID()) {
				if (!CFBooleanGetValue(value)) {
					result = SDMMD_AMDeviceSetValue(device, CFSTR(AMSVC_MOBILE_DEBUG), CFSTR(kEnableLockdownExtendedLogging), kCFBooleanTrue);
					if (SDM_MD_CallSuccessful(result)) {
						printf("Enabling extended logging...\n");
					}
				} else {
					printf("Extended logging already enabled.\n");
				}
			} else {
				PrintCFType(value);
			}
			CFSafeRelease(value);
		}
		SDMMD_AMDeviceStopSession(device);
		SDMMD_AMDeviceDisconnect(device);
	}
}

void AttachToSyslog(char *udid) {
	dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0x0), ^{
		SDMMD_AMDeviceRef device = FindDeviceFromUDID(udid);
		if (device) {
			//EnableExtendedLogging(device);
			SDMMD_AMConnectionRef syslog = AttachToDeviceAndService(device, AMSVC_SYSLOG_RELAY);
			newlineBytesLength = (SDMMD_device_os_is_at_least(device, CFSTR("6.0")) ? kNewlineBytesiOS6Up : kNewlineBytesiOS5);
			if (syslog) {
				printf("loading syslog...\n");
				sdmmd_return_t result;
				
				syslogBuffer = CFDataCreateMutable(kCFAllocatorDefault, 0x0);
				// header to syslog
				unsigned char syslogHeaderBuffer[SysLogHeaderSize*0x3];
				CFDataRef syslogHeader = CFDataCreate(kCFAllocatorDefault, syslogHeaderBuffer, SysLogHeaderSize*0x3);
				result = SDMMD_DirectServiceReceive(SDMMD_TranslateConnectionToSocket(syslog), (CFDataRef*)&syslogHeader);
				CFSafeRelease(syslogHeader);
				
				while (SDM_MD_CallSuccessful(result)) {
					if (SDMMD_AMDeviceIsValid(device)) {
						unsigned char syslogRelayBuffer[SysLogBufferSize];
						CFDataRef syslogData = CFDataCreate(kCFAllocatorDefault, syslogRelayBuffer, SysLogBufferSize);
						result = SDMMD_DirectServiceReceive(SDMMD_TranslateConnectionToSocket(syslog), (CFDataRef*)&syslogData);
						dispatch_sync(operatingQueue, ^{
							CFDataAppendBytes(syslogBuffer, CFDataGetBytePtr(syslogData), SysLogBufferSize);
							notify_post(updateLogNotifyName);
						});
						CFSafeRelease(syslogData);
					} else {
						break;
					}
				}
				dispatch_suspend(operatingQueue);
				notify_post(exitNotifyName);
			}
		}
	});
}

void PrintSysLog() {
	int token;
	uint32_t status[0x2] = {0x0};
	status[0x0] = notify_register_dispatch(updateLogNotifyName, &token, updatelogQueue, ^(int token){
		dispatch_sync(operatingQueue, ^{
			CFDataRef logData;
			Boolean foundLine = false;
			Boolean foundAll = false;
			CFIndex offset = 0x0, length = 0x0;
			while (!foundAll) {
				foundLine = false;
				length = CFDataGetLength(syslogBuffer);
				logData = CFDataCreateWithBytesNoCopy(kCFAllocatorDefault, CFDataGetBytePtr(syslogBuffer), length, kCFAllocatorDefault);
				CFDataRef newlineData = CFDataCreate(kCFAllocatorDefault, newlineBytes, newlineBytesLength);
				CFRange line = CFDataFind(logData, newlineData, CFRangeMake(offset, length), 0x0);
				if (line.location != kCFNotFound) {
					foundLine = true;
					length = line.location+line.length;
				} else {
					foundAll = true;
				}
				if (foundLine) {
					UInt8 *lineBuffer = calloc(0x1, 0x1000); // 4096 is the max line length for syslog
					CFDataGetBytes(logData, CFRangeMake(offset, length), lineBuffer);
					// lineBuffer now contains the whole line.
					char month[0x4], process[0x80], type[0x80], message[0x1000], pidstr[0x80],name[0x100];
					uint32_t day, hour, minute, second, pid, count;
					if (lineBuffer[0x10] == '=') {
						LogArg(COLOR_NRM,"%s\n",lineBuffer);
					} else {
						bool hasName = (lineBuffer[0x10] == ' ' ? false : true);
						if (hasName) {
							count = sscanf((char*)lineBuffer, "%3s %d %d:%d:%d %s %[^\[][%[^]]] %[^\(:]: %4096[^\0]s",month,&day,&hour,&minute,&second,name,process,pidstr,type,message);
						} else {
							count = sscanf((char*)lineBuffer, "%3s %d %d:%d:%d  %[^\[][%[^]]] %[^\(:]: %4096[^\0]s",month,&day,&hour,&minute,&second,process,pidstr,type,message);
						}
						pid = atoi(pidstr);
						if (count == 0x9 + hasName) {
							LogArg(COLOR_NRM,"%3s %2d %02d:%02d:%02d",month,day,hour,minute,second);
							LogArg(COLOR_NRM," ");
							LogArg(COLOR_GRN,"%s",process);
							LogArg(COLOR_NRM," ");
							LogArg(COLOR_NRM,"[");
							LogArg(COLOR_BLU,"%i",pid);
							LogArg(COLOR_NRM,"]");
							LogArg(COLOR_NRM," ");
							if (strncmp(type, kDebugType, strlen(kDebugType)) == 0x0) {
								LogArg(COLOR_MAG,"%s",type);
							} else if (strncmp(type, kWarningType, strlen(kWarningType)) == 0x0) {
								LogArg(COLOR_YEL,"%s",type);
							} else if (strncmp(type, kErrorType, strlen(kErrorType)) == 0x0) {
								LogArg(COLOR_RED,"%s",type);
							} else {
								LogArg(COLOR_CYN,"%s",type);
							}
							LogArg(COLOR_NRM,": ");
							LogArg(COLOR_NRM,"%s",message);
						}
					}
					CFDataDeleteBytes(syslogBuffer, CFRangeMake(offset, length));
					Safe(free, lineBuffer);
				}
				CFSafeRelease(newlineData);
			}
		});
	});
	status[0x1] = notify_register_dispatch(exitNotifyName, &token, dispatch_get_main_queue(), ^(int token){
		printf("\n\nLost Connection with Device\n");
		CFRunLoopStop(CFRunLoopGetMain());
	});
	if (status[0x0] == KERN_SUCCESS && status[0x1] == KERN_SUCCESS) {
		CFRunLoopRun();
	} else {
		printf("Failed to register callback to parse syslog.");
	}
}

#endif