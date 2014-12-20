/*
 *  SDMMD_AMDevice.h
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

#ifndef _SDM_MD_ADMDEVICE_H_
#define _SDM_MD_ADMDEVICE_H_

#include "SDMMD_AMDevice_Class.h"

#include "SDMMD_Error.h"
#include "SDMMD_Keys.h"
/* Private
#include "SDMMD_MRecoveryModeDevice.h"
#include "SDMMD_MRUSBDevice.h"
#include "SDMMD_MDFUModeDevice.h"
#include "SDMMD_MRestoreModeDevice.h"
#include "SDMMD_MRestorableDevice.h"
*/
#include "SDMMD_Types.h"


typedef enum _AMDInterfaceConnectionType {
    kAMDInterfaceConnectionTypeInvalid  = -1,
    kAMDInterfaceConnectionTypeUnknown  = 0,
    kAMDInterfaceConnectionTypeDirect   = 1,
    kAMDInterfaceConnectionTypeIndirect = 2
} AMDInterfaceConnectionType;
typedef AMDInterfaceConnectionType sdmmd_interface_return_t;


/*!
 @function SDMMD_AMDeviceCreateCopy
 @discussion
 	Copy and return an existing device object.
 @param device
 	Device object to create a copy of
 */
CF_RETURNS_RETAINED SDMMD_AMDeviceRef SDMMD_AMDeviceCreateCopy(SDMMD_AMDeviceRef device);

/*!
 @function SDMMD_AMDeviceCreateFromProperties
 @discussion
 	Create and return a SDMMD_AMDeviceRef object from passed properties dictionary, this is used by the USBmuxd for creating devices when they are attached.
 @param dict
 	CFDictionaryRef of the device properties, this is created by a USBmuxd callback.
 */
CF_RETURNS_RETAINED SDMMD_AMDeviceRef SDMMD_AMDeviceCreateFromProperties(CFDictionaryRef dict);

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
sdmmd_return_t SDMMD_AMDevicePair(SDMMD_AMDeviceRef device);
sdmmd_return_t SDMMD_AMDevicePairWithOptions(SDMMD_AMDeviceRef device, CFDictionaryRef options);
sdmmd_return_t SDMMD_AMDeviceExtendedPairWithOptions(SDMMD_AMDeviceRef device, CFDictionaryRef options, CFDictionaryRef * extendedResponse);
sdmmd_return_t SDMMD_AMDeviceUnpair(SDMMD_AMDeviceRef device);

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

CF_RETURNS_RETAINED CFStringRef SDMMD_AMDeviceCopyUDID(SDMMD_AMDeviceRef device);

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
CF_RETURNS_RETAINED CFTypeRef SDMMD_AMDeviceCopyValue(SDMMD_AMDeviceRef device, CFStringRef domain, CFStringRef key);

/*!
 @function SDMMD_AMDeviceSetValue
 @discussion
    Sets data associated with a particular device key in a domain, see SDMMD_Keys.h for domain and key pairs
 @param device
    Device object to set key-value on
 @param domain
    CFStringRef of the domain name associated with a key, this can be NULL.
 @param key
    CFStringRef of the key of the value to be set on the device.
 @param value
    CFTypeRef of the value to be set on the device.
 */
sdmmd_return_t SDMMD_AMDeviceSetValue(SDMMD_AMDeviceRef device, CFStringRef domain, CFStringRef key, CFTypeRef value);

/*!
 @function SDMMD_AMDCreateDeviceList
 @discussion
 	Returns a CFArrayRef of all attached devices.
 */
CF_RETURNS_RETAINED CFArrayRef SDMMD_AMDCreateDeviceList();

sdmmd_return_t SDMMD_AMDeviceMountImage(SDMMD_AMDeviceRef device, CFStringRef path, CFDictionaryRef dict, CallBack handle, void* unknown);

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

/*!
 @function SDMMD_AMDeviceGetInterfaceType
 @discussion
    Get the interface type ie USB / WiFi.
 @param device
    The device to query
 */
sdmmd_interface_return_t SDMMD_AMDeviceGetInterfaceType(SDMMD_AMDeviceRef device);


#endif