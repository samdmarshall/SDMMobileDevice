/*
 *  SDMMDApplications.h
 *  SDM_MD_Demo
 *
 *  Created by sam on 6/6/13.
 *  Copyright 2013 Sam Marshall. All rights reserved.
 *
 */

#ifndef _SDMMDAPPLICATIONS_H_
#define _SDMMDAPPLICATIONS_H_

#import "SDMMDError.h"
#import "SDMMDevice.h"

#pragma mark -
#pragma mark TYPES
#pragma mark -

#define kAppLookupKeyCFBundleIdentifier					"CFBundleIdentifier"
#define kAppLookupKeyApplicationDSID					"ApplicationDSID"
#define kAppLookupKeyApplicationType					"ApplicationType"
#define kAppLookupKeyCFBundleExecutable					"CFBundleExecutable"
#define kAppLookupKeyCFBundleDisplayName				"CFBundleDisplayName"
#define kAppLookupKeyCFBundleIconFile					"CFBundleIconFile"
#define kAppLookupKeyCFBundleName						"CFBundleName"
#define kAppLookupKeyCFBundleShortVersionString			"CFBundleShortVersionString"
#define kAppLookupKeyCFBundleSupportedPlatforms			"CFBundleSupportedPlatforms"
#define kAppLookupKeyCFBundleURLTypes					"CFBundleURLTypes"
#define kAppLookupKeyCFBundleVersion					"CFBundleVersion"
#define kAppLookupKeyCodeInfoIdentifier					"CodeInfoIdentifier"
#define kAppLookupKeyContainer							"Container"
#define kAppLookupKeyEntitlements						"Entitlements"
#define kAppLookupKeyHasSettingsBundle					"HasSettingsBundle"
#define kAppLookupKeyIsUpgradeable						"IsUpgradeable"
#define kAppLookupKeyMinimumOSVersion					"MinimumOSVersion"
#define kAppLookupKeyPath								"Path"
#define kAppLookupKeySignerIdentity						"SignerIdentity"
#define kAppLookupKeyUIDeviceFamily						"UIDeviceFamily"
#define kAppLookupKeyUIFileSharingEnabled				"UIFileSharingEnabled"
#define kAppLookupKeyUIStatusBarHidden					"UIStatusBarHidden"
#define kAppLookupKeyUISupportedInterfaceOrientations	"UISupportedInterfaceOrientations"
#define kAppLookupKeyCFBundlePackageType				"CFBundlePackageType"
#define kAppLookupKeyBuildMachineOSBuild				"BuildMachineOSBuild"
#define kAppLookupKeyCFBundleResourceSpecification		"CFBundleResourceSpecification"
#define kAppLookupKeyDTPlatformBuild					"DTPlatformBuild"
#define kAppLookupKeyDTCompiler							"DTCompiler"
#define kAppLookupKeyCFBundleSignature					"CFBundleSignature"
#define kAppLookupKeyDTSDKName							"DTSDKName"
#define kAppLookupKeyNSBundleResolvedPath				"NSBundleResolvedPath"
#define kAppLookupKeyDTXcode							"DTXcode"
#define kAppLookupKeyCFBundleInfoDictionaryVersion		"CFBundleInfoDictionaryVersion"
#define kAppLookupKeyDTXcodeBuild						"DTXcodeBuild"
#define kAppLookupKeyUIStatusBarTintParameters			"UIStatusBarTintParameters"
#define kAppLookupKeyDTPlatformVersion					"DTPlatformVersion"
#define kAppLookupKeyDTPlatformName						"DTPlatformName"
#define kAppLookupKeyCFBundleDevelopmentRegion			"CFBundleDevelopmentRegion"
#define kAppLookupKeyDTSDKBuild							"DTSDKBuild"

#pragma mark -
#pragma mark FUNCTIONS
#pragma mark -

sdmmd_return_t SDM_AMDeviceLookupApplications(SDM_AMDeviceRef device, CFArrayRef options, CFDictionaryRef *results);

#endif