/*
 *  SDMMD_Functions.h
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

#ifndef _SDM_MD_FUNCTIONS_H_
#define _SDM_MD_FUNCTIONS_H_

// Ignore OS X SSL deprecation warnings
#pragma clang diagnostic ignored "-Wdeprecated-declarations"

#include <CoreFoundation/CoreFoundation.h>
#include "SDMMD_Types.h"
#include <pthread.h>
#include <stdio.h>
#include <uuid/uuid.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "SDMMD_Types.h"
#include "SDMMD_MCP.h"
#include "SDMMD_Error.h"
#include "SDMMD_AMDevice.h"
#include "SDMMD_Applications.h"
#include "CFTypeAddition.h"
#include "SDMMD_SSL_Functions.h"

#if WIN32
#define CFRangeMake(a, b) (CFRange){a, b}
#endif

// Missing base function: "mobdevlog"

kern_return_t sdmmd_mutex_init(pthread_mutex_t thread);
int SDMMD__mutex_lock(pthread_mutex_t mutex);
int SDMMD__mutex_unlock(pthread_mutex_t mutex);
const void* SDMMD___AppendValue(CFTypeRef append, CFMutableDataRef context);
void SDMMD___ConvertDictEntry(const void* key, const void* value, void* context);
CFDataRef SDMMD__CreateDataFromFileContents(char *path);
CFMutableDictionaryRef SDMMD__CreateDictFromFileContents(char *path);
CFMutableDictionaryRef SDMMD_create_dict();
CFMutableDictionaryRef SDMMD__CreateRequestDict(CFStringRef type);
CFMutableDictionaryRef SDMMD__CreateMessageDict(CFStringRef type);
CFStringRef SDMGetCurrentDateString();
char* SDMCFStringGetString(CFStringRef str);
char* SDMCFURLGetString(CFURLRef url);
CFStringRef SDMMD__GetPairingRecordDirectoryPath();
void SDMMD__PairingRecordPathForIdentifier(CFStringRef udid, char *path);
sdmmd_return_t SDMMD_store_dict(CFDictionaryRef dict, char *path, bool mode);
CFTypeRef SDMMD_AMDCopySystemBonjourUniqueID();
sdmmd_return_t SDMMD__CreatePairingRecordFromRecordOnDiskForIdentifier(SDMMD_AMDeviceRef device, CFMutableDictionaryRef *dict);

#define DefaultApplicationLookupDictionaryCount 6

CFArrayRef SDMMD_ApplicationLookupDictionary();
CFURLRef SDMMD__AMDCFURLCreateFromFileSystemPathWithSmarts(CFStringRef path);
CFURLRef SDMMD__AMDCFURLCreateWithFileSystemPathRelativeToBase(CFAllocatorRef allocator, CFStringRef path, CFURLPathStyle style, Boolean dir);
Boolean SDMMD__AMDCFURLGetCStringForFileSystemPath(CFURLRef urlRef, char *cpath);
void SDMMD_fire_callback(CallBack handle, void* unknown, CFStringRef status);
void SDMMD_fire_callback_767f4(CallBack handle, void* unknown, uint32_t percent, CFStringRef string);
sdmmd_return_t SDMMD_AMDeviceDigestFile(CFStringRef path, unsigned char **digest);

enum SDM_MobileDevice_Model_ID {
	SDM_MobileDevice_Model_ID_Invalid,
	SDM_MobileDevice_Model_ID_iPad1_1,
	SDM_MobileDevice_Model_ID_iPad2_1,
	SDM_MobileDevice_Model_ID_iPad2_2,
	SDM_MobileDevice_Model_ID_iPad2_3,
	SDM_MobileDevice_Model_ID_iPad2_4,
	SDM_MobileDevice_Model_ID_iPad2_5,
	SDM_MobileDevice_Model_ID_iPad2_6,
	SDM_MobileDevice_Model_ID_iPad2_7,
	SDM_MobileDevice_Model_ID_iPad3_1,
	SDM_MobileDevice_Model_ID_iPad3_2,
	SDM_MobileDevice_Model_ID_iPad3_3,
	SDM_MobileDevice_Model_ID_iPad3_4,
	SDM_MobileDevice_Model_ID_iPad3_5,
	SDM_MobileDevice_Model_ID_iPad3_6,
	SDM_MobileDevice_Model_ID_iPad4_1,
	SDM_MobileDevice_Model_ID_iPad4_2,
	SDM_MobileDevice_Model_ID_iPad4_3,
	SDM_MobileDevice_Model_ID_iPad4_4,
	SDM_MobileDevice_Model_ID_iPad4_5,
	SDM_MobileDevice_Model_ID_iPad4_6,
	SDM_MobileDevice_Model_ID_iPhone1_1,
	SDM_MobileDevice_Model_ID_iPhone1_2,
	SDM_MobileDevice_Model_ID_iPhone2_1,
	SDM_MobileDevice_Model_ID_iPhone3_1,
	SDM_MobileDevice_Model_ID_iPhone3_2,
	SDM_MobileDevice_Model_ID_iPhone3_3,
	SDM_MobileDevice_Model_ID_iPhone4_1,
	SDM_MobileDevice_Model_ID_iPhone5_1,
	SDM_MobileDevice_Model_ID_iPhone5_2,
	SDM_MobileDevice_Model_ID_iPhone5_3,
	SDM_MobileDevice_Model_ID_iPhone5_4,
	SDM_MobileDevice_Model_ID_iPhone6_1,
	SDM_MobileDevice_Model_ID_iPhone6_2,
	SDM_MobileDevice_Model_ID_iPod1_1,
	SDM_MobileDevice_Model_ID_iPod2_1,
	SDM_MobileDevice_Model_ID_iPod3_1,
	SDM_MobileDevice_Model_ID_iPod4_1,
	SDM_MobileDevice_Model_ID_iPod5_1,
	SDM_MobileDevice_Model_ID_Count
};

struct SDM_MobileDevice_Model_Info {
	char *model;
	char *name;
};

static struct SDM_MobileDevice_Model_Info SDM_MobileDevice_Model_ID_Names[SDM_MobileDevice_Model_ID_Count] = {
	{"", "Invalid"},
	{"iPad1,1", "iPad 1"},
	{"iPad2,1", "iPad 2"},
	{"iPad2,2", "iPad 2"},
	{"iPad2,3", "iPad 2"},
	{"iPad2,4", "iPad 2"},
	{"iPad2,5", "iPad Mini 1"},
	{"iPad2,6", "iPad Mini 1"},
	{"iPad2,7", "iPad Mini 1"},
	{"iPad3,1", "iPad 3"},
	{"iPad3,2", "iPad 3"},
	{"iPad3,3", "iPad 3"},
	{"iPad3,4", "iPad 4"},
	{"iPad3,5", "iPad 4"},
	{"iPad3,6", "iPad 4"},
	{"iPad4,1", "iPad Air"},
	{"iPad4,2", "iPad Air"},
	{"iPad4,3", "iPad Air"},
	{"iPad4,4", "iPad Mini 2"},
	{"iPad4,5", "iPad Mini 2"},
	{"iPad4,6", "iPad Mini 2"},
	{"iPhone1,1", "iPhone 2G"},
	{"iPhone1,2", "iPhone 3G"},
	{"iPhone2,1", "iPhone 3GS"},
	{"iPhone3,1", "iPhone 4"},
	{"iPhone3,2", "iPhone 4"},
	{"iPhone3,3", "iPhone 4"},
	{"iPhone4,1", "iPhone 4S"},
	{"iPhone5,1", "iPhone 5"},
	{"iPhone5,2", "iPhone 5"},
	{"iPhone5,3", "iPhone 5C"},
	{"iPhone5,4", "iPhone 5C"},
	{"iPhone6,1", "iPhone 5S"},
	{"iPhone6,2", "iPhone 5S"},
	{"iPod1,1", "iPod Touch 1G"},
	{"iPod2,1", "iPod Touch 2G"},
	{"iPod3,1", "iPod Touch 3G"},
	{"iPod4,1", "iPod Touch 4G"},
	{"iPod5,1", "iPod Touch 5G"}
};

char* SDMMD_ResolveModelToName(CFStringRef model);

#endif
