/*
 *  SDMAFCLock.h
 *  SDM_MD_Demo
 *
 *  Created by sam on 5/26/13.
 *  Copyright 2013 Sam Marshall. All rights reserved.
 *
 */

#ifndef _SDMAFCLOCK_H_
#define _SDMAFCLOCK_H_

#import <stdint.h>

#pragma mark -
#pragma mark TYPES
#pragma mark -

typedef struct AFCLockClassHeader {
	unsigned char header[16];
} __attribute__ ((packed)) AFCLockClassHeader; // 0x10

typedef struct AFCLockClassBody {
	int32_t mutex_lock;			// 16
	unsigned char padding[44];	// 20
} __attribute__ ((packed)) AFCLockClassBody; // 0x40

typedef struct afc_lock {
	struct AFCLockClassHeader head;
	struct AFCLockClassBody body;
} __attribute__ ((packed)) afc_lock;

typedef struct afc_lock SDM_AFCLockClass;

#define SDM_AFCLockRef SDM_AFCLockClass*

#pragma mark -
#pragma mark FUNCTIONS
#pragma mark -

#endif