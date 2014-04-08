//
//  SDMMD_AppleFileConduit.h
//  SDMMobileDevice-Framework
//
//  Created by Sam Marshall on 4/5/14.
//
//

#ifndef SDMMobileDevice_Framework_SDMMD_AppleFileConduit_h
#define SDMMobileDevice_Framework_SDMMD_AppleFileConduit_h

#include <CoreFoundation/CoreFoundation.h>
#include "SDMMD_Error.h"
#include "SDMMD_Service.h"

enum SDMMD_AFC_Packet_Type {
	SDMMD_AFC_Packet_Invalid = 0,
	SDMMD_AFC_Packet_Status,
	SDMMD_AFC_Packet_Data,
	SDMMD_AFC_Packet_ReadDirectory = 3,
	SDMMD_AFC_Packet_ReadFile,
	SDMMD_AFC_Packet_WriteFile,
	SDMMD_AFC_Packet_WritePart,
	SDMMD_AFC_Packet_TruncFile,
	SDMMD_AFC_Packet_RemovePath = 8,
	SDMMD_AFC_Packet_MakeDirectory = 9,
	SDMMD_AFC_Packet_GetFileInfo = 10,
	SDMMD_AFC_Packet_GetDeviceInfo = 11,
	SDMMD_AFC_Packet_WriteFileAtomic = 12,
	SDMMD_AFC_Packet_FileRefOpen = 13,
	SDMMD_AFC_Packet_FileRefOpenResult = 14,
	SDMMD_AFC_Packet_FileRefRead = 15,
	SDMMD_AFC_Packet_FileRefWrite = 16,
	SDMMD_AFC_Packet_FileRefSeek,
	SDMMD_AFC_Packet_FileRefTell,
	SDMMD_AFC_Packet_FileRefTellResult,
	SDMMD_AFC_Packet_FileRefClose,
	SDMMD_AFC_Packet_FileRefSetFileSize,
	SDMMD_AFC_Packet_GetConnectionInfo = 22,
	SDMMD_AFC_Packet_SetConnectionOptions,
	SDMMD_AFC_Packet_RenamePath = 24,
	SDMMD_AFC_Packet_SetFSBlockSize,
	SDMMD_AFC_Packet_SetSocketBlockSize,
	SDMMD_AFC_Packet_FileRefLock = 27,
	SDMMD_AFC_Packet_MakeLink = 28,
	SDMMD_AFC_Packet_GetFileHash = 29,
	SDMMD_AFC_Packet_SetModTime = 30,
	SDMMD_AFC_Packet_GetFileHashWithRange,
	SDMMD_AFC_Packet_FileRefSetImmutableHint,
	SDMMD_AFC_Packet_GetSizeOfPathContents,
	SDMMD_AFC_Packet_RemovePathAndContents,
	SDMMD_AFC_Packet_DirectoryEnumeratorRefOpen,
	SDMMD_AFC_Packet_DirectoryEnumeratorRefOpenResult,
	SDMMD_AFC_Packet_DirectoryEnumeratorRefRead,
	SDMMD_AFC_Packet_DirectoryEnumeratorRefClose,
	SDMMD_AFC_Packet_Count
};

static char* SDMMD_gAFCPacketTypeNames[SDMMD_AFC_Packet_Count] = {
	"Invalid",
	"Status",
	"Data",
	"ReadDirectory",
	"ReadFile",
	"WriteFile",
	"WritePart",
	"TruncFile",
	"RemovePath",
	"MakeDirectory",
	"GetFileInfo",
	"GetDeviceInfo",
	"WriteFileAtomic",
	"FileRefOpen",
	"FileRefOpenResult",
	"FileRefRead",
	"FileRefWrite",
	"FileRefSeek",
	"FileRefTell",
	"FileRefTellResult",
	"FileRefClose",
	"FileRefSetFileSize",
	"GetConnectionInfo",
	"SetConnectionOptions",
	"RenamePath",
	"SetFSBlockSize",
	"SetSocketBlockSize",
	"FileRefLock",
	"MakeLink",
	"GetFileHash",
	"SetModTime",
	"GetFileHashWithRange",
	"FileRefSetImmutableHint",
	"GetSizeOfPathContents",
	"RemovePathAndContents",
	"DirectoryEnumeratorRefOpen",
	"DirectoryEnumeratorRefOpenResult",
	"DirectoryEnumeratorRefRead",
	"DirectoryEnumeratorRefClose"
};

struct sdmmd_AFCConnectionClass {
	SDMMD_AMConnectionRef handle;
	__unsafe_unretained dispatch_queue_t operationQueue;
	__unsafe_unretained dispatch_semaphore_t semaphore;
	uint64_t operationCount;
} sdmmd_AFCConnectionClass;

typedef struct sdmmd_AFCConnectionClass* SDMMD_AFCConnectionRef;

typedef struct SDMMD_AFCPacketHeader {
	uint64_t signature;
	uint64_t packetLen;
	uint64_t headerLen;
	uint64_t pid;
	uint64_t type;
} ATR_PACK SDMMD_AFCPacketHeader;

struct sdmmd_AFCPacket {
	SDMMD_AFCPacketHeader header;
	char* header_data;
	char* body_data;
	CFTypeRef response;
} sdmmd_AFCPacket;

struct sdmmd_AFCOperation {
	struct sdmmd_AFCPacket *packet;
	dispatch_time_t timeout;
} sdmmd_AFCOperation;

typedef struct sdmmd_AFCOperation* SDMMD_AFCOperationRef;

#pragma mark -
#pragma mark Functions

SDMMD_AFCConnectionRef SDMMD_AFCConnectionCreate(SDMMD_AMConnectionRef conn);

sdmmd_return_t SDMMD_AFCProcessOperation(SDMMD_AFCConnectionRef conn, SDMMD_AFCOperationRef *operation);

void SDMMD_AFCOperationRelease(SDMMD_AFCOperationRef operation);
void SDMMD_AFCConnectionRelease(SDMMD_AFCConnectionRef conn);

CFStringRef SDMMD_CreateConvertedResponseString(CFDataRef response_data);
CFArrayRef SDMMD_CreateConvertedResponseArray(CFDataRef response_data);
CFDictionaryRef SDMMD_CreateConvertedResponseDictionary(CFDataRef response_data);

#pragma mark -
#pragma mark Operations

SDMMD_AFCOperationRef SDMMD_AFCOperationCreateReadDirectory(CFStringRef path);
SDMMD_AFCOperationRef SDMMD_AFCOperationCreateRemovePath(CFStringRef path);
SDMMD_AFCOperationRef SDMMD_AFCOperationCreateMakeDirectory(CFStringRef path);
SDMMD_AFCOperationRef SDMMD_AFCOperationCreateGetFileInfo(CFStringRef path);
SDMMD_AFCOperationRef SDMMD_AFCOperationCreateGetDeviceInfo();
SDMMD_AFCOperationRef SDMMD_AFCOperationCreateFileRefOpen(CFStringRef path, uint64_t mode, Pointer *fileRef);
SDMMD_AFCOperationRef SDMMD_AFCFileDescriptorCreateReadOperation(Pointer fileRef, uint32_t length);
SDMMD_AFCOperationRef SDMMD_AFCFileDescriptorCreateWriteOperation(Pointer fileRef, CFDataRef data);
SDMMD_AFCOperationRef SDMMD_AFCFileDescriptorCreateSeekOperation(Pointer fileRef, uint32_t pos);
SDMMD_AFCOperationRef SDMMD_AFCFileDescriptorCreateTellOperation(Pointer fileRef);
SDMMD_AFCOperationRef SDMMD_AFCFileDescriptorCreateCloseOperation(Pointer fileRef);

SDMMD_AFCOperationRef SDMMD_AFCOperationCreateGetConnectionInfo();

SDMMD_AFCOperationRef SDMMD_AFCOperationCreateRenamePath(CFStringRef old, CFStringRef new);


SDMMD_AFCOperationRef SDMMD_AFCFileDescriptorCreateLockOperation(void* fileRef);
SDMMD_AFCOperationRef SDMMD_AFCOperationCreateLinkPath(uint64_t linkType, CFStringRef target, CFStringRef link);
SDMMD_AFCOperationRef SDMMD_AFCOperationCreateGetFileHash(CFStringRef path);
SDMMD_AFCOperationRef SDMMD_AFCOperationCreateSetModTime(CFStringRef ref);
SDMMD_AFCOperationRef SDMMD_AFCOperationCreateGetFileHashWithRange(CFStringRef path, Range range);

#pragma mark -
#pragma mark Alias Operations

sdmmd_return_t SDMMD_AMDeviceCopyFile(void *thing, void *thing2, void *thing3, SDMMD_AFCConnectionRef conn, char *local, char *remote);
sdmmd_return_t SDMMD_check_can_touch(SDMMD_AFCConnectionRef conn, CFDataRef *unknown);

#endif
