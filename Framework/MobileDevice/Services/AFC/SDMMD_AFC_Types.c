/*
 *  SDMMD_AFC_Types.c
 *  SDMMobileDevice
 *
 * Copyright (c) 2014, Samantha Marshall
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
 * 3. Neither the name of Samantha Marshall nor the names of its contributors may be used to endorse or promote products derived from this
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

#include "SDMMD_AFC_Types.h"

CFStringRef AFC_Device_Info_Keys[AFC_Device_Info_Key_count];
CFStringRef AFC_Connection_Info_Keys[AFC_Connection_Info_Key_count];
CFStringRef AFC_File_Info_Keys[AFC_File_Info_Key_count];

void SDMMD_AFC_Types_Initialize()
{
	AFC_Device_Info_Keys[AFC_Device_Info_Key_FSBlockSize] = CFSTR(kAFC_Device_Info_FSBlockSize);
	AFC_Device_Info_Keys[AFC_Device_Info_Key_FSFreeBytes] = CFSTR(kAFC_Device_Info_FSFreeBytes);
	AFC_Device_Info_Keys[AFC_Device_Info_Key_FSTotalBytes] = CFSTR(kAFC_Device_Info_FSTotalBytes);
	AFC_Device_Info_Keys[AFC_Device_Info_Key_Model] = CFSTR(kAFC_Device_Info_Model);

	AFC_Connection_Info_Keys[AFC_Connection_Info_Key_ExtendedStatus] = CFSTR(kAFC_Connection_Info_ExtendedStatus);
	AFC_Connection_Info_Keys[AFC_Connection_Info_Key_Version] = CFSTR(kAFC_Connection_Info_Version);

	AFC_File_Info_Keys[AFC_File_Info_Key_birthtime] = CFSTR(kAFC_File_Info_st_birthtime);
	AFC_File_Info_Keys[AFC_File_Info_Key_blocks] = CFSTR(kAFC_File_Info_st_blocks);
	AFC_File_Info_Keys[AFC_File_Info_Key_ifmt] = CFSTR(kAFC_File_Info_st_ifmt);
	AFC_File_Info_Keys[AFC_File_Info_Key_mtime] = CFSTR(kAFC_File_Info_st_mtime);
	AFC_File_Info_Keys[AFC_File_Info_Key_nlink] = CFSTR(kAFC_File_Info_st_nlink);
	AFC_File_Info_Keys[AFC_File_Info_Key_size] = CFSTR(kAFC_File_Info_st_size);
}
