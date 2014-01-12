//
//  CFDictionaryAddition.c
//  Core
//
//  Created by Sam Marshall on 1/4/14.
//  Copyright (c) 2014 Sam Marshall. All rights reserved.
//

#ifndef Core_CFDictionaryAddition_c
#define Core_CFDictionaryAddition_c

#include "CFDictionaryAddition.h"
#include "Pointer.h"
#include "Logging.h"
#include "CFTypeAddition.h"

extern void PrintCFTypeInternalFormat(CFTypeRef value, uint32_t depth);

void PrintCFDictionaryInternalFormatting(CFDictionaryRef dictionary, uint32_t depth) {
	CFIndex keyCount = CFDictionaryGetCount(dictionary);
	Pointer keys[keyCount];
	Pointer values[keyCount];
	CFDictionaryGetKeysAndValues(dictionary, PtrCast(keys,const void**), PtrCast(values,const void**));
	for (uint32_t i = 0x0; i < keyCount; i++) {
		CFStringRef key = PtrCast(keys[i],CFStringRef);
		CFTypeRef value = PtrCast(values[i],CFTypeRef);
		PrintDepth(depth,"%s: ",(char*)CFStringGetCStringPtr(key,kCFStringEncodingUTF8));
		PrintCFTypeInternalFormat(value, depth);
	}
}

void PrintCFDictionary(CFDictionaryRef dictionary) {
	PrintCFDictionaryInternalFormatting(dictionary, 0x0);
}

#endif