/*
 *  SDMMD_Error.h
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

#ifndef _SDM_MD_ERROR_H_
#define _SDM_MD_ERROR_H_

#include <CoreFoundation/CoreFoundation.h>

typedef enum SDMMD_Errors {
	kAMDSuccess = 0x0,
	kAMDUndefinedError,
	kAMDBadHeaderError,
	kAMDNoResourcesError,
	kAMDReadError,
	kAMDWriteError,
	kAMDUnknownPacketError,
	kAMDInvalidArgumentError,
	kAMDNotFoundError,
	kAMDIsDirectoryError,
	kAMDPermissionError,
	kAMDNotConnectedError,
	kAMDTimeOutError,
	kAMDOverrunError,
	kAMDEOFError,
	kAMDUnsupportedError,
	kAMDFileExistsError,
	kAMDBusyError,
	kAMDCryptoError,
	kAMDTooBigError,
	kAMDInvalidResponseError,
	kAMDMissingKeyError,
	kAMDMissingValueError,
	kAMDGetProhibitedError,
	kAMDSetProhibitedError,
	kAMDRemoveProhibitedError,
	kAMDImmutableValueError,
	kAMDPasswordProtectedError,
	kAMDUserDeniedPairingError,
	kAMDPairingDialogResponsePendingError,
	kAMDMissingHostIDError,
	kAMDInvalidHostIDError,
	kAMDSessionActiveError,
	kAMDSessionInactiveError,
	kAMDMissingSessionIDError,
	kAMDInvalidSessionIDError,
	kAMDMissingServiceError,
	kAMDInvalidServiceError,
	kAMDServiceLimitError,
	kAMDCheckinSetupFailedError,
	kAMDInvalidCheckinError,
	kAMDCheckinTimeoutError,
	kAMDCheckinConnectionFailedError,
	kAMDCheckinReceiveFailedError,
	kAMDCheckinResponseFailedError,
	kAMDCheckinOutOfMemoryError,
	kAMDCheckinSendFailedError,
	kAMDMissingPairRecordError,
	kAMDInvalidPairRecordError,
	kAMDSavePairRecordFailedError,
	kAMDInvalidActivationRecordError,
	kAMDMissingActivationRecordError,
	kAMDServiceProhibitedError,
	kAMDEscrowLockedError,
	kAMDPairingProhibitedError,
	kAMDProhibitedBySupervision,
	kAMDWrongDroidError,
	kAMDSUVerificationError,
	kAMDSUPatchError,
	kAMDSUFirmwareError,
	kAMDProvisioningProfileNotValid,
	kAMDSendMessageError,
	kAMDReceiveMessageError,
	kAMDMissingOptionsError,
	kAMDMissingImageTypeError,
	kAMDDigestFailedError,
	kAMDStartServiceError,
	kAMDInvalidDiskImageError,
	kAMDMissingDigestError,
	kAMDMuxError,
	kAMDApplicationAlreadyInstalledError,
	kAMDApplicationMoveFailedError,
	kAMDApplicationSINFCaptureFailedError,
	kAMDApplicationSandboxFailedError,
	kAMDApplicationVerificationFailedError,
	kAMDArchiveDestructionFailedError,
	kAMDBundleVerificationFailedError,
	kAMDCarrierBundleCopyFailedError,
	kAMDCarrierBundleDirectoryCreationFailedError,
	kAMDCarrierBundleMissingSupportedSIMsError,
	kAMDCommCenterNotificationFailedError,
	kAMDContainerCreationFailedError,
	kAMDContainerP0wnFailedError,
	kAMDContainerRemovalFailedError,
	kAMDEmbeddedProfileInstallFailedError,
	kAMDErrorError,
	kAMDExecutableTwiddleFailedError,
	kAMDExistenceCheckFailedError,
	kAMDInstallMapUpdateFailedError,
	kAMDManifestCaptureFailedError,
	kAMDMapGenerationFailedError,
	kAMDMissingBundleExecutableError,
	kAMDMissingBundleIdentifierError,
	kAMDMissingBundlePathError,
	kAMDMissingContainerError,
	kAMDNotificationFailedError,
	kAMDPackageExtractionFailedError,
	kAMDPackageInspectionFailedError,
	kAMDPackageMoveFailedError,
	kAMDPathConversionFailedError,
	kAMDRestoreContainerFailedError,
	kAMDSeatbeltProfileRemovalFailedError,
	kAMDStageCreationFailedError,
	kAMDSymlinkFailedError,
	kAMDiTunesArtworkCaptureFailedError,
	kAMDiTunesMetadataCaptureFailedError,
	kAMDAlreadyArchivedError,
	kAMDUnknownCommandError,
	kAMDAPIInternalError,
	kAMDMuxCreateListenerError,
	kAMDMuxGetListenerError,
	kAMDMuxConnectError,
	kAMDDeviceTooNewError,
	kAMDDeviceRefNoGood,
	kAMDDeviceDisconnectedError,
	kAMDCannotTranslateError,
	kAMDMobileImageMounterMissingImageSignature,
	kAMDMobileImageMounterResponseCreationFailed,
	kAMDMobileImageMounterMissingImageType,
	kAMDMobileImageMounterMissingImagePath,
	kAMDMobileImageMounterImageMapLoadFailed,
	kAMDMobileImageMounterAlreadyMounted,
	kAMDMobileImageMounterImageMoveFailed,
	kAMDMobileImageMounterMountPathMissing,
	kAMDMobileImageMounterMountPathNotEmpty,
	kAMDMobileImageMounterImageMountFailed,
	kAMDMobileImageMounterTrustCacheLoadFailed,
	kAMDMobileImageMounterDigestFailed,
	kAMDMobileImageMounterDigestCreationFailed,
	kAMDMobileImageMounterImageVerificationFailed,
	kAMDMobileImageMounterImageInfoCreationFailed,
	kAMDMobileImageMounterImageMapStoreFailed,
	kAMDBonjourSetupError,
	kAMDNoWifiSyncSupportError,
	kAMDDeviceOSVersionTooLow,
	kAMDDeviceFamilyNotSupported,
	kAMDPackagePatchFailedError,
	kAMDIncorrectArchitectureError,
	kAMDPluginCopyFailedError,
	kAMDBreadcrumbFailedError,
	kAMDBreadcrumbUnlockError,
	kAMDGeoJSONCaptureFailedError,
	kAMDNewsstandArtworkCaptureFailedError,
	kAMDMissingCommandError,
	kAMDNotEntitledError,
	kAMDMissingPackagePathError,
	kAMDMissingContainerPathError,
	kAMDMissingApplicationIdentifierError,
	kAMDMissingAttributeValueError,
	kAMDLookupFailedError,
	kAMDDictCreationFailedError,
	kAMDInstallProhibitedError,
	kAMDUninstallProhibitedError,
	kAMDMissingBundleVersionError,
	kAMDFMiPProtectedError,
	kAMDMCProtected,
	kAMDMCChallengeRequired
} SDMMD_Errors;

#define sdmmd_return_t enum SDMMD_Errors

struct SDMMD_DebugReturnCode {
	enum SDMMD_Errors result;
	CFDataRef data;
} SDMMD_DebugReturnCode;

#define sdmmd_debug_return_t struct SDMMD_DebugReturnCode

typedef enum SDMMD_NotificationErrorCodes {
	MDERR_NOTIFICATION_MSG_CONNECTED = 1,
	MDERR_NOTIFICATION_MSG_DISCONNECTED = 2,
	MDERR_NOTIFICATION_MSG_UNKNOWN = 3
} SDMMD_NotificationErrorCodes;

#define sdmmd_notification_return_t enum SDMMD_NotificationErrorCodes

typedef enum SDMMD_USBMuxErrorCodes {
	MDERR_USBMUX_OK = 0x0,
	MDERR_USBMUX_ARG_NULL = 0x16,
	MDERR_USBMUX_FAILED = 0xffffffff
} SDMMD_USBMuxErrorCodes;

#define sdmmd_usbmux_return_t enum SDMMD_USBMuxErrorCodes

typedef struct SDMMD_SIMStatusCode {
	CFStringRef codeName;
	uint32_t resultCode;
} SDMMD_SIMStatusCode;

#define sdmmd_sim_return_t struct SDMMD_SIMStatusCode

#define kKnownSIMCodesNum 13

static struct SDMMD_SIMStatusCode KnownSIMCodes[kKnownSIMCodesNum] = {
	{CFSTR("kCTSIMSupportSIMStatusUnavailable"), 0},
	{CFSTR("kCTSIMSupportSIMStatusReady"), 1},
	{CFSTR("kCTSIMSupportSIMStatusNotReady"), 2},
	{CFSTR("kCTSIMSupportSIMStatusPINLocked"), 3},
	{CFSTR("kCTSIMSupportSIMStatusPUKLocked"), 4},
	{CFSTR("kCTSIMSupportSIMStatusNetworkLocked"), 5},
	{CFSTR("kCTSIMSupportSIMStatusCorporateLocked"), 6},
	{CFSTR("kCTSIMSupportSIMStatusOperatorLocked"), 7},
	{CFSTR("kCTSIMSupportSIMStatusOperatorSubsetLocked"), 8},
	{CFSTR("kCTSIMSupportSIMStatusServiceProviderLocked"), 9},
	{CFSTR("kCTSIMSupportSIMStatusMemoryFailure"), 10},
	{CFSTR("kCTSIMSupportSIMStatusFixedDialingLocked"), 11},
	{CFSTR("kCTSIMSupportSIMStatusBlacklisted"), 12}
};

typedef struct SDMMD_ActivationState {
	CFStringRef statusName;
	uint32_t statusCode;
} SDMMD_ActivationState;

#define sdmmd_activation_return_t struct SDMMD_ActivationState

#define kKnownActivationStatesNum 9

static struct SDMMD_ActivationState KnownActivationStates[kKnownActivationStatesNum] = {
	{CFSTR("Unactivated"), 0},
	{CFSTR("Activated"), 1},
	{CFSTR("FactoryActivated"), 2},
	{CFSTR("SoftActivated"), 3},
	{CFSTR("MismatchedIMEI"), 4},
	{CFSTR("MismatchedICCID"), 5},
	{CFSTR("MissingSIM"), 6},
	{CFSTR("WildcardActivated"), 7},
	{CFSTR("Expired"), 8},
};

typedef enum SDMMD_LockdownError {
	LD_ERR_OK = 0x0,
	LD_ERR_InvalidResponse = 0xe8000013,
	LD_ERR_MissingKey = 0xe8000014,
	LD_ERR_MissingValue = 0xe8000015,
	LD_ERR_GetProhibited = 0xe8000016,
	LD_ERR_SetProhibited = 0xe8000017,
	LD_ERR_RemoveProhibited = 0xe8000018,
	LD_ERR_ImmutableValue = 0xe8000019,
	LD_ERR_PasswordProtected = 0xe800001a,
	LD_ERR_MissingHostID = 0xe800001b,
	LD_ERR_InvalidHostID = 0xe800001c,
	LD_ERR_SessionActive = 0xe800001d,
	LD_ERR_SessionInactive = 0xe800001e,
	LD_ERR_MissingSessionID = 0xe800001f,
	LD_ERR_InvalidSessionID = 0xe8000020,
	LD_ERR_MissingService = 0xe8000021,
	LD_ERR_InvalidService = 0xe8000022,
	LD_ERR_InvalidCheckin = 0xe8000023,
	LD_ERR_CheckinTimeout = 0xe8000024,
	LD_ERR_MissingPairRecord = 0xe8000025,
	LD_ERR_InvalidActivationRecord = 0xe8000026,
	LD_ERR_MissingActivationRecord = 0xe8000027,
	LD_ERR_WrongDroid = 0xe8000028,
	LD_ERR_ServiceLimit = 0xe800005b,
	LD_ERR_InvalidPairRecord = 0xe800005c,
	LD_ERR_ServiceProhibited = 0xe800005d,
	LD_ERR_CheckinSetupFailed = 0xe800005e,
	LD_ERR_CheckinConnectionFailed = 0xe800005f,
	LD_ERR_CheckinReceiveFailed = 0xe8000060,
	LD_ERR_CheckinResponseFailed = 0xe8000061,
	LD_ERR_CheckinSendFailed = 0xe8000062,
	LD_ERR_SavePairRecordFailed = 0xe8000068,
	LD_ERR_EscrowLocked = 0xe8000081,
	LD_ERR_PairingProhibitedOverThisConnection = 0xe8000082,
	LD_ERR_NotAValidChaperoneHost = 0xe8000083,
} SDMMD_LockdownError;

#define sdmmd_dl_return_t enum SDMMD_LockdownError

sdmmd_dl_return_t SDMMD__ConvertLockdowndError(CFStringRef error);

sdmmd_return_t SDMMD__ConvertServiceError(CFStringRef error);

static char* SDMMD_ServiceConnectionErrorString[157] = { "kAMDSuccess", "kAMDUndefinedError", "kAMDBadHeaderError", "kAMDNoResourcesError", "kAMDReadError", "kAMDWriteError", "kAMDUnknownPacketError", "kAMDInvalidArgumentError", "kAMDNotFoundError", "kAMDIsDirectoryError", "kAMDPermissionError", "kAMDNotConnectedError", "kAMDTimeOutError", "kAMDOverrunError", "kAMDEOFError", "kAMDUnsupportedError", "kAMDFileExistsError", "kAMDBusyError", "kAMDCryptoError", "kAMDTooBigError", "kAMDInvalidResponseError", "kAMDMissingKeyError", "kAMDMissingValueError", "kAMDGetProhibitedError", "kAMDSetProhibitedError", "kAMDRemoveProhibitedError", "kAMDImmutableValueError", "kAMDPasswordProtectedError", "kAMDUserDeniedPairingError", "kAMDPairingDialogResponsePendingError", "kAMDMissingHostIDError", "kAMDInvalidHostIDError", "kAMDSessionActiveError", "kAMDSessionInactiveError", "kAMDMissingSessionIDError", "kAMDInvalidSessionIDError", "kAMDMissingServiceError", "kAMDInvalidServiceError", "kAMDServiceLimitError", "kAMDCheckinSetupFailedError", "kAMDInvalidCheckinError", "kAMDCheckinTimeoutError", "kAMDCheckinConnectionFailedError", "kAMDCheckinReceiveFailedError", "kAMDCheckinResponseFailedError", "kAMDCheckinOutOfMemoryError", "kAMDCheckinSendFailedError", "kAMDMissingPairRecordError", "kAMDInvalidPairRecordError", "kAMDSavePairRecordFailedError", "kAMDInvalidActivationRecordError", "kAMDMissingActivationRecordError", "kAMDServiceProhibitedError", "kAMDEscrowLockedError", "kAMDPairingProhibitedError", "kAMDProhibitedBySupervision", "kAMDWrongDroidError", "kAMDSUVerificationError", "kAMDSUPatchError", "kAMDSUFirmwareError", "kAMDProvisioningProfileNotValid", "kAMDSendMessageError", "kAMDReceiveMessageError", "kAMDMissingOptionsError", "kAMDMissingImageTypeError", "kAMDDigestFailedError", "kAMDStartServiceError", "kAMDInvalidDiskImageError", "kAMDMissingDigestError", "kAMDMuxError", "kAMDApplicationAlreadyInstalledError", "kAMDApplicationMoveFailedError", "kAMDApplicationSINFCaptureFailedError", "kAMDApplicationSandboxFailedError", "kAMDApplicationVerificationFailedError", "kAMDArchiveDestructionFailedError", "kAMDBundleVerificationFailedError", "kAMDCarrierBundleCopyFailedError", "kAMDCarrierBundleDirectoryCreationFailedError", "kAMDCarrierBundleMissingSupportedSIMsError", "kAMDCommCenterNotificationFailedError", "kAMDContainerCreationFailedError", "kAMDContainerP0wnFailedError", "kAMDContainerRemovalFailedError", "kAMDEmbeddedProfileInstallFailedError", "kAMDErrorError", "kAMDExecutableTwiddleFailedError", "kAMDExistenceCheckFailedError", "kAMDInstallMapUpdateFailedError", "kAMDManifestCaptureFailedError", "kAMDMapGenerationFailedError", "kAMDMissingBundleExecutableError", "kAMDMissingBundleIdentifierError", "kAMDMissingBundlePathError", "kAMDMissingContainerError", "kAMDNotificationFailedError", "kAMDPackageExtractionFailedError", "kAMDPackageInspectionFailedError", "kAMDPackageMoveFailedError", "kAMDPathConversionFailedError", "kAMDRestoreContainerFailedError", "kAMDSeatbeltProfileRemovalFailedError", "kAMDStageCreationFailedError", "kAMDSymlinkFailedError", "kAMDiTunesArtworkCaptureFailedError", "kAMDiTunesMetadataCaptureFailedError", "kAMDAlreadyArchivedError", "kAMDUnknownCommandError", "kAMDAPIInternalError", "kAMDMuxCreateListenerError", "kAMDMuxGetListenerError", "kAMDMuxConnectError", "kAMDDeviceTooNewError", "kAMDDeviceRefNoGood", "kAMDDeviceDisconnectedError", "kAMDCannotTranslateError", "kAMDMobileImageMounterMissingImageSignature", "kAMDMobileImageMounterResponseCreationFailed", "kAMDMobileImageMounterMissingImageType", "kAMDMobileImageMounterMissingImagePath", "kAMDMobileImageMounterImageMapLoadFailed", "kAMDMobileImageMounterAlreadyMounted", "kAMDMobileImageMounterImageMoveFailed", "kAMDMobileImageMounterMountPathMissing", "kAMDMobileImageMounterMountPathNotEmpty", "kAMDMobileImageMounterImageMountFailed", "kAMDMobileImageMounterTrustCacheLoadFailed", "kAMDMobileImageMounterDigestFailed", "kAMDMobileImageMounterDigestCreationFailed", "kAMDMobileImageMounterImageVerificationFailed", "kAMDMobileImageMounterImageInfoCreationFailed", "kAMDMobileImageMounterImageMapStoreFailed", "kAMDBonjourSetupError", "kAMDNoWifiSyncSupportError", "kAMDDeviceOSVersionTooLow", "kAMDDeviceFamilyNotSupported", "kAMDPackagePatchFailedError", "kAMDIncorrectArchitectureError", "kAMDPluginCopyFailedError", "kAMDBreadcrumbFailedError", "kAMDBreadcrumbUnlockError", "kAMDGeoJSONCaptureFailedError", "kAMDNewsstandArtworkCaptureFailedError", "kAMDMissingCommandError", "kAMDNotEntitledError", "kAMDMissingPackagePathError", "kAMDMissingContainerPathError", "kAMDMissingApplicationIdentifierError", "kAMDMissingAttributeValueError", "kAMDLookupFailedError", "kAMDDictCreationFailedError", "kAMDInstallProhibitedError", "kAMDUninstallProhibitedError", "kAMDMissingBundleVersionError", "kAMDFMiPProtectedError", "kAMDMCProtected", "kAMDMCChallengeRequired"};

static char*  SDMMD_AMDErrorString(uint32_t error) {
	return SDMMD_ServiceConnectionErrorString[error & 0xff];
}

static char* SDMMD_AFCConnectionErrorString[24] = {"kAFCSuccess", "kAFCUndefinedError", "kAFCBadHeaderError", "kAFCNoResourcesError", "kAFCReadError", "kAFCWriteError", "kAFCUnknownPacketError", "kAFCInvalidArgumentError", "kAFCNotFoundError", "kAFCIsDirectoryError", "kAFCPermissionError", "kAFCNotConnectedError", "kAFCTimeOutError", "kAFCOverrunError", "kAFCEOFError", "kAFCUnsupportedError", "kAFCFileExistsError", "kAFCBusyError", "kAFCNoSpaceError", "kAFCWouldBlockError", "kAFCInputOutputError", "kAFCInterruptedError", "kAFCInProgressError", "kAFCInternalError"};

static char* SDMMD_AFCErrorString(uint32_t error) {
	return SDMMD_AFCConnectionErrorString[error & 0xff];
}

#define SDM_MD_CallSuccessful(result_code) ((result_code == kAMDSuccess) || (result_code == MDERR_USBMUX_OK) || (result_code == LD_ERR_OK))

#endif