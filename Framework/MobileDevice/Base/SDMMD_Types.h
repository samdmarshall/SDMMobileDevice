/*
 *  SDMMD_Types.h
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

#ifndef _SDM_MD_TYPES_H_
#define _SDM_MD_TYPES_H_

#include <CoreFoundation/CoreFoundation.h>

#define kTestFileForAFC "iTunes_Control/iTunes/IC-Info.sidv"
#define kTestCreateFileForAFC "sdm_md_test_file.dat"

typedef void (*CallBack)(CFDictionaryRef dict, void* arg);

__attribute__ ((unused)) static void SDMMD_Default_AFC_CopyFile_Callback(CFDictionaryRef dict, void* arg) {
    uint32_t percent = 0;
    CFStringRef status = CFDictionaryGetValue(dict, CFSTR("Status"));
	if (CFEqual(status, CFSTR("CopyingFile"))) {
		CFStringRef local_path = CFDictionaryGetValue(dict, CFSTR("LocalPath"));
		CFStringRef remote_path = CFDictionaryGetValue(dict, CFSTR("RemotePath"));
		CFNumberGetValue(CFDictionaryGetValue(dict, CFSTR("PercentComplete")), kCFNumberSInt32Type, &percent);
		printf("[%3d%%] Copying \"%s\" to \"%s\"\n", percent, CFStringGetCStringPtr(local_path, kCFStringEncodingUTF8), CFStringGetCStringPtr(remote_path, kCFStringEncodingUTF8));
	}
}

__attribute__ ((unused)) static void SDMMD_Default_mount_callback(CFDictionaryRef dict, void* arg) {
	CFStringRef status = CFDictionaryGetValue(dict, CFSTR("Status"));
	if (CFEqual(status, CFSTR("LookingUpImage"))) {
		printf("[  0%%] Looking up developer disk image\n");
	} else if (CFEqual(status, CFSTR("CopyingImage"))) {
		printf("[ 30%%] Copying \"DeveloperDiskImage.dmg\" to device\n");
    } else if (CFEqual(status, CFSTR("MountingImage"))) {
		printf("[ 90%%] Mounting developer disk image\n");
	}
}

__attribute__ ((unused)) static void SDMMD_Default_transfer_callback(CFDictionaryRef dict, void* arg) {
    int percent;
    CFStringRef status = CFDictionaryGetValue(dict, CFSTR("Status"));
    CFNumberGetValue(CFDictionaryGetValue(dict, CFSTR("PercentComplete")), kCFNumberSInt32Type, &percent);
    if (CFEqual(status, CFSTR("CopyingFile"))) {
        CFStringRef path = CFDictionaryGetValue(dict, CFSTR("Path"));
        if (!CFStringHasSuffix(path, CFSTR(".ipa"))) {
            printf("[%3d%%] Copying %s to device\n", percent, CFStringGetCStringPtr(path, kCFStringEncodingUTF8));
			
        }
    }
}

__attribute__ ((unused)) static void SDMMD_Default_install_callback(CFDictionaryRef dict, void* arg) {
    int percent;
    CFStringRef status = CFDictionaryGetValue(dict, CFSTR("Status"));
    CFNumberGetValue(CFDictionaryGetValue(dict, CFSTR("PercentComplete")), kCFNumberSInt32Type, &percent);
    printf("[%3d%%] %s\n", percent, CFStringGetCStringPtr(status, kCFStringEncodingUTF8));
}

#endif
