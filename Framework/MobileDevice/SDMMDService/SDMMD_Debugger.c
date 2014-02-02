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

#define FDVENDOR_PATH  "/tmp/sdmmd-remote-debugserver"
#define PREP_CMDS_PATH "/tmp/sdmmd-gdb-prep-cmds"
#define GDB_SHELL      "/Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/usr/libexec/gdb/gdb-arm-apple-darwin --arch armv7 -q -x " PREP_CMDS_PATH

// approximation of what Xcode does:
#define GDB_PREP_CMDS CFSTR("set mi-show-protections off\n\
    set auto-raise-load-levels 1\n\
    set shlib-path-substitutions /usr \"{ds_path}/Symbols/usr\" /System \"{ds_path}/Symbols/System\" \"{device_container}\" \"{disk_container}\" \"/private{device_container}\" \"{disk_container}\" /Developer \"{ds_path}/Symbols/Developer\"\n\
    set remote max-packet-size 1024\n\
    set sharedlibrary check-uuids on\n\
    set env NSUnbufferedIO YES\n\
    set minimal-signal-handling 1\n\
    set sharedlibrary load-rules \\\".*\\\" \\\".*\\\" container\n\
    set inferior-auto-start-dyld 0\n\
    file \"{disk_app}\"\n\
    set remote executable-directory {device_app}\n\
    set remote noack-mode 1\n\
    set trust-readonly-sections 1\n\
    target remote-mobile " FDVENDOR_PATH "\n\
    mem 0x1000 0x3fffffff cache\n\
    mem 0x40000000 0xffffffff none\n\
    mem 0x00000000 0x0fff none\n\
    run {args}\n\
    set minimal-signal-handling 0\n\
    set inferior-auto-start-cfm off\n\
    set sharedLibrary load-rules dyld \".*libobjc.*\" all dyld \".*CoreFoundation.*\" all dyld \".*Foundation.*\" all dyld \".*libSystem.*\" all dyld \".*AppKit.*\" all dyld \".*PBGDBIntrospectionSupport.*\" all dyld \".*/usr/lib/dyld.*\" all dyld \".*CarbonDataFormatters.*\" all dyld \".*libauto.*\" all dyld \".*CFDataFormatters.*\" all dyld \"/System/Library/Frameworks\\\\\\\\|/System/Library/PrivateFrameworks\\\\\\\\|/usr/lib\" extern dyld \".*\" all exec \".*\" all\n\
    sharedlibrary apply-load-rules all\n\
    set inferior-auto-start-dyld 1")

static char *kHexEncode = "0123456789ABCDEF";
#define kHexDecode(byte) ((byte >= '0' && byte <= '9') ? (byte - '0') : ( (byte >= 'a' && byte <= 'f') ? (10 + byte - 'a') : ((byte >= 'A' && byte <= 'F') ? (10 + byte - 'A') : byte)))

uint32_t GenerateChecksumForData(char *strPtr, uint32_t length) {
	uint32_t checksum = 0;
	for (uint32_t i = 0; i < length; i++) {
		checksum += strPtr[i];
	}
	return checksum;
}

sdmmd_return_t SDMMD_StartDebuggingSessionOnDevice(SDMMD_AMDeviceRef device, SDMMD_AMDebugConnectionRef *connection) {
	sdmmd_return_t result = SDMMD_AMDeviceConnect(device);
	if (SDM_MD_CallSuccessful(result)) {
		result = SDMMD_AMDeviceStartSession(device);
		if (SDM_MD_CallSuccessful(result)) {
			CFDictionaryRef dict = NULL;
			*connection = SDMMD_AMDServiceConnectionCreate(0, NULL, dict);
			result = SDMMD_AMDeviceStartService(device, CFSTR(AMSVC_DEBUG_SERVER), NULL, connection);
		}
	}
	return result;
}

sdmmd_return_t SDMMD_StopDebuggingSessionOnDevice(SDMMD_AMDeviceRef device, SDMMD_AMDebugConnectionRef *connection) {
	sdmmd_return_t result = kAMDSuccess;
	if (SDM_MD_CallSuccessful(result)) {
		result = SDMMD_AMDeviceStopSession(device);
		if (SDM_MD_CallSuccessful(result)) {
			result = SDMMD_AMDeviceDisconnect(device);
		}
	}
	return result;
}

CFStringRef SDMMD_CreateEncodeForDebuggingCommand(CFStringRef command) {
	unsigned char *commandString = malloc(CFStringGetLength(command));
	CFIndex length = CFStringGetBytes(command, CFRangeMake(0,CFStringGetLength(command)), kCFStringEncodingUTF8, 0, true, commandString, CFStringGetLength(command), NULL);
	unsigned char *encodedCommand = malloc(sizeof(unsigned char));
	uint32_t pos = 0;
	for (uint32_t i = 0; i < length; i++) {
		pos = (i*(sizeof(char)*2));
		encodedCommand = realloc(encodedCommand, pos+(sizeof(unsigned char)*2));
		encodedCommand[pos] = kHexEncode[commandString[i] >> 4];
		encodedCommand[pos+1] = kHexEncode[commandString[i] & 0x0f];
	}
	Safe(free, commandString);
	CFStringRef resultString = CFStringCreateWithBytes(kCFAllocatorDefault, encodedCommand, length*2, kCFStringEncodingUTF8, true);
	Safe(free, encodedCommand);
	return resultString;
}

sdmmd_debug_return_t SDMMD_DebuggingSend(SDMMD_AMDebugConnectionRef connection, SDMMD_DebugCommandType commandType, CFStringRef encodedCommand) {
	CFDataRef data = NULL;
	char *commandData = calloc(1, sizeof(char));
	uint32_t pos = 0;
	commandData[pos++] = '$';
	for (uint32_t i = 0; i < strlen(commandType.code); i++) {
		commandData[pos++] = commandType.code[i];
		commandData = realloc(commandData, sizeof(char)*(pos+1));
	}
	if (encodedCommand) {
		unsigned char *commandString = calloc(1, CFStringGetLength(encodedCommand)+1);
		CFIndex length = CFStringGetBytes(encodedCommand, CFRangeMake(0,CFStringGetLength(encodedCommand)), kCFStringEncodingUTF8, 0, true, commandString, CFStringGetLength(encodedCommand), NULL);
		for (uint32_t i = 0; i < length; i++) {
			commandData[pos++] = commandString[i];
			commandData = realloc(commandData, sizeof(char)*(pos+1));
		}
		Safe(free,commandString);
	}
	uint32_t checksum = GenerateChecksumForData(&commandData[1], pos-1);
	commandData = realloc(commandData, sizeof(char)*(pos+3));
	commandData[pos++] = '#';
	commandData[pos++] = kHexEncode[(checksum >> 4) & 0xf];
	commandData[pos++] = kHexEncode[checksum & 0xf];
	//printf("debug send: %s\n",commandData);
	CFDataRef sending = CFDataCreateWithBytesNoCopy(kCFAllocatorDefault, (UInt8 *)commandData, pos+3, kCFAllocatorDefault);
	sdmmd_return_t result = SDMMD_ServiceSend(SDMMD_TranslateConnectionToSocket(connection), sending);
	if (SDM_MD_CallSuccessful(result)) {
		result = SDMMD_DebuggingReceive(connection, &data).result;
	}
	Safe(free, commandData);
	//CFSafeRelease(sending);
	return (sdmmd_debug_return_t){result, data};
}

sdmmd_debug_return_t SDMMD_DebuggingReceive(SDMMD_AMDebugConnectionRef connection, CFDataRef *data) {
	unsigned char ackBuffer[1];
	CFDataRef ackData = CFDataCreate(kCFAllocatorDefault, ackBuffer, 1);
	sdmmd_return_t result = SDMMD_DirectServiceReceive(SDMMD_TranslateConnectionToSocket(connection), &ackData);
	if (CFDataGetBytePtr(ackData)[0] == '+') {
		unsigned char commandBuffer[1];
		CFDataRef commandData = CFDataCreate(kCFAllocatorDefault, commandBuffer, 1);
		result = SDMMD_DirectServiceReceive(SDMMD_TranslateConnectionToSocket(connection), &commandData);	
		if (CFDataGetBytePtr(commandData)[0] == '$') {
			char *response = malloc(sizeof(char));			
			uint32_t position = 0, checksumCount = 3;
			while (checksumCount) {
				CFDataRef responseData = CFDataCreate(kCFAllocatorDefault, (UInt8 *)&response[position], 1);
				result = SDMMD_DirectServiceReceive(SDMMD_TranslateConnectionToSocket(connection), &responseData);
				response[position] = CFDataGetBytePtr(responseData)[0];
				if (response[position] == '#' || checksumCount < 3) {
					checksumCount--;
				}
				position++;
				response = realloc(response, sizeof(char)*(position+1));
				CFSafeRelease(responseData);
			}
			uint32_t checksum = GenerateChecksumForData(response, position-3);
			if (kHexDecode(response[position-2]) == ((checksum >> 4) & 0xf) && kHexDecode(response[position-1]) == (checksum & 0xf)) {
				*data = CFDataCreate(kCFAllocatorDefault, (UInt8 *)response, position-3);
			} else {
				result = kAMDInvalidResponseError;
			}
			Safe(free, response);
		}
	}
	return (sdmmd_debug_return_t){result, *data};
}

/*
CFURLRef copy_device_app_url(SDMMD_AMDeviceRef device, CFStringRef identifier) {
	CFArrayRef values = SDMMD_ApplicationLookupDictionary();
	CFMutableDictionaryRef optionsDict = SDMMD_create_dict();
	CFDictionarySetValue(optionsDict, CFSTR("ReturnAttributes"), values);
	CFDictionaryRef response;
    assert(SDMMD_AMDeviceLookupApplications(device, optionsDict, &response) == 0);
	
    CFDictionaryRef app_dict = CFDictionaryGetValue(response, identifier);
    assert(app_dict != NULL);
	
    CFStringRef app_path = CFDictionaryGetValue(app_dict, CFSTR("Path"));
    assert(app_path != NULL);
	
    CFURLRef url = CFURLCreateWithFileSystemPath(NULL, app_path, kCFURLPOSIXPathStyle, true);
    CFSafeRelease(response);
    return url;
}

CFStringRef copy_disk_app_identifier(CFURLRef disk_app_url) {
    CFURLRef plist_url = CFURLCreateCopyAppendingPathComponent(NULL, disk_app_url, CFSTR("Info.plist"), false);
    CFReadStreamRef plist_stream = CFReadStreamCreateWithFile(NULL, plist_url);
    CFReadStreamOpen(plist_stream);
    CFPropertyListRef plist = CFPropertyListCreateWithStream(NULL, plist_stream, 0, kCFPropertyListImmutable, NULL, NULL);
    CFStringRef bundle_identifier = CFRetain(CFDictionaryGetValue(plist, CFSTR("CFBundleIdentifier")));
    CFReadStreamClose(plist_stream);
	
    CFSafeRelease(plist_url);
    CFSafeRelease(plist_stream);
    CFSafeRelease(plist);
	
    return bundle_identifier;
}

Boolean path_exists(CFTypeRef path) {
    if (CFGetTypeID(path) == CFStringGetTypeID()) {
        CFURLRef url = CFURLCreateWithFileSystemPath(NULL, path, kCFURLPOSIXPathStyle, true);
        Boolean result = CFURLResourceIsReachable(url, NULL);
        CFSafeRelease(url);
        return result;
    } else if (CFGetTypeID(path) == CFURLGetTypeID()) {
        return CFURLResourceIsReachable(path, NULL);
    } else {
        return false;
    }
}

CFStringRef copy_device_support_path(SDMMD_AMDeviceRef device) {
    CFStringRef version = SDMMD_AMDeviceCopyValue(device, 0, CFSTR("ProductVersion"));
    CFStringRef build = SDMMD_AMDeviceCopyValue(device, 0, CFSTR("BuildVersion"));
    const char* home = getenv("HOME");
    CFStringRef path;
    bool found = false;
	PrintCFType(version);
    path = CFStringCreateWithFormat(NULL, NULL, CFSTR("%s/Library/Developer/Xcode/iOS DeviceSupport/%@ (%@)"), home, version, build);
    found = path_exists(path);
	
    if (!found) {
        path = CFStringCreateWithFormat(NULL, NULL, CFSTR("/Developer/Platforms/iPhoneOS.platform/DeviceSupport/%@ (%@)"), version, build);
        found = path_exists(path);
    }
    if (!found) {
        path = CFStringCreateWithFormat(NULL, NULL, CFSTR("%s/Library/Developer/Xcode/iOS DeviceSupport/%@"), home, version);
        found = path_exists(path);
    }
	if (!found) {
        path = CFStringCreateWithFormat(NULL, NULL, CFSTR("/Developer/Platforms/iPhoneOS.platform/DeviceSupport/%@"), version);
        found = path_exists(path);
    }
    if (!found) {
        path = CFStringCreateWithFormat(NULL, NULL, CFSTR("%s/Library/Device Support/%@"),home,version);
        found = path_exists(path);
    }
    if (!found) {
        path = CFStringCreateWithFormat(NULL, NULL, CFSTR("/Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/DeviceSupport/%@"), version);
        found = path_exists(path);
    }
	
    CFSafeRelease(version);
    CFSafeRelease(build);
	
    if (!found) {
        CFSafeRelease(path);
        printf("[ !! ] Unable to locate DeviceSupport directory.\n");
        exit(1);
    }
	
    return path;
}

void gdb_ready_handler(int signum) {
	_exit(0);
}

void fdvendor_callback(CFSocketRef s, CFSocketCallBackType callbackType, CFDataRef address, const void *data, void *info) {
    CFSocketNativeHandle socket = (CFSocketNativeHandle)(*((CFSocketNativeHandle *)data));
	
    struct msghdr message;
    struct iovec iov[1];
    struct cmsghdr *control_message = NULL;
    char ctrl_buf[CMSG_SPACE(sizeof(int))];
    char dummy_data[1];
	
    memset(&message, 0, sizeof(struct msghdr));
    memset(ctrl_buf, 0, CMSG_SPACE(sizeof(int)));
	
    dummy_data[0] = ' ';
    iov[0].iov_base = dummy_data;
    iov[0].iov_len = sizeof(dummy_data);
	
    message.msg_name = NULL;
    message.msg_namelen = 0;
    message.msg_iov = iov;
    message.msg_iovlen = 1;
    message.msg_controllen = CMSG_SPACE(sizeof(int));
    message.msg_control = ctrl_buf;
	
    control_message = CMSG_FIRSTHDR(&message);
    control_message->cmsg_level = SOL_SOCKET;
    control_message->cmsg_type = SCM_RIGHTS;
    control_message->cmsg_len = CMSG_LEN(sizeof(int));
	
    *((int *) CMSG_DATA(control_message)) = (int)info;
	
    sendmsg(socket, &message, 0);
    CFSocketInvalidate(s);
    CFSafeRelease(s);
}


void SDMMD_StartDebugger(SDMMD_AMDebugConnectionRef connection, CFStringRef bundleId) {
	
	CFSocketContext context = { 0, (*(void**)&(connection->ivars.socket)), NULL, NULL, NULL };
	CFSocketRef fdvendor = CFSocketCreate(NULL, AF_UNIX, 0, 0, kCFSocketAcceptCallBack, &fdvendor_callback, &context);
	
    int yes = 1;
    setsockopt(CFSocketGetNative(fdvendor), SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
	
    struct sockaddr_un address;
    memset(&address, 0, sizeof(address));
    address.sun_family = AF_UNIX;
    strcpy(address.sun_path, FDVENDOR_PATH);
    address.sun_len = SUN_LEN(&address);
    CFDataRef address_data = CFDataCreate(NULL, (const UInt8 *)&address, sizeof(address));
	
    unlink(FDVENDOR_PATH);
	
    CFSocketSetAddress(fdvendor, address_data);
    CFSafeRelease(address_data);
    CFRunLoopAddSource(CFRunLoopGetMain(), CFSocketCreateRunLoopSource(NULL, fdvendor, 0), kCFRunLoopCommonModes);
	
	CFStringRef path = bundleId;
    CFURLRef relative_url = CFURLCreateWithFileSystemPath(NULL, path, kCFURLPOSIXPathStyle, false);
    CFURLRef disk_app_url = CFURLCopyAbsoluteURL(relative_url);
	
	SDMMD_AMDeviceRef device = connection->ivars.device;
	CFMutableStringRef cmds = CFStringCreateMutableCopy(NULL, 0, GDB_PREP_CMDS);
    CFRange range = { 0, CFStringGetLength(cmds) };

    CFStringRef ds_path = copy_device_support_path(device);
    CFStringFindAndReplace(cmds, CFSTR("{ds_path}"), ds_path, range, 0);
    range.length = CFStringGetLength(cmds);


	CFStringFindAndReplace(cmds, CFSTR(" {args}"), CFSTR(""), range, 0);
    range.length = CFStringGetLength(cmds);

    CFStringRef bundle_identifier = copy_disk_app_identifier(disk_app_url);
    CFURLRef device_app_url = copy_device_app_url(device, bundle_identifier);
    CFStringRef device_app_path = CFURLCopyFileSystemPath(device_app_url, kCFURLPOSIXPathStyle);
    CFStringFindAndReplace(cmds, CFSTR("{device_app}"), device_app_path, range, 0);
    range.length = CFStringGetLength(cmds);

    CFStringRef disk_app_path = CFURLCopyFileSystemPath(disk_app_url, kCFURLPOSIXPathStyle);
    CFStringFindAndReplace(cmds, CFSTR("{disk_app}"), disk_app_path, range, 0);
    range.length = CFStringGetLength(cmds);

    CFURLRef device_container_url = CFURLCreateCopyDeletingLastPathComponent(NULL, device_app_url);
    CFStringRef device_container_path = CFURLCopyFileSystemPath(device_container_url, kCFURLPOSIXPathStyle);
    CFMutableStringRef dcp_noprivate = CFStringCreateMutableCopy(NULL, 0, device_container_path);
    range.length = CFStringGetLength(dcp_noprivate);
    CFStringFindAndReplace(dcp_noprivate, CFSTR("/private/var/"), CFSTR("/var/"), range, 0);
    range.length = CFStringGetLength(cmds);
    CFStringFindAndReplace(cmds, CFSTR("{device_container}"), dcp_noprivate, range, 0);
    range.length = CFStringGetLength(cmds);

    CFURLRef disk_container_url = CFURLCreateCopyDeletingLastPathComponent(NULL, disk_app_url);
    CFStringRef disk_container_path = CFURLCopyFileSystemPath(disk_container_url, kCFURLPOSIXPathStyle);
    CFStringFindAndReplace(cmds, CFSTR("{disk_container}"), disk_container_path, range, 0);

    CFDataRef cmds_data = CFStringCreateExternalRepresentation(NULL, cmds, kCFStringEncodingASCII, 0);
    FILE *out = fopen(PREP_CMDS_PATH, "w");
    fwrite(CFDataGetBytePtr(cmds_data), CFDataGetLength(cmds_data), 1, out);
    fclose(out);

    CFSafeRelease(cmds);
    CFSafeRelease(ds_path);
    CFSafeRelease(bundle_identifier);
    CFSafeRelease(device_app_url);
    CFSafeRelease(device_app_path);
    CFSafeRelease(disk_app_path);
    CFSafeRelease(device_container_url);
    CFSafeRelease(device_container_path);
    CFSafeRelease(dcp_noprivate);
    CFSafeRelease(disk_container_url);
    CFSafeRelease(disk_container_path);
    CFSafeRelease(cmds_data);

    signal(SIGHUP, gdb_ready_handler);

    pid_t parent = getpid();
    int pid = fork();
    if (pid == 0) {
        system(GDB_SHELL);      // launch gdb
        kill(parent, SIGHUP);  // "No. I am your father."
        _exit(0);
    }
	CFRunLoopRun();
}
*/

#endif