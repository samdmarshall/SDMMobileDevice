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

bool setAppleDevice(struct am_device *device) {
	if (device->device_id == sdm_test_device->ivars.device_id) {
		apple_test_device = device;
		dispatch_semaphore_signal(got_apple_device);
		return true;
	}
	return false;
}

void callback(struct AMDeviceNotificationCallbackInformation *CallbackInfo) {

	struct am_device *deviceHandle = CallbackInfo->deviceHandle;
	
	if (apple_test_device == NULL) {
		switch (CallbackInfo->msgType) {
			case ADNCI_MSG_CONNECTED: {
				if (sdm_test_device != NULL) {
					setAppleDevice(deviceHandle);
				}
				else {
					CFArrayRef device_array = SDMMD_AMDCreateDeviceList();
					
					CFIndex count = CFArrayGetCount(device_array);
					
					if (count != 0) {
						for (CFIndex index = 0; index < count; index++) {
							sdm_test_device = (SDMMD_AMDeviceRef)CFArrayGetValueAtIndex(device_array, index);
							if (!setAppleDevice(deviceHandle)) {
								sdm_test_device = NULL;
							}
							else {
								break;
							}
						}
					}
					
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
	
	
}

kern_return_t StartListener(dispatch_semaphore_t sema) {
	
	__block kern_return_t result = KERN_SUCCESS;
	
	SDMMobileDevice;
	
	CFArrayRef device_array = SDMMD_AMDCreateDeviceList();
	
	CFIndex count = CFArrayGetCount(device_array);
	
	if (count != 0) {
		SDMMD_AMDeviceRef sdm_device = (SDMMD_AMDeviceRef)CFArrayGetValueAtIndex(device_array, 0);
		sdm_test_device = sdm_device;
	}
	
	got_apple_device = dispatch_semaphore_create(0);
	
	dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
		struct am_device_notification *subscribe;
		kern_return_t sub_response = AMDeviceNotificationSubscribe((am_device_notification_callback)callback, 0,0,0, &subscribe);
		if (sub_response == KERN_SUCCESS) {
			CFRunLoopRun();
		}
		else {
			result = KERN_FAILURE;
		}
	});
	
	dispatch_semaphore_wait(got_apple_device, DISPATCH_TIME_FOREVER);
	dispatch_release(got_apple_device);
	dispatch_semaphore_signal(sema);
	
	return result;
}

#endif
