//
//  dev.c
//  iOSConsole
//
//  Created by Sam Marshall on 4/17/14.
//  Copyright (c) 2014 Sam Marshall. All rights reserved.
//

#ifndef iOSConsole_dev_c
#define iOSConsole_dev_c

#include "dev.h"
#include "SDMMobileDevice.h"
#include "attach.h"
#include "SDMMD_Debugger_Internal.h"
#include "SDMMD_AFCConnection_Class.h"
#include <sys/socket.h>
#include <sys/un.h>
#include <CoreFoundation/CoreFoundation.h>

#define SDM_LLDB_SOCKET "/tmp/sdmmd-remote-debugserver"
#define PREP_CMDS_PATH "/tmp/sdmmd-lldb-prep"

#define LLDB_PREP_CMDS CFSTR("\
target modules search-paths add /usr \"{SYMBOLS_PATH}/usr\" /System \"{SYMBOLS_PATH}/System\" \"{CONTAINER_PATH}\" \"{DISK_CONTAINER}\" \"{CONTAINER_PATH}\" \"{DISK_CONTAINER}\" /Developer \"{SYMBOLS_PATH}/Developer\"\n\
")

void SetupDeviceForDevelopment(char *udid) {
	SDMMD_AMDeviceRef device = FindDeviceFromUDID(udid);
	if (device) {
		SDMMD_AMDeviceMountDeveloperImage(device);
	}
}

void TranferAppForInstallOnDevice(char *udid, char *app_path) {
	SDMMD_AMDeviceRef device = FindDeviceFromUDID(udid);
	if (device) {
		CFStringRef path = CFStringCreateWithBytes(kCFAllocatorDefault, (UInt8 *)app_path, strlen(app_path), kCFStringEncodingUTF8, false);
		SDMMD_AMDeviceCopyApplication(device, path);
		CFSafeRelease(path);
	}
}

void InstallApplicationOnDevice(char *udid, char *app_path) {
	SDMMD_AMDeviceRef device = FindDeviceFromUDID(udid);
	if (device) {
		CFStringRef path = CFStringCreateWithBytes(kCFAllocatorDefault, (UInt8 *)app_path, strlen(app_path), kCFStringEncodingUTF8, false);
		SDMMD_AMDeviceInstallApp(device, path);
		CFSafeRelease(path);
	}
}

void socket_callback(CFSocketRef s, CFSocketCallBackType callbackType, CFDataRef address, const void *data, void *info) {
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
	
    *((uintptr_t *) CMSG_DATA(control_message)) = (uintptr_t)info;
	
    sendmsg(socket, &message, 0);
    CFSocketInvalidate(s);
    CFRelease(s);
}

CFURLRef copy_device_app_url(SDMMD_AMDeviceRef device, CFStringRef identifier) {
	CFArrayRef values = SDMMD_ApplicationLookupDictionary();
	CFMutableDictionaryRef optionsDict = SDMMD_create_dict();
	CFDictionarySetValue(optionsDict, CFSTR("ReturnAttributes"), values);
	CFDictionaryRef response;
	SDMMD_AMDeviceConnect(device);
	SDMMD_AMDeviceStartSession(device);
    SDMMD_AMDeviceLookupApplications(device, optionsDict, &response);

    CFDictionaryRef app_dict = CFDictionaryGetValue(response, identifier);
    assert(app_dict != NULL);
	
    CFStringRef app_path = CFDictionaryGetValue(app_dict, CFSTR("Path"));
    assert(app_path != NULL);
	
    CFURLRef url = CFURLCreateWithFileSystemPath(NULL, app_path, kCFURLPOSIXPathStyle, true);
    CFRelease(response);
    return url;
}

CFStringRef copy_disk_app_identifier(CFURLRef disk_app_url) {
    CFURLRef plist_url = CFURLCreateCopyAppendingPathComponent(NULL, disk_app_url, CFSTR("Info.plist"), false);
    CFReadStreamRef plist_stream = CFReadStreamCreateWithFile(NULL, plist_url);
    CFReadStreamOpen(plist_stream);
    CFPropertyListRef plist = CFPropertyListCreateWithStream(NULL, plist_stream, 0, kCFPropertyListImmutable, NULL, NULL);
    CFStringRef bundle_identifier = CFRetain(CFDictionaryGetValue(plist, CFSTR("CFBundleIdentifier")));
    CFReadStreamClose(plist_stream);
	
    CFRelease(plist_url);
    CFRelease(plist_stream);
    CFRelease(plist);
	
    return bundle_identifier;
}

Boolean path_exists(CFTypeRef path) {
    if (CFGetTypeID(path) == CFStringGetTypeID()) {
        CFURLRef url = CFURLCreateWithFileSystemPath(NULL, path, kCFURLPOSIXPathStyle, true);
        Boolean result = CFURLResourceIsReachable(url, NULL);
        CFRelease(url);
        return result;
    } else if (CFGetTypeID(path) == CFURLGetTypeID()) {
        return CFURLResourceIsReachable(path, NULL);
    } else {
        return false;
    }
}

void StartDebuggingAndDetach(char *udid, char *app_path) {
	SDMMD_AMDeviceRef device = FindDeviceFromUDID(udid);
	if (device) {

		CFStringRef bundleId = CFStringCreateWithBytes(kCFAllocatorDefault, (UInt8 *)app_path, strlen(app_path), kCFStringEncodingUTF8, false);
		CFURLRef relative_url = CFURLCreateWithFileSystemPath(NULL, bundleId, kCFURLPOSIXPathStyle, false);
		CFURLRef disk_app_url = CFURLCopyAbsoluteURL(relative_url);
		CFStringRef bundle_identifier = copy_disk_app_identifier(disk_app_url);

		
		SDMMD_AMDebugConnectionRef debug = SDMMD_AMDebugConnectionCreateForDevice(device);
		SDMMD_AMDebugConnectionStart(debug);
		
		uintptr_t socket = SDMMD_AMDServiceConnectionGetSocket(debug->connection);
		CFSocketContext context = { 0, (void*)socket, NULL, NULL, NULL };
		CFSocketRef fdvendor = CFSocketCreate(NULL, AF_UNIX, 0, 0, kCFSocketAcceptCallBack, &socket_callback, &context);
		
		int yes = 1;
		setsockopt(CFSocketGetNative(fdvendor), SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
		
		struct sockaddr_un address;
		memset(&address, 0, sizeof(address));
		address.sun_family = AF_UNIX;
		strcpy(address.sun_path, SDM_LLDB_SOCKET);
		address.sun_len = SUN_LEN(&address);
		CFDataRef address_data = CFDataCreate(NULL, (const UInt8 *)&address, sizeof(address));
		
		unlink(SDM_LLDB_SOCKET);
		
		CFSocketSetAddress(fdvendor, address_data);
		CFRelease(address_data);
		CFRunLoopAddSource(CFRunLoopGetMain(), CFSocketCreateRunLoopSource(NULL, fdvendor, 0), kCFRunLoopCommonModes);
		
		SDMMD_AMDeviceRef device = SDMMD_AMDServiceConnectionGetDevice(debug->connection);
		CFMutableStringRef cmds = CFStringCreateMutableCopy(NULL, 0, LLDB_PREP_CMDS);
		CFRange range = { 0, CFStringGetLength(cmds) };
		
		CFURLRef device_app_url = copy_device_app_url(device, bundle_identifier);
		CFStringRef device_app_path = CFURLCopyFileSystemPath(device_app_url, kCFURLPOSIXPathStyle);
		CFStringFindAndReplace(cmds, CFSTR("{DEVICE_PATH}"), device_app_path, range, 0);
		range.length = CFStringGetLength(cmds);
		
		CFStringRef disk_app_path = CFURLCopyFileSystemPath(disk_app_url, kCFURLPOSIXPathStyle);
		CFStringFindAndReplace(cmds, CFSTR("{APP_PATH}"), disk_app_path, range, 0);
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
		
		signal(SIGHUP, exit);
		
		pid_t parent = getpid();
		int pid = fork();
		if (pid == 0) {
			system("xcrun -sdk iphoneos lldb /tmp/sdmmd-lldb-prep");
			kill(parent, SIGHUP);
			_exit(0);
		}
		CFRunLoopRun();
	}
}

#endif