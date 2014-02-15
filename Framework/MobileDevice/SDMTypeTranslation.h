//
//  SDMTypeTranslation.h
//  SDMMobileDevice-Framework
//
//  Created by Jeremy Agostino on 2/14/14.
//
//  This file is made available under the LICENSE of the containing project
//

#import "SDMMobileDevice.h"

#ifndef SDMMobileDevice_Framework_SDMTypeTranslation_h
#define SDMMobileDevice_Framework_SDMTypeTranslation_h

#pragma mark - Errors

typedef sdmmd_return_t amd_error_t;
typedef amd_error_t AMDError;

#pragma mark - MCP

typedef SDMMD_AMConnectionRef AMDConnectionRef;
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

#endif
