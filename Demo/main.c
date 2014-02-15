#include <CoreFoundation/CoreFoundation.h>
#include <SDMMobileDevice/SDMMobileDevice.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <sys/un.h>
#include <errno.h>

void DemoOne();
void DemoTwo();
void AFCTest();
void DemoThree(const char *path);
void DemoFour(const char *path);

void infoTest();
/*
void KeyExplore(char *domain, char *key) {
	CFArrayRef devices = SDMMD_AMDCreateDeviceList();
	
	uint32_t numberOfDevices = CFArrayGetCount(devices);
	
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
					// get device phone number
					CFStringRef domainKey = CFStringCreateWithCString(kCFAllocatorDefault,domain,kCFStringEncodingUTF8);
					CFStringRef keyValue = CFStringCreateWithCString(kCFAllocatorDefault,key,kCFStringEncodingUTF8);
					CFTypeRef keyTest = SDMMD_AMDeviceCopyValue(device, domainKey, keyValue);
					// print language settings
					if (keyTest)
						CFShow(keyTest);
					
					result = SDMMD_AMDeviceStopSession(device);
				}
				
				// disconnect when finished
				result = SDMMD_AMDeviceDisconnect(device);
			}
		}
	}

}

static void
read_descriptor (int fd)
{
	struct msghdr message;
    struct iovec iov[0x1];
    struct cmsghdr *control_message = NULL;
    char ctrl_buf[CMSG_SPACE(sizeof(int))];
    char dummy_data[0x1];
	int n;
	
    memset(&message, 0x0, sizeof(struct msghdr));
    memset(ctrl_buf, 0x0, CMSG_SPACE(sizeof(int)));
	
    dummy_data[0] = ' ';
    iov[0x0].iov_base = dummy_data;
    iov[0x0].iov_len = sizeof(dummy_data);
	
    message.msg_name = NULL;
    message.msg_namelen = 0x0;
    message.msg_iov = iov;
    message.msg_iovlen = 0x1;
    message.msg_controllen = CMSG_SPACE(sizeof(int));
    message.msg_control = ctrl_buf;
	
    control_message = CMSG_FIRSTHDR(&message);
    control_message->cmsg_level = SOL_SOCKET;
    control_message->cmsg_type = SCM_RIGHTS;
    control_message->cmsg_len = CMSG_LEN(sizeof(int));
	
	n = recvmsg (fd, &message, 0);
	printf("%i\n",n);
	close (fd);
	
	for (control_message = CMSG_FIRSTHDR (&message); control_message; control_message = CMSG_NXTHDR (&message, control_message))
    {
		int descriptor;
		
		if (control_message->cmsg_len   != CMSG_LEN (sizeof (int)) ||
			control_message->cmsg_level != SOL_SOCKET              ||
			control_message->cmsg_type  != SCM_RIGHTS)
			continue;
		
		descriptor = *((int *) CMSG_DATA (control_message));
		
    go_again:
		while (1)
		{
			char *buffer = calloc(0x1, 0x80);
			recv(descriptor, buffer, 0x80, 0x0);
			printf("%s",buffer);
			fflush (NULL);
		}
		
		close (descriptor);
    }
}
*/

int main (int argc, const char * argv[]) {
	// Needed to initialize the library and start the device listener (SDMMD_MCP.h)
	SDMMobileDevice;
	/*
	uint32_t result;
	uint32_t sock = socket(AF_UNIX, SOCK_STREAM, 0x0);
		char *mux = "/tmp/sdm_syslog";
		struct sockaddr_un address;
		address.sun_family = AF_UNIX;
		strncpy(address.sun_path, mux, 0x68);
        address.sun_len = SUN_LEN(&address);
		
		result = connect(sock, (const struct sockaddr *)&address, sizeof(struct sockaddr_un));
		//ioctl(sock, 0x8004667e, 0x0); // _USBMuxSetSocketBlockingMode
	if (result)
		printf("%s\n",strerror(errno));
	else {
		read_descriptor(sock);
	}
	 */
	//AFCTest();
	//KeyExplore("","BrickState");
	DemoOne();
	//DemoTwo();
	if (argc == 2) {
		//DemoThree(argv[1]);
		//DemoFour(argv[1]);
	}
	//infoTest();
	
	return 0;
}

void DemoOne() {
	// fetching the list of connected devices (SDMMD_AMDevice.h)
	CFArrayRef devices = SDMMD_AMDCreateDeviceList();
	
	CFIndex numberOfDevices = CFArrayGetCount(devices);
	printf("%ld device(s) connected!\n",numberOfDevices);
	
	
	if (numberOfDevices) {
		// return type (uint32_t) corresponds with known return codes (SDMMD_Error.h)
		sdmmd_return_t result;
		
		uint32_t index;
		// Iterating over connected devices
		for (index = 0; index < numberOfDevices; index++) {
			
			// getting the device object from the array of connected devices
			SDMMD_AMDeviceRef device = (SDMMD_AMDeviceRef)CFArrayGetValueAtIndex(devices, index);
			//SDMMD_AMDeviceConnect(device);
			
			CFDataRef sig_data = SDMMD__CreateDataFromFileContents("/Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/DeviceSupport/7.1 (11D5127c)/DeveloperDiskImage.dmg.signature");
			
			CFTypeRef keys[] = { CFSTR("ImageSignature"), CFSTR("ImageType") };
			CFTypeRef values[] = { sig_data, CFSTR("Developer") };
			CFDictionaryRef options = CFDictionaryCreate(NULL, (const void **)&keys, (const void **)&values, 2, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);

			
			result = SDMMD_AMDeviceMountImage(device, CFSTR("/Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/DeviceSupport/7.1 (11D5127c)/DeveloperDiskImage.dmg"), options, SDMMD_Default_mount_callback, NULL);
			printf("%08x\n",result);
			/*
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
				result = SDMMD_AMDeviceStartSession(device);
				if (SDM_MD_CallSuccessful(result)) {
					// get device phone number
					CFTypeRef devicePhoneNumber = SDMMD_AMDeviceCopyValue(device, NULL, CFSTR(kPhoneNumber));
					// print device phone number
					if (devicePhoneNumber)
						CFShow(devicePhoneNumber);
					
					// get device language settings
					CFTypeRef deviceLanguage = SDMMD_AMDeviceCopyValue(device, CFSTR("com.apple.MobileDevice"), CFSTR("LogLevel"));
					// print language settings
					if (deviceLanguage)
						CFShow(deviceLanguage);
					
					result = SDMMD_AMDeviceStopSession(device);
				}
				
				// disconnect when finished
				result = SDMMD_AMDeviceDisconnect(device);
			}
			 */
		}
	}
}

/*
void DemoOne() {
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
				result = SDMMD_AMDeviceStartSession(device);
				if (SDM_MD_CallSuccessful(result)) {
					// get device phone number
					CFTypeRef devicePhoneNumber = SDMMD_AMDeviceCopyValue(device, NULL, CFSTR(kPhoneNumber));
					// print device phone number
					if (devicePhoneNumber)
						CFShow(devicePhoneNumber);
					
					// get device language settings
					CFTypeRef deviceLanguage = SDMMD_AMDeviceCopyValue(device, CFSTR("com.apple.MobileDevice"), CFSTR("LogLevel"));
					// print language settings
					if (deviceLanguage)
						CFShow(deviceLanguage);
					
					result = SDMMD_AMDeviceStopSession(device);
				}
				
				// disconnect when finished
				result = SDMMD_AMDeviceDisconnect(device);
			}
		}
	}
}
*/

void DemoTwo() {
	// fetching the list of connected devices (SDMMD_AMDevice.h)
	CFArrayRef devices = SDMMD_AMDCreateDeviceList();
	
	CFIndex numberOfDevices = CFArrayGetCount(devices);
	printf("%ld device(s) connected!\n",numberOfDevices);
	
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
            printf("[%3d%%] Copying %s to device\n", percent, CFStringGetCStringPtr(path, kCFStringEncodingUTF8));
			
        }
    }
}

void install_callback(CFDictionaryRef dict, int arg) {
    int percent;
    CFStringRef status = CFDictionaryGetValue(dict, CFSTR("Status"));
    CFNumberGetValue(CFDictionaryGetValue(dict, CFSTR("PercentComplete")), kCFNumberSInt32Type, &percent);
    printf("[%3d%%] %s\n", percent, CFStringGetCStringPtr(status, kCFStringEncodingUTF8));
}


void DemoThree(const char *appPath) {
	
	CFArrayRef devices = SDMMD_AMDCreateDeviceList();
	
	CFIndex numberOfDevices = CFArrayGetCount(devices);
	printf("%ld device(s) connected!\n",numberOfDevices);
		
	if (numberOfDevices) {
		// return type (uint32_t) corresponds with known return codes (SDMMD_Error.h)
		//sdmmd_return_t result;
		
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
									(SDMMD_AMDeviceInstallApplication(device, path, options, SDMMD_Default_install_callback, NULL) == kAMDSuccess ?
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
	
	CFIndex numberOfDevices = CFArrayGetCount(devices);
	printf("%ld device(s) connected!\n",numberOfDevices);
	
	
	if (numberOfDevices) {
		// return type (uint32_t) corresponds with known return codes (SDMMD_Error.h)
		sdmmd_return_t result;
		
		uint32_t index;
		// Iterating over connected devices
		for (index = 0; index < numberOfDevices; index++) {
			SDMMD_AMDeviceRef device = (SDMMD_AMDeviceRef)CFArrayGetValueAtIndex(devices, index);
			
			CFStringRef bundleId = CFStringCreateWithBytes(kCFAllocatorDefault, appPath, strlen(appPath), kCFStringEncodingUTF8, false);
			
			SDMMD_AMDebugConnectionRef debug;
			//result = SDMMD_StartDebuggingSessionOnDevice(device, &debug);
			//SDMMD_StartDebugger(debug, bundleId);
		}
	}
	
}

void AFCTest() {
	
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
			
			CFStringRef path = CFSTR("/");
			SDMMD_AMConnectionRef afcFd;
			if (SDM_MD_CallSuccessful(SDMMD_AMDeviceConnect(device))) {
				if (SDM_MD_CallSuccessful(SDMMD_AMDeviceStartSession(device))) {
					if (SDM_MD_CallSuccessful(SDMMD_AMDeviceStartService(device, CFSTR(AMSVC_AFC), NULL, &afcFd))) {
						SDMMD_AFCConnectionRef afc = SDMMD_AFCConnectionCreate(afcFd);
						SDMMD_AFCOperationRef deviceInfo = SDMMD_AFCOperationCreateGetDeviceInfo();
						SDMMD_AFCOperationRef response;
						result = SDMMD_AFCProcessOperation(afc, deviceInfo, &response);
						CFDataRef test = SDMMD_GetDataResponseFromOperation(response);
						CFShow(test);
						
					} else {
						printf("could not start service\n");
					}
					SDMMD_AMDeviceStopSession(device);
				} else {
					printf("could not start session\n");
				}
				SDMMD_AMDeviceDisconnect(device);
			}
		}
	}
}

void infoTest() {
	CFArrayRef devices = SDMMD_AMDCreateDeviceList();
	uint32_t numberOfDevices = CFArrayGetCount(devices);	
	if (numberOfDevices) {
		sdmmd_return_t result;
		
		uint32_t index;
		// Iterating over connected devices
		for (index = 0; index < numberOfDevices; index++) {
			
			// getting the device object from the array of connected devices
			SDMMD_AMDeviceRef device = (SDMMD_AMDeviceRef)CFArrayGetValueAtIndex(devices, index);
			
			// attempting to connect to the device
			result = SDMMD_AMDeviceConnect(device);
			if (SDM_MD_CallSuccessful(result)) {
				result = SDMMD_AMDeviceIsPaired(device);
				if (result) {
				 	
				}
				// creating a session to talk to more
				result = SDMMD_AMDeviceStartSession(device);
				if (SDM_MD_CallSuccessful(result)) {
					
					CFTypeRef deviceModelCF = SDMMD_AMDeviceCopyValue(device, NULL, CFSTR(kProductType));
					char *deviceModel = SDMCFStringGetString(deviceModelCF);
					CFRelease(deviceModelCF);
					
					CFTypeRef deviceSerialCF = SDMMD_AMDeviceCopyValue(device, NULL, CFSTR(kSerialNumber));
					char *deviceSerial = SDMCFStringGetString(deviceSerialCF);
					CFRelease(deviceSerialCF);
					
					CFTypeRef deviceOSCF = SDMMD_AMDeviceCopyValue(device, NULL, CFSTR(kProductVersion));
					char *deviceOS = SDMCFStringGetString(deviceOSCF);
					CFRelease(deviceOSCF);
					
					CFTypeRef deviceOSBuildCF = SDMMD_AMDeviceCopyValue(device, NULL, CFSTR(kBuildVersion));
					char *deviceOSBuild = SDMCFStringGetString(deviceOSBuildCF);
					CFRelease(deviceOSBuildCF);
					
					CFTypeRef deviceIsCellular = SDMMD_AMDeviceCopyValue(device, NULL, CFSTR(kTelephonyCapability));
					if (CFGetTypeID(deviceIsCellular) == CFBooleanGetTypeID()) {
						Boolean isCellular = CFBooleanGetValue(deviceIsCellular);
						if (isCellular) {
							CFTypeRef deviceCarrier = SDMMD_AMDeviceCopyValue(device, NULL, CFSTR(kCarrierBundleInfoArray));
							CFShow(deviceCarrier);
							
							CFTypeRef deviceIMEI = SDMMD_AMDeviceCopyValue(device, NULL, CFSTR(kInternationalMobileEquipmentIdentity));
							CFShow(deviceIMEI);
							
						} else {
							
						}
					} else {
						
					}
					result = SDMMD_AMDeviceStopSession(device);
				}
				
				// disconnect when finished
				result = SDMMD_AMDeviceDisconnect(device);
			}
		}

	}
	CFRelease(devices);
}
