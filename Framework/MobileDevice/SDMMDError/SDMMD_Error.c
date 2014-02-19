/*
 *  SDMMD_Error.c
 *  SDMMobileDevice
 *
 * Copyright (c) 2013, Sam Marshall
* All rights reserved.
* 
* Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
* 
* 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
* 
* 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
* 
* 3. Neither the name of Sam Marshall nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
* 
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 */

#ifndef _SDM_MD_ERROR_C_
#define _SDM_MD_ERROR_C_

#include "SDMMD_Error.h"

sdmmd_dl_return_t SDMMD__ConvertLockdowndError(CFStringRef error) {
	sdmmd_dl_return_t result = 0xe8000013;
	if (error) {
		if (CFStringCompare(error, CFSTR("InvalidResponse"), 0) == 0) result = 0xe8000013;
		else if (CFStringCompare(error, CFSTR("MissingKey"), 0) == 0) result = 0xe8000014;
		else if (CFStringCompare(error, CFSTR("MissingValue"), 0) == 0) result = 0xe8000015;
		else if (CFStringCompare(error, CFSTR("GetProhibited"), 0) == 0) result = 0xe8000016;
		else if (CFStringCompare(error, CFSTR("SetProhibited"), 0) == 0) result = 0xe8000017;
		else if (CFStringCompare(error, CFSTR("RemoveProhibited"), 0) == 0) result = 0xe8000018;
		else if (CFStringCompare(error, CFSTR("ImmutableValue"), 0) == 0) result = 0xe8000019;
		else if (CFStringCompare(error, CFSTR("PasswordProtected"), 0) == 0) result = 0xe800001a;
		else if (CFStringCompare(error, CFSTR("MissingHostID"), 0) == 0) result = 0xe800001b;
		else if (CFStringCompare(error, CFSTR("InvalidHostID"), 0) == 0) result = 0xe800001c;
		else if (CFStringCompare(error, CFSTR("SessionActive"), 0) == 0) result = 0xe800001d;
		else if (CFStringCompare(error, CFSTR("SessionInactive"), 0) == 0) result = 0xe800001e;
		else if (CFStringCompare(error, CFSTR("MissingSessionID"), 0) == 0) result = 0xe800001f;
		else if (CFStringCompare(error, CFSTR("InvalidSessionID"), 0) == 0) result = 0xe8000020;
		else if (CFStringCompare(error, CFSTR("MissingService"), 0) == 0) result = 0xe8000021;
		else if (CFStringCompare(error, CFSTR("InvalidService"), 0) == 0) result = 0xe8000022;
		else if (CFStringCompare(error, CFSTR("ServiceLimit"), 0) == 0) result = 0xe800005b;
		else if (CFStringCompare(error, CFSTR("CheckinSetupFailed"), 0) == 0) result = 0xe800005e;
		else if (CFStringCompare(error, CFSTR("InvalidCheckin"), 0) == 0) result = 0xe8000023;
		else if (CFStringCompare(error, CFSTR("CheckinTimeout"), 0) == 0) result = 0xe8000024;
		else if (CFStringCompare(error, CFSTR("CheckinConnectionFailed"), 0) == 0) result = 0xe800005f;
		else if (CFStringCompare(error, CFSTR("CheckinReceiveFailed"), 0) == 0) result = 0xe8000060;
		else if (CFStringCompare(error, CFSTR("CheckinResponseFailed"), 0) == 0) result = 0xe8000061;
		else if (CFStringCompare(error, CFSTR("CheckinOutOfMemory"), 0) == 0) result = 0xe8000069;
		else if (CFStringCompare(error, CFSTR("CheckinSendFailed"), 0) == 0) result = 0xe8000062;
		else if (CFStringCompare(error, CFSTR("MissingPairRecord"), 0) == 0) result = 0xe8000025;
		else if (CFStringCompare(error, CFSTR("SavePairRecordFailed"), 0) == 0) result = 0xe8000068;
		else if (CFStringCompare(error, CFSTR("InvalidPairRecord"), 0) == 0) result = 0xe800005c;
		else if (CFStringCompare(error, CFSTR("InvalidActivationRecord"), 0) == 0) result = 0xe8000026;
		else if (CFStringCompare(error, CFSTR("MissingActivationRecord"), 0) == 0) result = 0xe8000027;
		else if (CFStringCompare(error, CFSTR("ServiceProhibited"), 0) == 0) result = 0xe800005d;
		else if (CFStringCompare(error, CFSTR("WrongDroid"), 0) == 0) result = 0xe8000028;
		else if (CFStringCompare(error, CFSTR("EscrowLocked"), 0) == 0) result = 0xe8000081;
		else if (CFStringCompare(error, CFSTR("NotAValidChaperoneHost"), 0) == 0) result = 0xe8000083;
		else if (CFStringCompare(error, CFSTR("PairingProhibitedOverThisConnection"), 0) == 0) result = 0xe8000082;
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
		if (CFStringCompare(error, CFSTR("AlreadyArchived"), 0) == 0) result = kAMDAlreadyArchivedError;
		else if (CFStringCompare(error, CFSTR("APIInternalError"), 0) == 0) result = kAMDAPIInternalError;
		else if (CFStringCompare(error, CFSTR("ApplicationAlreadyInstalled"), 0) == 0) result = kAMDApplicationAlreadyInstalledError;
		else if (CFStringCompare(error, CFSTR("ApplicationMoveFailed"), 0) == 0) result = kAMDApplicationMoveFailedError;
		else if (CFStringCompare(error, CFSTR("ApplicationSINFCaptureFailed"), 0) == 0) result = kAMDApplicationSINFCaptureFailedError;
		else if (CFStringCompare(error, CFSTR("ApplicationSandboxFailed"), 0) == 0) result = kAMDApplicationSandboxFailedError;
		else if (CFStringCompare(error, CFSTR("ApplicationVerificationFailed"), 0) == 0) result = kAMDApplicationVerificationFailedError;
		else if (CFStringCompare(error, CFSTR("ArchiveDestructionFailed"), 0) == 0) result = kAMDArchiveDestructionFailedError;
		else if (CFStringCompare(error, CFSTR("BundleVerificationFailed"), 0) == 0) result = kAMDBundleVerificationFailedError;
		else if (CFStringCompare(error, CFSTR("CarrierBundleCopyFailed"), 0) == 0) result = kAMDCarrierBundleCopyFailedError;
		else if (CFStringCompare(error, CFSTR("CarrierBundleDirectoryCreationFailed"), 0) == 0) result = kAMDCarrierBundleDirectoryCreationFailedError;
		else if (CFStringCompare(error, CFSTR("CarrierBundleMissingSupportedSIMs"), 0) == 0) result = kAMDCarrierBundleMissingSupportedSIMsError;
		else if (CFStringCompare(error, CFSTR("CommCenterNotificationFailed"), 0) == 0) result = kAMDCommCenterNotificationFailedError;
		else if (CFStringCompare(error, CFSTR("ContainerCreationFailed"), 0) == 0) result = kAMDContainerCreationFailedError;
		else if (CFStringCompare(error, CFSTR("ContainerP0wnFailed"), 0) == 0) result = kAMDContainerP0wnFailedError;
		else if (CFStringCompare(error, CFSTR("ContainerRemovalFailed"), 0) == 0) result = kAMDContainerRemovalFailedError;
		else if (CFStringCompare(error, CFSTR("EmbeddedProfileInstallFailed"), 0) == 0) result = kAMDEmbeddedProfileInstallFailedError;
		else if (CFStringCompare(error, CFSTR("Error"), 0) == 0) result = kAMDErrorError;
		else if (CFStringCompare(error, CFSTR("ExecutableTwiddleFailed"), 0) == 0) result = kAMDExecutableTwiddleFailedError;
		else if (CFStringCompare(error, CFSTR("ExistenceCheckFailed"), 0) == 0) result = kAMDExistenceCheckFailedError;
		else if (CFStringCompare(error, CFSTR("InstallMapUpdateFailed"), 0) == 0) result = kAMDInstallMapUpdateFailedError;
		else if (CFStringCompare(error, CFSTR("ManifestCaptureFailed"), 0) == 0) result = kAMDManifestCaptureFailedError;
		else if (CFStringCompare(error, CFSTR("MapGenerationFailed"), 0) == 0) result = kAMDMapGenerationFailedError;
		else if (CFStringCompare(error, CFSTR("MissingBundleExecutable"), 0) == 0) result = kAMDMissingBundleExecutableError;
		else if (CFStringCompare(error, CFSTR("MissingBundleIdentifier"), 0) == 0) result = kAMDMissingBundleIdentifierError;
		else if (CFStringCompare(error, CFSTR("MissingBundlePath"), 0) == 0) result = kAMDMissingBundlePathError;
		else if (CFStringCompare(error, CFSTR("MissingContainer"), 0) == 0) result = kAMDMissingContainerError;
		else if (CFStringCompare(error, CFSTR("NotificationFailed"), 0) == 0) result = kAMDNotificationFailedError;
		else if (CFStringCompare(error, CFSTR("PackageExtractionFailed"), 0) == 0) result = kAMDPackageExtractionFailedError;
		else if (CFStringCompare(error, CFSTR("PackageInspectionFailed"), 0) == 0) result = kAMDPackageInspectionFailedError;
		else if (CFStringCompare(error, CFSTR("PackageMoveFailed"), 0) == 0) result = kAMDPackageMoveFailedError;
		else if (CFStringCompare(error, CFSTR("PathConversionFailed"), 0) == 0) result = kAMDPathConversionFailedError;
		else if (CFStringCompare(error, CFSTR("RestoreContainerFailed"), 0) == 0) result = kAMDRestoreContainerFailedError;
		else if (CFStringCompare(error, CFSTR("SeatbeltProfileRemovalFailed"), 0) == 0) result = kAMDSeatbeltProfileRemovalFailedError;
		else if (CFStringCompare(error, CFSTR("StageCreationFailed"), 0) == 0) result = kAMDStageCreationFailedError;
		else if (CFStringCompare(error, CFSTR("SymlinkFailed"), 0) == 0) result = kAMDSymlinkFailedError;
		else if (CFStringCompare(error, CFSTR("UnknownCommand"), 0) == 0) result = kAMDUnknownCommandError;
		else if (CFStringCompare(error, CFSTR("iTunesArtworkCaptureFailed"), 0) == 0) result = kAMDiTunesArtworkCaptureFailedError;
		else if (CFStringCompare(error, CFSTR("iTunesMetadataCaptureFailed"), 0) == 0) result = kAMDiTunesMetadataCaptureFailedError;
		else if (CFStringCompare(error, CFSTR("DeviceOSVersionTooLow"), 0) == 0) result = kAMDDeviceOSVersionTooLow;
		else if (CFStringCompare(error, CFSTR("DeviceFamilyNotSupported"), 0) == 0) result = kAMDDeviceFamilyNotSupported;
		else if (CFStringCompare(error, CFSTR("PackagePatchFailed"), 0) == 0) result = kAMDPackagePatchFailedError;
		else if (CFStringCompare(error, CFSTR("IncorrectArchitecture"), 0) == 0) result = kAMDIncorrectArchitectureError;
		else if (CFStringCompare(error, CFSTR("PluginCopyFailed"), 0) == 0) result = kAMDPluginCopyFailedError;
		else if (CFStringCompare(error, CFSTR("BreadcrumbFailed"), 0) == 0) result = kAMDBreadcrumbFailedError;
		else if (CFStringCompare(error, CFSTR("BreadcrumbUnlockFailed"), 0) == 0) result = kAMDBreadcrumbUnlockError;
		else if (CFStringCompare(error, CFSTR("GeoJSONCaptureFailed"), 0) == 0) result = kAMDGeoJSONCaptureFailedError;
		else if (CFStringCompare(error, CFSTR("NewsstandArtworkCaptureFailed"), 0) == 0) result = kAMDNewsstandArtworkCaptureFailedError;
		else if (CFStringCompare(error, CFSTR("MissingCommand"), 0) == 0) result = kAMDNewsstandArtworkCaptureFailedError;
		else if (CFStringCompare(error, CFSTR("NotEntitled"), 0) == 0) result = kAMDNotEntitledError;
		else if (CFStringCompare(error, CFSTR("MissingPackagePath"), 0) == 0) result = kAMDMissingPackagePathError;
		else if (CFStringCompare(error, CFSTR("MissingContainerPath"), 0) == 0) result = kAMDMissingContainerPathError;
		else if (CFStringCompare(error, CFSTR("MissingApplicationIdentifier"), 0) == 0) result = kAMDMissingApplicationIdentifierError;
		else if (CFStringCompare(error, CFSTR("MissingAttributeValue"), 0) == 0) result = kAMDMissingAttributeValueError;
		else if (CFStringCompare(error, CFSTR("LookupFailed"), 0) == 0) result = kAMDLookupFailedError;
		else if (CFStringCompare(error, CFSTR("DictCreationFailed"), 0) == 0) result = kAMDDictCreationFailedError;
		else if (CFStringCompare(error, CFSTR("InstallProhibited"), 0) == 0) result = kAMDInstallProhibitedError;
		else if (CFStringCompare(error, CFSTR("UninstallProhibited"), 0) == 0) result = kAMDUninstallProhibitedError;
		else if (CFStringCompare(error, CFSTR("MissingBundleVersion"), 0) == 0) result = kAMDMissingBundleVersionError;
		else {
			result = 0xffffffff;
		}
	}
	return result;
}


#endif