/*
 *  SDMMRestoreModeDevice.h
 *  SDM_MD_Demo
 *
 *  Created by sam on 5/26/13.
 *  Copyright 2013 Sam Marshall. All rights reserved.
 *
 */

#ifndef _SDMMRESTOREMODEDEVICE_H_
#define _SDMMRESTOREMODEDEVICE_H_

#import "SDMMDevice.h"

typedef struct AMRestoreModeDeviceClassHeader {
	unsigned char header[16];
} __attribute__ ((packed)) AMRestoreModeDeviceClassHeader; // 0x10

typedef struct AMRestoreModeDeviceClassBody {
	// 10
	// 24
	int32_t d;			// 32
	int32_t a;			// 40
	int32_t b;			// 44
	int32_t c;			// 48
	// 56
	// 64
	// 72
	// 80
	int32_t e;			// 88
	int32_t f;			// 92
	int32_t g;			// 96
	int64_t h;			// 104
	int64_t i;			// 112
	int64_t j;			// 120
	int64_t k;			// 128
	// 136
} __attribute__ ((packed)) AMRestoreModeDeviceClassBody; // 0x88

typedef struct am_restore_device {
	struct AMRestoreModeDeviceClassHeader head;
	struct AMRestoreModeDeviceClassBody body;
} __attribute__ ((packed)) am_restore_device;

typedef struct am_restore_device SDM_AMRestoreModeDeviceClass;

#define SDM_AMRestoreModeDeviceRef SDM_AMRestoreModeDeviceClass*

#endif