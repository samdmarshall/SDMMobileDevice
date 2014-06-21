/*
 *  SDMTypeTranslation.h
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

#ifndef SDMMobileDevice_Framework_SDMTypeTranslation_h
#define SDMMobileDevice_Framework_SDMTypeTranslation_h

#include "SDMMobileDevice.h"

#pragma mark - Errors

typedef sdmmd_return_t amd_error_t;
typedef amd_error_t AMDError;

#pragma mark - MCP

typedef SDMMD_AMConnectionRef AMConnectionRef;
typedef SDMMD_AMConnectionRef AMDServiceConnectionRef;

#pragma mark - Device

typedef SDMMD_AMDeviceRef AMDeviceRef;

#pragma mark - RecoveryModeDevice

typedef SDMMD_AMRecoveryModeDeviceRef AMRecoveryModeDeviceRef;

#pragma mark - USBDevice

typedef SDMMD_AMRUSBDeviceRef AMRUSBDeviceRef;

#pragma mark - DFUDevice

typedef SDMMD_AMDFUModeDeviceRef AMDFUModeDeviceRef;

#pragma mark - RestoreModeDevice

typedef SDMMD_AMRestoreModeDeviceRef AMRestoreModeDeviceRef;

#pragma mark - RestoreableDevice

typedef SDMMD_AMRestorableDeviceRef AMRestorableDeviceRef;

#pragma mark - AFC

typedef SDMMD_AFCConnectionRef AFCConnectionRef;
typedef SDMMD_AFCOperationRef AFCOperationRef;

// arg_def should be formated as (ArgType1 arg1, ArgType2 arg2, ArgType3 arg3, etc)
// arg_imp should be formated as (arg1, arg2, arg3, etc)
#define AMD_INLINE(return_type, call, arg_def, arg_imp) static inline return_type call arg_def  { return SDMMD_##call arg_imp ; }

/*
AMD_INLINE(bool, AMDeviceIsValid, (AMDeviceRef device), (device))
AMD_INLINE(AMDError, AMDeviceConnect, (AMDeviceRef device), (device))
AMD_INLINE(AMDError, AMDeviceDisconnect, (AMDeviceRef device), (device))
AMD_INLINE(AMDError, AMDeviceActivate, (AMDeviceRef device, CFDictionaryRef options), (device, options))
AMD_INLINE(AMDError, AMDeviceDeactivate, (AMDeviceRef device), (device))
AMD_INLINE(bool, AMDeviceIsPaired, (AMDeviceRef device), (device))
AMD_INLINE(AMDError, AMDeviceValidatePairing, (AMDeviceRef device), (device))
AMD_INLINE(AMDError, AMDevicePair, (AMDeviceRef device), (device))
AMD_INLINE(AMDError, AMDevicePairWithOptions, (AMDeviceRef device, CFMutableDictionaryRef record), (device, record))
AMD_INLINE(AMDError, AMDeviceUnpair, (AMDeviceRef device), (device))
*/
 
#endif
