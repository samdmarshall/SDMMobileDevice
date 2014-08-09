/*
 *  SDMMD_Keys.h
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

#ifndef _SDM_MD_KEYS_H_
#define _SDM_MD_KEYS_H_

#define kAllDomains "AllDomains"
#define kAllKeys "AllKeys"

#pragma mark -
#pragma mark General Domain Keys
#pragma mark -

// these keys require no domain, pass NULL as the domain
#define kGlobalDomain							"NULL"

/*
 kCTPostponementInfoUniqueID
 kCTPostponementInfoPRIVersion
 kCTPostponementInfoPRLName
 kCTPostponementStatus
*/

#define kActivationPublicKey 					"ActivationPublicKey"
#define kActivationState						"ActivationState"
#define kActivationStateAcknowledged			"ActivationStateAcknowledged"
#define kActiveWirelessTechnology				"ActiveWirelessTechnology"
#define kActivityURL							"ActivityURL"
#define kAirplaneMode							"AirplaneMode"
#define kApNonce								"ApNonce"
#define kBasebandActivationTicketVersion		"BasebandActivationTicketVersion"
#define kBasebandBoardSerialNumber				"BasebandBoardSerialNumber"
#define kBasebandBootloaderVersion				"BasebandBootloaderVersion"
#define kBasebandCertId							"BasebandCertId"
#define kBasebandChipID							"BasebandChipID"
#define kBasebandGoldCertId						"BasebandGoldCertId"
#define kBasebandKeyHashInformation				"BasebandKeyHashInformation"
#define kBasebandMasterKeyHash					"BasebandMasterKeyHash"
#define kBasebandNonce							"BasebandNonce"
#define kBasebandRegionSKU						"BasebandRegionSKU"
#define kBasebandSerialNumber 					"BasebandSerialNumber"
#define kBasebandStatus 						"BasebandStatus"
#define kBasebandVersion 						"BasebandVersion"
#define kBoardId								"BoardId"
#define kBluetoothAddress 						"BluetoothAddress"
#define kBrickState								"BrickState"
#define kBuildExpireTime						"BuildExpireTime"
#define kBuildVersion 							"BuildVersion"
#define kCarrierBuild							"CarrierBuild"
#define kCarrierBundleInfo						"CarrierBundleInfo"
#define kCarrierBundleInfoArray					"CarrierBundleInfoArray"
#define kCertID									"CertID"
#define kChipID									"ChipID"
#define kChipSerialNo							"ChipSerialNo"
#define kCompassCalibration						"CompassCalibration"
#define kCPUArchitecture 						"CPUArchitecture"
#define kDeviceCertificate 						"DeviceCertificate"
#define kDeviceClass 							"DeviceClass"
#define kDeviceColor 							"DeviceColor"
#define kDeviceEnclosureColor					"DeviceEnclosureColor"
#define kDeviceName								"DeviceName"
#define kDevicePublicKey 						"DevicePublicKey"
#define kDeviceVariant							"DeviceVariant"
#define kDieID 									"DieID"
#define kEthernetAddress						"EthernetAddress"
#define kFirmwarePreflightInfo					"FirmwarePreflightInfo"
#define kFirmwareVersion 						"FirmwareVersion"
#define kFirstTime								"FirstTime"
#define kFusingStatus							"FusingStatus"
#define kHardwareModel 							"HardwareModel"
#define kHardwarePlatform 						"HardwarePlatform"
#define kHostAttached 							"HostAttached"
#define kIMLockdownEverRegisteredKey 			"IMLockdownEverRegisteredKey"
#define kIntegratedCircuitCardIdentity 			"IntegratedCircuitCardIdentity"
#define kInternationalMobileEquipmentIdentity 	"InternationalMobileEquipmentIdentity"
#define kInternationalMobileSubscriberIdentity 	"InternationalMobileSubscriberIdentity"
#define kiTunesHasConnected 					"iTunesHasConnected"
#define kMarketingName							"MarketingName"
#define kMLBSerialNumber 						"MLBSerialNumber"
#define kMobileEquipmentIdentifier				"MobileEquipmentIdentifier"
#define kMobileSubscriberCountryCode 			"MobileSubscriberCountryCode"
#define kMobileSubscriberNetworkCode 			"MobileSubscriberNetworkCode"
#define kModelNumber 							"ModelNumber"
#define kNonVolatileRAM							"NonVolatileRAM"
#define kPartitionType 							"PartitionType"
#define kPasswordProtected 						"PasswordProtected"
#define kPhoneNumber 							"PhoneNumber"
#define kPkHash									"PkHash"
#define kProductionSOC 							"ProductionSOC"
#define kProductType 							"ProductType"
#define kProductVersion 						"ProductVersion"
#define kProtocolVersion 						"ProtocolVersion"
#define kProximitySensorCalibration 			"ProximitySensorCalibration"
#define kRegionInfo 							"RegionInfo"
#define kReleaseType							"ReleaseType"
#define kRentalClockBias						"RentalClockBias"
#define kSBLockdownEverRegisteredKey 			"SBLockdownEverRegisteredKey"
#define kSerialNumber 							"SerialNumber"
#define kShowMarketing							"ShowMarketing"
#define kSIMGID1								"SIMGID1"
#define kSIMGID2								"SIMGID2"
#define kSIMStatus 								"SIMStatus"
#define kSIMTrayStatus							"SIMTrayStatus"
#define kSoftwareBehavior 						"SoftwareBehavior"
#define kSoftwareBundleVersion 					"SoftwareBundleVersion"
#define kSupportedDeviceFamilies 				"SupportedDeviceFamilies"
#define kTelephonyCapability 					"TelephonyCapability"
#define kTimeIntervalSince1970 					"TimeIntervalSince1970"
#define kTimeZone 								"TimeZone"
#define kTimeZoneOffsetFromUTC 					"TimeZoneOffsetFromUTC"
#define kTrustedHostAttached 					"TrustedHostAttached"
#define kUniqueChipID 							"UniqueChipID"
#define kUniqueDeviceID 						"UniqueDeviceID"
#define kUseActivityURL 						"UseActivityURL"
#define kUseRaptorCerts 						"UseRaptorCerts"
#define kUses24HourClock 						"Uses24HourClock"
#define kWeDelivered 							"WeDelivered"
#define kWiFiAddress 							"WiFiAddress"

enum SDM_MD_Global_Domain_Key {
	SDM_MD_Global_Domain_Key_kActivationPublicKey = 0x0,
	SDM_MD_Global_Domain_Key_kActivationState,
	SDM_MD_Global_Domain_Key_kActivationStateAcknowledged,
	SDM_MD_Global_Domain_Key_kActiveWirelessTechnology,
	SDM_MD_Global_Domain_Key_kActivityURL,
	SDM_MD_Global_Domain_Key_kAirplaneMode,
	SDM_MD_Global_Domain_Key_kApNonce,
	SDM_MD_Global_Domain_Key_kBasebandBoardSerialNumber,
	SDM_MD_Global_Domain_Key_kBasebandBootloaderVersion,
	SDM_MD_Global_Domain_Key_kBasebandGoldCertId,
	SDM_MD_Global_Domain_Key_kBasebandKeyHashInformation,
	SDM_MD_Global_Domain_Key_kBasebandNonce,
	SDM_MD_Global_Domain_Key_kBasebandSerialNumber,
	SDM_MD_Global_Domain_Key_kBasebandStatus,
	SDM_MD_Global_Domain_Key_kBasebandVersion,
	SDM_MD_Global_Domain_Key_kBoardId,
	SDM_MD_Global_Domain_Key_kBluetoothAddress,
	SDM_MD_Global_Domain_Key_kBrickState,
	SDM_MD_Global_Domain_Key_kBuildVersion,
	SDM_MD_Global_Domain_Key_kCarrierBuild,
	SDM_MD_Global_Domain_Key_kCarrierBundleInfo,
	SDM_MD_Global_Domain_Key_kCarrierBundleInfoArray,
	SDM_MD_Global_Domain_Key_kChipID,
	SDM_MD_Global_Domain_Key_kCompassCalibration,
	SDM_MD_Global_Domain_Key_kCPUArchitecture,
	SDM_MD_Global_Domain_Key_kDeviceCertificate,
	SDM_MD_Global_Domain_Key_kDeviceClass,
	SDM_MD_Global_Domain_Key_kDeviceColor,
	SDM_MD_Global_Domain_Key_kDeviceEnclosureColor,
	SDM_MD_Global_Domain_Key_kDeviceName,
	SDM_MD_Global_Domain_Key_kDevicePublicKey,
	SDM_MD_Global_Domain_Key_kDeviceVariant,
	SDM_MD_Global_Domain_Key_kDieID,
	SDM_MD_Global_Domain_Key_kEthernetAddress,
	SDM_MD_Global_Domain_Key_kFirmwarePreflightInfo,
	SDM_MD_Global_Domain_Key_kFirmwareVersion,
	SDM_MD_Global_Domain_Key_kFirstTime,
	SDM_MD_Global_Domain_Key_kHardwareModel,
	SDM_MD_Global_Domain_Key_kHardwarePlatform,
	SDM_MD_Global_Domain_Key_kHostAttached,
	SDM_MD_Global_Domain_Key_kIMLockdownEverRegisteredKey,
	SDM_MD_Global_Domain_Key_kIntegratedCircuitCardIdentity,
	SDM_MD_Global_Domain_Key_kInternationalMobileEquipmentIdentity,
	SDM_MD_Global_Domain_Key_kInternationalMobileSubscriberIdentit,
	SDM_MD_Global_Domain_Key_kiTunesHasConnected,
	SDM_MD_Global_Domain_Key_kMarketingName,
	SDM_MD_Global_Domain_Key_kMLBSerialNumber,
	SDM_MD_Global_Domain_Key_kMobileEquipmentIdentifier,
	SDM_MD_Global_Domain_Key_kMobileSubscriberCountryCode,
	SDM_MD_Global_Domain_Key_kMobileSubscriberNetworkCode,
	SDM_MD_Global_Domain_Key_kModelNumber,
	SDM_MD_Global_Domain_Key_kNonVolatileRAM,
	SDM_MD_Global_Domain_Key_kPartitionType,
	SDM_MD_Global_Domain_Key_kPasswordProtected,
	SDM_MD_Global_Domain_Key_kPhoneNumber,
	SDM_MD_Global_Domain_Key_kProductionSOC,
	SDM_MD_Global_Domain_Key_kProductType,
	SDM_MD_Global_Domain_Key_kProductVersion,
	SDM_MD_Global_Domain_Key_kProtocolVersion,
	SDM_MD_Global_Domain_Key_kProximitySensorCalibration,
	SDM_MD_Global_Domain_Key_kRegionInfo,
	SDM_MD_Global_Domain_Key_kReleaseType,
	SDM_MD_Global_Domain_Key_kRentalClockBias,
	SDM_MD_Global_Domain_Key_kSBLockdownEverRegisteredKey,
	SDM_MD_Global_Domain_Key_kSerialNumber,
	SDM_MD_Global_Domain_Key_kShowMarketing,
	SDM_MD_Global_Domain_Key_kSIMGID1,
	SDM_MD_Global_Domain_Key_kSIMGID2,
	SDM_MD_Global_Domain_Key_kSIMStatus,
	SDM_MD_Global_Domain_Key_kSIMTrayStatus,
	SDM_MD_Global_Domain_Key_kSoftwareBehavior,
	SDM_MD_Global_Domain_Key_kSoftwareBundleVersion,
	SDM_MD_Global_Domain_Key_kSupportedDeviceFamilies,
	SDM_MD_Global_Domain_Key_kTelephonyCapability,
	SDM_MD_Global_Domain_Key_kTimeIntervalSince1970,
	SDM_MD_Global_Domain_Key_kTimeZone,
	SDM_MD_Global_Domain_Key_kTimeZoneOffsetFromUTC,
	SDM_MD_Global_Domain_Key_kTrustedHostAttached,
	SDM_MD_Global_Domain_Key_kUniqueChipID,
	SDM_MD_Global_Domain_Key_kUniqueDeviceID,
	SDM_MD_Global_Domain_Key_kUseActivityURL,
	SDM_MD_Global_Domain_Key_kUseRaptorCerts,
	SDM_MD_Global_Domain_Key_kUses24HourClock,
	SDM_MD_Global_Domain_Key_kWeDelivered,
	SDM_MD_Global_Domain_Key_kWiFiAddress,
	SDM_MD_Global_Domain_Key_Count
};

static char *SDMMD_Global_DomainKeys[SDM_MD_Global_Domain_Key_Count] = {kActivationPublicKey, kActivationState, kActivationStateAcknowledged, kActiveWirelessTechnology, kActivityURL, kAirplaneMode, kApNonce, kBasebandBoardSerialNumber, kBasebandBootloaderVersion, kBasebandGoldCertId, kBasebandKeyHashInformation, kBasebandNonce, kBasebandSerialNumber, kBasebandStatus, kBasebandVersion, kBoardId, kBluetoothAddress, kBrickState, kBuildVersion, kCarrierBuild, kCarrierBundleInfo, kCarrierBundleInfoArray, kChipID, kCompassCalibration, kCPUArchitecture, kDeviceCertificate, kDeviceClass, kDeviceColor, kDeviceEnclosureColor, kDeviceName, kDevicePublicKey, kDeviceVariant, kDieID, kEthernetAddress, kFirmwarePreflightInfo, kFirmwareVersion, kFirstTime, kHardwareModel, kHardwarePlatform, kHostAttached, kIMLockdownEverRegisteredKey, kIntegratedCircuitCardIdentity, kInternationalMobileEquipmentIdentity, kInternationalMobileSubscriberIdentity, kiTunesHasConnected, kMarketingName, kMLBSerialNumber, kMobileEquipmentIdentifier, kMobileSubscriberCountryCode, kMobileSubscriberNetworkCode, kModelNumber, kNonVolatileRAM, kPartitionType, kPasswordProtected, kPhoneNumber, kProductionSOC, kProductType, kProductVersion, kProtocolVersion, kProximitySensorCalibration, kRegionInfo, kReleaseType, kRentalClockBias, kSBLockdownEverRegisteredKey, kSerialNumber, kShowMarketing, kSIMGID1, kSIMGID2, kSIMStatus, kSIMTrayStatus, kSoftwareBehavior, kSoftwareBundleVersion, kSupportedDeviceFamilies, kTelephonyCapability, kTimeIntervalSince1970, kTimeZone, kTimeZoneOffsetFromUTC, kTrustedHostAttached, kUniqueChipID, kUniqueDeviceID, kUseActivityURL, kUseRaptorCerts, kUses24HourClock, kWeDelivered, kWiFiAddress};

#pragma mark -
#pragma mark Wireless Lockdown Domain Keys
#pragma mark -

#define kWirelessLockdownDomain					"com.apple.mobile.wireless_lockdown"

#define kBonjourFullServiceName					"BonjourFullServiceName"
#define kEnableWifiConnections					"EnableWifiConnections"
#define kSupportsWifiSyncing					"SupportsWifiSyncing"
#define kSupportsWifi							"SupportsWifi"
#define kWirelessBuddyID						"WirelessBuddyID"

enum SDM_MD_Wireless_Domain_Key {
	SDM_MD_Wireless_Domain_Key_kBonjourFullServiceName = 0x0,
	SDM_MD_Wireless_Domain_Key_kEnableWifiConnections,
	SDM_MD_Wireless_Domain_Key_kSupportsWifiSyncing,
	SDM_MD_Wireless_Domain_Key_kSupportsWifi,
	SDM_MD_Wireless_Domain_Key_kWirelessBuddyID,
	SDM_MD_Wireless_Domain_Key_Count
};

static char *SDMMD_Wireless_DomainKeys[SDM_MD_Wireless_Domain_Key_Count] = {kBonjourFullServiceName, kEnableWifiConnections, kSupportsWifiSyncing, kSupportsWifi, kWirelessBuddyID};

#pragma mark -
#pragma mark Battery Domain Keys
#pragma mark -

#define kBatteryDomain 							"com.apple.mobile.battery"

#define kBatteryCurrentCapacity					"BatteryCurrentCapacity"
#define kBatteryIsCharging						"BatteryIsCharging"
#define kGasGaugeCapability						"GasGaugeCapability"
#define kExternalConnected						"ExternalConnected"
#define kExternalChargeCapable					"ExternalChargeCapable"
#define kFullyCharged							"FullyCharged"

enum SDM_MD_Battery_Domain_Key {
	SDM_MD_Battery_Domain_Key_kBatteryCurrentCapacity = 0x0,
	SDM_MD_Battery_Domain_Key_kBatteryIsCharging,
	SDM_MD_Battery_Domain_Key_kGasGaugeCapability,
	SDM_MD_Battery_Domain_Key_kExternalConnected,
	SDM_MD_Battery_Domain_Key_kExternalChargeCapable,
	SDM_MD_Battery_Domain_Key_kFullyCharged,
	SDM_MD_Battery_Domain_Key_Count
};

static char *SDMMD_Battery_DomainKeys[SDM_MD_Battery_Domain_Key_Count] = {kBatteryCurrentCapacity, kBatteryIsCharging, kGasGaugeCapability, kExternalConnected, kExternalChargeCapable, kFullyCharged};

#pragma mark -
#pragma mark User Preferences Domain Keys
#pragma mark -

#define kUserPreferencesDomain					"com.apple.mobile.user_preferences"

#define kUserSetLanaguage						"UserSetLanguage"
#define kUserSetLocale							"UserSetLocale"
#define kDiagnosticsAllowed						"DiagnosticsAllowed" // ???
#define kFileSharingEnabled						"FileSharingEnabled"

enum SDM_MD_UserPref_Domain_Key {
	SDM_MD_UserPref_Domain_Key_kUserSetLanaguage = 0x0,
	SDM_MD_UserPref_Domain_Key_kUserSetLocale,
	SDM_MD_UserPref_Domain_Key_kDiagnosticsAllowed,
	SDM_MD_UserPref_Domain_Key_kFileSharingEnabled,
	SDM_MD_UserPref_Domain_Key_Count
};

static char *SDMMD_UserPref_DomainKeys[SDM_MD_UserPref_Domain_Key_Count] = {kUserSetLanaguage, kUserSetLocale, kDiagnosticsAllowed, kFileSharingEnabled};

#pragma mark -
#pragma mark International Domain Keys
#pragma mark -

#define kInternationalDomain					"com.apple.international"

#define kLanguage								"Language"
#define kLocale									"Locale"
#define kKeyboard								"Keyboard"
#define kHostKeyboard							"HostKeyboard"
#define kSupportedLanguages						"SupportedLanguages"
#define kSupportedKeyboards						"SupportedKeyboards"
#define kSupportedLocales						"SupportedLocales"
#define kSortSections							"SortSections"

enum SDM_MD_International_Domain_Key {
	SDM_MD_International_Domain_Key_kLanguage = 0x0,
	SDM_MD_International_Domain_Key_kLocale,
	SDM_MD_International_Domain_Key_kKeyboard,
	SDM_MD_International_Domain_Key_kHostKeyboard,
	SDM_MD_International_Domain_Key_kSupportedLanguages,
	SDM_MD_International_Domain_Key_kSupportedKeyboards,
	SDM_MD_International_Domain_Key_kSupportedLocales,
	SDM_MD_International_Domain_Key_kSortSections,
	SDM_MD_International_Domain_Key_Count
};

static char *SDMMD_International_DomainKeys[SDM_MD_International_Domain_Key_Count] = {kLanguage, kLocale, kKeyboard, kHostKeyboard, kSupportedLanguages, kSupportedKeyboards, kSupportedLocales, kSortSections};

#pragma mark -
#pragma mark Internal Domain Keys
#pragma mark -

#define kInternalDomain							"com.apple.mobile.internal"

#define kIsInternal								"IsInternal"
#define kDevToolsAvailable						"DevToolsAvailable"
#define kDiagData								"DiagData"
#define kSysCfgData								"SysCfgData"
#define kCarrierBuild							"CarrierBuild"
#define kUIBuild								"UIBuild"

enum SDM_MD_Internal_Domain_Key {
	SDM_MD_Internal_Domain_Key_kIsInternal = 0x0,
	SDM_MD_Internal_Domain_Key_kDevToolsAvailable,
	SDM_MD_Internal_Domain_Key_kDiagData,
	SDM_MD_Internal_Domain_Key_kSysCfgData,
	SDM_MD_Internal_Domain_Key_Count
};

static char *SDMMD_Internal_DomainKeys[SDM_MD_Internal_Domain_Key_Count] = {kIsInternal, kDevToolsAvailable, kDiagData, kSysCfgData};

#pragma mark -
#pragma mark Disk Usage Domain Keys
#pragma mark -

#define kDiskUsageDomain						"com.apple.disk_usage"

#define kTotalDiskCapacity						"TotalDiskCapacity"
#define kTotalSystemCapacity					"TotalSystemCapacity"
#define kTotalSystemAvailable					"TotalSystemAvailable"
#define kTotalDataCapacity						"TotalDataCapacity"
#define kTotalDataAvailable						"TotalDataAvailable"
#define kAmountDataReserved						"AmountDataReserved"
#define kAmountDataAvailable					"AmountDataAvailable"
#define kPhotoUsage								"PhotoUsage"
#define kCameraUsage							"CameraUsage"
#define kNANDInfo								"NANDInfo"
#define kCalculateDiskUsage						"CalculateDiskUsage"
#define kMobileApplicationUsage					"MobileApplicationUsage"

enum SDM_MD_DiskUsage_Domain_Key {
	SDM_MD_DiskUsage_Domain_Key_kTotalDiskCapacity = 0x0,
	SDM_MD_DiskUsage_Domain_Key_kTotalSystemCapacity,
	SDM_MD_DiskUsage_Domain_Key_kTotalSystemAvailable,
	SDM_MD_DiskUsage_Domain_Key_kTotalDataCapacity,
	SDM_MD_DiskUsage_Domain_Key_kTotalDataAvailable,
	SDM_MD_DiskUsage_Domain_Key_kAmountDataReserved,
	SDM_MD_DiskUsage_Domain_Key_kAmountDataAvailable,
	SDM_MD_DiskUsage_Domain_Key_kPhotoUsage,
	SDM_MD_DiskUsage_Domain_Key_kCameraUsage,
	SDM_MD_DiskUsage_Domain_Key_kNANDInfo,
	SDM_MD_DiskUsage_Domain_Key_kCalculateDiskUsage,
	SDM_MD_DiskUsage_Domain_Key_kMobileApplicationUsage,
	SDM_MD_DiskUsage_Domain_Key_Count
};

static char *SDMMD_DiskUsage_DomainKeys[SDM_MD_DiskUsage_Domain_Key_Count] = {kTotalDiskCapacity, kTotalSystemCapacity, kTotalSystemAvailable, kTotalDataCapacity, kTotalDataAvailable, kAmountDataReserved, kAmountDataAvailable, kPhotoUsage, kCameraUsage, kNANDInfo, kCalculateDiskUsage, kMobileApplicationUsage};

#define kDiskUsageFactoryDomain					"com.apple.disk_usage.factory"

#pragma mark -
#pragma mark Backup Domain Keys
#pragma mark -

#define kBackupDomain							"com.apple.mobile.backup"

#define kCloudBackups							"CloudBackupEnabled"
#define kLastCloudBackup						"LastCloudBackupDate"

enum SDM_MD_Backup_Domain_Key {
	SDM_MD_Backup_Domain_Key_kCloudBackups = 0x0,
	SDM_MD_Backup_Domain_Key_kLastCloudBackup,
	SDM_MD_Backup_Domain_Key_Count
};

static char *SDMMD_Backup_DomainKeys[SDM_MD_Backup_Domain_Key_Count] = {kCloudBackups, kLastCloudBackup};

#pragma mark -
#pragma mark Sync Data Domain Keys
#pragma mark -

#define kSyncDataDomain							"com.apple.mobile.data_sync"


#pragma mark -
#pragma mark Sync Data Class Domain Keys
#pragma mark -

#define kSyncDataClassDomain					"com.apple.mobile.sync_data_class"


#pragma mark -
#pragma mark Restriction Domain Keys
#pragma mark -

#define kRestrictionDomain						"com.apple.mobile.restriction"
	
#define kProhibitAppInstall						"ProhibitAppInstall"
#define kProhibitAppRemove						"ProhibitAppRemove"

enum SDM_MD_Restriction_Domain_Key {
	SDM_MD_Restriction_Domain_Key_kProhibitAppInstall = 0x0,
	SDM_MD_Restriction_Domain_Key_kProhibitAppRemove,
	SDM_MD_Restriction_Domain_Key_Count
};

static char *SDMMD_Restriction_DomainKeys[SDM_MD_Restriction_Domain_Key_Count] = {kProhibitAppInstall, kProhibitAppRemove};

#pragma mark -
#pragma mark Fairplay Domain Keys
#pragma mark -

#define kFairplayDomain							"com.apple.fairplay"

#define kFairPlayRentalClockBias				"FairPlayRentalClockBias"
#define kRBRequestVersion						"RBRequestVersion"

enum SDM_MD_Fairplay_Domain_Key {
	SDM_MD_Fairplay_Domain_Key_kFairPlayRentalClockBias = 0x0,
	SDM_MD_Fairplay_Domain_Key_kRBRequestVersion,
	SDM_MD_Fairplay_Domain_Key_Count
};

static char *SDMMD_Fairplay_DomainKeys[SDM_MD_Fairplay_Domain_Key_Count] = {kFairPlayRentalClockBias, kRBRequestVersion};

#pragma mark -
#pragma mark Software Behavior Domain Keys
#pragma mark -

#define kSoftwareBehaviorDomain					"com.apple.mobile.software_behavior"

#define kSBValid								"Valid"
#define kSBGoogleMail							"GoogleMail"
#define kSBVolumeLimit							"VolumeLimit"
#define kSBShutterClick							"ShutterClick"
#define kSBNTSC									"NTSC"
#define kSBNoWiFi								"NoWiFi"
#define kSBChinaBrick							"ChinaBrick"
#define kSBNoVOIP								"NoVOIP"

enum SDM_MD_SoftBehavior_Domain_Key {
	SDM_MD_SoftBehavior_Domain_Key_kSBValid = 0x0,
	SDM_MD_SoftBehavior_Domain_Key_kSBGoogleMail,
	SDM_MD_SoftBehavior_Domain_Key_kSBVolumeLimit,
	SDM_MD_SoftBehavior_Domain_Key_kSBShutterClick,
	SDM_MD_SoftBehavior_Domain_Key_kSBNTSC,
	SDM_MD_SoftBehavior_Domain_Key_kSBNoWiFi,
	SDM_MD_SoftBehavior_Domain_Key_kSBChinaBrick,
	SDM_MD_SoftBehavior_Domain_Key_kSBNoVOIP,
	SDM_MD_SoftBehavior_Domain_Key_Count
};

static char *SDMMD_SoftBehavior_DomainKeys[SDM_MD_SoftBehavior_Domain_Key_Count] = {kSBValid, kSBGoogleMail, kSBVolumeLimit, kSBShutterClick, kSBNTSC, kSBNoWiFi, kSBChinaBrick, kSBNoVOIP};

#pragma mark -
#pragma mark iTunes Domain Keys
#pragma mark -

#define kiTunesDomain						"com.apple.iTunes"

#define kLibraryApplications				"LibraryApplications"
#define kDeletedApplications				"DeletedApplications"
#define kWiFiSyncingUserIDs					"WiFiSyncingUserIDs"

/*
 Looking up app data:
 https://itunes.apple.com/<LOCALE>/lookup?bundleId=<BUNDLE-ID>
*/


#pragma mark -
#pragma mark iTunes Mobile Domain Keys
#pragma mark -

#define kiTunesMobileDomain						"com.apple.mobile.iTunes"

#define kMinITunesVersion						"MinITunesVersion"
#define kMinMacOSVersion						"MinMacOSVersion"
#define kSupportsAccessibility					"SupportsAccessibility"
#define kAccessibilityLanguages					"AccessibilityLanguages"
#define kAlbumArt								"AlbumArt"
#define kAudioCodecs							"AudioCodecs"
#define kSupportsCarrierBundleInstall			"SupportsCarrierBundleInstall"
#define kChapterImageSpecs						"ChapterImageSpecs"
#define kiOSColorSyncProfile						"ColorSyncProfile"
#define kFamilyID								"FamilyID"
#define kDBVersion								"DBVersion"
#define kHomeScreenIconWidth					"HomeScreenIconWidth"
#define kHomeScreenIconHeight					"HomeScreenIconHeight"
#define kHomeScreenIconRows						"HomeScreenIconRows"
#define kHomeScreenIconColumns					"HomeScreenIconColumns"
#define kHomeScreenIconDockMaxCount				"HomeScreenIconDockMaxCount"
#define kHomeScreenMaxPages						"HomeScreenMaxPages"
#define kIconFolderRows							"IconFolderRows"
#define kIconFolderColumns						"IconFolderColumns"
#define kIconFolderMaxPages						"IconFolderMaxPages"
#define kImageSpecifications					"ImageSpecifications"
#define kRingtones								"Ringtones"
#define kScreenWidth							"ScreenWidth"
#define kScreenHeight							"ScreenHeight"
#define kScreenScaleFactor						"ScreenScaleFactor"
#define kSupportsAntiPhishing					"SupportsAntiPhishing"
#define kVideoCodecs							"VideoCodecs"
#define kBatteryPollInterval					"BatteryPollInterval"
#define kFairPlayCertificate					"FairPlayCertificate"
#define kFairPlayID								"FairPlayID"
#define kFairPlayDeviceType						"FairPlayDeviceType"
#define kKeyTypeSupportVersion					"KeyTypeSupportVersion"

enum SDM_MD_iTunes_Domain_Key {
	SDM_MD_iTunes_Domain_Key_kMinITunesVersion = 0x0,
	SDM_MD_iTunes_Domain_Key_kMinMacOSVersion,
	SDM_MD_iTunes_Domain_Key_kSupportsAccessibility,
	SDM_MD_iTunes_Domain_Key_kAccessibilityLanguages,
	SDM_MD_iTunes_Domain_Key_kAlbumArt,
	SDM_MD_iTunes_Domain_Key_kAudioCodecs,
	SDM_MD_iTunes_Domain_Key_kSupportsCarrierBundleInstall,
	SDM_MD_iTunes_Domain_Key_kChapterImageSpecs,
	SDM_MD_iTunes_Domain_Key_kColorSyncProfile,
	SDM_MD_iTunes_Domain_Key_kFamilyID,
	SDM_MD_iTunes_Domain_Key_kDBVersion,
	SDM_MD_iTunes_Domain_Key_kHomeScreenIconWidth,
	SDM_MD_iTunes_Domain_Key_kHomeScreenIconHeight,
	SDM_MD_iTunes_Domain_Key_kHomeScreenIconRows,
	SDM_MD_iTunes_Domain_Key_kHomeScreenIconColumns,
	SDM_MD_iTunes_Domain_Key_kHomeScreenIconDockMaxCount,
	SDM_MD_iTunes_Domain_Key_kIconFolderRows,
	SDM_MD_iTunes_Domain_Key_kIconFolderColumns,
	SDM_MD_iTunes_Domain_Key_kIconFolderMaxPages,
	SDM_MD_iTunes_Domain_Key_kImageSpecifications,
	SDM_MD_iTunes_Domain_Key_kRingtones,
	SDM_MD_iTunes_Domain_Key_kScreenWidth,
	SDM_MD_iTunes_Domain_Key_kScreenHeight,
	SDM_MD_iTunes_Domain_Key_kScreenScaleFactor,
	SDM_MD_iTunes_Domain_Key_kSupportsAntiPhishing,
	SDM_MD_iTunes_Domain_Key_kVideoCodecs,
	SDM_MD_iTunes_Domain_Key_kBatteryPollInterval,
	SDM_MD_iTunes_Domain_Key_kFairPlayCertificate,
	SDM_MD_iTunes_Domain_Key_kFairPlayID,
	SDM_MD_iTunes_Domain_Key_kFairPlayDeviceType,
	SDM_MD_iTunes_Domain_Key_kKeyTypeSupportVersion,
	SDM_MD_iTunes_Domain_Key_Count
};

static char *SDMMD_iTunes_DomainKeys[SDM_MD_iTunes_Domain_Key_Count] = {kMinITunesVersion, kMinMacOSVersion, kSupportsAccessibility, kAccessibilityLanguages, kAlbumArt, kAudioCodecs, kSupportsCarrierBundleInstall, kChapterImageSpecs, kiOSColorSyncProfile, kFamilyID, kDBVersion, kHomeScreenIconWidth, kHomeScreenIconHeight, kHomeScreenIconRows, kHomeScreenIconColumns, kHomeScreenIconDockMaxCount, kIconFolderRows, kIconFolderColumns, kIconFolderMaxPages, kImageSpecifications, kRingtones, kScreenWidth, kScreenHeight, kScreenScaleFactor, kSupportsAntiPhishing, kVideoCodecs, kBatteryPollInterval, kFairPlayCertificate, kFairPlayID, kFairPlayDeviceType, kKeyTypeSupportVersion};

#pragma mark -
#pragma mark Developer Domain Keys
#pragma mark -

#define kDeveloperDomain						"com.apple.xcode.developerdomain"

#define kDeveloperStatus						"DeveloperStatus"

enum SDM_MD_Developer_Domain_Key {
	SDM_MD_Developer_Domain_Key_kDeveloperStatus = 0x0,
	SDM_MD_Developer_Domain_Key_Count
};

static char *SDMMD_Developer_DomainKeys[SDM_MD_Developer_Domain_Key_Count] = {kDeveloperStatus};

#pragma mark -
#pragma mark Debug Domain Keys
#pragma mark -

#define kDebugDomain							"com.apple.mobile.debug"

#define kEnableVPNLogs							"EnableVPNLogs"
#define kEnableCLTMLogs							"EnableCLTMLogs"
#define kEnableCLTMTGraphLogs					"EnableCLTMTGraphLogs"
#define kEnableCLTMThermstatLogs				"EnableCLTMThermstatLogs"
#define kEnable8021XLogs						"Enable8021XLogs"
#define kEnableWiFiManagerLogs					"EnableWiFiManagerLogs"
#define kEnableLockdownLogToDisk				"EnableLockdownLogToDisk"
#define kEnableLockdownExtendedLogging			"EnableLockdownExtendedLogging"
#define kEnableMediaStreamLogs					"EnableMediaStreamLogs"
#define kEnableRaptorCerts						"EnableRaptorCerts"
#define kRemoveVPNLogs							"RemoveVPNLogs"
#define kRemoveCLTMLogs							"RemoveCLTMLogs"
#define kRemoveCLTMTGraphLogs					"RemoveCLTMTGraphLogs"
#define kRemoveCLTMThermstatLogs				"RemoveCLTMThermstatLogs"
#define kRemove8021XLogs						"Remove8021XLogs"
#define kRemoveLockdownLog						"RemoveLockdownLog"
#define kRemoveWiFiManagerLogs					"RemoveWiFiManagerLogs"

enum SDM_MD_Debug_Domain_Key {
	SDM_MD_Debug_Domain_Key_kEnableVPNLogs = 0x0,
	SDM_MD_Debug_Domain_Key_kEnableCLTMLogs,
	SDM_MD_Debug_Domain_Key_kEnableCLTMTGraphLogs,
	SDM_MD_Debug_Domain_Key_kEnableCLTMThermstatLogs,
	SDM_MD_Debug_Domain_Key_kEnable8021XLogs,
	SDM_MD_Debug_Domain_Key_kEnableWiFiManagerLogs,
	SDM_MD_Debug_Domain_Key_kEnableLockdownLogToDisk,
	SDM_MD_Debug_Domain_Key_kEnableLockdownExtendedLogging,
	SDM_MD_Debug_Domain_Key_kEnableMediaStreamLogs,
	SDM_MD_Debug_Domain_Key_kEnableRaptorCerts,
	SDM_MD_Debug_Domain_Key_kRemoveVPNLogs,
	SDM_MD_Debug_Domain_Key_kRemoveCLTMLogs,
	SDM_MD_Debug_Domain_Key_kRemoveCLTMTGraphLogs,
	SDM_MD_Debug_Domain_Key_kRemoveCLTMThermstatLogs,
	SDM_MD_Debug_Domain_Key_kRemove8021XLogs,
	SDM_MD_Debug_Domain_Key_kRemoveLockdownLog,
	SDM_MD_Debug_Domain_Key_kRemoveWiFiManagerLogs,
	SDM_MD_Debug_Domain_Key_Count
};

static char *SDMMD_Debug_DomainKeys[SDM_MD_Debug_Domain_Key_Count] = {kEnableVPNLogs, kEnableCLTMLogs, kEnableCLTMTGraphLogs, kEnableCLTMThermstatLogs, kEnable8021XLogs, kEnableWiFiManagerLogs, kEnableLockdownLogToDisk, kEnableLockdownExtendedLogging, kEnableMediaStreamLogs, kEnableRaptorCerts, kRemoveVPNLogs, kRemoveCLTMLogs, kRemoveCLTMTGraphLogs, kRemoveCLTMThermstatLogs, kRemove8021XLogs, kRemoveLockdownLog, kRemoveWiFiManagerLogs};

#pragma mark -
#pragma mark Accessibility Domain Keys
#pragma mark -

#define kAccessibilityDomain					"com.apple.Accessibility"

#define kVoiceOverEnabled						"VoiceOverTouchEnabledByiTunes"
#define kZoomEnabled							"ZoomTouchEnabledByiTunes"
#define kInvertDisplayEnabled					"InvertDisplayEnabledByiTunes"
#define kSpeakAutoCorrectionsEnabled			"SpeakAutoCorrectionsEnabledByiTunes"
#define kMonoAudioEnabled						"MonoAudioEnabledByiTunes"
#define kClosedCaptioningEnabled				"ClosedCaptioningEnabledByiTunes"

enum SDM_MD_Accessibility_Domain_Key {
	SDM_MD_Accessibility_Domain_Key_kVoiceOverEnabled = 0x0,
	SDM_MD_Accessibility_Domain_Key_kZoomEnabled,
	SDM_MD_Accessibility_Domain_Key_kInvertDisplayEnabled,
	SDM_MD_Accessibility_Domain_Key_kSpeakAutoCorrectionsEnabled,
	SDM_MD_Accessibility_Domain_Key_kMonoAudioEnabled,
	SDM_MD_Accessibility_Domain_Key_kClosedCaptioningEnabled,
	SDM_MD_Accessibility_Domain_Key_Count
};

static char *SDMMD_Accessibility_DomainKeys[SDM_MD_Accessibility_Domain_Key_Count] = {kVoiceOverEnabled, kZoomEnabled, kInvertDisplayEnabled, kSpeakAutoCorrectionsEnabled, kMonoAudioEnabled, kClosedCaptioningEnabled};

#pragma mark -
#pragma mark Purple Buddy Domain Keys
#pragma mark -

#define kPurpleBuddyDomain								"com.apple.purplebuddy"

#define kSetupState										"SetupState"
#define kSetupVersion									"SetupVersion"
#define kWebKitAcceleratedDrawingEnabled				"WebKitAcceleratedDrawingEnabled"
#define kWebKitLocalStorageDatabasePathPreferenceKey	"WebKitLocalStorageDatabasePathPreferenceKey"
#define kWebKitShrinksStandaloneImagesToFit				"WebKitShrinksStandaloneImagesToFit"
#define kLocationServicesPresented						"LocationServicesPresented"
#define kSSDeviceType_N42AP								"SSDeviceType_N42AP"
#define kGuessedCountry									"GuessedCountry"
#define kLocale											"Locale"
#define kAppleIDPB3Presented							"AppleIDPB3Presented"
#define kSetupFinishedAllSteps							"SetupFinishedAllSteps"
#define kKeychainSyncPresented							"KeychainSyncPresented"
#define kPBTCPresented									"PBTCPresented"
#define kLanguage										"Language"
#define kAppleIDForceUpgrade							"AppleIDForceUpgrade"
#define kAssistantPresented								"AssistantPresented"
#define kAppleIDPB5Presented							"AppleIDPB5Presented"
#define kWebDatabaseDirectory							"WebDatabaseDirectory"
#define kWebKitOfflineWebApplicationCacheEnabled		"WebKitOfflineWebApplicationCacheEnabled"
#define kWiFiPresented									"WiFiPresented"
#define kRestoreChoice									"RestoreChoice"
#define kWebKitDiskImageCacheSavedCacheDirectory		"WebKitDiskImageCacheSavedCacheDirectory"
#define kSetupDone										"SetupDone"
#define kPBDiagnosticsPresented							"PBDiagnosticsPresented"
#define kAppleIDPB2Presented							"AppleIDPB2Presented"
#define kSysCfg											"SysCfg"
#define kSysCfgData										"SysCfgData"


enum SDM_MD_Purple_Domain_Key {
	SDM_MD_Purple_Domain_Key_kSetupState = 0x0,
	SDM_MD_Purple_Domain_Key_Count
};

static char *SDMMD_Purple_DomainKeys[SDM_MD_Purple_Domain_Key_Count] = {kSetupState};

#pragma mark -
#pragma mark Find My iPhone Domain Keys
#pragma mark -

#define kfmipDomain								"com.apple.fmip"

#define kIsAssociated							"IsAssociated"

enum SDM_MD_FMIP_Domain_Key {
	SDM_MD_FMIP_Domain_Key_kIsAssociated = 0x0,
	SDM_MD_FMIP_Domain_Key_Count
};

static char *SDMMD_FMIP_DomainKeys[SDM_MD_FMIP_Domain_Key_Count] = {kIsAssociated};


#pragma mark -
#pragma mark iTunesStore
#pragma mark -

#define kiTunesStoreDomain						"com.apple.itunesstored"

#define kAppleID								"AppleID"
#define kAccountAvailableServiceTypes			"AccountAvailableServiceTypes"
#define kAccountKind							"AccountKind"
#define kAccountServiceTypes					"AccountServiceTypes"
#define kAccountSocialEnabled					"AccountSocialEnabled"
#define kAccountStoreFront						"AccountStoreFront"
#define kAccountURLBagType						"AccountURLBagType"
#define kCreditDisplayString					"CreditDisplayString"
#define kDSPersonID								"DSPersonID"
#define kTempStoreFront							"TempStorefront"


enum SDM_MD_iTunesStore_Domain_Key {
	SDM_MD_iTunesStore_Domain_Key_AppleID = 0x0,
	SDM_MD_iTunesStore_Domain_Key_AccountAvailableServiceTypes,
	SDM_MD_iTunesStore_Domain_Key_AccountKind,
	SDM_MD_iTunesStore_Domain_Key_AccountServiceTypes,
	SDM_MD_iTunesStore_Domain_Key_AccountSocialEnabled,
	SDM_MD_iTunesStore_Domain_Key_AccountStoreFront,
	SDM_MD_iTunesStore_Domain_Key_AccountURLBagType,
	SDM_MD_iTunesStore_Domain_Key_CreditDisplayString,
	SDM_MD_iTunesStore_Domain_Key_DSPersonID,
	SDM_MD_iTunesStore_Domain_Key_TempStoreFront,
	SDM_MD_iTunesStore_Domain_Key_Count
};

static char *SDMMD_iTunesStore_DomainKeys[SDM_MD_iTunesStore_Domain_Key_Count] = {kAppleID,kAccountAvailableServiceTypes,kAccountKind,kAccountServiceTypes,kAccountSocialEnabled,kAccountStoreFront,kAccountURLBagType,kCreditDisplayString,kDSPersonID,kTempStoreFront};

#pragma mark -
#pragma mark iTunesBackup Domain
#pragma mark -

#define kiTunesBackUpDomain						"com.apple.iTunes.backup"

#define kLastBackupComputerName					"LastBackupComputerName"
#define kLastBackupComputerType					"LastBackupComputerType"

enum SDM_MD_iTunesBackup_Domain_Key {
	SDM_MD_iTunesBackup_Domain_Key_LastBackupComputerName = 0x0,
	SDM_MD_iTunesBackup_Domain_Key_LastBackupComputerType,
	SDM_MD_iTunesBackup_Domain_Key_Count
};

static char *SDMMD_iTunesBackup_DomainKeys[SDM_MD_iTunesBackup_Domain_Key_Count] = {kLastBackupComputerName,kLastBackupComputerType};


#pragma mark -
#pragma mark SMF Domain
#pragma mark -

#define kSMFDomain								"com.apple.smf"

#define kIDLastSeenAtBoot						"IDLastSeenAtBoot"
#define kIDLastSeenAtBootBuild					"IDLastSeenAtBootBuild"
#define kIDCreatedWithBuild						"IDCreatedWithBuild"
#define kStoredIdentity							"StoredIdentity"


#pragma mark -

enum SDM_MD_Domain_Values {
	SDM_MD_Domain_Global = 0x0,
	SDM_MD_Domain_Wireless,
	SDM_MD_Domain_Battery,
	SDM_MD_Domain_UserPref,
	SDM_MD_Domain_International,
	SDM_MD_Domain_Internal,
	SDM_MD_Domain_DiskUsage,
	SDM_MD_Domain_Backup,
	SDM_MD_Domain_Restriction,
	SDM_MD_Domain_FairPlay,
	SDM_MD_Domain_SoftBehavior,
	SDM_MD_Domain_iTunes,
	SDM_MD_Domain_Developer,
	SDM_MD_Domain_Debug,
	SDM_MD_Domain_Accessibility,
	SDM_MD_Domain_Purple,
	SDM_MD_Domain_FMIP,
	SDM_MD_Domain_iTunesStore,
	SDM_MD_Domain_iTunesBackup,
	SDM_MD_Domain_Count
};

struct SDM_MD_Domain_Key {
	char *domain;
	char **keys;
	uint32_t keyCount;
};

__attribute__ ((unused)) static struct SDM_MD_Domain_Key SDMMDKnownDomain[SDM_MD_Domain_Count] = {
	{kGlobalDomain, SDMMD_Global_DomainKeys, SDM_MD_Global_Domain_Key_Count},
	{kWirelessLockdownDomain, SDMMD_Wireless_DomainKeys, SDM_MD_Wireless_Domain_Key_Count},
	{kBatteryDomain, SDMMD_Battery_DomainKeys, SDM_MD_Battery_Domain_Key_Count},
	{kUserPreferencesDomain, SDMMD_UserPref_DomainKeys, SDM_MD_UserPref_Domain_Key_Count},
	{kInternationalDomain, SDMMD_International_DomainKeys, SDM_MD_International_Domain_Key_Count},
	{kInternalDomain, SDMMD_Internal_DomainKeys, SDM_MD_Internal_Domain_Key_Count},
	{kDiskUsageDomain, SDMMD_DiskUsage_DomainKeys, SDM_MD_DiskUsage_Domain_Key_Count},
	{kBackupDomain, SDMMD_Backup_DomainKeys, SDM_MD_Backup_Domain_Key_Count},
	{kRestrictionDomain, SDMMD_Restriction_DomainKeys, SDM_MD_Restriction_Domain_Key_Count},
	{kFairplayDomain, SDMMD_Fairplay_DomainKeys, SDM_MD_Fairplay_Domain_Key_Count},
	{kSoftwareBehaviorDomain, SDMMD_SoftBehavior_DomainKeys, SDM_MD_SoftBehavior_Domain_Key_Count},
	{kiTunesMobileDomain, SDMMD_iTunes_DomainKeys, SDM_MD_iTunes_Domain_Key_Count},
	{kDeveloperDomain, SDMMD_Developer_DomainKeys, SDM_MD_Developer_Domain_Key_Count},
	{kDebugDomain, SDMMD_Debug_DomainKeys, SDM_MD_Debug_Domain_Key_Count},
	{kAccessibilityDomain, SDMMD_Accessibility_DomainKeys, SDM_MD_Accessibility_Domain_Key_Count},
	{kPurpleBuddyDomain, SDMMD_Purple_DomainKeys, SDM_MD_Purple_Domain_Key_Count},
	{kfmipDomain, SDMMD_FMIP_DomainKeys, SDM_MD_FMIP_Domain_Key_Count},
	{kiTunesStoreDomain, SDMMD_iTunesStore_DomainKeys, SDM_MD_iTunesStore_Domain_Key_Count},
	{kiTunesBackUpDomain, SDMMD_iTunesBackup_DomainKeys, SDM_MD_iTunesBackup_Domain_Key_Count}
};


#endif