/*
 *  SDMMD_AFC_Types.h
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

#ifndef _SDM_MD_AFC_TYPES_H_
#define _SDM_MD_AFC_TYPES_H_

#include <CoreFoundation/CoreFoundation.h>

// Initialize static CFString key arrays
void SDMMD_AFC_Types_Initialize(void);

#pragma mark -
#pragma mark DeviceInfo Keys

enum AFC_Device_Info_Key_Values {
	AFC_Device_Info_Key_FSBlockSize = 0,
	AFC_Device_Info_Key_FSFreeBytes,
	AFC_Device_Info_Key_FSTotalBytes,
	AFC_Device_Info_Key_Model,
	AFC_Device_Info_Key_count
};

#define kAFC_Device_Info_FSBlockSize "FSBlockSize"
#define kAFC_Device_Info_FSFreeBytes "FSFreeBytes"
#define kAFC_Device_Info_FSTotalBytes "FSTotalBytes"
#define kAFC_Device_Info_Model "Model"

extern CFStringRef AFC_Device_Info_Keys[AFC_Device_Info_Key_count];

#pragma mark -
#pragma mark ConnectionInfo Keys

enum AFC_Connection_Info_Key_Values {
	AFC_Connection_Info_Key_ExtendedStatus = 0,
	AFC_Connection_Info_Key_Version,
	AFC_Connection_Info_Key_count
};

#define kAFC_Connection_Info_ExtendedStatus "ExtendedStatus"
#define kAFC_Connection_Info_Version "Version"

extern CFStringRef AFC_Connection_Info_Keys[AFC_Connection_Info_Key_count];

#pragma mark -
#pragma mark FileInfo Keys

enum AFC_File_Info_Key_Values {
	AFC_File_Info_Key_birthtime = 0,
	AFC_File_Info_Key_blocks,
	AFC_File_Info_Key_ifmt,
	AFC_File_Info_Key_mtime,
	AFC_File_Info_Key_nlink,
	AFC_File_Info_Key_size,
	AFC_File_Info_Key_count
};

#define kAFC_File_Info_st_birthtime "st_birthtime"
#define kAFC_File_Info_st_blocks "st_blocks"
#define kAFC_File_Info_st_ifmt "st_ifmt"
#define kAFC_File_Info_st_mtime "st_mtime"
#define kAFC_File_Info_st_nlink "st_nlink"
#define kAFC_File_Info_st_size "st_size"

extern CFStringRef AFC_File_Info_Keys[AFC_File_Info_Key_count];

#pragma mark -
#pragma mark AFC Packet Type

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
	SDMMD_AFC_Packet_WriteAtPosition = 40,
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

#endif
