/*
 *  SDMMD_Applications.h
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

#ifndef _SDM_MD_APPLICATIONS_H_
#define _SDM_MD_APPLICATIONS_H_

#include "SDMMD_Types.h"
#include "SDMMD_Error.h"
#include "SDMMD_AMDevice.h"

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

sdmmd_return_t SDMMD_AMDeviceCopyApplication(SDMMD_AMDeviceRef device, CFStringRef path);
sdmmd_return_t SDMMD_AMDeviceInstallApp(SDMMD_AMDeviceRef device, CFStringRef path);

sdmmd_return_t SDMMD_AMDeviceLookupAppInfo(SDMMD_AMDeviceRef device, CFDictionaryRef options, CFDictionaryRef *response);
sdmmd_return_t SDMMD_AMDeviceLookupApplications(SDMMD_AMDeviceRef device, CFDictionaryRef options, CFDictionaryRef *response);
sdmmd_return_t SDMMD_AMDeviceInstallApplication(SDMMD_AMDeviceRef device, CFStringRef path, CFDictionaryRef options, CallBack installCallback, void* unknown);

#endif