//
//  main.c
//  SDM_MD_Tests
//
//  Created by Sam Marshall on 3/29/14.
//  Copyright (c) 2014 Sam Marshall. All rights reserved.
//

#include <CoreFoundation/CoreFoundation.h>
#include <SDMMobileDevice/SDMMobileDevice.h>
#include "tests.h"
#include "listener.h"

extern struct am_device *apple_test_device;
extern SDMMD_AMDeviceRef sdm_test_device;

int main(int argc, const char * argv[]) {
	
	dispatch_semaphore_t sema = dispatch_semaphore_create(0);
	
	dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
		kern_return_t result = StartListener(sema);
		if (result != KERN_SUCCESS) {
			exit(0);
		}
	});
	dispatch_semaphore_wait(sema, DISPATCH_TIME_FOREVER);
	dispatch_release(sema);

	Run_CompatibilityTests(apple_test_device, sdm_test_device);
	
    return 0;
}

