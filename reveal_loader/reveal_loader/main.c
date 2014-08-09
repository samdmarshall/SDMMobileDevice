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

int main(int argc, const char * argv[]) {
	
	int status = -1;
	
	if (argc != 2) {
		return status;
	}
	
	SDMMobileDevice;
	
	sdmmd_return_t result = KERN_SUCCESS;
	
	CFStringRef remote_path = NULL;
	
	CFStringRef path = CFStringCreateWithCString(kCFAllocatorDefault, argv[1], kCFStringEncodingUTF8);
	CFURLRef path_url = CFURLCreateWithString(kCFAllocatorDefault, path, NULL);
	CFURLRef bundle_url = CFURLCreateCopyDeletingLastPathComponent(kCFAllocatorDefault, path_url);
	
	CFStringRef bundleIdentifier = CFBundleGetIdentifier(CFBundleCreate(kCFAllocatorDefault, bundle_url));
	CFShow(bundleIdentifier);
	
	char *local_path = NULL;
	CFURLRef outAppURL = NULL;
	OSStatus find_app = LSFindApplicationForInfo(kLSUnknownCreator, CFSTR("com.ittybittyapps.Reveal"), NULL, NULL, &outAppURL);
	
	if (find_app != kLSApplicationNotFoundErr) {
		CFURLRef temp = CFURLCreateCopyAppendingPathComponent(kCFAllocatorDefault, outAppURL, CFSTR("Contents/SharedSupport/iOS-Libraries/libReveal.dylib"), false);
		CFSafeRelease(outAppURL);
		CFStringRef fs_path = CFURLCopyFileSystemPath(temp, kCFURLPOSIXPathStyle);
		local_path = CreateCStringFromCFStringRef(fs_path);
	}
	
	CFArrayRef devices = SDMMD_AMDCreateDeviceList();
	CFIndex device_count = CFArrayGetCount(devices);
	
	if (devices != NULL && device_count != 0) {
		
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
					if (SDM_MD_CallSuccessful(result)) {
						CFDictionaryRef details = NULL;
						if (CFDictionaryContainsKey(response, bundleIdentifier)) {
							details = CFDictionaryGetValue(response, bundleIdentifier);
							PrintCFType(details);
						}
						if (details != NULL) {
							CFStringRef container = CFDictionaryGetValue(response, CFSTR("Container"));
							CFStringRef container_uuid = NULL;
							// find a way to check the container
							
							break;
						}
						
						CFSafeRelease(details);
					}
					
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
				
				CFMutableDictionaryRef response;
				result = SDMMD_ServiceReceiveMessage(socket, (CFPropertyListRef *)&response);
				if (SDM_MD_CallSuccessful(result)) {
					SDMMD_AFCConnectionRef afc = SDMMD_AFCConnectionCreate(conn);
					if (afc) {
						SDMMD_AFCOperationRef remove_old = SDMMD_AFCOperationCreateRemovePath(CFSTR("Documents/libReveal.dylib"));
						SDMMD_AFCProcessOperation(afc, &remove_old);
						status = SDMMD_AMDeviceCopy(afc, local_path, "Documents/libReveal.dylib");
					}
				}
			}
			
			SDMMD_AMDeviceStopSession(device);
			SDMMD_AMDeviceDisconnect(device);
		}
	}

	CFSafeRelease(bundleIdentifier);

	if (local_path != NULL) {
		free(local_path);
	}

	return status;
}

