/*
 *  SDMTranslation.h
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

#import "SDMMobileDevice.h"
#import "SDMTypeTranslation.h"

#ifndef _SDM_Translation_h
#define _SDM_Translation_h

//Error
#define AMDErrorString SDMMD_AMDErrorString
#define AFCErrorString SDMMD_AFCErrorString

//MCP
#define AMDeviceNotificationSubscribe   SDMMD_AMDeviceNotificationSubscribe
#define AMDeviceNotificationUnsubscribe SDMMD_AMDeviceNotificationUnsubscribe

#define MobileDevice    SDMMobileDevice

//Service
#define perform_command SDMMD_perform_command

#define AMDServiceConnectionCreate  SDMMD_AMDServiceConnectionCreate

#define AMDeviceStartService        SDMMD_AMDeviceStartService
#define AMDeviceSecureStartService  SDMMD_AMDeviceSecureStartService

#define AMDServiceConnectionSetServiceName  SDMMD_AMDServiceConnectionSetServiceName
#define AMDServiceConnectionSetDevice       SDMMD_AMDServiceConnectionSetDevice

#define AMDServiceConnectionGetSocket           SDMMD_AMDServiceConnectionGetSocket
#define AMDServiceConnectionGetSecureIOContext  SDMMD_AMDServiceConnectionGetSecureIOContext

#define AMDServiceConnectionInvalidate  SDMMD_AMDServiceConnectionInvalidate

#define AMDeviceSecureStartSessionedService SDMMD_AMDeviceSecureStartSessionedService

//Device
#define AMDeviceGetTypeID   SDMMD_AMDeviceGetTypeID;

#define AMDeviceRetain CFRetain
#define AMDeviceRelease CFRelease

#define AMDeviceRefClassInitialize      SDMMD_AMDeviceRefClassInitialize
#define AMDeviceCreateEmpty             SDMMD_AMDeviceCreateEmpty
#define AMDeviceCreateCopy              SDMMD_AMDeviceCreateCopy;
#define AMDeviceCreateFromProperties    SDMMD_AMDeviceCreateFromProperties;

#define AMDeviceActivate    SDMMD_AMDeviceActivate
#define AMDeviceDeactivate  SDMMD_AMDeviceDeactivate

#define AMDeviceConnect             SDMMD_AMDeviceConnect
#define AMDeviceDisconnect          SDMMD_AMDeviceDisconnect
#define AMDeviceIsValid             SDMMD_AMDeviceIsValid
#define AMDeviceValidatePairing     SDMMD_AMDeviceValidatePairing
#define AMDeviceIsPaired            SDMMD_AMDeviceIsPaired
#define AMDevicePair                SDMMD_AMDevicePair
#define AMDevicePairWithOptions     SDMMD_AMDevicePairWithOptions
#define AMDeviceUnpair              SDMMD_AMDeviceUnpair
#define AMDeviceStartSession        SDMMD_AMDeviceStartSession
#define AMDeviceStopSession         SDMMD_AMDeviceStopSession
#define AMDeviceUSBDeviceID         SDMMD_AMDeviceUSBDeviceID
#define AMDeviceUSBLocationID       SDMMD_AMDeviceUSBLocationID
#define AMDeviceUSBProductID        SDMMD_AMDeviceUSBProductID
#define AMDeviceGetConnectionID     SDMMD_AMDeviceGetConnectionID
#define AMDeviceIsAttached          SDMMD_AMDeviceIsAttached
#define AMDeviceCopyValue           SDMMD_AMDeviceCopyValue
#define AMDeviceSetValue            SDMMD_AMDeviceSetValue
#define AMDCreateDeviceList         SDMMD_AMDCreateDeviceList
#define GetSIMStatusCode            SDMMD_GetSIMStatusCode
#define GetActivationStatus         SDMMD_GetActivationStatus
#define AMDeviceGetInterfaceType    SDMMD_AMDeviceGetInterfaceType
//#define AMDeviceMountImage          SDMMD_AMDeviceMountImage

//RecoveryModeDevice
#define AMRecoveryModeDeviceCreate      SDMMD_AMRecoveryModeDeviceCreate
#define AMRestoreGetResolvedBoardConfig SDMMD_AMRestoreGetResolvedBoardConfig

//USBDevice

//DFUDevice
#define AMDFUModeDeviceCreate   SDMMD_AMDFUModeDeviceCreate

//Restore Mode Device

//Restoreable Device

//AFC
#define AFCConnectionCreate                 SDMMD_AFCConnectionCreate
#define AFCConnectionRelease                SDMMD_AFCConnectionRelease
#define AFCOperationCreateReadDirectory     SDMMD_AFCOperationCreateReadDirectory
#define AFCOperationCreateRemovePath        SDMMD_AFCOperationCreateRemovePath
#define AFCOperationCreateMakeDirectory     SDMMD_AFCOperationCreateMakeDirectory
#define AFCOperationCreateGetFileInfo       SDMMD_AFCOperationCreateGetFileInfo
#define AFCOperationCreateGetDeviceInfo     SDMMD_AFCOperationCreateGetDeviceInfo
#define AFCOperationCreateOpenFile          SDMMD_AFCOperationCreateOpenFile
#define AFCOperationCreateRenamePath        SDMMD_AFCOperationCreateRenamePath
#define AFCOperationCreateLinkPath          SDMMD_AFCOperationCreateLinkPath
#define AFCOperationCreateGetFileHash       SDMMD_AFCOperationCreateGetFileHash
#define AFCOperationCreateSetModTime        SDMMD_AFCOperationCreateSetModTime
#define AFCConnectionPerformOperation       SDMMD_AFCConnectionPerformOperation
#define AFCProcessOperation                 SDMMD_AFCProcessOperation

/*
#define AFCLog                  SDMMD_AFCLog
#define AFCSetErrorInfoWithArgs SDMMD_AFCSetErrorInfoWithArgs
#define _AFCSetErrorResult      SDMMD__AFCSetErrorResult
#define AFCStringCopy           SDMMD_AFCStringCopy
#define AFCPacketTypeName       SDMMD_AFCPacketTypeName
#define AFCSendStatusExtended   SDMMD_AFCSendStatusExtended
#define AFCSendStatus           SDMMD_AFCSendStatus
#define AFCSendDataPacket       SDMMD_AFCSendDataPacket
#define AFCSendHeader           SDMMD_AFCSendHeader
#define AFCReadPacket           SDMMD_AFCReadPacket
#define AFCReadPacket           SDMMD_AFCReadPacket
#define AFCReadPacketBody       SDMMD_AFCReadPacketBody
#define AFCSendPacket           SDMMD_AFCSendPacket
*/

//AFCDevice
#define AFCConnectionRef                SDMMD_AFCConnectionRef
#define AFCConnectionGetTypeID          SDMMD_AFCConnectionGetTypeID
#define AFCConnectionRefClassInitialize SDMMD_AFCConnectionRefClassInitialize
#define AFCConnectionCreate             SDMMD_AFCConnectionCreate

//AFCIterator
#define AFCIteratorRef  SDMMD_AFCIteratorRef

//AFCOperationRef
#define AFCOperationRef SDMMD_AFCOperationRef

//AFCLock
#define AFCLockRef                  SDMMD_AFCLockRef
#define AFCLockGetTypeID            SDMMD_AFCLockGetTypeID
#define AFCLockRefClassInitialize   SDMMD_AFCLockRefClassInitialize
#define AFCLockCreate               SDMMD_AFCLockCreate
#define AFCLockLock                 SDMMD_AFCLockLock
#define AFCLockUnlock               SDMMD_AFCLockUnlock

//Applications
#define AMDeviceLookupApplications SDMMD_AMDeviceLookupApplications
#define AMDeviceInstallApplication SDMMD_AMDeviceInstallApplication

#endif
