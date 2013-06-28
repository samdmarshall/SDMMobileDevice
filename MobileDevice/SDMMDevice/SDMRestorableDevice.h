/*
 *  SDMRestorableDevice.h
 *  SDM_MD_Demo
 *
 *  Created by sam on 5/26/13.
 *  Copyright 2013 Sam Marshall. All rights reserved.
 *
 */

#ifndef _SDMMRESTORABLEDEVICE_H_
#define _SDMMRESTORABLEDEVICE_H_

typedef struct AMRestorableDeviceClassHeader {
	unsigned char header[16];
} __attribute__ ((packed)) AMRestorableDeviceClassHeader; // 0x10

typedef struct AMRestorableDeviceClassBody {
	// 10
	// 24
	// 32
	// 40
	// 48
	// 56
	// 64
	// 72
	// 80
	// 88
	// 96
} __attribute__ ((packed)) AMRestorableDeviceClassBody; // 0x58

typedef struct am_restorable_device {
	struct AMRestorableDeviceClassHeader head;
	struct AMRestorableDeviceClassBody body;
} __attribute__ ((packed)) am_restorable_device;

typedef struct am_restorable_device SDM_AMRestorableDeviceClass;

#define SDM_AMRestorableDeviceRef SDM_AMRestorableDeviceClass*

#endif