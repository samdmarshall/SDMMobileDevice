/*
 *  SDMMD_AMDevice_Class_Internal.h
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

#ifndef SDMMobileDevice_Framework_SDMMD_AMDevice_Internal_h
#define SDMMobileDevice_Framework_SDMMD_AMDevice_Internal_h

#include <CoreFoundation/CoreFoundation.h>
#include <SDMMobileDevice/CFRuntime.h>
#include <openssl/ssl.h>
#include <SDMMobileDevice/SDMMD_Error.h>

struct SDMMD_lockdown_conn_internal {
	uint64_t connection;			// 0
	SSL *ssl; 						// 8
	uint64_t *pointer;				// 16
	uint64_t length;				// 24
} __attribute__ ((packed));

struct AMDeviceClassHeader {
	unsigned char header[16];		// AMDeviceClass CF Header
} __attribute ((packed)) AMDeviceClassHeader; // size 0x10

enum {
	kAMDeviceConnectionTypeWiFi = 0,
	kAMDeviceConnectionTypeUSB = 1,
};
typedef int32_t AMDeviceConnectionType;

struct AMDeviceClassBody {
	int32_t device_id;							// 16
	int32_t location_id;						// 20
	uint16_t product_id;						// 24
	int16_t padding0;							// 26
	int32_t unknown1;							// 28
	CFStringRef unique_device_id;				// 32
	AMDeviceConnectionType connection_type;		// 40 (1 for USB, 0 for WiFi)
	SDMMD_lockdown_conn *lockdown_conn;			// 48
	CFStringRef session;						// 56 needs to be not zero in AMDeviceSecureStartService  -- connection
	char mutx[4];								// 64 "MUTX" string
	pthread_mutex_t mutex_lock;					// 68
	CFStringRef service_name;					// 128 bonjour service name
	int32_t interface_index;					// 136
	int8_t device_active;						// 140
	unsigned char unknown7[3];					// 141
	int64_t unknown8;							// 144
	CFDataRef unknownData;						// 152
	CFDataRef network_address;					// 160 stores a sockaddr_storage
} __attribute__ ((packed)) AMDeviceClassBody; // size 0x98

struct sdmmd_am_device {
	CFRuntimeBase base;
	struct AMDeviceClassBody ivars;
} __attribute__ ((packed)) sdmmd_am_device;

// Everything below here you shouldn't be calling, this is internal for the library
//=================================================================================
sdmmd_return_t SDMMD__CopyEscrowBag(SDMMD_AMDeviceRef device, CFDataRef *bag);

//SDMMD_lockdown_conn* SDMMD_lockdown_connection_create(uint32_t socket);
//sdmmd_return_t SDMMD_lockconn_enable_ssl(SDMMD_lockdown_conn *lockdown_conn, CFTypeRef hostCert, CFTypeRef deviceCert, CFTypeRef hostPrivKey, uint32_t num);

SSL* SDMMD_lockssl_handshake(uint64_t socket, CFTypeRef hostCert, CFTypeRef deviceCert, CFTypeRef hostPrivKey, uint32_t num);
sdmmd_return_t SDMMD__connect_to_port(SDMMD_AMDeviceRef device, uint32_t port, bool hasTimeout, uint32_t *socketConn, bool isSSL);

sdmmd_return_t SDMMD_lockconn_send_message(SDMMD_AMDeviceRef device, CFDictionaryRef dict);
sdmmd_return_t SDMMD_lockconn_receive_message(SDMMD_AMDeviceRef device, CFMutableDictionaryRef *dict);

//CFTypeRef SDMMD_copy_lockdown_value(SDMMD_AMDeviceRef device, CFStringRef domain, CFStringRef key, CFStringRef *err);

//sdmmd_return_t SDMMD_lockdown_connection_destory(SDMMD_lockdown_conn *lockdownCon);

#endif
