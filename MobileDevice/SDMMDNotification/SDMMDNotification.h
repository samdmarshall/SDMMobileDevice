/*
 *  SDMMDNotification.h
 *  SDM_MD_Demo
 *
 *  Created by sam on 6/2/13.
 *  Copyright 2013 Sam Marshall. All rights reserved.
 *
 */

#ifndef _SDMMDNOTIFICATION_H_
#define _SDMMDNOTIFICATION_H_

#pragma mark -
#pragma mark Notification Service Names
#pragma mark -

#define kNotificationiTunesSyncWillStart				"com.apple.itunes-mobdev.syncWillStart"
#define kNotificationiTunesSyncLock						"com.apple.itunes-mobdev.syncLockRequest"
#define kNotificationiTunesSyncDidStart					"com.apple.itunes-mobdev.syncDidStart"
#define kNotificationiTunesSyncDidFinish				"com.apple.itunes-mobdev.syncDidFinish"

#define kNotificationLockdownActivationState			"com.apple.mobile.lockdown.activation_state"
#define kNotificationDeviceNameChange					"com.apple.mobile.lockdown.device_name_changed"
#define kNotificationAppUninstalled						"com.apple.mobile.application_uninstalled"
#define kNotificationAppInstalled						"com.apple.mobile.application_installed"
#define kNotificationiTunesAppInstalled					"com.apple.itunesstored.application_installed"
#define kNotificationShutDown							"com.apple.springboard.deviceWillShutDown"
#define kNotificationProvisionProfileInstalled			"MISProvisioningProfileInstalled"
#define kNotificationProvisionProfileRemoved			"MISProvisioningProfileRemoved"
#define kNotificationProfileListChanged					"com.apple.ManagedConfiguration.profileListChanged"


#endif