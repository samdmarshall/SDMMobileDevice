/*
 *  SDMMD_Error.h
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

#ifndef _SDM_MD_ERROR_H_
#define _SDM_MD_ERROR_H_

#include <CoreFoundation/CoreFoundation.h>

#define EvalAndEval(cond, eval) \
if cond { \
	eval; \
	goto ExitLabel; \
}

#define ValueIsNullEval(value, eval) EvalAndEval((value == NULL), eval)

#define EvalConditionalAndReturn(cond) EvalConditionalAndReturnWithMessage(cond, "error")

#define EvalConditionalAndReturnWithMessage(cond, message) \
if cond { \
	printf("%s: %s\n",__FUNCTION__,message); \
	goto ExitLabel; \
}

#define CheckErrorAndReturn(value) \
if (!SDM_MD_CallSuccessful(value)) { \
	goto ExitLabel; \
}

#define ExitLabelAndReturn(value) \
ExitLabel: \
return value;

#define XStringify(s) Stringify(s)
#define Stringify(s) #s

#define setsockoptCond(socket, type, value, arg, cond) \
if (setsockopt(sock, type, value, &arg, sizeof(arg))) { \
	cond \
	int err = errno; \
	printf("%s: setsockopt %s failed: %d - %s\n",__FUNCTION__,XStringify(value),err,strerror(err)); \
}

#define AMDErrorMake(num) (0xe8000000 | (num))
typedef enum SDMMD_Errors {
	kAMDSuccess = 0x0,
    kAMDUndefinedError                              = AMDErrorMake(1),   //0xe8000001
 	kAMDBadHeaderError                              = AMDErrorMake(2),   //0xe8000002
 	kAMDNoResourcesError                            = AMDErrorMake(3),   //0xe8000003
 	kAMDReadError                                   = AMDErrorMake(4),   //0xe8000004
 	kAMDWriteError                                  = AMDErrorMake(5),   //0xe8000005
 	kAMDUnknownPacketError                          = AMDErrorMake(6),   //0xe8000006
 	kAMDInvalidArgumentError                        = AMDErrorMake(7),   //0xe8000007
 	kAMDNotFoundError                               = AMDErrorMake(8),   //0xe8000008
 	kAMDIsDirectoryError                            = AMDErrorMake(9),   //0xe8000009
 	kAMDPermissionError                             = AMDErrorMake(10),  //0xe800000a
 	kAMDNotConnectedError                           = AMDErrorMake(11),  //0xe800000b
 	kAMDTimeOutError                                = AMDErrorMake(12),  //0xe800000c
 	kAMDOverrunError                                = AMDErrorMake(13),  //0xe800000d
 	kAMDEOFError                                    = AMDErrorMake(14),  //0xe800000e
 	kAMDUnsupportedError                            = AMDErrorMake(15),  //0xe800000f
 	kAMDFileExistsError                             = AMDErrorMake(16),  //0xe8000010
 	kAMDBusyError                                   = AMDErrorMake(17),  //0xe8000011
 	kAMDCryptoError                                 = AMDErrorMake(18),  //0xe8000012
    
    /* Lockdown errors overlap in this range */
 	kAMDInvalidResponseError                        = AMDErrorMake(19),  //0xe8000013
 	kAMDMissingKeyError                             = AMDErrorMake(20),  //0xe8000014
 	kAMDMissingValueError                           = AMDErrorMake(21),  //0xe8000015
 	kAMDGetProhibitedError                          = AMDErrorMake(22),  //0xe8000016
 	kAMDSetProhibitedError                          = AMDErrorMake(23),  //0xe8000017
 	kAMDRemoveProhibitedError                       = AMDErrorMake(24),  //0xe8000018
 	kAMDImmutableValueError                         = AMDErrorMake(25),  //0xe8000019
 	kAMDPasswordProtectedError                      = AMDErrorMake(26),  //0xe800001a
 	kAMDMissingHostIDError                          = AMDErrorMake(27),  //0xe800001b
 	kAMDInvalidHostIDError                          = AMDErrorMake(28),  //0xe800001c
 	kAMDSessionActiveError                          = AMDErrorMake(29),  //0xe800001d
 	kAMDSessionInactiveError                        = AMDErrorMake(30),  //0xe800001e
 	kAMDMissingSessionIDError                       = AMDErrorMake(31),  //0xe800001f
 	kAMDInvalidSessionIDError                       = AMDErrorMake(32),  //0xe8000020
 	kAMDMissingServiceError                         = AMDErrorMake(33),  //0xe8000021
 	kAMDInvalidServiceError                         = AMDErrorMake(34),  //0xe8000022
 	kAMDInvalidCheckinError                         = AMDErrorMake(35),  //0xe8000023
 	kAMDCheckinTimeoutError                         = AMDErrorMake(36),  //0xe8000024
 	kAMDMissingPairRecordError                      = AMDErrorMake(37),  //0xe8000025
 	kAMDInvalidActivationRecordError                = AMDErrorMake(38),  //0xe8000026
 	kAMDMissingActivationRecordError                = AMDErrorMake(39),  //0xe8000027
 	kAMDWrongDroidError                             = AMDErrorMake(40),  //0xe8000028
 	kAMDSUVerificationError                         = AMDErrorMake(41),  //0xe8000029
 	kAMDSUPatchError                                = AMDErrorMake(42),  //0xe800002a
 	kAMDSUFirmwareError                             = AMDErrorMake(43),  //0xe800002b
 	kAMDProvisioningProfileNotValid                 = AMDErrorMake(44),  //0xe800002c
 	kAMDSendMessageError                            = AMDErrorMake(45),  //0xe800002d
 	kAMDReceiveMessageError                         = AMDErrorMake(46),  //0xe800002e
 	kAMDMissingOptionsError                         = AMDErrorMake(47),  //0xe800002f
 	kAMDMissingImageTypeError                       = AMDErrorMake(48),  //0xe8000030
 	kAMDDigestFailedError                           = AMDErrorMake(49),  //0xe8000031
 	kAMDStartServiceError                           = AMDErrorMake(50),  //0xe8000032
 	kAMDInvalidDiskImageError                       = AMDErrorMake(51),  //0xe8000033
 	kAMDMissingDigestError                          = AMDErrorMake(52),  //0xe8000034
 	kAMDMuxError                                    = AMDErrorMake(53),  //0xe8000035
 	kAMDApplicationAlreadyInstalledError            = AMDErrorMake(54),  //0xe8000036
 	kAMDApplicationMoveFailedError                  = AMDErrorMake(55),  //0xe8000037
 	kAMDApplicationSINFCaptureFailedError           = AMDErrorMake(56),  //0xe8000038
 	kAMDApplicationSandboxFailedError               = AMDErrorMake(57),  //0xe8000039
 	kAMDApplicationVerificationFailedError          = AMDErrorMake(58),  //0xe800003a
 	kAMDArchiveDestructionFailedError               = AMDErrorMake(59),  //0xe800003b
 	kAMDBundleVerificationFailedError               = AMDErrorMake(60),  //0xe800003c
 	kAMDCarrierBundleCopyFailedError                = AMDErrorMake(61),  //0xe800003d
 	kAMDCarrierBundleDirectoryCreationFailedError   = AMDErrorMake(62),  //0xe800003e
 	kAMDCarrierBundleMissingSupportedSIMsError      = AMDErrorMake(63),  //0xe800003f
 	kAMDCommCenterNotificationFailedError           = AMDErrorMake(64),  //0xe8000040
 	kAMDContainerCreationFailedError                = AMDErrorMake(65),  //0xe8000041
 	kAMDContainerP0wnFailedError                    = AMDErrorMake(66),  //0xe8000042
 	kAMDContainerRemovalFailedError                 = AMDErrorMake(67),  //0xe8000043
 	kAMDEmbeddedProfileInstallFailedError           = AMDErrorMake(68),  //0xe8000044
 	kAMDErrorError                                  = AMDErrorMake(69),  //0xe8000045
 	kAMDExecutableTwiddleFailedError                = AMDErrorMake(70),  //0xe8000046
 	kAMDExistenceCheckFailedError                   = AMDErrorMake(71),  //0xe8000047
 	kAMDInstallMapUpdateFailedError                 = AMDErrorMake(72),  //0xe8000048
 	kAMDManifestCaptureFailedError                  = AMDErrorMake(73),  //0xe8000049
 	kAMDMapGenerationFailedError                    = AMDErrorMake(74),  //0xe800004a
 	kAMDMissingBundleExecutableError                = AMDErrorMake(75),  //0xe800004b
 	kAMDMissingBundleIdentifierError                = AMDErrorMake(76),  //0xe800004c
 	kAMDMissingBundlePathError                      = AMDErrorMake(77),  //0xe800004d
 	kAMDMissingContainerError                       = AMDErrorMake(78),  //0xe800004e
 	kAMDNotificationFailedError                     = AMDErrorMake(79),  //0xe800004f
 	kAMDPackageExtractionFailedError                = AMDErrorMake(80),  //0xe8000050
 	kAMDPackageInspectionFailedError                = AMDErrorMake(81),  //0xe8000051
 	kAMDPackageMoveFailedError                      = AMDErrorMake(82),  //0xe8000052
 	kAMDPathConversionFailedError                   = AMDErrorMake(83),  //0xe8000053
 	kAMDRestoreContainerFailedError                 = AMDErrorMake(84),  //0xe8000054
 	kAMDSeatbeltProfileRemovalFailedError           = AMDErrorMake(85),  //0xe8000055
 	kAMDStageCreationFailedError                    = AMDErrorMake(86),  //0xe8000056
 	kAMDSymlinkFailedError                          = AMDErrorMake(87),  //0xe8000057
 	kAMDiTunesArtworkCaptureFailedError             = AMDErrorMake(88),  //0xe8000058
 	kAMDiTunesMetadataCaptureFailedError            = AMDErrorMake(89),  //0xe8000059
 	kAMDAlreadyArchivedError                        = AMDErrorMake(90),  //0xe800005a
 	kAMDServiceLimitError                           = AMDErrorMake(91),  //0xe800005b
 	kAMDInvalidPairRecordError                      = AMDErrorMake(92),  //0xe800005c
 	kAMDServiceProhibitedError                      = AMDErrorMake(93),  //0xe800005d
 	kAMDCheckinSetupFailedError                     = AMDErrorMake(94),  //0xe800005e
 	kAMDCheckinConnectionFailedError                = AMDErrorMake(95),  //0xe800005f
 	kAMDCheckinReceiveFailedError                   = AMDErrorMake(96),  //0xe8000060
 	kAMDCheckinResponseFailedError                  = AMDErrorMake(97),  //0xe8000061
 	kAMDCheckinSendFailedError                      = AMDErrorMake(98),  //0xe8000062
 	kAMDMuxCreateListenerError                      = AMDErrorMake(99),  //0xe8000063
 	kAMDMuxGetListenerError                         = AMDErrorMake(100), //0xe8000064
 	kAMDMuxConnectError                             = AMDErrorMake(101), //0xe8000065
 	kAMDUnknownCommandError                         = AMDErrorMake(102), //0xe8000066
 	kAMDAPIInternalError                            = AMDErrorMake(103), //0xe8000067
 	kAMDSavePairRecordFailedError                   = AMDErrorMake(104), //0xe8000068
 	kAMDCheckinOutOfMemoryError                     = AMDErrorMake(105), //0xe8000069
 	kAMDDeviceTooNewError                           = AMDErrorMake(106), //0xe800006a
 	kAMDDeviceRefNoGood                             = AMDErrorMake(107), //0xe800006b
 	kAMDCannotTranslateError                        = AMDErrorMake(108), //0xe800006c
 	kAMDMobileImageMounterMissingImageSignature     = AMDErrorMake(109), //0xe800006d
 	kAMDMobileImageMounterResponseCreationFailed    = AMDErrorMake(110), //0xe800006e
 	kAMDMobileImageMounterMissingImageType          = AMDErrorMake(111), //0xe800006f
 	kAMDMobileImageMounterMissingImagePath          = AMDErrorMake(112), //0xe8000070
 	kAMDMobileImageMounterImageMapLoadFailed        = AMDErrorMake(113), //0xe8000071
 	kAMDMobileImageMounterAlreadyMounted            = AMDErrorMake(114), //0xe8000072
 	kAMDMobileImageMounterImageMoveFailed           = AMDErrorMake(115), //0xe8000073
 	kAMDMobileImageMounterMountPathMissing          = AMDErrorMake(116), //0xe8000074
 	kAMDMobileImageMounterMountPathNotEmpty         = AMDErrorMake(117), //0xe8000075
 	kAMDMobileImageMounterImageMountFailed          = AMDErrorMake(118), //0xe8000076
 	kAMDMobileImageMounterTrustCacheLoadFailed      = AMDErrorMake(119), //0xe8000077
 	kAMDMobileImageMounterDigestFailed              = AMDErrorMake(120), //0xe8000078
 	kAMDMobileImageMounterDigestCreationFailed      = AMDErrorMake(121), //0xe8000079
 	kAMDMobileImageMounterImageVerificationFailed   = AMDErrorMake(122), //0xe800007a
 	kAMDMobileImageMounterImageInfoCreationFailed   = AMDErrorMake(123), //0xe800007b
 	kAMDMobileImageMounterImageMapStoreFailed       = AMDErrorMake(124), //0xe800007c
 	kAMDBonjourSetupError                           = AMDErrorMake(125), //0xe800007d
 	kAMDDeviceOSVersionTooLow                       = AMDErrorMake(126), //0xe800007e
 	kAMDNoWifiSyncSupportError                      = AMDErrorMake(127), //0xe800007f
 	kAMDDeviceFamilyNotSupported                    = AMDErrorMake(128), //0xe8000080
 	kAMDEscrowLockedError                           = AMDErrorMake(129), //0xe8000081
 	kAMDPairingProhibitedError                      = AMDErrorMake(130), //0xe8000082
 	kAMDProhibitedBySupervision                     = AMDErrorMake(131), //0xe8000083
 	kAMDDeviceDisconnectedError                     = AMDErrorMake(132), //0xe8000084
 	kAMDTooBigError                                 = AMDErrorMake(133), //0xe8000085
 	kAMDPackagePatchFailedError                     = AMDErrorMake(134), //0xe8000086
 	kAMDIncorrectArchitectureError                  = AMDErrorMake(135), //0xe8000087
 	kAMDPluginCopyFailedError                       = AMDErrorMake(136), //0xe8000088
 	kAMDBreadcrumbFailedError                       = AMDErrorMake(137), //0xe8000089
 	kAMDBreadcrumbUnlockError                       = AMDErrorMake(138), //0xe800008a
 	kAMDGeoJSONCaptureFailedError                   = AMDErrorMake(139), //0xe800008b
 	kAMDNewsstandArtworkCaptureFailedError          = AMDErrorMake(140), //0xe800008c
 	kAMDMissingCommandError                         = AMDErrorMake(141), //0xe800008d
 	kAMDNotEntitledError                            = AMDErrorMake(142), //0xe800008e
 	kAMDMissingPackagePathError                     = AMDErrorMake(143), //0xe800008f
 	kAMDMissingContainerPathError                   = AMDErrorMake(144), //0xe8000090
 	kAMDMissingApplicationIdentifierError           = AMDErrorMake(145), //0xe8000091
 	kAMDMissingAttributeValueError                  = AMDErrorMake(146), //0xe8000092
 	kAMDLookupFailedError                           = AMDErrorMake(147), //0xe8000093
 	kAMDDictCreationFailedError                     = AMDErrorMake(148), //0xe8000094
 	kAMDUserDeniedPairingError                      = AMDErrorMake(149), //0xe8000095
 	kAMDPairingDialogResponsePendingError           = AMDErrorMake(150), //0xe8000096
 	kAMDInstallProhibitedError                      = AMDErrorMake(151), //0xe8000097
 	kAMDUninstallProhibitedError                    = AMDErrorMake(152), //0xe8000098
 	kAMDFMiPProtectedError                          = AMDErrorMake(153), //0xe8000099
 	kAMDMCProtected                                 = AMDErrorMake(154), //0xe800009a
 	kAMDMCChallengeRequired                         = AMDErrorMake(155), //0xe800009b
 	kAMDMissingBundleVersionError                   = AMDErrorMake(156), //0xe800009c
} SDMMD_Errors;
//via: for (int i = 0; i < 157; i++) { NSLog(@"\t%s = AMDErrorMake(%i), //0x%x", AMDErrorString(0xe8000000 + i), i, 0xe8000000 + i); }

typedef kern_return_t sdmmd_return_t;

typedef struct SDMMD_DebugReturnCode {
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
	MDERR_USBMUX_OK = 0,
	MDERR_USBMUX_ARG_NULL = 0x16,
	MDERR_USBMUX_FAILED = 0xffffffff
} SDMMD_USBMuxErrorCodes;

#define sdmmd_usbmux_return_t enum SDMMD_USBMuxErrorCodes

typedef struct SDMMD_SIMStatusCode {
	CFStringRef codeName;
	uint32_t resultCode;
} __attribute__((packed)) SDMMD_SIMStatusCode;

// Implemented in SDMMD_AMDevice.c
// Initializes KnownSIMCodes and KnownActivationStates
void SDMMD_AMDevice_Status_Initialize(void);

#define sdmmd_sim_return_t struct SDMMD_SIMStatusCode

#define kKnownSIMCodesNum 0xd

typedef struct SDMMD_ActivationState {
	CFStringRef statusName;
	uint32_t statusCode;
} SDMMD_ActivationState;

#define sdmmd_activation_return_t struct SDMMD_ActivationState

#define kKnownActivationStatesNum 0x9

static sdmmd_activation_return_t KnownActivationStates[kKnownActivationStatesNum] = {0};

static sdmmd_sim_return_t KnownSIMCodes[kKnownSIMCodesNum] = {0};

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

typedef sdmmd_return_t sdmmd_dl_return_t;

typedef kern_return_t (*ErrorConvert)(CFStringRef error);

kern_return_t SDMMD__ErrorHandler(ErrorConvert converter, CFDictionaryRef response);

sdmmd_return_t SDMMD_ImageMounterErrorConvert(CFStringRef error);

sdmmd_dl_return_t SDMMD__ConvertLockdowndError(CFStringRef error);

sdmmd_return_t SDMMD__ConvertServiceError(CFStringRef error);

static char * * SDMMD_NewErrorLookup = {0};
static uint64_t SDMMD_ErrorsCreatedAtRuntime = 0;

#define kAMDTotalErrors 0x9d

static char* SDMMD_ServiceConnectionErrorString[kAMDTotalErrors] = { "kAMDSuccess", "kAMDUndefinedError", "kAMDBadHeaderError", "kAMDNoResourcesError", "kAMDReadError", "kAMDWriteError", "kAMDUnknownPacketError", "kAMDInvalidArgumentError", "kAMDNotFoundError", "kAMDIsDirectoryError", "kAMDPermissionError", "kAMDNotConnectedError", "kAMDTimeOutError", "kAMDOverrunError", "kAMDEOFError", "kAMDUnsupportedError", "kAMDFileExistsError", "kAMDBusyError", "kAMDCryptoError", "kAMDInvalidResponseError", "kAMDMissingKeyError", "kAMDMissingValueError", "kAMDGetProhibitedError", "kAMDSetProhibitedError", "kAMDRemoveProhibitedError", "kAMDImmutableValueError", "kAMDPasswordProtectedError", "kAMDMissingHostIDError", "kAMDInvalidHostIDError", "kAMDSessionActiveError", "kAMDSessionInactiveError", "kAMDMissingSessionIDError", "kAMDInvalidSessionIDError", "kAMDMissingServiceError", "kAMDInvalidServiceError", "kAMDInvalidCheckinError", "kAMDCheckinTimeoutError", "kAMDMissingPairRecordError", "kAMDInvalidActivationRecordError", "kAMDMissingActivationRecordError", "kAMDWrongDroidError", "kAMDSUVerificationError", "kAMDSUPatchError", "kAMDSUFirmwareError", "kAMDProvisioningProfileNotValid", "kAMDSendMessageError", "kAMDReceiveMessageError", "kAMDMissingOptionsError", "kAMDMissingImageTypeError", "kAMDDigestFailedError", "kAMDStartServiceError", "kAMDInvalidDiskImageError", "kAMDMissingDigestError", "kAMDMuxError", "kAMDApplicationAlreadyInstalledError", "kAMDApplicationMoveFailedError", "kAMDApplicationSINFCaptureFailedError", "kAMDApplicationSandboxFailedError", "kAMDApplicationVerificationFailedError", "kAMDArchiveDestructionFailedError", "kAMDBundleVerificationFailedError", "kAMDCarrierBundleCopyFailedError", "kAMDCarrierBundleDirectoryCreationFailedError", "kAMDCarrierBundleMissingSupportedSIMsError", "kAMDCommCenterNotificationFailedError", "kAMDContainerCreationFailedError", "kAMDContainerP0wnFailedError", "kAMDContainerRemovalFailedError", "kAMDEmbeddedProfileInstallFailedError", "kAMDErrorError", "kAMDExecutableTwiddleFailedError", "kAMDExistenceCheckFailedError", "kAMDInstallMapUpdateFailedError", "kAMDManifestCaptureFailedError", "kAMDMapGenerationFailedError", "kAMDMissingBundleExecutableError", "kAMDMissingBundleIdentifierError", "kAMDMissingBundlePathError", "kAMDMissingContainerError", "kAMDNotificationFailedError", "kAMDPackageExtractionFailedError", "kAMDPackageInspectionFailedError", "kAMDPackageMoveFailedError", "kAMDPathConversionFailedError", "kAMDRestoreContainerFailedError", "kAMDSeatbeltProfileRemovalFailedError", "kAMDStageCreationFailedError", "kAMDSymlinkFailedError", "kAMDiTunesArtworkCaptureFailedError", "kAMDiTunesMetadataCaptureFailedError", "kAMDAlreadyArchivedError", "kAMDServiceLimitError", "kAMDInvalidPairRecordError", "kAMDServiceProhibitedError", "kAMDCheckinSetupFailedError", "kAMDCheckinConnectionFailedError", "kAMDCheckinReceiveFailedError", "kAMDCheckinResponseFailedError", "kAMDCheckinSendFailedError", "kAMDMuxCreateListenerError", "kAMDMuxGetListenerError", "kAMDMuxConnectError", "kAMDUnknownCommandError", "kAMDAPIInternalError", "kAMDSavePairRecordFailedError", "kAMDCheckinOutOfMemoryError", "kAMDDeviceTooNewError", "kAMDDeviceRefNoGood", "kAMDCannotTranslateError", "kAMDMobileImageMounterMissingImageSignature", "kAMDMobileImageMounterResponseCreationFailed", "kAMDMobileImageMounterMissingImageType", "kAMDMobileImageMounterMissingImagePath", "kAMDMobileImageMounterImageMapLoadFailed", "kAMDMobileImageMounterAlreadyMounted", "kAMDMobileImageMounterImageMoveFailed", "kAMDMobileImageMounterMountPathMissing", "kAMDMobileImageMounterMountPathNotEmpty", "kAMDMobileImageMounterImageMountFailed", "kAMDMobileImageMounterTrustCacheLoadFailed", "kAMDMobileImageMounterDigestFailed", "kAMDMobileImageMounterDigestCreationFailed", "kAMDMobileImageMounterImageVerificationFailed", "kAMDMobileImageMounterImageInfoCreationFailed", "kAMDMobileImageMounterImageMapStoreFailed", "kAMDBonjourSetupError", "kAMDDeviceOSVersionTooLow", "kAMDNoWifiSyncSupportError", "kAMDDeviceFamilyNotSupported", "kAMDEscrowLockedError", "kAMDPairingProhibitedError", "kAMDProhibitedBySupervision", "kAMDDeviceDisconnectedError", "kAMDTooBigError", "kAMDPackagePatchFailedError", "kAMDIncorrectArchitectureError", "kAMDPluginCopyFailedError", "kAMDBreadcrumbFailedError", "kAMDBreadcrumbUnlockError", "kAMDGeoJSONCaptureFailedError", "kAMDNewsstandArtworkCaptureFailedError", "kAMDMissingCommandError", "kAMDNotEntitledError", "kAMDMissingPackagePathError", "kAMDMissingContainerPathError", "kAMDMissingApplicationIdentifierError", "kAMDMissingAttributeValueError", "kAMDLookupFailedError", "kAMDDictCreationFailedError", "kAMDUserDeniedPairingError", "kAMDPairingDialogResponsePendingError", "kAMDInstallProhibitedError", "kAMDUninstallProhibitedError", "kAMDFMiPProtectedError", "kAMDMCProtected", "kAMDMCChallengeRequired", "kAMDMissingBundleVersionError"};
__attribute__ ((unused)) static char*  SDMMD_AMDErrorString(kern_return_t error) {
    kern_return_t index = error & 0xff;
	int check = AMDErrorMake(error) - kAMDTotalErrors;
	if (index > kAMDTotalErrors-1 && (check > -1 && SDMMD_ErrorsCreatedAtRuntime > 0 && check < SDMMD_ErrorsCreatedAtRuntime)) {
		return SDMMD_NewErrorLookup[check];
	}
	else {
		return SDMMD_ServiceConnectionErrorString[index];
	}
}

static char* SDMMD_AFCConnectionErrorString[0x18] = {"kAFCSuccess", "kAFCUndefinedError", "kAFCBadHeaderError", "kAFCNoResourcesError", "kAFCReadError", "kAFCWriteError", "kAFCUnknownPacketError", "kAFCInvalidArgumentError", "kAFCNotFoundError", "kAFCIsDirectoryError", "kAFCPermissionError", "kAFCNotConnectedError", "kAFCTimeOutError", "kAFCOverrunError", "kAFCEOFError", "kAFCUnsupportedError", "kAFCFileExistsError", "kAFCBusyError", "kAFCNoSpaceError", "kAFCWouldBlockError", "kAFCInputOutputError", "kAFCInterruptedError", "kAFCInProgressError", "kAFCInternalError"};
__attribute__ ((unused)) static char* SDMMD_AFCErrorString(kern_return_t error) {
    uint32_t index = error & 0xff;
    if (index > 0x18) return NULL;
	return SDMMD_AFCConnectionErrorString[index];
}

#define SDM_MD_CallSuccessful(result_code) ((result_code == kAMDSuccess) || (result_code == MDERR_USBMUX_OK) || (result_code == LD_ERR_OK))

#define SDMMD_CondSuccess(code, block) if (SDM_MD_CallSuccessful(code)) { if (1) block } else { printf("%s:%s:%i %s\n",__FILE__,__FUNCTION__,__LINE__,SDMMD_AMDErrorString(result)); }
#define SDMMD_CondSuccessElse(code, block, elseBlock) if (SDM_MD_CallSuccessful(code)) { if (1) block } else { printf("%s:%s:%i %s\n",__FILE__,__FUNCTION__,__LINE__,SDMMD_AMDErrorString(result)); if (1) elseBlock }

#endif