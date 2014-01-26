//
//  usbmuxd.h
//  usbmuxd
//
//  Created by Sam Marshall on 1/25/14.
//  Copyright (c) 2014 Sam Marshall. All rights reserved.
//

#ifndef usbmuxd_usbmuxd_h
#define usbmuxd_usbmuxd_h

#include <CoreFoundation/CoreFoundation.h>

struct USBMuxAgentClass {
	uint32_t socket;
	bool isActive;
	dispatch_queue_t socketQueue;
	dispatch_source_t socketSource;
	dispatch_semaphore_t semaphore;
} USBMuxAgentClass;

typedef struct USBMuxAgentClass* USBMuxAgentRef;

void StartMux();

#endif
