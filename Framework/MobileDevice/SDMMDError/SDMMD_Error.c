/*
 *  SDMMD_Error.c
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

#ifndef _SDM_MD_ERROR_C_
#define _SDM_MD_ERROR_C_

#include "SDMMD_Error.h"

kern_return_t SDMMD__ErrorHandler(ErrorConvert converter, CFDictionaryRef response) {
	kern_return_t response_code = kAMDInvalidResponseError;
	CFTypeRef error = CFDictionaryGetValue(response, CFSTR("Error"));
	if (error) {
		response_code = converter(error);
	}
	else {
		response_code = kAMDSuccess;
	}
	return response_code;
}

sdmmd_return_t SDMMD_ImageMounterErrorConvert(CFStringRef error) {
	sdmmd_return_t result = kAMDSuccess;
	if (CFEqual(error, CFSTR("MissingImageSignature"))) {
		result = kAMDMobileImageMounterMissingImageSignature;
	}
	else if (CFEqual(error, CFSTR("ResponseCreationFailed"))) {
		result = kAMDMobileImageMounterResponseCreationFailed;
	}
	else if (CFEqual(error, CFSTR("MissingImageType"))) {
		result = kAMDMobileImageMounterMissingImageType;
	}
	else if (CFEqual(error, CFSTR("MissingImagePath"))) {
		result = kAMDMobileImageMounterMissingImagePath;
	}
	else if (CFEqual(error, CFSTR("ImageMapLoadFailed "))) {
		result = kAMDMobileImageMounterImageMapLoadFailed;
	}
	else if (CFEqual(error, CFSTR("AlreadyMounted"))) {
		result = kAMDMobileImageMounterAlreadyMounted;
	}
	else if (CFEqual(error, CFSTR("ImageMoveFailed"))) {
		result = kAMDMobileImageMounterImageMoveFailed;
	}
	else if (CFEqual(error, CFSTR("MountPathMissing"))) {
		result = kAMDMobileImageMounterMountPathMissing;
	}
	else if (CFEqual(error, CFSTR("MountPathNotEmpty"))) {
		result = kAMDMobileImageMounterMountPathNotEmpty;
	}
	else if (CFEqual(error, CFSTR("ImageMountFailed"))) {
		result = kAMDMobileImageMounterImageMountFailed;
	}
	else if (CFEqual(error, CFSTR("TrustCacheLoadFailed"))) {
		result = kAMDMobileImageMounterTrustCacheLoadFailed;
	}
	else if (CFEqual(error, CFSTR("DigestFailed"))) {
		result = kAMDMobileImageMounterDigestFailed;
	}
	else if (CFEqual(error, CFSTR("DigestCreationFailed"))) {
		result = kAMDMobileImageMounterDigestCreationFailed;
	}
	else if (CFEqual(error, CFSTR("ImageVerificationFailed"))) {
		result = kAMDMobileImageMounterImageVerificationFailed;
	}
	else if (CFEqual(error, CFSTR("ImageInfoCreationFailed"))) {
		result = kAMDMobileImageMounterImageInfoCreationFailed;
	}
	else if (CFEqual(error, CFSTR("ImageMapStoreFailed"))) {
		result = kAMDMobileImageMounterImageMapStoreFailed;
	}
	else {
		result = kAMDErrorError;
	}
	return result;
}

sdmmd_dl_return_t SDMMD__ConvertLockdowndError(CFStringRef error) {
	sdmmd_dl_return_t result = 0xe8000013;
	if (error) {
		if (CFStringCompare(error, CFSTR("InvalidResponse"), 0) == kCFCompareEqualTo) { result = 0xe8000013; }
		else if (CFStringCompare(error, CFSTR("MissingKey"), 0) == kCFCompareEqualTo) { result = 0xe8000014; }
		else if (CFStringCompare(error, CFSTR("MissingValue"), 0) == kCFCompareEqualTo) { result = 0xe8000015; }
		else if (CFStringCompare(error, CFSTR("GetProhibited"), 0) == kCFCompareEqualTo) { result = 0xe8000016; }
		else if (CFStringCompare(error, CFSTR("SetProhibited"), 0) == kCFCompareEqualTo) { result = 0xe8000017; }
		else if (CFStringCompare(error, CFSTR("RemoveProhibited"), 0) == kCFCompareEqualTo) { result = 0xe8000018; }
		else if (CFStringCompare(error, CFSTR("ImmutableValue"), 0) == kCFCompareEqualTo) { result = 0xe8000019; }
		else if (CFStringCompare(error, CFSTR("PasswordProtected"), 0) == kCFCompareEqualTo) { result = 0xe800001a; }
		else if (CFStringCompare(error, CFSTR("MissingHostID"), 0) == kCFCompareEqualTo) { result = 0xe800001b; }
		else if (CFStringCompare(error, CFSTR("InvalidHostID"), 0) == kCFCompareEqualTo) { result = 0xe800001c; }
		else if (CFStringCompare(error, CFSTR("SessionActive"), 0) == kCFCompareEqualTo) { result = 0xe800001d; }
		else if (CFStringCompare(error, CFSTR("SessionInactive"), 0) == kCFCompareEqualTo) { result = 0xe800001e; }
		else if (CFStringCompare(error, CFSTR("MissingSessionID"), 0) == kCFCompareEqualTo) { result = 0xe800001f; }
		else if (CFStringCompare(error, CFSTR("InvalidSessionID"), 0) == kCFCompareEqualTo) { result = 0xe8000020; }
		else if (CFStringCompare(error, CFSTR("MissingService"), 0) == kCFCompareEqualTo) { result = 0xe8000021; }
		else if (CFStringCompare(error, CFSTR("InvalidService"), 0) == kCFCompareEqualTo) { result = 0xe8000022; }
		else if (CFStringCompare(error, CFSTR("ServiceLimit"), 0) == kCFCompareEqualTo) { result = 0xe800005b; }
		else if (CFStringCompare(error, CFSTR("CheckinSetupFailed"), 0) == kCFCompareEqualTo) { result = 0xe800005e; }
		else if (CFStringCompare(error, CFSTR("InvalidCheckin"), 0) == kCFCompareEqualTo) { result = 0xe8000023; }
		else if (CFStringCompare(error, CFSTR("CheckinTimeout"), 0) == kCFCompareEqualTo) { result = 0xe8000024; }
		else if (CFStringCompare(error, CFSTR("CheckinConnectionFailed"), 0) == kCFCompareEqualTo) { result = 0xe800005f; }
		else if (CFStringCompare(error, CFSTR("CheckinReceiveFailed"), 0) == kCFCompareEqualTo) { result = 0xe8000060; }
		else if (CFStringCompare(error, CFSTR("CheckinResponseFailed"), 0) == kCFCompareEqualTo) { result = 0xe8000061; }
		else if (CFStringCompare(error, CFSTR("CheckinOutOfMemory"), 0) == kCFCompareEqualTo) { result = 0xe8000069; }
		else if (CFStringCompare(error, CFSTR("CheckinSendFailed"), 0) == kCFCompareEqualTo) { result = 0xe8000062; }
		else if (CFStringCompare(error, CFSTR("MissingPairRecord"), 0) == kCFCompareEqualTo) { result = 0xe8000025; }
		else if (CFStringCompare(error, CFSTR("SavePairRecordFailed"), 0) == kCFCompareEqualTo) { result = 0xe8000068; }
		else if (CFStringCompare(error, CFSTR("InvalidPairRecord"), 0) == kCFCompareEqualTo) { result = 0xe800005c; }
		else if (CFStringCompare(error, CFSTR("InvalidActivationRecord"), 0) == kCFCompareEqualTo) { result = 0xe8000026; }
		else if (CFStringCompare(error, CFSTR("MissingActivationRecord"), 0) == kCFCompareEqualTo) { result = 0xe8000027; }
		else if (CFStringCompare(error, CFSTR("ServiceProhibited"), 0) == kCFCompareEqualTo) { result = 0xe800005d; }
		else if (CFStringCompare(error, CFSTR("WrongDroid"), 0) == kCFCompareEqualTo) { result = 0xe8000028; }
		else if (CFStringCompare(error, CFSTR("EscrowLocked"), 0) == kCFCompareEqualTo) { result = 0xe8000081; }
		else if (CFStringCompare(error, CFSTR("NotAValidChaperoneHost"), 0) == kCFCompareEqualTo) { result = 0xe8000083; }
		else if (CFStringCompare(error, CFSTR("PairingProhibitedOverThisConnection"), 0) == kCFCompareEqualTo) { result = 0xe8000082; }
		else {
			//result = SDMMD__AddNewAMDError(error);
			result = 0xffffffff;
		}		
	}
	return result;
}

sdmmd_return_t SDMMD__ConvertServiceError(CFStringRef error) {
	sdmmd_return_t result = kAMDErrorError;
	if (error) {
		if (CFStringCompare(error, CFSTR("AlreadyArchived"), 0) == kCFCompareEqualTo) { result = kAMDAlreadyArchivedError; }
		else if (CFStringCompare(error, CFSTR("APIInternalError"), 0) == kCFCompareEqualTo) { result = kAMDAPIInternalError; }
		else if (CFStringCompare(error, CFSTR("ApplicationAlreadyInstalled"), 0) == kCFCompareEqualTo) { result = kAMDApplicationAlreadyInstalledError; }
		else if (CFStringCompare(error, CFSTR("ApplicationMoveFailed"), 0) == kCFCompareEqualTo) { result = kAMDApplicationMoveFailedError; }
		else if (CFStringCompare(error, CFSTR("ApplicationSINFCaptureFailed"), 0) == kCFCompareEqualTo) { result = kAMDApplicationSINFCaptureFailedError; }
		else if (CFStringCompare(error, CFSTR("ApplicationSandboxFailed"), 0) == kCFCompareEqualTo) { result = kAMDApplicationSandboxFailedError; }
		else if (CFStringCompare(error, CFSTR("ApplicationVerificationFailed"), 0) == kCFCompareEqualTo) { result = kAMDApplicationVerificationFailedError; }
		else if (CFStringCompare(error, CFSTR("ArchiveDestructionFailed"), 0) == kCFCompareEqualTo) { result = kAMDArchiveDestructionFailedError; }
		else if (CFStringCompare(error, CFSTR("BundleVerificationFailed"), 0) == kCFCompareEqualTo) { result = kAMDBundleVerificationFailedError; }
		else if (CFStringCompare(error, CFSTR("CarrierBundleCopyFailed"), 0) == kCFCompareEqualTo) { result = kAMDCarrierBundleCopyFailedError; }
		else if (CFStringCompare(error, CFSTR("CarrierBundleDirectoryCreationFailed"), 0) == kCFCompareEqualTo) { result = kAMDCarrierBundleDirectoryCreationFailedError; }
		else if (CFStringCompare(error, CFSTR("CarrierBundleMissingSupportedSIMs"), 0) == kCFCompareEqualTo) { result = kAMDCarrierBundleMissingSupportedSIMsError; }
		else if (CFStringCompare(error, CFSTR("CommCenterNotificationFailed"), 0) == kCFCompareEqualTo) { result = kAMDCommCenterNotificationFailedError; }
		else if (CFStringCompare(error, CFSTR("ContainerCreationFailed"), 0) == kCFCompareEqualTo) { result = kAMDContainerCreationFailedError; }
		else if (CFStringCompare(error, CFSTR("ContainerP0wnFailed"), 0) == kCFCompareEqualTo) { result = kAMDContainerP0wnFailedError; }
		else if (CFStringCompare(error, CFSTR("ContainerRemovalFailed"), 0) == kCFCompareEqualTo) { result = kAMDContainerRemovalFailedError; }
		else if (CFStringCompare(error, CFSTR("EmbeddedProfileInstallFailed"), 0) == kCFCompareEqualTo) { result = kAMDEmbeddedProfileInstallFailedError; }
		else if (CFStringCompare(error, CFSTR("Error"), 0) == kCFCompareEqualTo) { result = kAMDErrorError; }
		else if (CFStringCompare(error, CFSTR("ExecutableTwiddleFailed"), 0) == kCFCompareEqualTo) { result = kAMDExecutableTwiddleFailedError; }
		else if (CFStringCompare(error, CFSTR("ExistenceCheckFailed"), 0) == kCFCompareEqualTo) { result = kAMDExistenceCheckFailedError; }
		else if (CFStringCompare(error, CFSTR("InstallMapUpdateFailed"), 0) == kCFCompareEqualTo) { result = kAMDInstallMapUpdateFailedError; }
		else if (CFStringCompare(error, CFSTR("ManifestCaptureFailed"), 0) == kCFCompareEqualTo) { result = kAMDManifestCaptureFailedError; }
		else if (CFStringCompare(error, CFSTR("MapGenerationFailed"), 0) == kCFCompareEqualTo) { result = kAMDMapGenerationFailedError; }
		else if (CFStringCompare(error, CFSTR("MissingBundleExecutable"), 0) == kCFCompareEqualTo) { result = kAMDMissingBundleExecutableError; }
		else if (CFStringCompare(error, CFSTR("MissingBundleIdentifier"), 0) == kCFCompareEqualTo) { result = kAMDMissingBundleIdentifierError; }
		else if (CFStringCompare(error, CFSTR("MissingBundlePath"), 0) == kCFCompareEqualTo) { result = kAMDMissingBundlePathError; }
		else if (CFStringCompare(error, CFSTR("MissingContainer"), 0) == kCFCompareEqualTo) { result = kAMDMissingContainerError; }
		else if (CFStringCompare(error, CFSTR("NotificationFailed"), 0) == kCFCompareEqualTo) { result = kAMDNotificationFailedError; }
		else if (CFStringCompare(error, CFSTR("PackageExtractionFailed"), 0) == kCFCompareEqualTo) { result = kAMDPackageExtractionFailedError; }
		else if (CFStringCompare(error, CFSTR("PackageInspectionFailed"), 0) == kCFCompareEqualTo) { result = kAMDPackageInspectionFailedError; }
		else if (CFStringCompare(error, CFSTR("PackageMoveFailed"), 0) == kCFCompareEqualTo) { result = kAMDPackageMoveFailedError; }
		else if (CFStringCompare(error, CFSTR("PathConversionFailed"), 0) == kCFCompareEqualTo) { result = kAMDPathConversionFailedError; }
		else if (CFStringCompare(error, CFSTR("RestoreContainerFailed"), 0) == kCFCompareEqualTo) { result = kAMDRestoreContainerFailedError; }
		else if (CFStringCompare(error, CFSTR("SeatbeltProfileRemovalFailed"), 0) == kCFCompareEqualTo) { result = kAMDSeatbeltProfileRemovalFailedError; }
		else if (CFStringCompare(error, CFSTR("StageCreationFailed"), 0) == kCFCompareEqualTo) { result = kAMDStageCreationFailedError; }
		else if (CFStringCompare(error, CFSTR("SymlinkFailed"), 0) == kCFCompareEqualTo) { result = kAMDSymlinkFailedError; }
		else if (CFStringCompare(error, CFSTR("UnknownCommand"), 0) == kCFCompareEqualTo) { result = kAMDUnknownCommandError; }
		else if (CFStringCompare(error, CFSTR("iTunesArtworkCaptureFailed"), 0) == kCFCompareEqualTo) { result = kAMDiTunesArtworkCaptureFailedError; }
		else if (CFStringCompare(error, CFSTR("iTunesMetadataCaptureFailed"), 0) == kCFCompareEqualTo) { result = kAMDiTunesMetadataCaptureFailedError; }
		else if (CFStringCompare(error, CFSTR("DeviceOSVersionTooLow"), 0) == kCFCompareEqualTo) { result = kAMDDeviceOSVersionTooLow; }
		else if (CFStringCompare(error, CFSTR("DeviceFamilyNotSupported"), 0) == kCFCompareEqualTo) { result = kAMDDeviceFamilyNotSupported; }
		else if (CFStringCompare(error, CFSTR("PackagePatchFailed"), 0) == kCFCompareEqualTo) { result = kAMDPackagePatchFailedError; }
		else if (CFStringCompare(error, CFSTR("IncorrectArchitecture"), 0) == kCFCompareEqualTo) { result = kAMDIncorrectArchitectureError; }
		else if (CFStringCompare(error, CFSTR("PluginCopyFailed"), 0) == kCFCompareEqualTo) { result = kAMDPluginCopyFailedError; }
		else if (CFStringCompare(error, CFSTR("BreadcrumbFailed"), 0) == kCFCompareEqualTo) { result = kAMDBreadcrumbFailedError; }
		else if (CFStringCompare(error, CFSTR("BreadcrumbUnlockFailed"), 0) == kCFCompareEqualTo) { result = kAMDBreadcrumbUnlockError; }
		else if (CFStringCompare(error, CFSTR("GeoJSONCaptureFailed"), 0) == kCFCompareEqualTo) { result = kAMDGeoJSONCaptureFailedError; }
		else if (CFStringCompare(error, CFSTR("NewsstandArtworkCaptureFailed"), 0) == kCFCompareEqualTo) { result = kAMDNewsstandArtworkCaptureFailedError; }
		else if (CFStringCompare(error, CFSTR("MissingCommand"), 0) == kCFCompareEqualTo) { result = kAMDNewsstandArtworkCaptureFailedError; }
		else if (CFStringCompare(error, CFSTR("NotEntitled"), 0) == kCFCompareEqualTo) { result = kAMDNotEntitledError; }
		else if (CFStringCompare(error, CFSTR("MissingPackagePath"), 0) == kCFCompareEqualTo) { result = kAMDMissingPackagePathError; }
		else if (CFStringCompare(error, CFSTR("MissingContainerPath"), 0) == kCFCompareEqualTo) { result = kAMDMissingContainerPathError; }
		else if (CFStringCompare(error, CFSTR("MissingApplicationIdentifier"), 0) == kCFCompareEqualTo) { result = kAMDMissingApplicationIdentifierError; }
		else if (CFStringCompare(error, CFSTR("MissingAttributeValue"), 0) == kCFCompareEqualTo) { result = kAMDMissingAttributeValueError; }
		else if (CFStringCompare(error, CFSTR("LookupFailed"), 0) == kCFCompareEqualTo) { result = kAMDLookupFailedError; }
		else if (CFStringCompare(error, CFSTR("DictCreationFailed"), 0) == kCFCompareEqualTo) { result = kAMDDictCreationFailedError; }
		else if (CFStringCompare(error, CFSTR("InstallProhibited"), 0) == kCFCompareEqualTo) { result = kAMDInstallProhibitedError; }
		else if (CFStringCompare(error, CFSTR("UninstallProhibited"), 0) == kCFCompareEqualTo) { result = kAMDUninstallProhibitedError; }
		else if (CFStringCompare(error, CFSTR("MissingBundleVersion"), 0) == kCFCompareEqualTo) { result = kAMDMissingBundleVersionError; }
		else {
			result = 0xffffffff;
		}
	}
	return result;
}


#endif