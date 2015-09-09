/*
 *  SDMMD_Functions.c
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

#ifndef _SDM_MD_FUNCTIONS_C_
#define _SDM_MD_FUNCTIONS_C_

// Ignore OS X SSL deprecation warnings
#pragma clang diagnostic ignored "-Wdeprecated-declarations"

#include "SDMMD_Functions.h"
#include "SDMMD_AMDevice_Internal.h"
#include "SDMMD_SSL_Functions.h"
#include "Core.h"

// clang-format off
#if WIN32
#define CFRangeMake(a, b) (CFRange){a, b}
#endif
// clang-format on

// Missing base function: "mobdevlog"

kern_return_t sdmmd_mutex_init(pthread_mutex_t thread)
{
	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr);
	pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
	pthread_mutex_init(&thread, &attr);
	return pthread_mutexattr_destroy(&attr);
}

int SDMMD__mutex_lock(pthread_mutex_t mutex)
{
	return pthread_mutex_lock(&mutex);
}

int SDMMD__mutex_unlock(pthread_mutex_t mutex)
{
	return pthread_mutex_unlock(&mutex);
}

void SDMMD___AppendValue(CFTypeRef append, CFMutableDataRef context)
{
	// over-allocation, check hopper again because this seems to be inaccurate with the results of a previous version of MobileDevice
	CFTypeRef item = NULL;
	if (CFGetTypeID(append) == CFNumberGetTypeID()) {
		if (CFNumberIsFloatType(append)) {
			float num = 0;
			CFNumberGetValue(append, kCFNumberDoubleType, &num);
			item = CFStringCreateWithFormat(kCFAllocatorDefault, NULL, CFSTR("%g"), num);
		}
		else {
			uint64_t num = 0;
			CFNumberGetValue(append, kCFNumberSInt64Type, &num);
			item = CFStringCreateWithFormat(kCFAllocatorDefault, NULL, CFSTR("%qi"), num);
		}
	}
	else if (CFGetTypeID(append) == CFBooleanGetTypeID()) {
		item = (CFEqual(append, kCFBooleanTrue) ? CFSTR("1") : CFSTR("0"));
	}
	else if (CFGetTypeID(append) == CFStringGetTypeID()) {
		item = CFStringCreateCopy(kCFAllocatorDefault, append);
	}

	if (CFGetTypeID(item) == CFStringGetTypeID()) {
		CFIndex length = CFStringGetLength(item);
		CFIndex alloclen = CFStringGetMaximumSizeForEncoding(length, kCFStringEncodingUTF8);
		CFIndex usedlen = 0;

		uint8_t *alloc = calloc(1, alloclen + 1);

		CFStringGetBytes(item, CFRangeMake(0, length), kCFStringEncodingUTF8, 0, false, alloc, alloclen, &usedlen);
		CFDataAppendBytes(context, alloc, usedlen);
		Safe(free, alloc);
	}

	CFSafeRelease(item);
}

void SDMMD___ConvertDictEntry(const void *key, const void *value, void *context)
{
	if (key && value) {
		SDMMD___AppendValue(key, context);
		SDMMD___AppendValue(value, context);
	}
}

CFDataRef SDMMD__CreateDataFromFileContents(char *path)
{
	CFDataRef dataBuffer = NULL;
	unsigned char *data = NULL;

	EvalConditionalAndReturnWithMessage((path == NULL), "Invalid path.");

	struct stat pathStat;
	ssize_t result = lstat(path, &pathStat);
	EvalConditionalAndReturnWithMessage((result == -1), "Could not lstat.");

	int ref = open(path, O_RDONLY);
	EvalConditionalAndReturnWithMessage((ref == -1), "Could not open file.");

	struct stat fileStat;
	result = fstat(ref, &fileStat);
	EvalConditionalAndReturnWithMessage((result == -1), "Count not fstat.");

	data = calloc(1, (unsigned long)fileStat.st_size);
	result = read(ref, data, (size_t)fileStat.st_size);
	EvalConditionalAndReturnWithMessage((result != fileStat.st_size), "Could not read contents of file.");

	dataBuffer = CFDataCreate(kCFAllocatorDefault, data, result);

ExitLabel:
	Safe(free, data);

	return dataBuffer;
}

CFMutableDictionaryRef SDMMD__CreateDictFromFileContents(char *path)
{
	CFMutableDictionaryRef dict = NULL;
	CFDataRef fileData = NULL;
	CFTypeRef propList = NULL;

	EvalConditionalAndReturnWithMessage((path == NULL), "Invalid path.");

	fileData = SDMMD__CreateDataFromFileContents(path);
	EvalConditionalAndReturnWithMessage((fileData == NULL), "Could not read from file.");

	propList = CFPropertyListCreateWithData(kCFAllocatorDefault, fileData, kCFPropertyListMutableContainersAndLeaves, NULL, NULL);
	EvalConditionalAndReturnWithMessage((propList == NULL), "Could not create plist from file.");

	EvalConditionalAndReturnWithMessage((CFGetTypeID(propList) != CFDictionaryGetTypeID()), "Plist from file was not dictionary type.");

	dict = CFDictionaryCreateMutableCopy(kCFAllocatorDefault, 0, propList);

ExitLabel:
	CFSafeRelease(propList);
	CFSafeRelease(fileData);

	return dict;
}

CFMutableDictionaryRef SDMMD_create_dict()
{
	return CFDictionaryCreateMutable(NULL, 0, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
}

CFMutableDictionaryRef SDMMD__CreateRequestDict(CFStringRef type)
{
	CFMutableDictionaryRef dict = SDMMD_create_dict();
	if (dict) {
		CFDictionarySetValue(dict, CFSTR("Request"), type);
	}
	return dict;
}

CFMutableDictionaryRef SDMMD__CreateMessageDict(CFStringRef type)
{
	CFMutableDictionaryRef dict = SDMMD__CreateRequestDict(type);
	if (dict) {
		CFDictionarySetValue(dict, CFSTR("ProtocolVersion"), CFSTR("2"));
		const char *appName = getprogname();
		if (appName) {
			CFStringRef name = CFStringCreateWithCString(kCFAllocatorDefault, appName, kCFStringEncodingUTF8);
			if (name) {
				CFDictionarySetValue(dict, CFSTR("Label"), name);
				CFSafeRelease(name);
			}
		}
	}
	return dict;
}

CFStringRef SDMCreateCurrentDateString()
{
	CFLocaleRef currentLocale = CFLocaleCopyCurrent();
	CFDateRef date = CFDateCreate(kCFAllocatorDefault, CFAbsoluteTimeGetCurrent());
	CFDateFormatterRef customDateFormatter = CFDateFormatterCreate(NULL, currentLocale, kCFDateFormatterNoStyle, kCFDateFormatterNoStyle);
	CFStringRef customDateFormat = CFSTR("yyyy-MM-dd*HH:mm:ss");
	CFDateFormatterSetFormat(customDateFormatter, customDateFormat);
	CFStringRef customFormattedDateString = CFDateFormatterCreateStringWithDate(NULL, customDateFormatter, date);
	CFSafeRelease(currentLocale);
	CFSafeRelease(date);
	CFSafeRelease(customDateFormatter);
	return customFormattedDateString;
}

char *SDMCFStringGetString(CFStringRef str)
{
	CFIndex alloclen = CFStringGetMaximumSizeForEncoding(CFStringGetLength(str), kCFStringEncodingUTF8) + 1;
	char *cstr = calloc(1, alloclen);
	CFStringGetCString(str, cstr, alloclen, kCFStringEncodingUTF8);
	return cstr;
}

char *SDMCFURLGetString(CFURLRef url)
{
	return SDMCFStringGetString(CFURLGetString(url));
}

CFStringRef SDMMD__GetPairingRecordDirectoryPath()
{
	return CFSTR("/var/db/lockdown");
}

void SDMMD__PairingRecordPathForIdentifier(CFStringRef udid, char *path)
{
	char buffer1[1024] = {0}, buffer2[1024] = {0};

	CFStringGetCString(SDMMD__GetPairingRecordDirectoryPath(), buffer1, 1024, kCFStringEncodingUTF8);
	CFStringGetCString(udid, buffer2, 1024, kCFStringEncodingUTF8);
	snprintf(path, 1024, "%s%c%s.plist", buffer1, '/', buffer2);
}

sdmmd_return_t SDMMD_store_dict(CFDictionaryRef dict, char *path, bool mode)
{
	sdmmd_return_t result = kAMDSuccess;
	char buf[1025] = {0};

	// NOTE: Should implement all the error reporting here, including use of "mode"
	snprintf(buf, 1025, "%s.tmp", path);
	unlink(buf);
	mode_t fileMode = (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
	int ref = open(buf, O_CREAT | O_EXCL | O_WRONLY, fileMode);
	if (ref != -1) {
		CFDataRef xml = CFPropertyListCreateXMLData(kCFAllocatorDefault, dict);
		if (xml) {
			CFIndex length = CFDataGetLength(xml);
			result = (sdmmd_return_t)write(ref, CFDataGetBytePtr(xml), length);
			if (result == length) {
				rename(buf, path);
			}
		}
		close(ref);
		result = chmod(path, fileMode);
		CFSafeRelease(xml);
	}
	else {
		result = kAMDUndefinedError;
	}
	return result;
}

CFStringRef SDMMD_AMDCopySystemBonjourUniqueID()
{

	CFStringRef systemBUID = NULL;

	// Get SystemConfiguration dict from disk
	char sysconfigPath[1025] = {0};
	SDMMD__PairingRecordPathForIdentifier(CFSTR("SystemConfiguration"), sysconfigPath);
	CFMutableDictionaryRef sysconfigDict = SDMMD__CreateDictFromFileContents(sysconfigPath);

	if (sysconfigDict == NULL) {
		// No SystemConfiguration dict, create a new one
		sysconfigDict = SDMMD_create_dict();
	}

	if (sysconfigDict) {
		// Try to get SystemBUID value
		CFTypeRef value = CFDictionaryGetValue(sysconfigDict, CFSTR("SystemBUID"));

		if (value != NULL && CFGetTypeID(value) == CFStringGetTypeID()) {
			// Return existing value
			systemBUID = CFStringCreateCopy(kCFAllocatorDefault, value);
		}
		else {
			// No valid SystemBUID, generate a new one
			CFStringRef newUUID = SDMMD_CreateUUID();
			if (newUUID) {
				// New BUID generated, store to SystemConfiguration dict
				CFDictionarySetValue(sysconfigDict, CFSTR("SystemBUID"), newUUID);
				SDMMD_store_dict(sysconfigDict, sysconfigPath, true);

				systemBUID = newUUID;
			}
			else {
				printf("%s: Could not generate UUID!\n", __FUNCTION__);
			}
		}

		CFRelease(sysconfigDict);
	}

	return systemBUID;
}

sdmmd_return_t SDMMD__CreatePairingRecordFromRecordOnDiskForIdentifier(SDMMD_AMDeviceRef device, CFMutableDictionaryRef *dict)
{
	sdmmd_return_t result = kAMDInvalidArgumentError;
	char path[1024] = {0};

	if (device) {
		if (dict) {
			result = kAMDNoResourcesError;
			CFTypeRef bonjourId = SDMMD_AMDCopySystemBonjourUniqueID();
			if (bonjourId) {
				SDMMD__PairingRecordPathForIdentifier(device->ivars.unique_device_id, path);
				CFMutableDictionaryRef fileDict = SDMMD__CreateDictFromFileContents(path);
				result = kAMDMissingPairRecordError;
				if (fileDict) {
					CFTypeRef systemId = CFDictionaryGetValue(fileDict, CFSTR("SystemBUID"));
					if (systemId) {
						if (CFGetTypeID(systemId) == CFStringGetTypeID()) {
							CFDictionarySetValue(fileDict, CFSTR("SystemBUID"), bonjourId);
							result = SDMMD_store_dict(fileDict, path, true);
							if (result) {
								printf("%s: Could not store pairing record at '%s'.\n", __FUNCTION__, path);
								result = kAMDPermissionError;
							}
							else {
								CFRetain(fileDict);
								*dict = fileDict;
							}
						}
					}
					CFSafeRelease(fileDict);
				}
				CFSafeRelease(bonjourId);
			}
		}
	}
	return result;
}

CFArrayRef SDMMD_ApplicationLookupDictionary()
{
	// clang-format off
	const void *values[DefaultApplicationLookupDictionaryCount] = {
		CFSTR(kAppLookupKeyCFBundleExecutable),
		CFSTR(kAppLookupKeyCFBundleIdentifier),
		CFSTR(kAppLookupKeyApplicationType),
		CFSTR(kAppLookupKeyCFBundleDisplayName),
		CFSTR(kAppLookupKeyCFBundleName),
		CFSTR(kAppLookupKeyContainer),
		CFSTR(kAppLookupKeySignerIdentity),
		CFSTR(kAppLookupKeyApplicationDSID),
		CFSTR(kAppLookupKeyPath)
	};
	// clang-format on

	return CFArrayCreate(kCFAllocatorDefault, values, DefaultApplicationLookupDictionaryCount, &kCFTypeArrayCallBacks);
}

CFURLRef SDMMD__AMDCFURLCreateFromFileSystemPathWithSmarts(CFStringRef path)
{
	char cpath[1024] = {0};
	CFURLRef url = NULL;
	if (CFStringGetCString(path, cpath, 1024, kCFStringEncodingUTF8)) {
		struct stat buf;
		lstat(cpath, &buf);
		CFURLRef base = CFURLCreateWithString(kCFAllocatorDefault, CFSTR("file://localhost/"), NULL);
		url = CFURLCreateWithFileSystemPathRelativeToBase(kCFAllocatorDefault, path, kCFURLPOSIXPathStyle, S_ISDIR(buf.st_mode), base);
		CFSafeRelease(base);
	}
	return url;
}

CFURLRef SDMMD__AMDCFURLCreateWithFileSystemPathRelativeToBase(CFAllocatorRef allocator, CFStringRef path, CFURLPathStyle style, Boolean dir)
{
	CFURLRef base = CFURLCreateWithString(allocator, CFSTR("file://localhost/"), NULL);
	CFURLRef url = CFURLCreateWithFileSystemPathRelativeToBase(allocator, path, style, dir, base);
	CFSafeRelease(base);
	return url;
}

Boolean SDMMD__AMDCFURLGetCStringForFileSystemPath(CFURLRef urlRef, char *cpath)
{
	Boolean result = false;
	CFTypeRef url = CFURLCopyFileSystemPath(urlRef, kCFURLPOSIXPathStyle);
	if (url) {
		result = CFStringGetCString(url, cpath, 1024, kCFStringEncodingUTF8);
	}
	CFSafeRelease(url);
	return result;
}

void SDMMD_fire_callback(CallBack handle, void *unknown, CFStringRef status)
{
	if (handle) {
		CFMutableDictionaryRef dict = SDMMD_create_dict();
		if (dict) {
			CFDictionarySetValue(dict, CFSTR("Status"), status);
		}
		handle(dict, unknown);
	}
}

void SDMMD_fire_callback_767f4(CallBack handle, void *unknown, uint32_t percent, CFStringRef string)
{
	if (handle) {
		CFMutableDictionaryRef dict = SDMMD_create_dict();
		CFNumberRef num = CFNumberCreate(kCFAllocatorDefault, kCFNumberSInt32Type, &percent);
		if (dict) {
			CFDictionarySetValue(dict, CFSTR("Status"), string);
			CFDictionarySetValue(dict, CFSTR("PercentComplete"), num);
		}
		CFSafeRelease(num);
		handle(dict, unknown);
	}
}

sdmmd_return_t SDMMD_AMDeviceDigestFile(CFStringRef path, unsigned char **digest)
{
	sdmmd_return_t result = kAMDSuccess;
	CFDataRef data = CFDataCreateFromPath(path);
	if (data) {
		*digest = DataToSHA1(data);
	}
	else {
		result = kAMDDigestFailedError;
	}
	CFSafeRelease(data);

	return result;
}

sdmmd_return_t SDMMD_AMDeviceSHA256File(CFStringRef path, unsigned char **digest)
{
	sdmmd_return_t result = kAMDSuccess;
	CFDataRef data = CFDataCreateFromPath(path);
	if (data) {
		*digest = DataToSHA256(data);
	}
	else {
		result = kAMDDigestFailedError;
	}
	CFSafeRelease(data);

	return result;
}

char *SDMMD_ResolveModelToName(CFStringRef model)
{
	char *model_cstr = SDMCFStringGetString(model);
	char *model_name = "Unknown";
	for (uint32_t index = 0; index < SDM_MobileDevice_Model_ID_Count; index++) {
		if (strcmp(SDM_MobileDevice_Model_ID_Names[index].model, model_cstr) == 0) {
			model_name = SDM_MobileDevice_Model_ID_Names[index].name;
			break;
		}
	}
	Safe(free, model_cstr);

	return model_name;
}

#endif
