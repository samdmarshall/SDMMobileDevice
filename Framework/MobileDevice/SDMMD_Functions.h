/*
 *  SDMMD_Functions.h
 *  SDMMobileDevice
 *
 * Copyright (c) 2013, Sam Marshall
* All rights reserved.
* 
* Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
* 
* 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
* 
* 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
* 
* 3. Neither the name of Sam Marshall nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
* 
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 */

#ifndef _SDM_MD_FUNCTIONS_H_
#define _SDM_MD_FUNCTIONS_H_

// Ignore OS X SSL deprecation warnings
#pragma clang diagnostic ignored "-Wdeprecated-declarations"

#include <CoreFoundation/CoreFoundation.h>
#include <openssl/crypto.h>
#include <pthread.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <stdio.h>
#include <uuid/uuid.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "SDMMD_MCP.h"
#include "SDMMD_Error.h"
#include "SDMMD_AMDevice.h"
#include "SDMMD_Applications.h"

#if WIN32
#define CFRangeMake(a, b) (CFRange){a, b}
#endif

// Missing base function: "mobdevlog"

__attribute__ ((unused)) static kern_return_t sdmmd_mutex_init(pthread_mutex_t thread) {
	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr);
	pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
	pthread_mutex_init(&thread, &attr);
	return pthread_mutexattr_destroy(&attr);
}

__attribute__ ((unused)) static int SDMMD__mutex_lock(pthread_mutex_t mutex) {
	return pthread_mutex_lock(&mutex);
}

__attribute__ ((unused)) static int SDMMD__mutex_unlock(pthread_mutex_t mutex) {
	return pthread_mutex_unlock(&mutex);
}

static const void* SDMMD___AppendValue(CFTypeRef append, CFMutableDataRef context) {
	// over-allocation, check hopper again because this seems to be inaccurate with the results of a previous version of MobileDevice
	if (CFGetTypeID(append) == CFNumberGetTypeID()) {
		if (CFNumberIsFloatType(append)) {
			float num = 0;
			CFNumberGetValue(append, kCFNumberDoubleType, &num);
			append = CFStringCreateWithFormat(kCFAllocatorDefault, NULL, CFSTR("%g"), num);
		} else {
			uint64_t num = 0;
			CFNumberGetValue(append, kCFNumberSInt64Type, &num);
			append = CFStringCreateWithFormat(kCFAllocatorDefault, NULL, CFSTR("%qi"), num);
		}
	} else if (CFGetTypeID(append) == CFBooleanGetTypeID()) {
		append = (CFEqual(append, kCFBooleanTrue) ? CFSTR("1") : CFSTR("0"));
	}
	if (CFGetTypeID(append) == CFStringGetTypeID()) {
		CFIndex length = CFStringGetLength(append),
				alloclen = CFStringGetMaximumSizeForEncoding(length, kCFStringEncodingUTF8),
				usedlen = 0;
		uint8_t *alloc = calloc(1, alloclen + 1);
		
		CFStringGetBytes(append, CFRangeMake(0, length), kCFStringEncodingUTF8, 0, false, alloc, alloclen, &usedlen);
		CFDataAppendBytes(context, alloc, usedlen);
		free(alloc);
	}
	return NULL;
}

__attribute__ ((unused)) static void SDMMD___ConvertDictEntry(const void* key, const void* value, void* context) {
	if (key && value) {
		SDMMD___AppendValue(key, context);
		SDMMD___AppendValue(value, context); 
	}
}

static CFMutableDictionaryRef SDMMD__CreateDictFromFileContents(char *path) {
	CFMutableDictionaryRef dict = NULL;
	if (path) {
		struct stat pathStat;
		ssize_t result = lstat(path, &pathStat);
		if (result != -1) {
			int ref = open(path, O_RDONLY);
			if (ref != -1) {
				struct stat fileStat;
				result = fstat(ref, &fileStat);
				if (result != -1) {
					unsigned char *data = calloc(1, fileStat.st_size);
					result = read(ref, data, fileStat.st_size);
					if (result == fileStat.st_size) {
						CFDataRef fileData = CFDataCreateWithBytesNoCopy(kCFAllocatorDefault, data, result, kCFAllocatorNull);
						if (fileData) {
							CFTypeRef propList = CFPropertyListCreateWithData(kCFAllocatorDefault, fileData, kCFPropertyListMutableContainersAndLeaves, NULL, NULL);
							if (propList) {
								if (CFGetTypeID(propList) == CFDictionaryGetTypeID()) {
									dict = CFDictionaryCreateMutableCopy(kCFAllocatorDefault, 0, propList);
								} else {
									printf("_CreateDictFromFileAtPath: Plist from file %s was not dictionary type.\n",path);
								}
							} else {
								printf("_CreateDictFromFileAtPath: Could not create plist from file %s.\n",path);
							}
							CFRelease(fileData);
						}
					} else {
						printf("_CreateDictFromFileAtPath: Could not read contents at file %s.\n",path);
					}
				} else {
					printf("_CreateDictFromFileAtPath: Could not fstat.\n");
				}
			} else {
				printf("_CreateDictFromFileAtPath: Could not open file %s\n",path);
			}
		} else {
			printf("_CreateDictFromFileAtPath: Could not lstat.\n");
		}
	}
	return dict;
}

__attribute__ ((unused)) static CFMutableDictionaryRef SDMMD__CreateMessageDict(CFStringRef type) {
	CFMutableDictionaryRef dict = CFDictionaryCreateMutable(NULL, 0, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
	if (dict) {
		CFDictionarySetValue(dict, CFSTR("Request"), type);
		CFDictionarySetValue(dict, CFSTR("ProtocolVersion"), CFSTR("2"));
		const char *appName = getprogname();
		if (appName) {
			CFStringRef name = CFStringCreateWithCString(kCFAllocatorDefault, appName, kCFStringEncodingUTF8);
			if (name) {
				CFDictionarySetValue(dict, CFSTR("Label"), name);
				CFRelease(name);
			}
		}
	}
	return dict;
	
}

static CFMutableDictionaryRef SDMMD_create_dict() {
	return CFDictionaryCreateMutable(NULL, 0, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
}

/*static void SDMMD_openSSLLockCallBack(int mode, int n, const char * file, int line) {
	if (mode & CRYPTO_LOCK)
		SDMMD__mutex_lock(SDMMobileDevice->sslLocks[n]);
	else
		SDMMD__mutex_unlock(SDMMobileDevice->sslLocks[n]);
}*/

__attribute__ ((unused)) static unsigned long SDMMD_openSSLThreadIDCallBack() {
	return (unsigned long)((uintptr_t)pthread_self()); // cast a pointer to uintptr_t before converting to integer
}

__attribute__ ((unused)) static uint32_t SDMMD_lockssl_init() {
	return SSL_get_ex_new_index(0, "peer certificate data", NULL, NULL, NULL);
}

__attribute__ ((unused)) static char *SDMMD_ssl_strerror(SSL *ssl, uint32_t ret) {
	static char buffer[200] = {0};
	int result = SSL_get_error(ssl, ret);
	char *err = NULL;
	
	switch (result) {
		case SSL_ERROR_NONE:
			break;
		case SSL_ERROR_SSL:
			if (ERR_peek_error()) {
				snprintf(buffer, 200, "SSL_ERROR_SSL (%s)", ERR_error_string(ERR_peek_error(), NULL));
				err = buffer;
			} else
				err = "SSL_ERROR_SSL unknown error";
			break;
		case SSL_ERROR_WANT_READ:
			err = "SSL_ERROR_WANT_READ";
			break;
		case SSL_ERROR_WANT_WRITE:
			err = "SSL_ERROR_WANT_WRITE";
			break;
		case SSL_ERROR_WANT_X509_LOOKUP:
			err = "SSL_ERROR_WANT_X509_LOOKUP";
			break;
		case SSL_ERROR_SYSCALL:
			if (ERR_peek_error() == 0 && ret == 0) {
				err = "SSL_ERROR_SYSCALL (Early EOF reached)";
			} else if (ERR_peek_error() == 0 && ret == -1) {
				snprintf(buffer, 200, "SSL_ERROR_SYSCALL errno (%s)", strerror(errno));
				err = buffer;
			} else if (ERR_peek_error() == 0) {
				err = "SSL_ERROR_SYSCALL (WTFERROR)";
			} else {
				snprintf(buffer, 200, "SSL_ERROR_SYSCALL internal (%s)", ERR_error_string(ERR_peek_error(), NULL));
				err = buffer;
			}
			break;
		case SSL_ERROR_ZERO_RETURN:
			err = "SSL_ERROR_ZERO_RETURN";
			break;
		case SSL_ERROR_WANT_CONNECT:
			err = "SSL_ERROR_WANT_CONNECT";
			break;
		case SSL_ERROR_WANT_ACCEPT:
			err = "SSL_ERROR_WANT_ACCEPT";
			break;
		default:
			ERR_print_errors_fp(stderr);
			fputc('\n', stderr);
			err = "Unknown SSL error type";
			break;
	}
	ERR_clear_error();
	return err;
}

__attribute__ ((unused)) static CFStringRef SDMGetCurrentDateString() {
	CFLocaleRef currentLocale = CFLocaleCopyCurrent();
	CFDateRef date = CFDateCreate(kCFAllocatorDefault, CFAbsoluteTimeGetCurrent());
	CFDateFormatterRef customDateFormatter = CFDateFormatterCreate(NULL, currentLocale, kCFDateFormatterNoStyle, kCFDateFormatterNoStyle);
	CFStringRef customDateFormat = CFSTR("yyyy-MM-dd*HH:mm:ss");
	CFDateFormatterSetFormat(customDateFormatter, customDateFormat);
	CFStringRef customFormattedDateString = CFDateFormatterCreateStringWithDate(NULL, customDateFormatter, date);
	CFRelease(currentLocale);
	CFRelease(date);
	CFRelease(customDateFormatter);
	return customFormattedDateString;
}

static char* SDMCFStringGetString(CFStringRef str) {
	CFIndex alloclen = CFStringGetMaximumSizeForEncoding(CFStringGetLength(str), CFStringGetFastestEncoding(str)) + 1;
	char *cstr = calloc(1, alloclen);
	CFStringGetCString(str, cstr, alloclen, CFStringGetFastestEncoding(str));
	return cstr;
}

__attribute__ ((unused)) static char* SDMCFURLGetString(CFURLRef url) {
	return SDMCFStringGetString(CFURLGetString(url));
}

static CFStringRef SDMMD__GetPairingRecordDirectoryPath() {
	return CFSTR("/var/db/lockdown");
}

static void SDMMD__PairingRecordPathForIdentifier(CFStringRef udid, char *path) {
	char buffer1[1024] = {0}, buffer2[1024] = {0};
	
	CFStringGetCString(SDMMD__GetPairingRecordDirectoryPath(), buffer1, 1024, kCFStringEncodingUTF8);
	CFStringGetCString(udid, buffer2, 1024, kCFStringEncodingUTF8);
	snprintf(path, 1024, "%s%c%s.plist", buffer1, '/', buffer2);
}

static CFTypeRef SDMMD_CreateUUID() {
	CFUUIDRef uuid = CFUUIDCreate(kCFAllocatorDefault);
	CFStringRef str = CFUUIDCreateString(kCFAllocatorDefault, uuid);
	CFRelease(uuid);
	return str;
}

static sdmmd_return_t SDMMD_store_dict(CFDictionaryRef dict, char *path, bool mode) {
	sdmmd_return_t result = 0;
	char buf[1025] = {0};
	
	// NOTE: Should implement all the error reporting here, including use of "mode"
	snprintf(buf, 1025, "%s.tmp", path);
	unlink(buf);
	int ref = open(buf, O_CREAT | O_EXCL | O_WRONLY);
	if (ref != -1) {
		CFDataRef xml = CFPropertyListCreateXMLData(kCFAllocatorDefault, dict);
		if (xml) {
			result = (sdmmd_return_t)write(ref, CFDataGetBytePtr(xml), CFDataGetLength(xml));
			result = rename(buf, path);
		}
		close(ref);
		result = chmod(path, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
	} else {
		result = kAMDUndefinedError;
	}
	return result;
}

static CFTypeRef SDMMD_AMDCopySystemBonjourUniqueID() {
	char record[1025] = {0};
	CFTypeRef value = NULL;
	SDMMD__PairingRecordPathForIdentifier(CFSTR("SystemConfiguration"), record);
	CFMutableDictionaryRef dict = SDMMD__CreateDictFromFileContents(record) ?: SDMMD_create_dict();
	if (dict) {
		value = CFDictionaryGetValue(dict, CFSTR("SystemBUID"));
		if (value == NULL) {
			value = SDMMD_CreateUUID();
			if (value) {
				CFDictionarySetValue(dict, CFSTR("SystemBUID"), value);
				SDMMD_store_dict(dict, record, true);
			} else {
				printf("SDMMD_AMDCopySystemBonjourUniqueID: Could not generate UUID!\n");
			}
		}
	}
	return value;
}

__attribute__ ((unused)) static sdmmd_return_t SDMMD__CreatePairingRecordFromRecordOnDiskForIdentifier(SDMMD_AMDeviceRef device, CFMutableDictionaryRef *dict) {
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
								printf("SDMMD__CreatePairingRecordFromRecordOnDiskForIdentifier: Could not store pairing record at '%s'.\n",path);
								result = kAMDPermissionError;
							} else {
								CFRetain(fileDict);
								*dict = fileDict;
							}
						}
					}
					CFRelease(fileDict);
				}
				CFRelease(bonjourId);
			}
		}
	}
	return result;
}

__attribute__ ((unused)) static CFArrayRef SDMMD_ApplicationLookupDictionary() {
	const void* values[6] = {CFSTR(kAppLookupKeyCFBundleIdentifier), CFSTR(kAppLookupKeyApplicationType), CFSTR(kAppLookupKeyCFBundleDisplayName), CFSTR(kAppLookupKeyCFBundleName), CFSTR(kAppLookupKeyContainer), CFSTR(kAppLookupKeyPath)};
	return CFArrayCreate(kCFAllocatorDefault, values, 6, &kCFTypeArrayCallBacks);
}

__attribute__ ((unused)) static CFURLRef SDMMD__AMDCFURLCreateFromFileSystemPathWithSmarts(CFStringRef path) {
	char cpath[1024] = {0};
	CFURLRef url = NULL;
	if (CFStringGetCString(path, cpath, 1024, kCFStringEncodingUTF8)) {
		struct stat buf;
		lstat(cpath, &buf);
		CFURLRef base = CFURLCreateWithString(kCFAllocatorDefault, CFSTR("file://localhost/"), NULL);
		url = CFURLCreateWithFileSystemPathRelativeToBase(kCFAllocatorDefault, path, kCFURLPOSIXPathStyle, S_ISDIR(buf.st_mode), base);
	}
	return url;
}

__attribute__ ((unused)) static CFURLRef SDMMD__AMDCFURLCreateWithFileSystemPathRelativeToBase(CFAllocatorRef allocator, CFStringRef path, CFURLPathStyle style, Boolean dir) {
	CFURLRef base = CFURLCreateWithString(allocator, CFSTR("file://localhost/"), NULL);
	CFURLRef url = CFURLCreateWithFileSystemPathRelativeToBase(allocator, path, style, dir, base);
	CFRelease(base);
	return url;
}

__attribute__ ((unused)) static Boolean SDMMD__AMDCFURLGetCStringForFileSystemPath(CFURLRef urlRef, char *cpath) {
	CFTypeRef url = CFURLCopyFileSystemPath(urlRef, kCFURLPOSIXPathStyle);
	if (url) {
		return CFStringGetCString(url, cpath, 1025, kCFStringEncodingUTF8);
	}
	return false;
}

#endif
