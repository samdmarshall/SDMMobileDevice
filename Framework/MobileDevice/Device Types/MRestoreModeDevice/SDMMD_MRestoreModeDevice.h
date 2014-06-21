/*
 *  SDMMD_MRestoreModeDevice.h
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

#ifndef _SDM_MD_MRESTOREMODEDEVICE_H_
#define _SDM_MD_MRESTOREMODEDEVICE_H_

#import "SDMMD_AMDevice.h"

struct AMRestoreModeDeviceClassHeader {
	unsigned char header[16];
} __attribute__ ((packed)) AMRestoreModeDeviceClassHeader; // 0x10

struct AMRestoreModeDeviceClassBody {
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

struct sdm_am_restore_device {
	struct AMRestoreModeDeviceClassHeader base;
	struct AMRestoreModeDeviceClassBody ivars;
} __attribute__ ((packed)) sdm_am_restore_device;

typedef struct sdm_am_restore_device SDMMD_AMRestoreModeDeviceClass;

#define SDMMD_AMRestoreModeDeviceRef SDMMD_AMRestoreModeDeviceClass*

#endif