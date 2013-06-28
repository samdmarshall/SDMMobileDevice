/*
 *  SDMMRecoveryModeDevice.h
 *  SDM_MD_Demo
 *
 *  Created by sam on 5/25/13.
 *  Copyright 2013 Sam Marshall. All rights reserved.
 *
 */

#ifndef _SDMMRECOVERYMODEDEVICE_H_
#define _SDMMRECOVERYMODEDEVICE_H_

#import "SDMMDevice.h"
#import "SDMMRUSBDevice.h"

#pragma mark -
#pragma mark TYPES
#pragma mark -

typedef struct AMRecoveryModeDeviceClassHeader {
	unsigned char header[16];
} __attribute__ ((packed)) AMRecoveryModeDeviceClassHeader; // size 0x10

typedef struct AMRecoveryModeDeviceClassBody {
	// 16
	// 24
	int32_t current_progress;		// 32
	int32_t overall_progress;		// 36
	SDM_AMRUSBDeviceRef device;		// 40 what is this?
	int32_t zero0;					// 48
	int8_t device_is_recovery_mode;	// 56 product Id is equal to 0x1281
	int32_t d;						// 57
	int8_t c; 						// 61
	// 64
	// 72
	int8_t firmware_info_loaded;	// 80
	int32_t chip_id;				// 88
	int32_t board_id;				// 92
	int32_t security_epoch;			// 96
	CFStringRef serial_number;		// 112
	CFStringRef IMEI;				// 120
	int64_t ecid;					// 136
	int32_t security_domain;		// 148
	int8_t production_mode;			// 152
	// 160
} __attribute__ ((packed)) AMRecoveryModeDeviceClassBody; // size 0x98

typedef struct am_recovery_device {
	struct AMRecoveryModeDeviceClassHeader head;
	struct AMRecoveryModeDeviceClassBody body;
} __attribute__ ((packed)) am_recovery_device;

typedef struct am_recovery_device SDM_AMRecoveryModeDeviceClass;

#define SDM_AMRecoveryModeDeviceRef SDM_AMRecoveryModeDeviceClass*


#pragma mark -
#pragma mark FUNCTIONS
#pragma mark -

sdmmd_return_t AMRecoveryModeDeviceCreate(CFAllocatorRef allocator, void *unknown);
sdmmd_return_t AMRestoreGetResolvedBoardConfig(int32_t board_id, int32_t chip_id, void *unknown);

#endif