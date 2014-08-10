//
//  main.c
//  revealer
//
//  Created by Sam Marshall on 8/8/14.
//  Copyright (c) 2014 Sam Marshall. All rights reserved.
//

#include <CoreFoundation/CoreFoundation.h>
#include <SDMMobileDevice/SDMMobileDevice.h>
#include <SDMMobileDevice/SDMMD_Connection_Private.h>
#include <SDMMobileDevice/SDMMD_Connection_Internal.h>
#include "CFStringAddition.h"
#include <ApplicationServices/ApplicationServices.h>

#define SafeMacro(x) do { x } while(0)

#ifdef DEBUG
#define DBLog(...) SafeMacro(printf(__VA_ARGS__);)
#else
#define DBLog(...) SafeMacro()
#endif

enum ModeType {
	ModeType_Invalid = -1,
	ModeType_Signing = 1,
	ModeType_Deploy = 2
};

char * GetRevealPath() {
	char *local_path = NULL;
	CFURLRef outAppURL = NULL;
	OSStatus find_app = LSFindApplicationForInfo(kLSUnknownCreator, CFSTR("com.ittybittyapps.Reveal"), NULL, NULL, &outAppURL);
	
	if (find_app != kLSApplicationNotFoundErr) {
		CFURLRef temp = CFURLCreateCopyAppendingPathComponent(kCFAllocatorDefault, outAppURL, CFSTR("Contents/SharedSupport/iOS-Libraries/libReveal.dylib"), false);
		CFSafeRelease(outAppURL);
		CFStringRef fs_path = CFURLCopyFileSystemPath(temp, kCFURLPOSIXPathStyle);
		local_path = CreateCStringFromCFStringRef(fs_path);
		CFSafeRelease(fs_path);
		CFSafeRelease(temp);
	}
	
	return local_path;
}

int RunSigning(const char * argv[]) {
	int status = -1;
	
	char *local_path = GetRevealPath();
	
	if (local_path != NULL) {
		int result = execl("/usr/bin/codesign", "-f", "-s", "'iPhone Developer'", local_path);
		status = (result != -1 ? 0 : -1);
	}
	
	if (local_path != NULL) {
		free(local_path);
	}
	
	return status;
}

int RunDeployment(const char * argv[]) {
	int status = -1;
	
	SDMMobileDevice;
	
	sdmmd_return_t result = KERN_SUCCESS;
	
	CFStringRef remote_path = NULL;
	
	CFStringRef local_bundle_path = CFStringCreateWithCString(kCFAllocatorDefault, argv[2], kCFStringEncodingUTF8);
	CFStringRef remote_container_path = CFStringCreateWithCString(kCFAllocatorDefault, argv[3], kCFStringEncodingUTF8);
	
	CFURLRef path_url = CFURLCreateWithString(kCFAllocatorDefault, local_bundle_path, NULL);
	CFURLRef bundle_url = CFURLCreateCopyDeletingLastPathComponent(kCFAllocatorDefault, path_url);
	CFSafeRelease(path_url);
	CFSafeRelease(local_bundle_path);
	
	CFBundleRef local_app_bundle = CFBundleCreate(kCFAllocatorDefault, bundle_url);
	CFSafeRelease(bundle_url);
	CFStringRef bundleIdentifier = CFBundleGetIdentifier(local_app_bundle);
	
	char *local_path = GetRevealPath();
	
	CFArrayRef devices = SDMMD_AMDCreateDeviceList();
	CFIndex device_count = CFArrayGetCount(devices);
	
	if (devices != NULL && device_count != 0 && local_path != NULL) {
		
		SDMMD_AMDeviceRef device = NULL;
		
		for (CFIndex device_index = 0; device_index < device_count; device_index++) {
			device = (SDMMD_AMDeviceRef)CFArrayGetValueAtIndex(devices, device_index);
			
			result = SDMMD_AMDeviceConnect(device);
			if (SDM_MD_CallSuccessful(result)) {
				result = SDMMD_AMDeviceStartSession(device);
				if (SDM_MD_CallSuccessful(result)) {
					CFDictionaryRef response;
					CFArrayRef lookupValues = SDMMD_ApplicationLookupDictionary();
					CFMutableDictionaryRef optionsDict = SDMMD_create_dict();
					CFDictionarySetValue(optionsDict, CFSTR("ReturnAttributes"), lookupValues);
					CFDictionarySetValue(optionsDict, CFSTR("BundleIDs"), kCFBooleanTrue);
					
					result = SDMMD_AMDeviceLookupApplications(device, optionsDict, &response);
					CFSafeRelease(optionsDict);
					if (SDM_MD_CallSuccessful(result)) {
						CFDictionaryRef details = NULL;
						if (CFDictionaryContainsKey(response, bundleIdentifier)) {
							details = CFDictionaryGetValue(response, bundleIdentifier);
							PrintCFType(details);
						}
						if (details != NULL) {
							CFStringRef container = CFDictionaryGetValue(response, CFSTR("Path"));
							if (CFStringCompare(remote_container_path, container, 0) == 0) {
								remote_path = CFStringCreateWithFormat(kCFAllocatorDefault, NULL, CFSTR("%@/libReveal.dylib"),container);
								
								break;
							}
						}
						
					}
					CFSafeRelease(response);
					
					SDMMD_AMDeviceStopSession(device);
				}
				SDMMD_AMDeviceDisconnect(device);
				
				device = NULL;
			}
			
		}
		
		if (device != NULL) {
			CFDictionaryRef dict = NULL;
			SDMMD_AMConnectionRef conn = SDMMD_AMDServiceConnectionCreate(0, NULL, dict);
			
			CFShow(device);
			
			result = SDMMD_AMDeviceSecureStartService(device, CFSTR(AMSVC_HOUSE_ARREST), NULL, &conn);
			if (SDM_MD_CallSuccessful(result)) {
				CFMutableDictionaryRef optionsDict = SDMMD_create_dict();
				CFDictionarySetValue(optionsDict, CFSTR("Command"), CFSTR("VendContainer"));
				CFDictionarySetValue(optionsDict, CFSTR("Identifier"), bundleIdentifier);
				
				SocketConnection socket = SDMMD_TranslateConnectionToSocket(conn);
				
				result = SDMMD_ServiceSendMessage(socket, optionsDict, kCFPropertyListXMLFormat_v1_0);
				CFSafeRelease(optionsDict);
				if (SDM_MD_CallSuccessful(result)) {
					CFMutableDictionaryRef response;
					result = SDMMD_ServiceReceiveMessage(socket, (CFPropertyListRef *)&response);
					if (SDM_MD_CallSuccessful(result)) {
						SDMMD_AFCConnectionRef afc = SDMMD_AFCConnectionCreate(conn);
						if (afc) {
							SDMMD_AFCOperationRef remove_old = SDMMD_AFCOperationCreateRemovePath(remote_path);
							SDMMD_AFCProcessOperation(afc, &remove_old);
							char *copy_path = CreateCStringFromCFStringRef(remote_path);
							status = SDMMD_AMDeviceCopy(afc, local_path, copy_path);
							free(copy_path);
						}
					}
				}
			}
			CFSafeRelease(conn);
			
			SDMMD_AMDeviceStopSession(device);
			SDMMD_AMDeviceDisconnect(device);
		}
	}
	
	CFSafeRelease(local_app_bundle);
	CFSafeRelease(devices);
	CFSafeRelease(remote_container_path);
	CFSafeRelease(remote_path);
	
	if (local_path != NULL) {
		free(local_path);
	}
	
	return status;
}

int main(int argc, const char * argv[]) {
	
	int status = -1;
	
	char *signing_mode = "-s";
	char *deploy_mode = "-d";

	int mode = ModeType_Invalid;
	
	if (argc > 2) {
		if (strncmp(argv[1], signing_mode, sizeof(char[2])) == 0) {
			mode = ModeType_Signing;
		}
		if (strncmp(argv[1], deploy_mode, sizeof(char[2])) == 0) {
			mode = ModeType_Deploy;
		}
	}
	
	switch (mode) {
		case ModeType_Invalid: {
			printf("Invalid launch arguments!\n");
			break;
		}
		case ModeType_Signing: {
			status = RunSigning(argv);
			break;
		}
		case ModeType_Deploy: {
			status = RunDeployment(argv);
			break;
		}
		default: {
			break;
		}
	}

	return status;
}

