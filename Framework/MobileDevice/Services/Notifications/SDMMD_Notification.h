/*
 *  SDMMD_Notification.h
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

#ifndef _SDM_MD_NOTIFICATION_H_
#define _SDM_MD_NOTIFICATION_H_

#pragma mark -
#pragma mark Notification Service Names
#pragma mark -

#define kNotificationiTunesSyncWillStart				"com.apple.itunes-mobdev.syncWillStart"
#define kNotificationiTunesSyncLock						"com.apple.itunes-mobdev.syncLockRequest"
#define kNotificationiTunesSyncDidStart					"com.apple.itunes-mobdev.syncDidStart"
#define kNotificationiTunesSyncDidFinish				"com.apple.itunes-mobdev.syncDidFinish"

#define kNotificationiTunesSyncCancel					"com.apple.itunes-client.syncCancelRequest"
#define kNotificationiTunesSyncSuspend					"com.apple.itunes-client.syncSuspendRequest"
#define kNotificationiTunesSyncResume					"com.apple.itunes-client.syncResumeRequest"

#define kNotificationMobileSyncStarted					"com.apple.MobileSync.SyncAgent.kSyncAgentSyncStarted"
#define kNotificationMobileSyncEnded					"com.apple.MobileSync.SyncAgent.kSyncAgentSyncEnded"

#define kNotificationDataSyncDomainChanged				"com.apple.mobile.data_sync.domain_changed"

#define kNotificationLockdownActivationState			"com.apple.mobile.lockdown.activation_state"
#define kNotificationLockdownDeviceNameChange			"com.apple.mobile.lockdown.device_name_changed"
#define kNotificationLockdownTrustedHost				"com.apple.mobile.lockdown.trusted_host_attached"
#define kNotificationLockdownHostDetatch				"com.apple.mobile.lockdown.host_detached"
#define kNotificationLockdownHostAttach					"com.apple.mobile.lockdown.host_attached"
#define kNotificationLockdownPhoneChanged				"com.apple.mobile.lockdown.phone_number_changed"
#define kNotificationLockdownRegFailed					"com.apple.mobile.lockdown.registration_failed"
#define kNotificationLockdownActivationState			"com.apple.mobile.lockdown.activation_state"
#define kNotificationLockdownBrickState					"com.apple.mobile.lockdown.brick_state"

#define kNotificationDevImageMounted					"com.apple.mobile.developer_image_mounted"

#define kNotificationAppUninstalled						"com.apple.mobile.application_uninstalled"
#define kNotificationAppInstalled						"com.apple.mobile.application_installed"

#define kNotificationiTunesAppInstalled					"com.apple.itunesstored.application_installed"

#define kNotificationSpringboardShutDown				"com.apple.springboard.deviceWillShutDown"
#define kNotificationSpringboardAttemptActivation		"com.apple.springboard.attemptactivation"

#define kNotificationProvisionProfileInstalled			"MISProvisioningProfileInstalled"
#define kNotificationProvisionProfileRemoved			"MISProvisioningProfileRemoved"

#define kNotificationProfileListChanged					"com.apple.ManagedConfiguration.profileListChanged"

#define kNotificationLanguageChanged					"com.apple.language.changed"
#define kNotificationContactsPrefsChanged				"com.apple.AddressBook.PreferenceChanged"


#endif