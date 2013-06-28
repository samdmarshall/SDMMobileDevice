/*
 *  SDMMDConnection.h
 *  SDM_MD_Demo
 *
 *  Created by sam on 6/5/13.
 *  Copyright 2013 Sam Marshall. All rights reserved.
 *
 */

#ifndef _SDMMDCONNECTION_H_
#define _SDMMDCONNECTION_H_

#import <CoreFoundation/CoreFoundation.h>
#import "SDMMDError.h"
#import "SDMMDevice.h"

typedef struct AMConnectionClassHeader {
	unsigned char header[16];		// AMConnectionClass CF Header 
} __attribute ((packed)) AMConnectionClassHeader; // size 0x10

typedef struct AMConnectionClassBody {
	uint32_t socket;			// 16
	unsigned char unknown0[4];	// 20
	int32_t secure_context;		// 24
	unsigned char unknown1[4];	// 28
	int8_t one0;				// 32
	int8_t one1;				// 33
	unsigned char unknown2[2];	// 34
	int32_t device_id; 			// 36
	unsigned char unknown3[112];// 40
} __attribute__ ((packed)) AMConnectionClassBody; // size 0x98

typedef struct am_connection {
	struct AMConnectionClassHeader head;
	struct AMConnectionClassBody body;
} __attribute__ ((packed)) am_connection;

/* Classes */
typedef struct am_connection SDM_AMConnectionClass;

#define SDM_AMConnectionRef SDM_AMConnectionClass*

#pragma mark -
#pragma mark FUNCTIONS
#pragma mark -
SDM_AMConnectionRef SDM_AMDServiceConnectionCreate(CFAllocatorRef allocator, SDM_AMDeviceRef device, CFDictionaryRef dict);
sdmmd_return_t SDM_AMDServiceConnectionInvalidate(SDM_AMConnectionRef connection);

#endif