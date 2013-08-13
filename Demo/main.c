#include <CoreFoundation/CoreFoundation.h>
#include <SDMMobileDevice/SDMMobileDevice.h>

void DemoOne();
void DemoTwo();
void DemoThree(const char *path);
void DemoFour(const char *path);

int main (int argc, const char * argv[]) {
	// Needed to initialize the library and start the device listener (SDMMD_MCP.h)
	SDMMobileDevice;
	
	//DemoOne();
	//DemoTwo();
	if (argc == 2) {
		//DemoThree(argv[1]);
		DemoFour(argv[1]);
	}
	return 0;
}

void DemoOne() {
	// fetching the list of connected devices (SDMMD_AMDevice.h)
	CFArrayRef devices = SDMMD_AMDCreateDeviceList();
	
	uint32_t numberOfDevices = CFArrayGetCount(devices);
	printf("%i device(s) connected!\n",numberOfDevices);
	
	
	/*if (numberOfDevices) {
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
				
				// if the connection was successful, then ask for a system value from it, in this case the device name
				// (SDMMD_AMDevice.h for function and see SDMMD_Keys.h for values)
				CFTypeRef deviceName = SDMMD_AMDeviceCopyValue(device, NULL, CFSTR(kDeviceName));
				// print device name
				CFShow(deviceName);
				
				// get operating system version
				CFTypeRef deviceOS = SDMMD_AMDeviceCopyValue(device, NULL, CFSTR(kProductVersion));
				// print operating system version
				CFShow(deviceOS);
				
				// creating a session to talk to more
				//result = SDMMD_AMDeviceStartSession(device);
				if (SDM_MD_CallSuccessful(result)) {
					// get device phone number
					CFTypeRef devicePhoneNumber = SDMMD_AMDeviceCopyValue(device, NULL, CFSTR(kPhoneNumber));
					// print device phone number
					if (devicePhoneNumber)
						CFShow(devicePhoneNumber);
					
					// get device language settings
					CFTypeRef deviceLanguage = SDMMD_AMDeviceCopyValue(device, CFSTR(kInternationalDomain), CFSTR(kLanguage));
					// print language settings
					if (deviceLanguage)
						CFShow(deviceLanguage);
					
				//result = SDMMD_AMDeviceStopSession(device);
				}
				
				// disconnect when finished
				result = SDMMD_AMDeviceDisconnect(device);
			}
		}
	}*/
}

void DemoTwo() {
	// fetching the list of connected devices (SDMMD_AMDevice.h)
	CFArrayRef devices = SDMMD_AMDCreateDeviceList();
	
	uint32_t numberOfDevices = CFArrayGetCount(devices);
	printf("%i device(s) connected!\n",numberOfDevices);
	
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
				
				// creating a session to talk to more
				result = SDMMD_AMDeviceStartSession(device);
				if (SDM_MD_CallSuccessful(result)) {
					
					CFDictionaryRef response;
					CFArrayRef values = SDMMD_ApplicationLookupDictionary();
					CFMutableDictionaryRef optionsDict = SDMMD_create_dict();
					CFDictionarySetValue(optionsDict, CFSTR("ReturnAttributes"), values);
					
					result = SDMMD_AMDeviceLookupApplications(device, optionsDict, &response);
					CFShow(response);
					
					result = SDMMD_AMDeviceStopSession(device);
				}
				
				// disconnect when finished
				result = SDMMD_AMDeviceDisconnect(device);
			}
		}
	}
}

void transfer_callback(CFDictionaryRef dict, int arg) {
    int percent;
    CFStringRef status = CFDictionaryGetValue(dict, CFSTR("Status"));
    CFNumberGetValue(CFDictionaryGetValue(dict, CFSTR("PercentComplete")), kCFNumberSInt32Type, &percent);
    if (CFEqual(status, CFSTR("CopyingFile"))) {
        CFStringRef path = CFDictionaryGetValue(dict, CFSTR("Path"));
        if (/*(last_path == NULL || !CFEqual(path, last_path)) &&*/ !CFStringHasSuffix(path, CFSTR(".ipa"))) {
            printf("[%3d%%] Copying %s to device\n", percent, CFStringGetCStringPtr(path, kCFStringEncodingMacRoman));
			
        }
    }
}

void install_callback(CFDictionaryRef dict, int arg) {
    int percent;
    CFStringRef status = CFDictionaryGetValue(dict, CFSTR("Status"));
    CFNumberGetValue(CFDictionaryGetValue(dict, CFSTR("PercentComplete")), kCFNumberSInt32Type, &percent);
    printf("[%3d%%] %s\n", percent, CFStringGetCStringPtr(status, kCFStringEncodingMacRoman));
}


void DemoThree(const char *appPath) {
	
	CFArrayRef devices = SDMMD_AMDCreateDeviceList();
	
	uint32_t numberOfDevices = CFArrayGetCount(devices);
	printf("%i device(s) connected!\n",numberOfDevices);
		
	if (numberOfDevices) {
		// return type (uint32_t) corresponds with known return codes (SDMMD_Error.h)
		sdmmd_return_t result;
		
		uint32_t index;
		// Iterating over connected devices
		for (index = 0; index < numberOfDevices; index++) {
			
			// getting the device object from the array of connected devices
			SDMMD_AMDeviceRef device = (SDMMD_AMDeviceRef)CFArrayGetValueAtIndex(devices, index);
			
			CFStringRef path = CFStringCreateWithBytes(kCFAllocatorDefault, appPath, strlen(appPath), kCFStringEncodingUTF8, false);
			CFShow(path);
			SDMMD_AMConnectionRef afcFd;
			CFStringRef keys[] = { CFSTR("PackageType") };
			CFStringRef values[] = { CFSTR("Developer") };
			CFDictionaryRef options = CFDictionaryCreate(NULL, (const void **)&keys, (const void **)&values, 1, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
			SDMMD_AMConnectionRef installFd;
			bool copyResult = FALSE, installResult = FALSE;
			copyResult = (SDMMD_AMDeviceConnect(device) == kAMDSuccess ?
						  (SDMMD_AMDeviceIsPaired(device) ?
						   (SDMMD_AMDeviceValidatePairing(device) == kAMDSuccess ?
							(SDMMD_AMDeviceStartSession(device) == kAMDSuccess ?
							 (SDMMD_AMDeviceStartService(device, CFSTR(AMSVC_AFC), NULL, &afcFd) == kAMDSuccess ?
							  (SDMMD_AMDeviceStopSession(device) == kAMDSuccess ?
							   (SDMMD_AMDeviceDisconnect(device) == kAMDSuccess ?
								(AMDeviceTransferApplication(afcFd->ivars.socket, path, NULL, transfer_callback, NULL) == kAMDSuccess ? TRUE : FALSE)
								: FALSE)
							   : FALSE)
							  : FALSE)
							 : FALSE)
							: FALSE)
						   : FALSE)
						  : FALSE);
			//if (copyResult)
				installResult = (SDMMD_AMDeviceConnect(device) == kAMDSuccess ?
								 (SDMMD_AMDeviceIsPaired(device) ?
								  (SDMMD_AMDeviceValidatePairing(device) == kAMDSuccess ?
								   (SDMMD_AMDeviceStartSession(device) == kAMDSuccess ?
									(SDMMD_AMDeviceInstallApplication(device, path, options, install_callback, NULL) == kAMDSuccess ? 
									 (SDMMD_AMDeviceStopSession(device) == kAMDSuccess ?
									  (SDMMD_AMDeviceDisconnect(device) == kAMDSuccess ?
									   : FALSE)
									  : FALSE)
									 : FALSE)
									: FALSE)
								   : FALSE)
								  : FALSE)
								 : FALSE);
			CFRelease(options);			
		}
	}	
}

void DemoFour(const char *appPath) {
	CFArrayRef devices = SDMMD_AMDCreateDeviceList();
	
	uint32_t numberOfDevices = CFArrayGetCount(devices);
	printf("%i device(s) connected!\n",numberOfDevices);
	
	
	if (numberOfDevices) {
		// return type (uint32_t) corresponds with known return codes (SDMMD_Error.h)
		sdmmd_return_t result;
		
		uint32_t index;
		// Iterating over connected devices
		for (index = 0; index < numberOfDevices; index++) {
			SDMMD_AMDeviceRef device = (SDMMD_AMDeviceRef)CFArrayGetValueAtIndex(devices, index);
			
			CFStringRef bundleId = CFStringCreateWithBytes(kCFAllocatorDefault, appPath, strlen(appPath), kCFStringEncodingUTF8, false);
			
			SDMMD_AMDebugConnectionRef debug;
			result = SDMMD_StartDebuggingSessionOnDevice(device, &debug);
			SDMMD_StartDebugger(debug, bundleId);
		}
	}
	
}