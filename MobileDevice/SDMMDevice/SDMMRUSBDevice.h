/*
 *  SDMMRUSBDevice.h
 *  SDM_MD_Demo
 *
 *  Created by sam on 5/26/13.
 *  Copyright 2013 Sam Marshall. All rights reserved.
 *
 */

#ifndef _SDMMRUSBDEVICE_H_
#define _SDMMRUSBDEVICE_H_

typedef struct AMRUSBDeviceClassHeader {

} __attribute__ ((packed)) AMRUSBDeviceClassHeader;

typedef struct AMRUSBDeviceClassBody {
	
} __attribute__ ((packed)) AMRUSBDeviceClassBody;

typedef struct am_rusb_device {
	struct AMRUSBDeviceClassHeader head;
	struct AMRUSBDeviceClassBody body;
} __attribute__ ((packed)) am_rusb_device;

typedef struct am_rusb_device SDM_AMRUSBDeviceClass;

#define SDM_AMRUSBDeviceRef SDM_AMRUSBDeviceClass*

#endif