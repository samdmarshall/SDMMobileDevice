/*
 *  SDMMD_AMDevice.h
 *  SDMMobileDevice
 *
 *  Copyright (c) 2013, Sam Marshall
 *  All rights reserved.
 * 
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 *  1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
 *  3. All advertising materials mentioning features or use of this software must display the following acknowledgement:
 *  	This product includes software developed by the Sam Marshall.
 *  4. Neither the name of the Sam Marshall nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
 * 
 *  THIS SOFTWARE IS PROVIDED BY Sam Marshall ''AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL Sam Marshall BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 */

#ifndef _SDM_MD_ADMDEVICE_H_
#define _SDM_MD_ADMDEVICE_H_

#include <CoreFoundation/CoreFoundation.h>
#include <openssl/ssl.h>
#include "SDMMD_Error.h"
#include "SDMMD_Keys.h"
#include "SDMMD_MRecoveryModeDevice.h"
#include "SDMMD_MRUSBDevice.h"
#include "SDMMD_MDFUModeDevice.h"
#include "SDMMD_MRestoreModeDevice.h"
#include "SDMMD_MRestorableDevice.h"

// Please do not access ivars of the AMDevice objects if you can avoid it, they are subject to change.

typedef struct SDMMD_lockdown_conn {
	uint64_t connection;			// 0
	SSL *ssl; 						// 8
	uint64_t *pointer;				// 16
	uint64_t length;				// 24
} __attribute__ ((packed)) SDMMD_lockdown_conn;

struct AMDeviceClassHeader {
	unsigned char header[16];		// AMDeviceClass CF Header 
} __attribute ((packed)) AMDeviceClassHeader; // size 0x10

struct AMDeviceClassBody {
	int32_t device_id;					// 16
	int32_t location_id;				// 20
	uint16_t product_id;				// 24
	int16_t padding0;					// 26
	int32_t unknown1;					// 28
	CFStringRef unique_device_id;		// 32
	int32_t unknown2;					// 36
	int32_t connection_type;			// 40 (0 for USB, 1 for WiFi)
	unsigned char unknown3[4];			// 44
	SDMMD_lockdown_conn *lockdown_conn; // 48
	unsigned char unknown4[4];			// 52
	CFStringRef session;				// 56 needs to be not zero in AMDeviceSecureStartService  -- connection
	int32_t padding1;					// 60
	pthread_mutex_t mutex_lock;			// 64
	unsigned char unknown5[60];			// 68
	CFStringRef service_name;			// 128 bonjour service name
	unsigned int unknown6;				// 132
	int32_t interface_index;			// 136
	int8_t device_active;				// 140
	unsigned char unknown7[3];			// 141
	int32_t unknown8;					// 144
	unsigned char unknown9[4];			// 148
	CFDataRef network_address;			// 152 stores a sockaddr_storage
	unsigned char unknown10[4];			// 156
	CFDataRef unknown11;				// 160 
	unsigned char unknown12[4];			// 164
} __attribute__ ((packed)) AMDeviceClassBody; // size 0x98

struct sdmmd_am_device {
	struct AMDeviceClassHeader base;
	struct AMDeviceClassBody ivars;
} __attribute__ ((packed)) sdmmd_am_device;


CFTypeID SDMMD_AMDeviceGetTypeID(void);

/* Classes */

#define SDMMD_AMDeviceRef struct sdmmd_am_device*

#pragma mark -
void SDMMD_AMDeviceRefClassInitialize();

/*!
 @function SDMMD_AMDeviceCreateEmpty
 @discussion
 	Creating an empty device object, used in conjunction with SDMMD_AMDeviceCreateFromProperties(), returns a SDMMD_AMDeviceRef object.
 */
SDMMD_AMDeviceRef SDMMD_AMDeviceCreateEmpty();

/*!
 @function SDMMD_AMDeviceCreateCopy
 @discussion
 	Copy and return an existing device object.
 @param device
 	Device object to create a copy of
 */
SDMMD_AMDeviceRef SDMMD_AMDeviceCreateCopy(SDMMD_AMDeviceRef device);

/*!
 @function SDMMD_AMDeviceCreateFromProperties
 @discussion
 	Create and return a SDMMD_AMDeviceRef object from passed properties dictionary, this is used by the USBmuxd for creating devices when they are attached.
 @param dict
 	CFDictionaryRef of the device properties, this is created by a USBmuxd callback.
 */
SDMMD_AMDeviceRef SDMMD_AMDeviceCreateFromProperties(CFDictionaryRef dict);

sdmmd_return_t SDMMD_AMDeviceActivate(SDMMD_AMDeviceRef device, CFDictionaryRef options);
sdmmd_return_t SDMMD_AMDeviceDeactivate(SDMMD_AMDeviceRef device);


/*!
 @function SDMMD_AMDeviceConnect
 @discussion
 	Connect to a device, returns a code that will give an return status or error code.
 @param device
 	The device object to connect to.
 */
sdmmd_return_t SDMMD_AMDeviceConnect(SDMMD_AMDeviceRef device);

/*!
 @function SDMMD_AMDeviceDisconnect
 @discussion
 	Disconnect from a device, returns a code that will give an return status or error code.
 @param device
	 The device object to disconnect from.
 */
sdmmd_return_t SDMMD_AMDeviceDisconnect(SDMMD_AMDeviceRef device);

/*!
 @function SDMMD_AMDeviceIsValid
 @discussion
 	checks to see if a device is still an attached and valid device to use
 @param device
 	device to check
 */
bool SDMMD_AMDeviceIsValid(SDMMD_AMDeviceRef device);

/*!
 @function SDMMD_AMDeviceValidatePairing
 @discussion
 	Performs a check to see if a device pairing record is correct
 @param device
 	device object validate the pairing record for
 */
sdmmd_return_t SDMMD_AMDeviceValidatePairing(SDMMD_AMDeviceRef device);

/*!
 @function SDMMD_AMDeviceIsPaired
 @discussion
 	Returns if the passed device is paired with the host
 @param device
 	device object to check if pairing record exists
 */
bool SDMMD_AMDeviceIsPaired(SDMMD_AMDeviceRef device);

// Pairing is fully tested and finished yet
//sdmmd_return_t SDMMD_AMDevicePairWithOptions(SDMMD_AMDeviceRef device, CFMutableDictionaryRef record);

/*!
 @function SDMMD_AMDeviceStartSession
 @discussion
 	starts a lockdown session on the device. this is necessary to start services and access some device key-value pairs.
 @param device
 	device to start the session with
 */
sdmmd_return_t SDMMD_AMDeviceStartSession(SDMMD_AMDeviceRef device);

/*!
 @function SDMMD_AMDeviceStopSession
 @discussion
 	stops a lockdown session on the device.
 @param device
 	device to stop the session with
 */
sdmmd_return_t SDMMD_AMDeviceStopSession(SDMMD_AMDeviceRef device);

/*!
 @function SDMMD_AMDeviceUSBDeviceID
 @discussion
 	returns the device identifier for a specific device, this is unique to the device's attached state. this changes between attaches. this is also used for associating connections to a specific device
 @param device
 	device to get the device identifer of
 
 */
uint32_t SDMMD_AMDeviceUSBDeviceID(SDMMD_AMDeviceRef device);

/*!
 @function SDMMD_AMDeviceUSBLocationID
 @discussion
 	returns the usb location identifier
 @param device
 	device to get the usb location identifer of
 */
uint32_t SDMMD_AMDeviceUSBLocationID(SDMMD_AMDeviceRef device);

/*!
 @function SDMMD_AMDeviceUSBProductID
 @discussion
 	returns the usb product identifier
 @param device
 	device to get the usb product identifer of
 */
uint16_t SDMMD_AMDeviceUSBProductID(SDMMD_AMDeviceRef device);

/*!
 @function SDMMD_AMDeviceGetConnectionID
 @discussion
 	returns the connection ID associated with a device
 @param device
 	Device to get the connection ID of
 */
uint32_t SDMMD_AMDeviceGetConnectionID(SDMMD_AMDeviceRef device);

/*!
 @function SDMMD_AMDeviceIsAttached
 @discussion
 	This is a sanity check to ensure that a specific device is still attached before performing an action.
 @param device
 	Device to check if attached.
 */
bool SDMMD_AMDeviceIsAttached(SDMMD_AMDeviceRef device);

/*!
 @function SDMMD_AMDeviceCopyValue
 @discussion
 	Fetchs data associated with a particular device key in a domain, see SDMMD_Keys.h for domain and key pairs
 @param device
 	Device object to fetch key-value from
 @param domain
 	CFStringRef of the domain name associated with a key, this can be NULL.
 @param key
 	CFStringRef of the key of the value to be requested from the device.
 */
CFTypeRef SDMMD_AMDeviceCopyValue(SDMMD_AMDeviceRef device, CFStringRef domain, CFStringRef key);

/*!
 @function SDMMD_AMDCreateDeviceList
 @discussion
 	Returns a CFArrayRef of all attached devices.
 */
CFArrayRef SDMMD_AMDCreateDeviceList();

/*!
 @function SDMMD_GetSIMStatusCode
 @discussion
 	returns a struct containing the SIM status code number and corresponding status string
 @param device
 	device to check the SIM of
 */
sdmmd_sim_return_t SDMMD_GetSIMStatusCode(SDMMD_AMDeviceRef device);

/*!
 @function SDMMD_GetActivationStatus
 @discussion
 	returns a struct containing the activation code number and status string
 @param device
 	device to check activation status of
 */
sdmmd_activation_return_t SDMMD_GetActivationStatus(SDMMD_AMDeviceRef device);

// Everything below here you shouldn't be calling, this is internal for the library
//=================================================================================
sdmmd_return_t SDMMD__CopyEscrowBag(SDMMD_AMDeviceRef device, CFDataRef *bag);

SDMMD_lockdown_conn* SDMMD_lockdown_connection_create(uint32_t socket);
//sdmmd_return_t SDMMD_lockconn_enable_ssl(SDMMD_lockdown_conn *lockdown_conn, CFTypeRef hostCert, CFTypeRef deviceCert, CFTypeRef hostPrivKey, uint32_t num);

SSL* SDMMD_lockssl_handshake(SDMMD_lockdown_conn *lockdown_conn, CFTypeRef hostCert, CFTypeRef deviceCert, CFTypeRef hostPrivKey, uint32_t num);

sdmmd_return_t SDMMD_lockconn_send_message(SDMMD_AMDeviceRef device, CFDictionaryRef dict);
sdmmd_return_t SDMMD_lockconn_receive_message(SDMMD_AMDeviceRef device, CFMutableDictionaryRef *dict);

//CFTypeRef SDMMD_copy_lockdown_value(SDMMD_AMDeviceRef device, CFStringRef domain, CFStringRef key, CFStringRef *err);

//sdmmd_return_t SDMMD_lockdown_connection_destory(SDMMD_lockdown_conn *lockdownCon);

//sdmmd_return_t SDMMD_AMDeviceMountImage(SDMMD_AMDeviceRef device, CFStringRef path, CFDictionaryRef dict, void* three, void* four);

#endif