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
#include <openssl/crypto.h>
#include <openssl/pem.h>
#include <openssl/conf.h>
#include <openssl/x509v3.h>
#include <pthread.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
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
#include "Core.h"

#if WIN32
#define CFRangeMake(a, b) (CFRange){a, b}
#endif

// Missing base function: "mobdevlog"

ATR_UNUSED static kern_return_t sdmmd_mutex_init(pthread_mutex_t thread) {
	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr);
	pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
	pthread_mutex_init(&thread, &attr);
	return pthread_mutexattr_destroy(&attr);
}

ATR_UNUSED static int SDMMD__mutex_lock(pthread_mutex_t mutex) {
	return pthread_mutex_lock(&mutex);
}

ATR_UNUSED static int SDMMD__mutex_unlock(pthread_mutex_t mutex) {
	return pthread_mutex_unlock(&mutex);
}

static const void* SDMMD___AppendValue(CFTypeRef append, CFMutableDataRef context) {
	// over-allocation, check hopper again because this seems to be inaccurate with the results of a previous version of MobileDevice
	if (CFGetTypeID(append) == CFNumberGetTypeID()) {
		if (CFNumberIsFloatType(append)) {
			float num = 0;
			CFNumberGetValue(append, kCFNumberDoubleType, &num);
			append = CFStringCreateWithFormat(kCFAllocatorDefault, NULL, CFSTR("%g"), num);
		}
		else {
			uint64_t num = 0;
			CFNumberGetValue(append, kCFNumberSInt64Type, &num);
			append = CFStringCreateWithFormat(kCFAllocatorDefault, NULL, CFSTR("%qi"), num);
		}
	}
	else if (CFGetTypeID(append) == CFBooleanGetTypeID()) {
		append = (CFEqual(append, kCFBooleanTrue) ? CFSTR("1") : CFSTR("0"));
	}
	if (CFGetTypeID(append) == CFStringGetTypeID()) {
		CFIndex length = CFStringGetLength(append);
		CFIndex alloclen = CFStringGetMaximumSizeForEncoding(length, kCFStringEncodingUTF8);
		CFIndex usedlen = 0;
		
		uint8_t *alloc = calloc(1, alloclen + 1);
		
		CFStringGetBytes(append, CFRangeMake(0, length), kCFStringEncodingUTF8, 0, false, alloc, alloclen, &usedlen);
		CFDataAppendBytes(context, alloc, usedlen);
		Safe(free,alloc);
	}
	return NULL;
}

ATR_UNUSED static void SDMMD___ConvertDictEntry(const void* key, const void* value, void* context) {
	if (key && value) {
		SDMMD___AppendValue(key, context);
		SDMMD___AppendValue(value, context); 
	}
}

ATR_UNUSED static CFDataRef SDMMD__CreateDataFromFileContents(char *path) {
	CFDataRef dataBuffer = NULL;
	if (path) {
		struct stat pathStat;
		ssize_t result = lstat(path, &pathStat);
		if (result != -1) {
			int ref = open(path, O_RDONLY);
			if (ref != -1) {
				struct stat fileStat;
				result = fstat(ref, &fileStat);
				if (result != -1) {
					unsigned char *data = calloc(1, (unsigned long)fileStat.st_size);
					result = read(ref, data, (size_t)fileStat.st_size);
					if (result == fileStat.st_size) {
						dataBuffer = CFDataCreate(kCFAllocatorDefault, data, result);
					}
					else {
						printf("%s: Could not read contents at file %s.\n",__FUNCTION__,path);
					}
					Safe(free,data);
				}
				else {
					printf("%s: Could not fstat.\n",__FUNCTION__);
				}
				close(ref);
			}
			else {
				printf("%s: Could not open file %s\n",__FUNCTION__,path);
			}
		}
		else {
			printf("%s: Could not lstat.\n",__FUNCTION__);
		}
	}
	return dataBuffer;
}

static CFMutableDictionaryRef SDMMD__CreateDictFromFileContents(char *path) {
	CFMutableDictionaryRef dict = NULL;
	if (path) {
		CFDataRef fileData = SDMMD__CreateDataFromFileContents(path);
		if (fileData) {
			CFTypeRef propList = CFPropertyListCreateWithData(kCFAllocatorDefault, fileData, kCFPropertyListMutableContainersAndLeaves, NULL, NULL);
			if (propList) {
				if (CFGetTypeID(propList) == CFDictionaryGetTypeID()) {
					dict = CFDictionaryCreateMutableCopy(kCFAllocatorDefault, 0, propList);
				}
				else {
					printf("%s: Plist from file %s was not dictionary type.\n",__FUNCTION__,path);
				}
			}
			else {
				printf("%s: Could not create plist from file %s.\n",__FUNCTION__,path);
			}
			CFSafeRelease(propList);
		}
		CFSafeRelease(fileData);
	}
	return dict;
}

static CFMutableDictionaryRef SDMMD_create_dict() {
	return CFDictionaryCreateMutable(NULL, 0, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
}

ATR_UNUSED static CFMutableDictionaryRef SDMMD__CreateRequestDict(CFStringRef type) {
	CFMutableDictionaryRef dict = SDMMD_create_dict();
	if (dict) {
		CFDictionarySetValue(dict, CFSTR("Request"), type);
	}
	return dict;
}

ATR_UNUSED static CFMutableDictionaryRef SDMMD__CreateMessageDict(CFStringRef type) {
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

ATR_UNUSED static void SDMMD_openSSLLockCallBack(int mode, int n, const char * file, int line) {
	if (mode & CRYPTO_LOCK) {
		//SDMMD__mutex_lock(SDMMobileDevice->sslLocks[n]);
	}
	else {
		//SDMMD__mutex_unlock(SDMMobileDevice->sslLocks[n]);
 	}
}

ATR_UNUSED static unsigned long SDMMD_openSSLThreadIDCallBack() {
	return (unsigned long)((uintptr_t)pthread_self()); // cast a pointer to uintptr_t before converting to integer
}

ATR_UNUSED static uint32_t SDMMD_lockssl_init() {
	return SSL_get_ex_new_index(0, "peer certificate data", NULL, NULL, NULL);
}

ATR_UNUSED static char *SDMMD_ssl_strerror(SSL *ssl, uint32_t ret) {
	static char buffer[200] = {0};
	int result = SSL_get_error(ssl, ret);
	char *err = NULL;
	
	switch (result) {
		case SSL_ERROR_NONE: {
			break;
		}
		case SSL_ERROR_SSL: {
			if (ERR_peek_error()) {
				snprintf(buffer, 200, "SSL_ERROR_SSL (%s)", ERR_error_string(ERR_peek_error(), NULL));
				err = buffer;
			}
			else {
				err = "SSL_ERROR_SSL unknown error";
			}
			break;
		}
		case SSL_ERROR_WANT_READ: {
			err = "SSL_ERROR_WANT_READ";
			break;
		}
		case SSL_ERROR_WANT_WRITE: {
			err = "SSL_ERROR_WANT_WRITE";
			break;
		}
		case SSL_ERROR_WANT_X509_LOOKUP: {
			err = "SSL_ERROR_WANT_X509_LOOKUP";
			break;
		}
		case SSL_ERROR_SYSCALL: {
			if (ERR_peek_error() == 0 && ret == 0) {
				err = "SSL_ERROR_SYSCALL (Early EOF reached)";
			}
			else if (ERR_peek_error() == 0 && ret == -1) {
				snprintf(buffer, 200, "SSL_ERROR_SYSCALL errno (%s)", strerror(errno));
				err = buffer;
			}
			else if (ERR_peek_error() == 0) {
				err = "SSL_ERROR_SYSCALL (WTFERROR)";
			}
			else {
				snprintf(buffer, 200, "SSL_ERROR_SYSCALL internal (%s)", ERR_error_string(ERR_peek_error(), NULL));
				err = buffer;
			}
			break;
		}
		case SSL_ERROR_ZERO_RETURN: {
			err = "SSL_ERROR_ZERO_RETURN";
			break;
		}
		case SSL_ERROR_WANT_CONNECT: {
			err = "SSL_ERROR_WANT_CONNECT";
			break;
		}
		case SSL_ERROR_WANT_ACCEPT: {
			err = "SSL_ERROR_WANT_ACCEPT";
			break;
		}
		default: {
			ERR_print_errors_fp(stderr);
			fputc('\n', stderr);
			err = "Unknown SSL error type";
			break;
		}
	}
	ERR_clear_error();
	return err;
}

ATR_UNUSED static CFStringRef SDMGetCurrentDateString() {
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

static char* SDMCFStringGetString(CFStringRef str) {
	CFIndex alloclen = CFStringGetMaximumSizeForEncoding(CFStringGetLength(str), kCFStringEncodingUTF8) + 1;
	char *cstr = calloc(1, alloclen);
	CFStringGetCString(str, cstr, alloclen, kCFStringEncodingUTF8);
	return cstr;
}

ATR_UNUSED static char* SDMCFURLGetString(CFURLRef url) {
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

static CFStringRef SDMMD_CreateUUID() {
	CFUUIDRef uuid = CFUUIDCreate(kCFAllocatorDefault);
	CFStringRef str = CFUUIDCreateString(kCFAllocatorDefault, uuid);
	CFSafeRelease(uuid);
	return str;
}

static sdmmd_return_t SDMMD_store_dict(CFDictionaryRef dict, char *path, bool mode) {
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

static CFTypeRef SDMMD_AMDCopySystemBonjourUniqueID() {
	char record[1025] = {0};
	CFTypeRef value = NULL;
	SDMMD__PairingRecordPathForIdentifier(CFSTR("SystemConfiguration"), record);
	CFMutableDictionaryRef dict = SDMMD__CreateDictFromFileContents(record);
	if (!dict) {
		dict = SDMMD_create_dict();
	}
	if (dict) {
		value = CFStringCreateCopy(kCFAllocatorDefault, CFDictionaryGetValue(dict, CFSTR("SystemBUID")));
		if (value == NULL) {
			CFStringRef newUUID = SDMMD_CreateUUID();
			if (newUUID) {
				CFDictionarySetValue(dict, CFSTR("SystemBUID"), newUUID);
				SDMMD_store_dict(dict, record, true);
				
				value = newUUID;
			}
			else {
				printf("%s: Could not generate UUID!\n",__FUNCTION__);
			}
		}
		CFSafeRelease(dict);
	}
	return value;
}

ATR_UNUSED static sdmmd_return_t SDMMD__CreatePairingRecordFromRecordOnDiskForIdentifier(SDMMD_AMDeviceRef device, CFMutableDictionaryRef *dict) {
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
								printf("%s: Could not store pairing record at '%s'.\n",__FUNCTION__,path);
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

#define DefaultApplicationLookupDictionaryCount 6

ATR_UNUSED static CFArrayRef SDMMD_ApplicationLookupDictionary() {
	const void* values[DefaultApplicationLookupDictionaryCount] = {CFSTR(kAppLookupKeyCFBundleIdentifier), CFSTR(kAppLookupKeyApplicationType), CFSTR(kAppLookupKeyCFBundleDisplayName), CFSTR(kAppLookupKeyCFBundleName), CFSTR(kAppLookupKeyContainer), CFSTR(kAppLookupKeyPath)};
	return CFArrayCreate(kCFAllocatorDefault, values, DefaultApplicationLookupDictionaryCount, &kCFTypeArrayCallBacks);
}

ATR_UNUSED static CFURLRef SDMMD__AMDCFURLCreateFromFileSystemPathWithSmarts(CFStringRef path) {
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

ATR_UNUSED static CFURLRef SDMMD__AMDCFURLCreateWithFileSystemPathRelativeToBase(CFAllocatorRef allocator, CFStringRef path, CFURLPathStyle style, Boolean dir) {
	CFURLRef base = CFURLCreateWithString(allocator, CFSTR("file://localhost/"), NULL);
	CFURLRef url = CFURLCreateWithFileSystemPathRelativeToBase(allocator, path, style, dir, base);
	CFSafeRelease(base);
	return url;
}

ATR_UNUSED static Boolean SDMMD__AMDCFURLGetCStringForFileSystemPath(CFURLRef urlRef, char *cpath) {
	Boolean result = false;
	CFTypeRef url = CFURLCopyFileSystemPath(urlRef, kCFURLPOSIXPathStyle);
	if (url) {
		result = CFStringGetCString(url, cpath, 1024, kCFStringEncodingUTF8);
	}
	CFSafeRelease(url);
	return result;
}

ATR_UNUSED static void SDMMD_fire_callback(CallBack handle, void* unknown, CFStringRef status) {
	if (handle) {
		CFMutableDictionaryRef dict = SDMMD_create_dict();
		if (dict) {
			CFDictionarySetValue(dict, CFSTR("Status"), status);
		}
		handle(dict, unknown);
	}
}

ATR_UNUSED static void SDMMD_fire_callback_767f4(CallBack handle, void* unknown, uint32_t percent, CFStringRef string) {
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

ATR_UNUSED static int SDMMD__add_ext(X509 *cert, int flag, char *name) {
	int result = 0x0;
	X509V3_CTX ctx;
    X509V3_set_ctx(&ctx, cert, cert, 0x0, 0x0, 0x0);
    X509_EXTENSION *ex = X509V3_EXT_conf_nid(0x0, &ctx, flag, name);
    if (ex) {
		result = X509_add_ext(cert, ex, -1);
		X509_EXTENSION_free(ex);
    }
    return result;
}

ATR_UNUSED static CFDataRef SDMMD__create_data_from_bp(BIO* bio) {
	BUF_MEM * memptr = NULL;
    BIO_get_mem_ptr(bio, &memptr);
    CFDataRef data = CFDataCreate(kCFAllocatorDefault, (void *)memptr->data, memptr->length);
    return data;
}

ATR_UNUSED static CFDataRef SDMMD_CreateDataFromX509Certificate(X509 *cert) {
	BIO *bio = BIO_new(BIO_s_mem());
	CFDataRef data = NULL;
    if (bio) {
		int result = PEM_write_bio_X509(bio, cert);
		if (result) {
			data = SDMMD__create_data_from_bp(bio);
		}
		BIO_free(bio);
    }
    return data;
}

ATR_UNUSED static CFDataRef SDMMD_CreateDataFromPrivateKey(EVP_PKEY *key) {
    BIO_METHOD *method = BIO_s_mem();
	BIO *bio = BIO_new(method);
	CFDataRef data = NULL;
    if (bio) {
		int result = PEM_write_bio_PrivateKey(bio, key, 0x0, 0x0, 0x0, 0x0, 0x0);
		if (result) {
			data = SDMMD__create_data_from_bp(bio);
		}
		BIO_free(bio);
    }
    return data;
}

ATR_UNUSED static BIO* SDMMD__create_bio_from_data(CFDataRef data) {
	BIO *bio = NULL;
	if (data) {
		bio = BIO_new_mem_buf((void*)CFDataGetBytePtr(data),(uint32_t)CFDataGetLength(data));
	}
	return bio;
}

ATR_UNUSED static CFMutableDictionaryRef SDMMD__CreatePairingMaterial(CFDataRef devicePubkey) {
	CFMutableDictionaryRef record = NULL;
	RSA *rsaBIOData = NULL;
    BIO *deviceBIO = SDMMD__create_bio_from_data(devicePubkey);
    if (deviceBIO) {
	    PEM_read_bio_RSAPublicKey(deviceBIO, &rsaBIOData, NULL, NULL);
	    BIO_free(deviceBIO);
	}
	else {
		printf("Could not decode device public key\\n");
	}
	
	RSA *rootKeyPair = RSA_generate_key(2048, 65537, NULL, NULL);
	if (!rootKeyPair) {
		printf("Could not allocate root key pair\n");
	}
	
	RSA *hostKeyPair = RSA_generate_key(2048, 65537, NULL, NULL);
	if (!hostKeyPair) {
		printf("Could not allocate host key pair\n");
	}
	
	sdmmd_return_t result = kAMDSuccess;
	
	EVP_PKEY *rootEVP = EVP_PKEY_new();
	if (!rootEVP) {
		printf("Could not allocate root EVP key\\n");
	}
	else {
		result = EVP_PKEY_assign(rootEVP, EVP_CTRL_RAND_KEY, PtrCast(rootKeyPair,char*));
		if (!result) {
			printf("Could not assign root key pair\n");
		}
	}
	
	EVP_PKEY *hostEVP = EVP_PKEY_new();
	if (!hostEVP) {
		printf("Could not allocate host EVP key\\n");
	}
	else {
		result = EVP_PKEY_assign(hostEVP, EVP_CTRL_RAND_KEY, PtrCast(hostKeyPair,char*));
		if (!result) {
			printf("Could not assign host key pair\n");
		}
	}
	
	EVP_PKEY *deviceEVP = EVP_PKEY_new();
	if (!deviceEVP) {
		printf("Could not allocate device EVP key\\n");
	}
	else {
		result = EVP_PKEY_assign(deviceEVP, EVP_CTRL_RAND_KEY, PtrCast(rsaBIOData,char*));
		if (!result) {
			printf("Could not assign device key pair\n");
		}
	}
	
	X509 *rootX509 = X509_new();
	if (!rootX509) {
		printf("Could not create root X509\\n");
	}
	else {
		X509_set_pubkey(rootX509, rootEVP);
		X509_set_version(rootX509, 0x2);
		
		ASN1_INTEGER *rootSerial = X509_get_serialNumber(rootX509);
		ASN1_INTEGER_set(rootSerial, 0x0);
		
		ASN1_TIME* rootAsn1time = ASN1_TIME_new();
		ASN1_TIME_set(rootAsn1time, 0x0);
		X509_set_notBefore(rootX509, rootAsn1time);
		ASN1_TIME_set(rootAsn1time, 0x12cc0300);
		X509_set_notAfter(rootX509, rootAsn1time);
		ASN1_TIME_free(rootAsn1time);
		
		SDMMD__add_ext(rootX509, NID_basic_constraints, "critical,CA:TRUE");
		SDMMD__add_ext(rootX509, NID_subject_key_identifier, "hash");
		
		result = X509_sign(rootX509, rootEVP, EVP_sha1());
		if (!result) {
			printf("Could not sign root cert\\n");
		}
	}
	
	X509 *hostX509 = X509_new();
	if (!hostX509) {
		printf("Could not create host X509\\n");
	}
	else {
		X509_set_pubkey(hostX509, hostEVP);
		X509_set_version(hostX509, 0x2);
		
		ASN1_INTEGER *hostSerial = X509_get_serialNumber(hostX509);
		ASN1_INTEGER_set(hostSerial, 0x0);
		
		ASN1_TIME* hostAsn1time = ASN1_TIME_new();
		ASN1_TIME_set(hostAsn1time, 0x0);
		X509_set_notBefore(hostX509, hostAsn1time);
		ASN1_TIME_set(hostAsn1time, 0x12cc0300);
		X509_set_notAfter(hostX509, hostAsn1time);
		ASN1_TIME_free(hostAsn1time);
		
		SDMMD__add_ext(hostX509, NID_basic_constraints, "critical,CA:FALSE");
		SDMMD__add_ext(hostX509, NID_subject_key_identifier, "hash");
		SDMMD__add_ext(hostX509, NID_key_usage, "critical,digitalSignature,keyEncipherment");
		
		result = X509_sign(hostX509, rootEVP, EVP_sha1());
		if (!result) {
			printf("Could not sign host cert\\n");
		}
	}
	
	X509 *deviceX509 = X509_new();
	if (!deviceX509) {
		printf("Could not create device X509\\n");
	}
	else {
		X509_set_pubkey(deviceX509, deviceEVP);
		X509_set_version(deviceX509, 0x2);
		
		ASN1_INTEGER *deviceSerial = X509_get_serialNumber(deviceX509);
		ASN1_INTEGER_set(deviceSerial, 0x0);
		
		ASN1_TIME* deviceAsn1time = ASN1_TIME_new();
		ASN1_TIME_set(deviceAsn1time, 0x0);
		X509_set_notBefore(deviceX509, deviceAsn1time);
		ASN1_TIME_set(deviceAsn1time, 0x12cc0300);
		X509_set_notAfter(deviceX509, deviceAsn1time);
		ASN1_TIME_free(deviceAsn1time);
		
		SDMMD__add_ext(deviceX509, NID_basic_constraints, "critical,CA:FALSE");
		SDMMD__add_ext(deviceX509, NID_subject_key_identifier, "hash");
		SDMMD__add_ext(deviceX509, NID_key_usage, "critical,digitalSignature,keyEncipherment");
		
		result = X509_sign(deviceX509, rootEVP, EVP_sha1());
		if (!result) {
			printf("Could not sign device cert\\n");
		}
	}
	
	CFDataRef rootCert = SDMMD_CreateDataFromX509Certificate(rootX509);
	CFDataRef hostCert = SDMMD_CreateDataFromX509Certificate(hostX509);
	CFDataRef deviceCert = SDMMD_CreateDataFromX509Certificate(deviceX509);
	CFDataRef rootPrivKey = SDMMD_CreateDataFromPrivateKey(rootEVP);
	CFDataRef hostPrivKey = SDMMD_CreateDataFromPrivateKey(hostEVP);
	CFStringRef hostId = SDMMD_CreateUUID();
	
	record = CFDictionaryCreateMutable(kCFAllocatorDefault, 0x0, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
	if (record) {
		CFDictionarySetValue(record, CFSTR("RootCertificate"), rootCert);
		
		CFDictionarySetValue(record, CFSTR("HostCertificate"), hostCert);
		
		CFDictionarySetValue(record, CFSTR("DeviceCertificate"), deviceCert);
		
		CFDictionarySetValue(record, CFSTR("RootPrivateKey"), rootPrivKey);
		
		CFDictionarySetValue(record, CFSTR("HostPrivateKey"), hostPrivKey);
		
		CFDictionarySetValue(record, CFSTR("HostID"), hostId);
	}
	CFSafeRelease(rootCert);
	CFSafeRelease(hostCert);
	CFSafeRelease(deviceCert);
	CFSafeRelease(rootPrivKey);
	CFSafeRelease(hostPrivKey);
	CFSafeRelease(hostId);
	
	Safe(EVP_PKEY_free,rootEVP);
	Safe(EVP_PKEY_free,hostEVP);
	Safe(EVP_PKEY_free,deviceEVP);
	Safe(X509_free,rootX509);
	Safe(X509_free,hostX509);
	Safe(X509_free,deviceX509);
	
    return record;
}

#define HASH_LENGTH SHA_DIGEST_LENGTH

ATR_UNUSED static unsigned char * DataToSHA1(CFDataRef data) {
	unsigned char hash[HASH_LENGTH];
	SHA_CTX ctx;
	SHA1_Init(&ctx);
	for (size_t index = 0; index < CFDataGetLength(data); index++) {
		SHA1_Update(&ctx, PtrAdd(CFDataGetBytePtr(data), index), sizeof(unsigned char));
	}
	SHA1_Final(hash, &ctx);
	unsigned char *digest = (unsigned char *)malloc(HASH_LENGTH);
	memset(digest, 0, sizeof(unsigned char[HASH_LENGTH]));
	memcpy(digest, hash, sizeof(char[HASH_LENGTH]));
	return digest;
}

ATR_UNUSED static sdmmd_return_t SDMMD_AMDeviceDigestFile(CFStringRef path, unsigned char **digest) {
	sdmmd_return_t result = kAMDSuccess;
	CFDataRef data = CFDataCreateFromPath(path);
	if (data) {
		*digest = DataToSHA1(data);
	}
	else {
		result = kAMDDigestFailedError;
	}
	return result;
}

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

ATR_UNUSED static char* SDMMD_ResolveModelToName(CFStringRef model) {
	char *model_cstr = SDMCFStringGetString(model);
	char *model_name = "Unknown";
	for (uint32_t index = 0; index < SDM_MobileDevice_Model_ID_Count; index++) {
		if (strcmp(SDM_MobileDevice_Model_ID_Names[index].model, model_cstr) == 0) {
			model_name = SDM_MobileDevice_Model_ID_Names[index].name;
			break;
		}
	}
	return model_name;
}

#endif
