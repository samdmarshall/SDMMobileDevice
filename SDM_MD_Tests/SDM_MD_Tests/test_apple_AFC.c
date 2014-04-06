//
//  test_apple_AFC.c
//  apple_MD_Tests
//
//  Created by Sam Marshall on 4/5/14.
//  Copyright (c) 2014 Sam Marshall. All rights reserved.
//

#ifndef apple_MD_Tests_test_apple_AFC_h
#define apple_MD_Tests_test_apple_AFC_h

#include "test_apple_AFC.h"
#include <SDMMobileDevice/SDMMobileDevice.h>

kern_return_t test_apple_AFCConnectionCreate(struct am_device *apple) {
	kern_return_t apple_return = kAMDUndefinedError;
	kern_return_t result = AMDeviceConnect(apple);
	if (SDM_MD_CallSuccessful(result)) {
		result = AMDeviceStartSession(apple);
		if (SDM_MD_CallSuccessful(result)) {
			service_conn_t test_apple_afc_conn;
			result = AMDeviceStartService(apple, CFSTR(AMSVC_AFC), &test_apple_afc_conn, NULL);
			if (SDM_MD_CallSuccessful(result)) {
				struct afc_connection *afc = NULL;
				result = AFCConnectionOpen(test_apple_afc_conn, 0, &afc);
				if (afc) {
					apple_return = kAMDSuccess;
					AFCConnectionClose(afc);
				}
			}
			AMDeviceStopSession(apple);
		}
		AMDeviceDisconnect(apple);
	}
	return apple_return;
}

kern_return_t test_apple_AFCOperationCreateGetDeviceInfo(struct am_device *apple, CFTypeRef *response) {
	kern_return_t apple_return = kAMDUndefinedError;
	kern_return_t result = AMDeviceConnect(apple);
	if (SDM_MD_CallSuccessful(result)) {
		result = AMDeviceStartSession(apple);
		if (SDM_MD_CallSuccessful(result)) {
			service_conn_t test_apple_afc_conn;
			result = AMDeviceStartService(apple, CFSTR(AMSVC_AFC), &test_apple_afc_conn, NULL);
			if (SDM_MD_CallSuccessful(result)) {
				struct afc_connection *afc = NULL;
				result = AFCConnectionOpen(test_apple_afc_conn, 0, &afc);
				if (afc) {
					afc_operation deviceInfo = AFCOperationCreateGetDeviceInfo(kCFAllocatorDefault, NULL);
					result = AFCConnectionProcessOperation(afc, deviceInfo, 0);
					if (SDM_MD_CallSuccessful(result)) {
						CFTypeRef test = AFCOperationGetResultObject(deviceInfo);
						if (test) {
							*response = test;
							apple_return = kAMDSuccess;
						}
					}
					AFCConnectionClose(afc);
				}
			}
			AMDeviceStopSession(apple);
		}
		AMDeviceDisconnect(apple);
	}
	return apple_return;
}

kern_return_t test_apple_AFCOperationCreateGetConnectionInfo(struct am_device *apple, CFTypeRef *response) {
	kern_return_t apple_return = kAMDUndefinedError;
	kern_return_t result = AMDeviceConnect(apple);
	if (SDM_MD_CallSuccessful(result)) {
		result = AMDeviceStartSession(apple);
		if (SDM_MD_CallSuccessful(result)) {
			service_conn_t test_apple_afc_conn;
			result = AMDeviceStartService(apple, CFSTR(AMSVC_AFC), &test_apple_afc_conn, NULL);
			if (SDM_MD_CallSuccessful(result)) {
				struct afc_connection *afc = NULL;
				result = AFCConnectionOpen(test_apple_afc_conn, 0, &afc);
				if (afc) {
					afc_operation conn_info = AFCOperationCreateGetConnectionInfo(kCFAllocatorDefault, NULL);
					result = AFCConnectionProcessOperation(afc, conn_info, 0);
					if (SDM_MD_CallSuccessful(result)) {
						CFTypeRef test = AFCOperationGetResultObject(conn_info);
						if (test) {
							*response = test;
							apple_return = kAMDSuccess;
						}
					}
					AFCConnectionClose(afc);
				}
			}
			AMDeviceStopSession(apple);
		}
		AMDeviceDisconnect(apple);
	}
	return apple_return;
}

kern_return_t test_apple_AFCOperationCreateReadDirectory(struct am_device *apple, CFTypeRef *response) {
	kern_return_t apple_return = kAMDUndefinedError;
	kern_return_t result = AMDeviceConnect(apple);
	if (SDM_MD_CallSuccessful(result)) {
		result = AMDeviceStartSession(apple);
		if (SDM_MD_CallSuccessful(result)) {
			service_conn_t test_apple_afc_conn;
			result = AMDeviceStartService(apple, CFSTR(AMSVC_AFC), &test_apple_afc_conn, NULL);
			if (SDM_MD_CallSuccessful(result)) {
				struct afc_connection *afc = NULL;
				result = AFCConnectionOpen(test_apple_afc_conn, 0, &afc);
				if (afc) {
					afc_operation read_dir = AFCOperationCreateReadDirectory(kCFAllocatorDefault, CFSTR(""), NULL);
					result = AFCConnectionProcessOperation(afc, read_dir, 0);
					if (SDM_MD_CallSuccessful(result)) {
						CFTypeRef test = AFCOperationGetResultObject(read_dir);
						if (test) {
							*response = test;
							apple_return = kAMDSuccess;
						}
					}
					AFCConnectionClose(afc);
				}
			}
			AMDeviceStopSession(apple);
		}
		AMDeviceDisconnect(apple);
	}
	return apple_return;
}

// Safari/goog-phish-shavar.db

#endif