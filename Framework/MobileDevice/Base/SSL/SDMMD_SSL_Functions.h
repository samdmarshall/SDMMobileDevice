/*
 *  SDMMD_SSL_Functions.h
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

#ifndef _SDM_MD_SSL_FUNCTIONS_H_
#define _SDM_MD_SSL_FUNCTIONS_H_

#include <openssl/crypto.h>
#include <openssl/pem.h>
#include <openssl/conf.h>
#include <openssl/x509v3.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <pthread.h>
#include <CoreFoundation/CoreFoundation.h>

#define SHA1_HASH_LENGTH SHA_DIGEST_LENGTH

CF_RETURNS_RETAINED CFStringRef SDMMD_CreateUUID();
void SDMMD_openSSLLockCallBack(int mode, int n, const char * file, int line);
unsigned long SDMMD_openSSLThreadIDCallBack();
uint32_t SDMMD_lockssl_init();
char* SDMMD_ssl_strerror(SSL *ssl, uint32_t ret);
int SDMMD__add_ext(X509 *cert, int flag, char *name);
CF_RETURNS_RETAINED CFDataRef SDMMD__create_data_from_bp(BIO* bio);
CF_RETURNS_RETAINED CFDataRef SDMMD_CreateDataFromX509Certificate(X509 *cert);
CF_RETURNS_RETAINED CFDataRef SDMMD_CreateDataFromPrivateKey(EVP_PKEY *key);
BIO* SDMMD__create_bio_from_data(CFDataRef data);
CF_RETURNS_RETAINED CFMutableDictionaryRef SDMMD__CreatePairingMaterial(CFDataRef devicePubkey);
unsigned char * DataToSHA1(CFDataRef data);
unsigned char * DataToSHA256(CFDataRef data);

#endif