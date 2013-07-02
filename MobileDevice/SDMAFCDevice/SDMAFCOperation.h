/*
 *  SDMAFCOperation.h
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

#ifndef _SDMAFCOPERATION_H_
#define _SDMAFCOPERATION_H_

#import "SDMAFCLock.h"

#pragma mark -
#pragma mark TYPES
#pragma mark -

typedef struct AFCOperationClassHeader {
	unsigned char header[16];
} __attribute__ ((packed)) AFCOperationClassHeader; // 0x10

typedef struct AFCOperationClassBody {
	void *context;			// 16
	int32_t state;			// 24
	int32_t a;				// 40
	// 48
	// 64
	void *packet_data;		// 72
	int32_t data_size;		// 80
	// 88 CFType
	// 96
	// 104
	// 112
	void *result;			// 240
	SDM_AFCLockRef lock0;	// 248
	//int32_t condition; 		// 256 AFCConditionCreate();
} __attribute__ ((packed)) AFCOperationClassBody; // 0xf8

typedef struct afc_operation {
	struct AFCOperationClassHeader head;
	struct AFCOperationClassBody body;
} __attribute__ ((packed)) afc_operation;

typedef struct afc_operation SDM_AFCOperationClass;

#define SDM_AFCOperationRef SDM_AFCOperationClass*

#pragma mark -
#pragma mark FUNCTIONS
#pragma mark -

#endif