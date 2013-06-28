/*
 *  SDMAFCIterator.h
 *  SDM_MD_Demo
 *
 *  Created by sam on 5/26/13.
 *  Copyright 2013 Sam Marshall. All rights reserved.
 *
 */

#ifndef _SDMAFCITERATOR_H_
#define _SDMAFCITERATOR_H_

#import <CoreFoundation/CoreFoundation.h>

#pragma mark -
#pragma mark TYPES
#pragma mark -

typedef struct AFCIteratorClassHeader {
	unsigned char header[16];		// CF Object header
} __attribute__ ((packed)) AFCIteratorClassHeader; // 0x10

typedef struct AFCIteratorClassBody {
	// 16
	CFArrayRef keys_values;			// 24
	int32_t count0;					// 32
	CFMutableDataRef key_buffer; 	// 40
	CFMutableDataRef value_buffer;	// 48
} __attribute__ ((packed)) AFCIteratorClassBody; // 0x28

typedef struct afc_iterator {
	struct AFCIteratorClassHeader head;
	struct AFCIteratorClassBody body;
} __attribute__ ((packed)) afc_iterator;

typedef struct afc_iterator SDM_AFCIteratorClass;

#define SDM_AFCIteratorRef SDM_AFCIteratorClass*

#pragma mark -
#pragma mark FUNCTIONS
#pragma mark -

#endif