//
//  listener.c
//  SDM_MD_Tests
//
//  Created by Sam Marshall on 3/29/14.
//  Copyright (c) 2014 Sam Marshall. All rights reserved.
//

#ifndef SDM_MD_Tests_listener_c
#define SDM_MD_Tests_listener_c

#include "listener.h"
#include "test_Type.h"

dispatch_semaphore_t got_apple_device;

struct AMDeviceNotificationCallbackInformation {
	void* deviceHandle;
	uint32_t msgType;
};

void callback(struct AMDeviceNotificationCallbackInformation *CallbackInfo) {

	struct am_device *deviceHandle = CallbackInfo->deviceHandle;
	
	switch (CallbackInfo->msgType) {
		case ADNCI_MSG_CONNECTED: {
			if (deviceHandle->device_id == sdm_test_device->ivars.device_id) {
				apple_test_device = deviceHandle;
				dispatch_semaphore_signal(got_apple_device);
			}
			break;
		}
		case ADNCI_MSG_DISCONNECTED: {
			break;
		}
		case ADNCI_MSG_UNKNOWN: {
			break;
		}
		default: {
			break;
		}
	}
	
}

void StartListener(dispatch_semaphore_t sema) {
	
	SDMMobileDevice;
	
	CFArrayRef device_array = SDMMD_AMDCreateDeviceList();
	
	CFIndex count = CFArrayGetCount(device_array);
	
	if (count != 0) {
		SDMMD_AMDeviceRef sdm_device = (SDMMD_AMDeviceRef)CFArrayGetValueAtIndex(device_array, 0);
		sdm_test_device = sdm_device;
	}
	
	got_apple_device = dispatch_semaphore_create(0);
	
	dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
		void *subscribe;
		kern_return_t sub_response = AMDeviceNotificationSubscribe(callback, 0,0,0, &subscribe);
		CFRunLoopRun();
	});
	
	dispatch_semaphore_wait(got_apple_device, DISPATCH_TIME_FOREVER);
	dispatch_release(got_apple_device);
	dispatch_semaphore_signal(sema);

}

#endif
