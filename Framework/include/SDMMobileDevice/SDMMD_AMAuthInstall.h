/*
 *  SDMMD_AMAuthInstall.h
 *  SDMMobileDevice
 *
 * Copyright (c) 2014, Samantha Marshall
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
 * 3. Neither the name of Samantha Marshall nor the names of its contributors may be used to endorse or promote products derived from this
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

#ifndef _SDM_MD_AMAUTHINSTALL_H_
#define _SDM_MD_AMAUTHINSTALL_H_

#include <CoreFoundation/CoreFoundation.h>

struct unknown {
	CFMutableDictionaryRef a;
	//NULL; // 0x8
	//NULL; // 0x10
};

struct AMAuthInstallClassHeader {
	char header[16];
} __attribute ((packed)) AMAuthInstallClassHeader;

struct AMAuthInstallClassBody {
	// 16
	int8_t VariantSpecifiesRestoreBehavior; // 40
	Boolean ApPersonalizationEnabled; // 41
	Boolean BasebandPersonalizationEnabled; // 42
	CFLocaleRef locale; // 48
	CFStringRef signingServer; // 56 (contains "http://gs.apple.com:80/")
	CFUUIDRef uuid; // 64
	struct unknown *buffer; // 88 (calloc(0x1, 0x18))
	CFDataRef VendorData; // 96
	CFStringRef fusingServer; // 120 (contains "http://17.209.80.108:8080/vegads/fuser")
	CFMutableDictionaryRef f; // 152
	CFMutableDictionaryRef g; // 160
	CFMutableDictionaryRef h; // 168
	int32_t debugFlags; // 296
	CFMutableDictionaryRef c; // 304
	CFMutableDictionaryRef keys; // 320
	CFMutableDictionaryRef digest; // 328
	CFMutableDictionaryRef e; // 360
	CFTypeRef entitlements; // 368
} __attribute ((packed)) AMAuthInstallClassBody;

struct AMAuthInstallClass {
	struct AMAuthInstallClassHeader header;
	struct AMAuthInstallClassBody ivars;
} __attribute ((packed)) AMAuthInstallClass;

typedef struct AMAuthInstallClass* AMAuthInstallClassRef;

#pragma mark -
#pragma mark Functions

void AMAuthInstallLogHandle(char *log_message);
AMAuthInstallClassRef AMAuthInstallCreate();

Boolean AMAuthInstallBasebandPersonalizationEnabled(AMAuthInstallClassRef install);
void AMAuthInstallBasebandEnabledPersonalization(AMAuthInstallClassRef install, Boolean enable);

Boolean AMAuthInstallApPersonalizationEnabled(AMAuthInstallClassRef install);
void AMAuthInstallEnableApPersonalization(AMAuthInstallClassRef install, Boolean enable);

void AMAuthInstallBasebandSetVendorData(AMAuthInstallClassRef install, CFDataRef vendor_data);

void AMAuthInstallSetSigningServer(AMAuthInstallClassRef install, CFStringRef host, uint32_t port);
void AMAuthInstallSetSigningServerURL(AMAuthInstallClassRef install, CFStringRef url);
void AMAuthInstallSetFusingServerURL(AMAuthInstallClassRef install, CFStringRef url);

#endif
