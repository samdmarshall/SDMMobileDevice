/*
 *  SDMMD_RestorableDevice.h
 *  SDM_MD_Demo
 *
 *  Copyright (c) 2013, Sam Marshall
 *  All rights reserved.
 * 
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 *  1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
 *  3. All advertising materials mentioning features or use of this software must display the following acknowledgement:
 *  	This product includes software developed by the Sam Marshall.
 *  4. Neither the name of the Sam Marshall nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
 * 
 *  THIS SOFTWARE IS PROVIDED BY Sam Marshall ''AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL Sam Marshall BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 */

#ifndef _SDM_MD_MRESTORABLEDEVICE_H_
#define _SDM_MD_MRESTORABLEDEVICE_H_

typedef struct AMRestorableDeviceClassHeader {
	unsigned char header[16];
} __attribute__ ((packed)) AMRestorableDeviceClassHeader; // 0x10

typedef struct AMRestorableDeviceClassBody {
	// 10
	// 24
	// 32
	// 40
	// 48
	// 56
	// 64
	// 72
	// 80
	// 88
	// 96
} __attribute__ ((packed)) AMRestorableDeviceClassBody; // 0x58

typedef struct am_restorable_device {
	struct AMRestorableDeviceClassHeader base;
	struct AMRestorableDeviceClassBody ivars;
} __attribute__ ((packed)) am_restorable_device;

typedef struct am_restorable_device SDMMD_AMRestorableDeviceClass;

#define SDMMD_AMRestorableDeviceRef SDMMD_AMRestorableDeviceClass*

#endif