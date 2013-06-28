/*
 *  SDMMDeviceKeys.h
 *  SDM_MD_Demo
 *
 *  Created by sam on 5/25/13.
 *  Copyright 2013 Sam Marshall. All rights reserved.
 *
 */

#ifndef _SDMMDEVICEKEYS_H_
#define _SDMMDEVICEKEYS_H_

#import "SDMMDevice.h"

#pragma mark -
#pragma mark General Domain Keys
#pragma mark -

#define kActivationPublicKey 					"ActivationPublicKey"
#define kActivationState						"ActivationState"
#define kActivationStateAcknowledged			"ActivationStateAcknowledged"
#define kActiveWirelessTechnology				"ActiveWirelessTechnology"
#define kActivityURL							"ActivityURL"
#define kAirplaneMode							"AirplaneMode"
#define kApNonce								"ApNonce"
#define kBasebandBoardSerialNumber				"BasebandBoardSerialNumber"
#define kBasebandBootloaderVersion				"BasebandBootloaderVersion"
#define kBasebandGoldCertId						"BasebandGoldCertId"
#define kBasebandKeyHashInformation				"BasebandKeyHashInformation"
#define kBasebandNonce							"BasebandNonce"
#define kBasebandSerialNumber 					"BasebandSerialNumber"
#define kBasebandStatus 						"BasebandStatus"
#define kBasebandVersion 						"BasebandVersion"
#define kBoardId								"BoardId"
#define kBluetoothAddress 						"BluetoothAddress"
#define kBrickState								"BrickState"
#define kBuildVersion 							"BuildVersion"
#define kCarrierBuild							"CarrierBuild"
#define kCarrierBundleInfo						"CarrierBundleInfo"
#define kCarrierBundleInfoArray					"CarrierBundleInfoArray"
#define kChipID									"ChipID"
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

#pragma mark -
#pragma mark Wireless Lockdown Domain Keys
#pragma mark -

#define kWirelessLockdownDomain					"com.apple.mobile.wireless_lockdown"

#define kBonjourFullServiceName					"BonjourFullServiceName"
#define kEnableWifiConnections					"EnableWifiConnections"
#define kSupportsWifiSyncing					"SupportsWifiSyncing"
#define kSupportsWifi							"SupportsWifi"
#define kWirelessBuddyID						"WirelessBuddyID"

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

#pragma mark -
#pragma mark User Preferences Domain Keys
#pragma mark -

#define kUserPreferencesDomain					"com.apple.mobile.user_preferences"

#define kUserSetLanaguage						"UserSetLanguage"
#define kUserSetLocale							"UserSetLocale"
#define kDiagnosticsAllowed						"DiagnosticsAllowed"
#define kFileSharingEnabled						"FileSharingEnabled"

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

#pragma mark -
#pragma mark Internal Domain Keys
#pragma mark -

#define kInternalDomain							"com.apple.mobile.internal"

#define kIsInternal								"IsInternal"
#define kDevToolsAvailable						"DevToolsAvailable"
#define kDiagData								"DiagData"
#define kSysCfgData								"SysCfgData"


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

#define kDiskUsageFactoryDomain					"com.apple.disk_usage.factory"

#pragma mark -
#pragma mark Backup Domain Keys
#pragma mark -

#define kBackupDomain							"com.apple.mobile.backup"

#define kCloudBackups							"CloudBackupEnabled"
#define kLastCloudBackup						"LastCloudBackupDate"

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


#pragma mark -
#pragma mark Fairplay Domain Keys
#pragma mark -

#define kFairplayDomain							"com.apple.fairplay"

#define kFairPlayRentalClockBias				"FairPlayRentalClockBias"
#define kRBRequestVersion						"RBRequestVersion"

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

#pragma mark -
#pragma mark iTunes Domain Keys
#pragma mark -

#define kiTunesDomain							"com.apple.mobile.iTunes"

#define kMinITunesVersion						"MinITunesVersion"
#define kMinMacOSVersion						"MinMacOSVersion"
#define kSupportsAccessibility					"SupportsAccessibility"
#define kAccessibilityLanguages					"AccessibilityLanguages"
#define kAlbumArt								"AlbumArt"
#define kAudioCodecs							"AudioCodecs"
#define kSupportsCarrierBundleInstall			"SupportsCarrierBundleInstall"
#define kChapterImageSpecs						"ChapterImageSpecs"
#define kColorSyncProfile						"ColorSyncProfile"
#define kFamilyID								"FamilyID"
#define kDBVersion								"DBVersion"
#define kHomeScreenIconWidth					"HomeScreenIconWidth"
#define kHomeScreenIconHeight					"HomeScreenIconHeight"
#define kHomeScreenIconRows						"HomeScreenIconRows"
#define kHomeScreenIconColumns					"HomeScreenIconColumns"
#define kHomeScreenIconDockMaxCount				"HomeScreenIconDockMaxCount"
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

#pragma mark -
#pragma mark Developer Domain Keys
#pragma mark -

#define kDeveloperDomain						"com.apple.xcode.developerdomain"

#define kDeveloperStatus						"DeveloperStatus"

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

#pragma mark -
#pragma mark Accessibility Domain Keys
#pragma mark -

#define kAccessibility							"com.apple.Accessibility"

#define kVoiceOverEnabled						"VoiceOverTouchEnabledByiTunes"
#define kZoomEnabled							"ZoomTouchEnabledByiTunes"
#define kInvertDisplayEnabled					"InvertDisplayEnabledByiTunes"
#define kSpeakAutoCorrectionsEnabled			"SpeakAutoCorrectionsEnabledByiTunes"
#define kMonoAudioEnabled						"MonoAudioEnabledByiTunes"
#define kClosedCaptioningEnabled				"ClosedCaptioningEnabledByiTunes"

#pragma mark -
#pragma mark Purple Buddy Domain Keys
#pragma mark -

#define kPurpleBuddyDomain						"com.apple.purplebuddy"

#define kSetupState								"SetupState"

#pragma mark -
#pragma mark FUNCTIONS
#pragma mark -

CFTypeRef AMDeviceCopyValue(SDM_AMDeviceRef device, CFStringRef domain, CFStringRef key);

CFTypeRef SDMDeviceCopyValue(SDM_AMDeviceRef device, CFStringRef domain, CFStringRef key);


#endif