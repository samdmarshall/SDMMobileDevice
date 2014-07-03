/*
 *  SDMMD_MDFUModeDevice.h
 *  SDMMobileDevice
 *
 * Copyright (c) 2014, Sam Marshall
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
 * 3. Neither the name of Sam Marshall nor the names of its contributors may be used to endorse or promote products derived from this
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

#ifndef _SDM_MD_MDFUMODEDEVICE_H_
#define _SDM_MD_MDFUMODEDEVICE_H_

#include "SDMMD_AMDevice.h"

#pragma mark -
#pragma mark TYPES
#pragma mark -

struct AMDFUModeDeviceClassHeader {
	unsigned char header[16];	// 16
} __attribute__ ((packed)) AMDFUModeDeviceClassHeader; // 0x10

struct AMDFUModeDeviceClassBody {
	// 16
	// 24
	uint32_t current_progress;	// 32
	uint32_t overall_progress;	// 36
	// 40
	int8_t a;					// 48
	int32_t valid_product; 		// 52
	// 56
} __attribute__ ((packed)) AMDFUModeDeviceClassBody; // 0x30

struct am_dfu_device {
	struct AMDFUModeDeviceClassHeader base;
	struct AMDFUModeDeviceClassBody ivars;
} __attribute__ ((packed)) am_dfu_device;

typedef struct am_dfu_device SDMMD_AMDFUModeDeviceClass;

#define SDMMD_AMDFUModeDeviceRef SDMMD_AMDFUModeDeviceClass*

#pragma mark -
#pragma mark FUNCTIONS
#pragma mark -

sdmmd_return_t SDMMD_AMDFUModeDeviceCreate(CFAllocatorRef allocator, void *unknown);

#endif