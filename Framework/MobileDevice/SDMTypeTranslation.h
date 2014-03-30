//
//  SDMTypeTranslation.h
//  SDMMobileDevice-Framework
//
//  Created by Jeremy Agostino on 2/14/14.
//
//  This file is made available under the LICENSE of the containing project
//

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
