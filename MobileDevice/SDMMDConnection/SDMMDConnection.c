/*
 *  SDMMDConnection.c
 *  SDM_MD_Demo
 *
 *  Created by sam on 6/5/13.
 *  Copyright 2013 Sam Marshall. All rights reserved.
 *
 */

#ifndef _SDMMDCONNECTION_C_
#define _SDMMDCONNECTION_C_

#include "SDMMDConnection.h"
#import <dlfcn.h>
#import "MobileDevice.h"

typedef void* (*functionCall)();

SDM_AMConnectionRef SDM_AMDServiceConnectionCreate(CFAllocatorRef allocator, SDM_AMDeviceRef device, CFDictionaryRef dict) {
	SDM_AMConnectionRef results = NULL;
	functionCall serviceConnectionCreate = SDMSTSymbolLookup(SDMMDController->lookupTable, "AMDServiceConnectionCreate");
	if (serviceConnectionCreate) {
		results = (SDM_AMConnectionRef)serviceConnectionCreate(allocator, device, dict);
	}
	return results;
}

sdmmd_return_t SDM_AMDServiceConnectionInvalidate(SDM_AMConnectionRef connection) {
	sdmmd_return_t results = 0x0;
	functionCall serviceConnectionInvalidate = SDMSTSymbolLookup(SDMMDController->lookupTable, "AMDServiceConnectionInvalidate");
	if (serviceConnectionInvalidate) {
		results = (sdmmd_return_t)serviceConnectionInvalidate(connection);
	}
	return results;
}


#endif

