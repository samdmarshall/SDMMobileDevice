/*
 *  SDMAFCOperation.h
 *  SDM_MD_Demo
 *
 *  Created by sam on 5/26/13.
 *  Copyright 2013 Sam Marshall. All rights reserved.
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