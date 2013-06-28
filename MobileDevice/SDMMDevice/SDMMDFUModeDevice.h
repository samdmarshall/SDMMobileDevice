/*
 *  SDMMDFUModeDevice.h
 *  SDM_MD_Demo
 *
 *  Created by sam on 5/26/13.
 *  Copyright 2013 Sam Marshall. All rights reserved.
 *
 */

#ifndef _SDMMDFUMODEDEVICE_H_
#define _SDMMDFUMODEDEVICE_H_

#import "SDMMDevice.h"

#pragma mark -
#pragma mark TYPES
#pragma mark -

typedef struct AMDFUModeDeviceClassHeader {
	unsigned char header[16];	// 16
} __attribute__ ((packed)) AMDFUModeDeviceClassHeader; // 0x10

typedef struct AMDFUModeDeviceClassBody {
	// 16
	// 24
	uint32_t current_progress;	// 32
	uint32_t overall_progress;	// 36
	// 40
	int8_t a;					// 48
	int32_t valid_product; 		// 52
	// 56
} __attribute__ ((packed)) AMDFUModeDeviceClassBody; // 0x30

typedef struct am_dfu_device {
	struct AMDFUModeDeviceClassHeader head;
	struct AMDFUModeDeviceClassBody body;
} __attribute__ ((packed)) am_dfu_device;

typedef struct am_dfu_device SDM_AMDFUModeDeviceClass;

#define SDM_AMDFUModeDeviceRef SDM_AMDFUModeDeviceClass*

#pragma mark -
#pragma mark FUNCTIONS
#pragma mark -

sdmmd_return_t AMDFUModeDeviceCreate(CFAllocatorRef allocator, void *unknown);

#endif