//
//  main.c
//  SDM_MD_Tests
//
//  Created by Samantha Marshall on 3/29/14.
//  Copyright (c) 2014 Samantha Marshall. All rights reserved.
//

#include <CoreFoundation/CoreFoundation.h>
#include <SDMMobileDevice/SDMMobileDevice.h>
#include "tests.h"
#include "listener.h"

extern struct am_device *apple_test_device;
extern SDMMD_AMDeviceRef sdm_test_device;

int main(int argc, const char * argv[]) {
	
	dispatch_semaphore_t sema = dispatch_semaphore_create(0);
	
	printf("Looking for attached device...\n");
	dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
		kern_return_t result = StartListener(sema);
		if (result != KERN_SUCCESS) {
			printf("Error in starting the MobileDevice device listener!\n");
			exit(0);
		}
	});
	dispatch_time_t timeout = dispatch_time(DISPATCH_TIME_NOW, 35LL * NSEC_PER_SEC);
	dispatch_semaphore_wait(sema, timeout);
	dispatch_release(sema);
	
	if (apple_test_device != NULL && sdm_test_device != NULL) {
		CFTypeRef deviceUDID = SDMMD_AMDeviceCopyUDID(sdm_test_device);
		if (!deviceUDID) {
			deviceUDID = SDMMD_AMDeviceCopyValue(sdm_test_device, NULL, CFSTR(kUniqueDeviceID));
		}
		printf("Found device with ID: %s\n",CFStringGetCStringPtr(deviceUDID,kCFStringEncodingUTF8));
		CFSafeRelease(deviceUDID);
		
		RunFunctionalityTests(apple_test_device, sdm_test_device);
		printf("\n");
		RunCompatibilityTests(apple_test_device, sdm_test_device);
	}
	else {
		printf("Please attach a device to run tests\n");
	}
	
    return 0;
}

