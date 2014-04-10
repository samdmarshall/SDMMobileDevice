//
//  test_sdm_AFC.c
//  SDM_MD_Tests
//
//  Created by Sam Marshall on 4/5/14.
//  Copyright (c) 2014 Sam Marshall. All rights reserved.
//

#ifndef SDM_MD_Tests_test_sdm_AFC_c
#define SDM_MD_Tests_test_sdm_AFC_c

#include "test_sdm_AFC.h"

char *test_write_bytes = "This is a test of the write operation.";

kern_return_t test_sdm_AFCConnectionCreate(SDMMD_AMDeviceRef sdm) {
	kern_return_t sdm_return = kAMDUndefinedError;
	kern_return_t result = SDMMD_AMDeviceConnect(sdm);
	if (SDM_MD_CallSuccessful(result)) {
		result = SDMMD_AMDeviceStartSession(sdm);
		if (SDM_MD_CallSuccessful(result)) {
			SDMMD_AMConnectionRef test_sdm_afc_conn;
			result = SDMMD_AMDeviceStartService(sdm, CFSTR(AMSVC_AFC), NULL, &test_sdm_afc_conn);
			if (SDM_MD_CallSuccessful(result)) {
				SDMMD_AFCConnectionRef afc = SDMMD_AFCConnectionCreate(test_sdm_afc_conn);
				if (afc) {
					sdm_return = kAMDSuccess;
					SDMMD_AMDServiceConnectionInvalidate(test_sdm_afc_conn);
					SDMMD_AFCConnectionRelease(afc);
				}
			}
			SDMMD_AMDeviceStopSession(sdm);
		}
		SDMMD_AMDeviceDisconnect(sdm);
	}
	return sdm_return;
}


kern_return_t test_sdm_AFCOperationCreateGetDeviceInfo(SDMMD_AMDeviceRef sdm, CFTypeRef *response) {
	kern_return_t sdm_return = kAMDUndefinedError;
	kern_return_t result = SDMMD_AMDeviceConnect(sdm);
	if (SDM_MD_CallSuccessful(result)) {
		result = SDMMD_AMDeviceStartSession(sdm);
		if (SDM_MD_CallSuccessful(result)) {
			SDMMD_AMConnectionRef test_sdm_afc_conn;
			result = SDMMD_AMDeviceStartService(sdm, CFSTR(AMSVC_AFC), NULL, &test_sdm_afc_conn);
			if (SDM_MD_CallSuccessful(result)) {
				SDMMD_AFCConnectionRef afc = SDMMD_AFCConnectionCreate(test_sdm_afc_conn);
				if (afc) {
					SDMMD_AFCOperationRef deviceInfo = SDMMD_AFCOperationCreateGetDeviceInfo();
					result = SDMMD_AFCProcessOperation(afc, &deviceInfo);
					if (SDM_MD_CallSuccessful(result)) {
						CFTypeRef test = deviceInfo->packet->response;
						if (test) {
							*response = test;
							sdm_return = kAMDSuccess;
						}
					}
					SDMMD_AFCConnectionRelease(afc);
				}
			}
			SDMMD_AMDeviceStopSession(sdm);
		}
		SDMMD_AMDeviceDisconnect(sdm);
	}
	return sdm_return;
}

kern_return_t test_sdm_AFCOperationCreateGetConnectionInfo(SDMMD_AMDeviceRef sdm, CFTypeRef *response) {
	kern_return_t sdm_return = kAMDUndefinedError;
	kern_return_t result = SDMMD_AMDeviceConnect(sdm);
	if (SDM_MD_CallSuccessful(result)) {
		result = SDMMD_AMDeviceStartSession(sdm);
		if (SDM_MD_CallSuccessful(result)) {
			SDMMD_AMConnectionRef test_sdm_afc_conn;
			result = SDMMD_AMDeviceStartService(sdm, CFSTR(AMSVC_AFC), NULL, &test_sdm_afc_conn);
			if (SDM_MD_CallSuccessful(result)) {
				SDMMD_AFCConnectionRef afc = SDMMD_AFCConnectionCreate(test_sdm_afc_conn);
				if (afc) {
					SDMMD_AFCOperationRef conn_info = SDMMD_AFCOperationCreateGetConnectionInfo();
					result = SDMMD_AFCProcessOperation(afc, &conn_info);
					if (SDM_MD_CallSuccessful(result)) {
						CFTypeRef test = conn_info->packet->response;
						if (test) {
							*response = test;
							sdm_return = kAMDSuccess;
						}
					}
					SDMMD_AFCConnectionRelease(afc);
				}
			}
			SDMMD_AMDeviceStopSession(sdm);
		}
		SDMMD_AMDeviceDisconnect(sdm);
	}
	return sdm_return;
}

kern_return_t test_sdm_AFCOperationCreateReadDirectory(SDMMD_AMDeviceRef sdm, CFTypeRef *response) {
	kern_return_t sdm_return = kAMDUndefinedError;
	kern_return_t result = SDMMD_AMDeviceConnect(sdm);
	if (SDM_MD_CallSuccessful(result)) {
		result = SDMMD_AMDeviceStartSession(sdm);
		if (SDM_MD_CallSuccessful(result)) {
			SDMMD_AMConnectionRef test_sdm_afc_conn;
			result = SDMMD_AMDeviceStartService(sdm, CFSTR(AMSVC_AFC), NULL, &test_sdm_afc_conn);
			if (SDM_MD_CallSuccessful(result)) {
				SDMMD_AFCConnectionRef afc = SDMMD_AFCConnectionCreate(test_sdm_afc_conn);
				if (afc) {
					SDMMD_AFCOperationRef read_dir = SDMMD_AFCOperationCreateReadDirectory(CFSTR(""));
					result = SDMMD_AFCProcessOperation(afc, &read_dir);
					if (SDM_MD_CallSuccessful(result)) {
						CFTypeRef test = read_dir->packet->response;
						if (test) {
							*response = test;
							sdm_return = kAMDSuccess;
						}
					}
					SDMMD_AFCConnectionRelease(afc);
				}
			}
			SDMMD_AMDeviceStopSession(sdm);
		}
		SDMMD_AMDeviceDisconnect(sdm);
	}
	return sdm_return;
}

kern_return_t test_sdm_AFCOperationCreateGetFileInfo(SDMMD_AMDeviceRef sdm, CFTypeRef *response) {
	kern_return_t sdm_return = kAMDUndefinedError;
	kern_return_t result = SDMMD_AMDeviceConnect(sdm);
	if (SDM_MD_CallSuccessful(result)) {
		result = SDMMD_AMDeviceStartSession(sdm);
		if (SDM_MD_CallSuccessful(result)) {
			SDMMD_AMConnectionRef test_sdm_afc_conn;
			result = SDMMD_AMDeviceStartService(sdm, CFSTR(AMSVC_AFC), NULL, &test_sdm_afc_conn);
			if (SDM_MD_CallSuccessful(result)) {
				SDMMD_AFCConnectionRef afc = SDMMD_AFCConnectionCreate(test_sdm_afc_conn);
				if (afc) {
					SDMMD_AFCOperationRef file_info = SDMMD_AFCOperationCreateGetFileInfo(CFSTR(kTestFileForAFC));
					result = SDMMD_AFCProcessOperation(afc, &file_info);
					if (SDM_MD_CallSuccessful(result)) {
						CFTypeRef test = file_info->packet->response;
						if (test) {
							*response = test;
							sdm_return = kAMDSuccess;
						}
					}
					SDMMD_AFCConnectionRelease(afc);
				}
			}
			SDMMD_AMDeviceStopSession(sdm);
		}
		SDMMD_AMDeviceDisconnect(sdm);
	}
	return sdm_return;
}

kern_return_t test_sdm_AFCOperationCreateFileRefOpen(SDMMD_AMDeviceRef sdm, CFTypeRef *response) {
	kern_return_t sdm_return = kAMDUndefinedError;
	kern_return_t result = SDMMD_AMDeviceConnect(sdm);
	if (SDM_MD_CallSuccessful(result)) {
		result = SDMMD_AMDeviceStartSession(sdm);
		if (SDM_MD_CallSuccessful(result)) {
			SDMMD_AMConnectionRef test_sdm_afc_conn;
			result = SDMMD_AMDeviceStartService(sdm, CFSTR(AMSVC_AFC), NULL, &test_sdm_afc_conn);
			if (SDM_MD_CallSuccessful(result)) {
				SDMMD_AFCConnectionRef afc = SDMMD_AFCConnectionCreate(test_sdm_afc_conn);
				if (afc) {
					SDMMD_AFCOperationRef file_ref = SDMMD_AFCOperationCreateFileRefOpen(CFSTR(kTestFileForAFC), 2);
					result = SDMMD_AFCProcessOperation(afc, &file_ref);
					if (SDM_MD_CallSuccessful(result)) {
						CFTypeRef test = file_ref->packet->response;
						if (test) {
							*response = test;
							sdm_return = kAMDSuccess;
						}
					}
					SDMMD_AFCConnectionRelease(afc);
				}
			}
			SDMMD_AMDeviceStopSession(sdm);
		}
		SDMMD_AMDeviceDisconnect(sdm);
	}
	return sdm_return;
}

kern_return_t test_sdm_AFCFileDescriptorCreateWriteOperation(SDMMD_AMDeviceRef sdm, CFTypeRef *response) {
	kern_return_t sdm_return = kAMDUndefinedError;
	kern_return_t result = SDMMD_AMDeviceConnect(sdm);
	if (SDM_MD_CallSuccessful(result)) {
		result = SDMMD_AMDeviceStartSession(sdm);
		if (SDM_MD_CallSuccessful(result)) {
			SDMMD_AMConnectionRef test_sdm_afc_conn;
			result = SDMMD_AMDeviceStartService(sdm, CFSTR(AMSVC_AFC), NULL, &test_sdm_afc_conn);
			if (SDM_MD_CallSuccessful(result)) {
				SDMMD_AFCConnectionRef afc = SDMMD_AFCConnectionCreate(test_sdm_afc_conn);
				if (afc) {
					SDMMD_AFCOperationRef file_ref = SDMMD_AFCOperationCreateFileRefOpen(CFSTR(kTestCreateFileForAFC), 2);
					result = SDMMD_AFCProcessOperation(afc, &file_ref);
					if (SDM_MD_CallSuccessful(result)) {
						uint64_t fd = 0;
						memcpy(&fd, CFDataGetBytePtr(file_ref->packet->response), sizeof(uint64_t));
						CFDataRef write_data = CFDataCreate(kCFAllocatorDefault, (const UInt8 *)test_write_bytes, strlen(test_write_bytes));
						SDMMD_AFCOperationRef file_ref_read = SDMMD_AFCFileDescriptorCreateWriteOperation(fd, write_data);
						result = SDMMD_AFCProcessOperation(afc, &file_ref_read);
						if (SDM_MD_CallSuccessful(result)) {
							CFTypeRef test = file_ref_read->packet->response;
							if (test) {
								*response = test;
								sdm_return = kAMDSuccess;
							}
						}
					}
					SDMMD_AFCConnectionRelease(afc);
				}
			}
			SDMMD_AMDeviceStopSession(sdm);
		}
		SDMMD_AMDeviceDisconnect(sdm);
	}
	return sdm_return;
}

kern_return_t test_sdm_AFCFileDescriptorCreateReadOperation(SDMMD_AMDeviceRef sdm, CFTypeRef *response) {
	kern_return_t sdm_return = kAMDUndefinedError;
	kern_return_t result = SDMMD_AMDeviceConnect(sdm);
	if (SDM_MD_CallSuccessful(result)) {
		result = SDMMD_AMDeviceStartSession(sdm);
		if (SDM_MD_CallSuccessful(result)) {
			SDMMD_AMConnectionRef test_sdm_afc_conn;
			result = SDMMD_AMDeviceStartService(sdm, CFSTR(AMSVC_AFC), NULL, &test_sdm_afc_conn);
			if (SDM_MD_CallSuccessful(result)) {
				SDMMD_AFCConnectionRef afc = SDMMD_AFCConnectionCreate(test_sdm_afc_conn);
				if (afc) {
					SDMMD_AFCOperationRef file_info = SDMMD_AFCOperationCreateGetFileInfo(CFSTR(kTestCreateFileForAFC));
					result = SDMMD_AFCProcessOperation(afc, &file_info);
					if (SDM_MD_CallSuccessful(result)) {
						int32_t size = CFStringGetIntValue(CFDictionaryGetValue(file_info->packet->response, AFC_File_Info_Keys[AFC_File_Info_Key_size]));
						SDMMD_AFCOperationRef file_ref = SDMMD_AFCOperationCreateFileRefOpen(CFSTR(kTestCreateFileForAFC), 2);
						result = SDMMD_AFCProcessOperation(afc, &file_ref);
						if (SDM_MD_CallSuccessful(result)) {
							uint64_t fd = 0;
							memcpy(&fd, CFDataGetBytePtr(file_ref->packet->response), sizeof(uint64_t));
							SDMMD_AFCOperationRef file_ref_read = SDMMD_AFCFileDescriptorCreateReadOperation(fd, size);
							result = SDMMD_AFCProcessOperation(afc, &file_ref_read);
							if (SDM_MD_CallSuccessful(result)) {
								CFTypeRef test = file_ref_read->packet->response;
								if (test) {
									*response = test;
									sdm_return = kAMDSuccess;
								}
							}
						}
					}
					SDMMD_AFCConnectionRelease(afc);
				}
			}
			SDMMD_AMDeviceStopSession(sdm);
		}
		SDMMD_AMDeviceDisconnect(sdm);
	}
	return sdm_return;
}

#endif
