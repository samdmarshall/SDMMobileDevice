/*
 *  SDMAFCDevice.h
 *  SDM_MD_Demo
 *
 *  Created by sam on 5/25/13.
 *  Copyright 2013 Sam Marshall. All rights reserved.
 *
 */

#ifndef _SDMAFCDEVICE_H_
#define _SDMAFCDEVICE_H_

#import <CoreFoundation/CoreFoundation.h>
#import "SDMMDError.h"
#import "SDMAFCLock.h"

#pragma mark -
#pragma mark TYPES
#pragma mark -

#define SDM_AFCConditionRef int32_t

typedef struct AFCConnectionClassHeader {
	unsigned char header[16];
} __attribute__ ((packed)) AFCConnectionClassHeader; // 0x10

typedef struct AFCConnectionClassBody {
	int32_t state;							// 16 set to 0x1
	SDM_AFCConditionRef cond0;				// 24
	int8_t is_valid1;						// 32
	SDM_AFCConditionRef cond_signal;		// 40
	CFSocketRef socket;						// 48
	int32_t socket0;						// 56
	int8_t connection_active;				// 60
	unsigned char padding0[3];				// 61		
	int32_t status_ptr;						// 64
	int8_t e;								// 68
	int32_t last_error;						// 72 last error
	int32_t operation_count;				// 80 on sending operation is incremented
	int32_t fs_block_size;					// 88
	int32_t socket_block_size;				// 96
	int32_t io_timeout;						// 104 set to 0x3c
	int32_t h;								// 112 set to 0x1
	int32_t context;						// 120
	SDM_AFCLockRef lock0;					// 136
	SDM_AFCLockRef lock1;					// 144
	CFMutableArrayRef operation_enqueue;	// 152
	CFMutableArrayRef operation_dequeue;	// 160
	CFMutableSetRef file_descriptors;		// 168
	CFMutableSetRef directory_enumerator; 	// 176
	CFRunLoopSourceRef source;				// 192
	CFRunLoopRef runloop;					// 200
	int32_t secure_context;					// 216
	void *callback;							// 224
	int32_t queue;							// 1264
} __attribute__ ((packed)) AFCConnectionClassBody; // 0x4f0

typedef struct afc_connection {
	struct AFCConnectionClassHeader head;
	struct AFCConnectionClassBody body;
} __attribute__ ((packed)) afc_connection;

typedef struct afc_connection SDM_AFCConnectionClass;

#define SDM_AFCConnectionRef SDM_AFCConnectionClass*

#pragma mark -
#pragma mark FUNCTIONS
#pragma mark -

sdmmd_afc_return_t AFCDeviceInfoOpen(SDM_AFCConnectionRef conn, CFDictionaryRef *dict);

#endif