//
//  main.c
//  revealer
//
//  Created by Samantha Marshall on 8/8/14.
//  Copyright (c) 2014 Samantha Marshall. All rights reserved.
//

#include <Foundation/Foundation.h>
#include <CoreFoundation/CoreFoundation.h>
#include <SDMMobileDevice/SDMMobileDevice.h>
#include <SDMMobileDevice/SDMMD_Connection_Private.h>
#include <SDMMobileDevice/SDMMD_Connection_Internal.h>

#include <ApplicationServices/ApplicationServices.h>
#include <ServiceManagement/ServiceManagement.h>
#include <Security/Authorization.h>
#include <Security/Security.h>
#include <Security/SecCertificate.h>
#include <Security/SecCode.h>
#include <Security/SecStaticCode.h>
#include <Security/SecCodeHost.h>
#include <Security/SecRequirement.h>

#define SafeMacro(x) do { x } while(0)

#ifdef DEBUG
#define DBLog(...) SafeMacro(printf(__VA_ARGS__);)
#else
#define DBLog(...) SafeMacro()
#endif

#define kTmpReveal "/tmp/libReveal.dylib"

enum ModeType {
	ModeType_Invalid = -1,
	ModeType_Signing = 1,
	ModeType_Deploy = 2
};

int TestSigning();

extern char *CreateCStringFromCFStringRef(CFStringRef string);

CFStringRef GetRevealPath() {
	CFURLRef outAppURL = NULL;
	OSStatus find_app = LSFindApplicationForInfo(kLSUnknownCreator, CFSTR("com.ittybittyapps.Reveal"), NULL, NULL, &outAppURL);
	
	if (find_app != kLSApplicationNotFoundErr) {
		CFURLRef temp = CFURLCreateCopyAppendingPathComponent(kCFAllocatorDefault, outAppURL, CFSTR("Contents/SharedSupport/iOS-Libraries/libReveal.dylib"), false);
		return CFURLCopyFileSystemPath(temp, kCFURLPOSIXPathStyle);
	}
	
	return NULL;
}

CFStringRef GetRevealTempPath() {
	CFStringRef reveal_path = GetRevealPath();
	if (reveal_path != NULL) {
		if ([[NSFileManager defaultManager] fileExistsAtPath:@kTmpReveal] == YES) {
			if (TestSigning() != -1) {
				return CFSTR(kTmpReveal);
			}
		}
		else {
			if ([[NSFileManager defaultManager] copyItemAtPath:(__bridge NSString *)reveal_path toPath:@kTmpReveal error:nil]) {
				return CFSTR(kTmpReveal);
			}
		}
	}
	
	return NULL;
}

Boolean LookupSigningCert(CFStringRef *cert_name) {
	Boolean found_cert = false;
	CFTypeRef results = NULL;
	CFMutableDictionaryRef search = CFDictionaryCreateMutable(NULL, 0, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
	CFDictionaryAddValue(search, kSecClass, kSecClassCertificate);
	CFDictionaryAddValue(search, kSecMatchSubjectContains, CFSTR("iPhone Developer:"));
	CFDictionaryAddValue(search, kSecMatchLimit, kSecMatchLimitAll);
	OSStatus status = SecItemCopyMatching(search, &results);
	if (status == errSecSuccess) {
		CFDataRef signing_cert;
		status = SecItemExport(results, kSecFormatX509Cert, 0, NULL, &signing_cert);
		if (status == errSecSuccess) {
			found_cert = true;
		}
		SecCertificateRef certificate = SecCertificateCreateWithData(kCFAllocatorDefault, signing_cert);
		
        CFStringRef commonName = NULL;
        SecCertificateCopyCommonName(certificate, &commonName);
        *cert_name = CFStringCreateWithFormat(kCFAllocatorDefault, NULL, CFSTR("%@"), commonName);
		
        CFSafeRelease(commonName);
	}
	return found_cert;
}

int TestSigning() {
		
	CFStringRef cert = NULL;
	LookupSigningCert(&cert);
	
	NSPipe *output = [[NSPipe alloc] init];
		
	NSTask *runCodeSign = [[NSTask alloc] init];
	[runCodeSign setLaunchPath:@"/usr/bin/codesign"];
	[runCodeSign setArguments:@[@"-d", (__bridge NSString *)CFSTR(kTmpReveal)]];
	[runCodeSign setStandardOutput:output];
	[runCodeSign launch];
	[runCodeSign waitUntilExit];
		
	NSData *outputData = [[output fileHandleForReading] availableData];
	NSString *outputString = [[NSString alloc] initWithData:outputData encoding:NSUTF8StringEncoding];
	if ([outputString rangeOfString:@"is not signed"].location == NSNotFound) {
		return 0;
	}
	
	return -1;

}

int RunSigning() {
	int status = -1;
	
	
	CFStringRef local_path = GetRevealTempPath();
		
	if (local_path != NULL) {
		CFStringRef cert = NULL;
		LookupSigningCert(&cert);
		
		NSPipe *output = [[NSPipe alloc] init];
		NSTask *runCodeSign = [[NSTask alloc] init];
		[runCodeSign setEnvironment:[[NSProcessInfo processInfo] environment]];
		[runCodeSign setLaunchPath:@"/usr/bin/codesign"];
		[runCodeSign setArguments:@[@"--sign", (__bridge NSString *)cert, @"-fvvv", (__bridge NSString *)local_path]];
		[runCodeSign setStandardError:output];
		[runCodeSign launch];
		[runCodeSign waitUntilExit];
		
		NSData *outputData = [[output fileHandleForReading] availableData];
		NSString *outputString = [[NSString alloc] initWithData:outputData encoding:NSUTF8StringEncoding];
		if ([outputString rangeOfString:@"signed Mach-O"].location != NSNotFound) {
			status = 0;
		}
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
	
	CFShow(local_bundle_path);
	//CFShow(remote_container_path);
	
	CFURLRef path_url = CFURLCreateWithString(kCFAllocatorDefault, local_bundle_path, NULL);
	CFURLRef bundle_url = CFURLCreateCopyDeletingLastPathComponent(kCFAllocatorDefault, path_url);
	CFSafeRelease(path_url);
	CFSafeRelease(local_bundle_path);
	
	CFBundleRef local_app_bundle = CFBundleCreate(kCFAllocatorDefault, bundle_url);
	CFSafeRelease(bundle_url);
	CFStringRef bundleIdentifier = CFBundleGetIdentifier(local_app_bundle);
	
	NSString *local_path = (__bridge NSString *)GetRevealTempPath();
	
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
							CFStringRef container = CFDictionaryGetValue(details, CFSTR("Path"));
							CFRange found_range = CFStringFind(container, remote_container_path, 0);
							if (found_range.location != kCFNotFound && found_range.length != 0) {
								remote_path = CFStringCreateWithFormat(kCFAllocatorDefault, NULL, CFSTR("%@/libReveal.dylib"),(__bridge CFStringRef)[(__bridge NSString *)container lastPathComponent]);
								
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
							status = SDMMD_AMDeviceCopy(afc, (char *)[local_path UTF8String], "Documents/libReveal.dylib");
							SDMMD_AFCOperationRef move_op = SDMMD_AFCOperationCreateRenamePath(CFSTR("Documents/libReveal.dylib"), remote_path);
							SDMMD_AFCProcessOperation(afc, &move_op);
							free(copy_path);
						}
					}
				}
			}
			
			SDMMD_AMDeviceStopSession(device);
			SDMMD_AMDeviceDisconnect(device);
		}
	}
	
	CFSafeRelease(local_app_bundle);
	CFSafeRelease(devices);
	CFSafeRelease(remote_container_path);
	CFSafeRelease(remote_path);
	
	return status;
}

int main(int argc, const char * argv[]) {
	
	int status = -1;
	
	char *signing_mode = "-s";
	char *deploy_mode = "-d";

	int mode = ModeType_Invalid;
	
	if (argc >= 2) {
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
			status = RunSigning();
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

