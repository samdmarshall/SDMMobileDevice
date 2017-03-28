/*
 *  SDMMD_SSL_Functions.c
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

#ifndef _SDM_MD_SSL_FUNCTIONS_C_
#define _SDM_MD_SSL_FUNCTIONS_C_

#include "SDMMD_SSL_Functions.h"
#include "Core.h"
#include "SDMMD_Error.h"

#pragma clang diagnostic ignored "-Wdeprecated-declarations"

typedef struct Hash_Type {
	FunctionPointer hash_init;
	FunctionPointer hash_update;
	FunctionPointer hash_close;
	int hash_length;
} ATR_PACK SDM_Hash_Type;

SDM_Hash_Type sha1_hash = {
	(FunctionPointer)SHA1_Init,
	(FunctionPointer)SHA1_Update,
	(FunctionPointer)SHA1_Final,
	SHA_DIGEST_LENGTH};

SDM_Hash_Type sha256_hash = {
	(FunctionPointer)SHA256_Init,
	(FunctionPointer)SHA256_Update,
	(FunctionPointer)SHA256_Final,
	SHA256_DIGEST_LENGTH};

CFStringRef SDMMD_CreateUUID()
{
	CFUUIDRef uuid = CFUUIDCreate(kCFAllocatorDefault);
	CFStringRef str = CFUUIDCreateString(kCFAllocatorDefault, uuid);
	CFSafeRelease(uuid);
	return str;
}

void SDMMD_openSSLLockCallBack(int mode, int n, const char *file, int line)
{
	if (mode & CRYPTO_LOCK) {
		//SDMMD__mutex_lock(SDMMobileDevice->sslLocks[n]);
	}
	else {
		//SDMMD__mutex_unlock(SDMMobileDevice->sslLocks[n]);
	}
}

unsigned long SDMMD_openSSLThreadIDCallBack()
{
	return (unsigned long)((uintptr_t)pthread_self()); // cast a pointer to uintptr_t before converting to integer
}

uint32_t SDMMD_lockssl_init()
{
	return SSL_get_ex_new_index(0, "peer certificate data", NULL, NULL, NULL);
}

char *SDMMD_ssl_strerror(SSL *ssl, uint32_t ret)
{
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

int SDMMD__add_ext(X509 *cert, int flag, char *name)
{
	int result = 0;
	X509V3_CTX ctx;
	X509V3_set_ctx(&ctx, cert, cert, 0, 0, 0);
	X509_EXTENSION *ex = X509V3_EXT_conf_nid(0, &ctx, flag, name);
	if (ex) {
		result = X509_add_ext(cert, ex, -1);
		X509_EXTENSION_free(ex);
	}
	return result;
}

CFDataRef SDMMD__create_data_from_bp(BIO *bio)
{
	BUF_MEM *memptr = NULL;
	BIO_get_mem_ptr(bio, &memptr);
	CFDataRef data = CFDataCreate(kCFAllocatorDefault, (void *)memptr->data, memptr->length);
	return data;
}

CFDataRef SDMMD_CreateDataFromX509Certificate(X509 *cert)
{
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

CFDataRef SDMMD_CreateDataFromPrivateKey(EVP_PKEY *key)
{
	BIO_METHOD *method = BIO_s_mem();
	BIO *bio = BIO_new(method);
	CFDataRef data = NULL;
	if (bio) {
		int result = PEM_write_bio_PrivateKey(bio, key, 0, 0, 0, 0, 0);
		if (result) {
			data = SDMMD__create_data_from_bp(bio);
		}
		BIO_free(bio);
	}
	return data;
}

BIO *SDMMD__create_bio_from_data(CFDataRef data)
{
	BIO *bio = NULL;
	if (data) {
		bio = BIO_new_mem_buf((void *)CFDataGetBytePtr(data), (uint32_t)CFDataGetLength(data));
	}
	return bio;
}

CFMutableDictionaryRef SDMMD__CreatePairingMaterial(CFDataRef devicePubkey)
{
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
		result = EVP_PKEY_assign(rootEVP, EVP_CTRL_RAND_KEY, PtrCast(rootKeyPair, char *));
		if (!result) {
			printf("Could not assign root key pair\n");
		}
	}

	EVP_PKEY *hostEVP = EVP_PKEY_new();
	if (!hostEVP) {
		printf("Could not allocate host EVP key\\n");
	}
	else {
		result = EVP_PKEY_assign(hostEVP, EVP_CTRL_RAND_KEY, PtrCast(hostKeyPair, char *));
		if (!result) {
			printf("Could not assign host key pair\n");
		}
	}

	EVP_PKEY *deviceEVP = EVP_PKEY_new();
	if (!deviceEVP) {
		printf("Could not allocate device EVP key\\n");
	}
	else {
		result = EVP_PKEY_assign(deviceEVP, EVP_CTRL_RAND_KEY, PtrCast(rsaBIOData, char *));
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
		X509_set_version(rootX509, 2);

		ASN1_INTEGER *rootSerial = X509_get_serialNumber(rootX509);
		ASN1_INTEGER_set(rootSerial, 0);

		ASN1_TIME *rootAsn1time = ASN1_TIME_new();
		ASN1_TIME_set(rootAsn1time, 0);
		X509_set_notBefore(rootX509, rootAsn1time);
		ASN1_TIME_set(rootAsn1time, 0x12cc0300); // 60 sec * 60 minutes * 24 hours * 365 days * 10 years
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
		X509_set_version(hostX509, 2);

		ASN1_INTEGER *hostSerial = X509_get_serialNumber(hostX509);
		ASN1_INTEGER_set(hostSerial, 0);

		ASN1_TIME *hostAsn1time = ASN1_TIME_new();
		ASN1_TIME_set(hostAsn1time, 0);
		X509_set_notBefore(hostX509, hostAsn1time);
		ASN1_TIME_set(hostAsn1time, 0x12cc0300); // 60 sec * 60 minutes * 24 hours * 365 days * 10 years
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
		X509_set_version(deviceX509, 2);

		ASN1_INTEGER *deviceSerial = X509_get_serialNumber(deviceX509);
		ASN1_INTEGER_set(deviceSerial, 0);

		ASN1_TIME *deviceAsn1time = ASN1_TIME_new();
		ASN1_TIME_set(deviceAsn1time, 0);
		X509_set_notBefore(deviceX509, deviceAsn1time);
		ASN1_TIME_set(deviceAsn1time, 0x12cc0300); // 60 sec * 60 minutes * 24 hours * 365 days * 10 years
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

	record = CFDictionaryCreateMutable(kCFAllocatorDefault, 0, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
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

	Safe(EVP_PKEY_free, rootEVP);
	Safe(EVP_PKEY_free, hostEVP);
	Safe(EVP_PKEY_free, deviceEVP);
	Safe(X509_free, rootX509);
	Safe(X509_free, hostX509);
	Safe(X509_free, deviceX509);

	return record;
}

unsigned char *DataToHash(CFDataRef data, Pointer context, SDM_Hash_Type info)
{
	unsigned char *digest = (unsigned char *)calloc(info.hash_length, sizeof(unsigned char));
	if (info.hash_init != NULL && info.hash_update != NULL && info.hash_close != NULL && info.hash_length != 0) {
		unsigned char hash[info.hash_length];

		info.hash_init(context);
		for (size_t index = 0; index < CFDataGetLength(data); index++) {
			info.hash_update(context, PtrAdd(CFDataGetBytePtr(data), index), sizeof(unsigned char));
		}
		info.hash_close(hash, context);

		memcpy(digest, hash, sizeof(char[info.hash_length]));
	}
	return digest;
}

unsigned char *DataToSHA1(CFDataRef data)
{
	SHA_CTX ctx;
	return DataToHash(data, PtrCast(&ctx, Pointer), sha1_hash);
}

unsigned char *DataToSHA256(CFDataRef data)
{
	SHA256_CTX ctx;
	return DataToHash(data, PtrCast(&ctx, Pointer), sha256_hash);
}

#endif