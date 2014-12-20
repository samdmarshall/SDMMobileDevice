/*
 *  SDMMD_AppleFileConduit.h
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

#ifndef SDMMobileDevice_Framework_SDMMD_AppleFileConduit_h
#define SDMMobileDevice_Framework_SDMMD_AppleFileConduit_h

#include <CoreFoundation/CoreFoundation.h>
#include "SDMMD_AFC_Types.h"
#include "SDMMD_AFCConnection_Class.h"
#include "SDMMD_AFCOperation_Class.h"
#include "SDMMD_Error.h"
#include "SDMMD_Service.h"

#pragma mark -
#pragma mark Functions

CF_RETURNS_RETAINED SDMMD_AFCConnectionRef SDMMD_AFCConnectionCreate(SDMMD_AMConnectionRef conn);

sdmmd_return_t SDMMD_AFCProcessOperation(SDMMD_AFCConnectionRef conn, SDMMD_AFCOperationRef *operation);

CF_RETURNS_RETAINED CFStringRef SDMMD_ConvertResponseString(CFDataRef CF_RELEASES_ARGUMENT response_data);
CF_RETURNS_RETAINED CFArrayRef SDMMD_ConvertResponseArray(CFDataRef CF_RELEASES_ARGUMENT response_data);
CF_RETURNS_RETAINED CFDictionaryRef SDMMD_ConvertResponseDictionary(CFDataRef CF_RELEASES_ARGUMENT response_data);

#pragma mark -
#pragma mark Operations

CF_RETURNS_RETAINED SDMMD_AFCOperationRef SDMMD_AFCOperationCreateReadDirectory(CFStringRef path);
CF_RETURNS_RETAINED SDMMD_AFCOperationRef SDMMD_AFCOperationCreateRemovePath(CFStringRef path);
CF_RETURNS_RETAINED SDMMD_AFCOperationRef SDMMD_AFCOperationCreateMakeDirectory(CFStringRef path);
CF_RETURNS_RETAINED SDMMD_AFCOperationRef SDMMD_AFCOperationCreateGetFileInfo(CFStringRef path);
CF_RETURNS_RETAINED SDMMD_AFCOperationRef SDMMD_AFCOperationCreateGetDeviceInfo();
CF_RETURNS_RETAINED SDMMD_AFCOperationRef SDMMD_AFCOperationCreateFileRefOpen(CFStringRef path, uint64_t mode);
CF_RETURNS_RETAINED SDMMD_AFCOperationRef SDMMD_AFCFileDescriptorCreateReadOperation(uint64_t fileRef, uint64_t length);
CF_RETURNS_RETAINED SDMMD_AFCOperationRef SDMMD_AFCFileDescriptorCreateWriteOperation(uint64_t fileRef, CFDataRef data);
CF_RETURNS_RETAINED SDMMD_AFCOperationRef SDMMD_AFCFileDescriptorCreateSeekOperation(uint64_t fileRef, uint32_t pos);
CF_RETURNS_RETAINED SDMMD_AFCOperationRef SDMMD_AFCFileDescriptorCreateTellOperation(uint64_t fileRef);
CF_RETURNS_RETAINED SDMMD_AFCOperationRef SDMMD_AFCFileDescriptorCreateCloseOperation(uint64_t fileRef);
CF_RETURNS_RETAINED SDMMD_AFCOperationRef SDMMD_AFCFileDescriptorCreateSetFileSizeOperation(uint64_t fileRef, uint64_t file_size);
CF_RETURNS_RETAINED SDMMD_AFCOperationRef SDMMD_AFCOperationCreateGetConnectionInfo();
CF_RETURNS_RETAINED SDMMD_AFCOperationRef SDMMD_AFCOperationCreateSetConnectionOptions(CFDictionaryRef options);
CF_RETURNS_RETAINED SDMMD_AFCOperationRef SDMMD_AFCOperationCreateRenamePath(CFStringRef old, CFStringRef new);
CF_RETURNS_RETAINED SDMMD_AFCOperationRef SDMMD_AFCOperationCreateSetFSBlockSize(uint64_t fs_block_size);
CF_RETURNS_RETAINED SDMMD_AFCOperationRef SDMMD_AFCOperationCreateSetSocketBlockSize(uint64_t socket_block_size);
CF_RETURNS_RETAINED SDMMD_AFCOperationRef SDMMD_AFCFileDescriptorCreateLockOperation(uint64_t fileRef);
CF_RETURNS_RETAINED SDMMD_AFCOperationRef SDMMD_AFCOperationCreateLinkPath(uint64_t linkType, CFStringRef target, CFStringRef link);
CF_RETURNS_RETAINED SDMMD_AFCOperationRef SDMMD_AFCOperationCreateGetFileHash(CFStringRef path);
CF_RETURNS_RETAINED SDMMD_AFCOperationRef SDMMD_AFCOperationCreateSetModTime(CFStringRef ref);
CF_RETURNS_RETAINED SDMMD_AFCOperationRef SDMMD_AFCOperationCreateGetFileHashWithRange(CFStringRef path, CFRange range);
CF_RETURNS_RETAINED SDMMD_AFCOperationRef SDMMD_AFCFileDescriptorCreateSetImmutableHintOperation(uint64_t fileRef, uint64_t immutable_hint);
CF_RETURNS_RETAINED SDMMD_AFCOperationRef SDMMD_AFCOperationCreateGetSizeOfPathContents(CFStringRef path);
CF_RETURNS_RETAINED SDMMD_AFCOperationRef SDMMD_AFCOperationCreateRemovePathAndContents(CFStringRef path);

#pragma mark -
#pragma mark Alias Operations

sdmmd_return_t SDMMD_AMDeviceCopy(SDMMD_AFCConnectionRef conn, char *local, char *remote_root);
sdmmd_return_t SDMMD_AMDeviceCopyFile(CallBack callback, void *thing2, void *thing3, SDMMD_AFCConnectionRef conn, char *local, char *remote);
sdmmd_return_t SDMMD_AMDeviceRemoteCopyFile(CallBack callback, void *thing2, void *thing3, SDMMD_AFCConnectionRef conn, char *local, char *remote);
sdmmd_return_t SDMMD_check_can_touch(SDMMD_AFCConnectionRef conn, CFDataRef *unknown);

#endif
