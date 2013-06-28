/*
 *  MobileDevice.h
 *  SDM_MD_Demo
 *
 *  Created by sam on 5/24/13.
 *  Copyright 2013 Sam Marshall. All rights reserved.
 *
 */

#ifndef _SDM_MobileDevice_H_
#define _SDM_MobileDevice_H_

#import "SDMMDError.h"
#import "SDMMDGeneralFunctions.h"
#import "SDMMDService.h"
#import "SDMMDConnection.h"

#import "SDMMDevice.h"
#import "SDMMDeviceKeys.h"
#import "SDMMRecoveryModeDevice.h"
#import "SDMMRUSBDevice.h"
#import "SDMMDFUModeDevice.h"
#import "SDMMRestoreModeDevice.h"
#import "SDMRestorableDevice.h"


#import "SDMAFCDevice.h"
#import "SDMAFCIterator.h"
#import "SDMAFCOperation.h"
#import "SDMAFCLock.h"


#import "SDMMDNotification.h"

#import "SDMMDDebugger.h"

#import "SDMMDApplications.h"

#import "SDMUSBmuxListener.h"

#import "SDMSymbolTable.h"

typedef struct sdm_mobiledevice {
	SDMUSBMuxListenerRef usbmuxd;
	CFArrayRef deviceList;
	struct LibrarySymbolTable *lookupTable;
} __attribute__ ((packed)) sdm_mobiledevice;

#define SDMMobileDeviceRef struct sdm_mobiledevice *

SDMMobileDeviceRef InitializeSDMMobileDevice();

#define SDMMDController InitializeSDMMobileDevice()

#endif