#include <CoreFoundation/CoreFoundation.h>
#include <SDMMobileDevice/SDMMobileDevice.h>

void DemoOne();
void DemoTwo();
void DemoThree();

int main (int argc, const char * argv[]) {
	// Needed to initialize the library and start the device listener (SDMMD_MCP.h)
	SDMMobileDevice;
	
	//DemoOne();
	//DemoTwo();
	//DemoThree();
	
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

void DemoThree() {
	
}