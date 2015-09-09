/*
 *  SDMMD_MRecoveryModeDevice.h
 *  SDMMobileDevice
 *
 * Copyright (c) 2014, Samantha Marshall
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the 
 * following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer 
 * 		in the documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of Samantha Marshall nor the names of its contributors may be used to endorse or promote products derived from this
 * 		software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef _SDM_MD_MRECOVERYMODEDEVICE_H_
#define _SDM_MD_MRECOVERYMODEDEVICE_H_

#include "SDMMD_AMDevice.h"
#include "SDMMD_MRUSBDevice.h"

#pragma mark -
#pragma mark TYPES
#pragma mark -

struct AMRecoveryModeDeviceClassHeader {
	unsigned char header[16];
} ATR_PACK AMRecoveryModeDeviceClassHeader; // size 0x10

struct AMRecoveryModeDeviceClassBody {
	// 16
	// 24
	int32_t current_progress;		// 32
	int32_t overall_progress;		// 36
	SDMMD_AMRUSBDeviceRef device;   // 40 what is this?
	int32_t zero0;					// 48
	int8_t device_is_recovery_mode; // 56 product Id is equal to 0x1281
	int32_t d;						// 57
	int8_t c;						// 61
	// 64
	// 72
	int8_t firmware_info_loaded;		  // 80
	int8_t bootstrap_only;				  // 84
	int32_t chip_id;					  // 88
	int32_t board_id;					  // 92
	int32_t security_epoch;				  // 96
	CFStringRef serial_number;			  // 112
	CFStringRef IMEI;					  // 120
	int64_t ecid;						  // 136
	int32_t security_domain;			  // 148
	int8_t production_mode;				  // 152
										  // 160
} ATR_PACK AMRecoveryModeDeviceClassBody; // size 0x98

struct sdm_am_recovery_device {
	struct AMRecoveryModeDeviceClassHeader base;
	struct AMRecoveryModeDeviceClassBody ivars;
} ATR_PACK sdm_am_recovery_device;

typedef struct sdm_am_recovery_device SDMMD_AMRecoveryModeDeviceClass;

#define SDMMD_AMRecoveryModeDeviceRef SDMMD_AMRecoveryModeDeviceClass *

#pragma mark -
#pragma mark FUNCTIONS
#pragma mark -

sdmmd_return_t SDMMD_AMRecoveryModeDeviceCreate(CFAllocatorRef allocator, void *unknown);
sdmmd_return_t SDMMD_AMRestoreGetResolvedBoardConfig(int32_t board_id, int32_t chip_id, void *unknown);

#endif