/*
 *  SDMMD_AFC.c
 *  SDM_MD_Demo
 *
 *  Copyright (c) 2013, Sam Marshall
 *  All rights reserved.
 * 
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 *  1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
 *  3. All advertising materials mentioning features or use of this software must display the following acknowledgement:
 *  	This product includes software developed by the Sam Marshall.
 *  4. Neither the name of the Sam Marshall nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
 * 
 *  THIS SOFTWARE IS PROVIDED BY Sam Marshall ''AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL Sam Marshall BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 */

#ifndef _SDM_MD_AFC_C_
#define _SDM_MD_AFC_C_

#include "SDMMD_AFC.h"

char* SDMMD_AFCStringCopy(char *dest, uint32_t destLength, char *source, uint32_t sourceLength) {
	if (sourceLength) {
		if (sourceLength < destLength) {
			sourceLength = destLength;
		}
		strncpy(dest, source, sourceLength);
	} else {
		strlcpy(dest, source, destLength);
	}
	return dest;
}

char* SDMMD_AFCPacketTypeName(uint32_t packetType) {
	char *name = "Unknown";
	if (packetType != 0x0 && packetType <= 0x28) {
		name = _gAFCPacketTypeNames[packetType*8];
	}
	return name;
}

void* SDMMD_AFCSendStatusExtended(CFTypeRef a, void*b, uint32_t packetType, CFDictionaryRef ref) {
	void* result = 0x0;
	SDMMD_AFCLog(0x5, "Writing status packet %d info %p\\n",packetType,ref);
	if (a && packetType+0x17ffbffe >= 0x15) {
		SDMMD_AFCLog(0x5, "Oh no!");
	}
	if ((a+0x44) == 0x0) {
		uint32_t length = 0x30;
		CFDataRef afcData = NULL;
		if (ref != 0x0 && ((a+0x80) & 1) != 0x0) {
			afcData = SDMMD___AFCCreateAFCDataWithDictionary(ref);
			length += CFDataGetLength(afcData);
		}
		uint64_t header = 0x4141504c36414643;
		uint32_t data24 = 0x30;
		uint32_t data40 = 0x1;
		uint32_t packetMask = packetType & 0x1fff;
		uint32_t data32 = 0x0;
		if (b != 0x0) {
			data32 = (b+0x18);
		} else {
			data32 = 0xffffffff;
		}
		result = SDMMD_AFCSendHeader(a, &header);
		if (afcData != 0x0) {
			if (result == 0x0) {
				result = SDMMD_AFCSendData(a, CFDataGetBytePtr(afcData), length);
			}
		}
		CFRelease(afcData);
	} else {
		result = (a+0x40);	
	}
	return result;
}

void* SDMMD_AFCSendStatus(CFTypeRef a, void*b, void*c) {
	CFTypeRef arg = SDMMD_AFCCopyAndClearLastErrorInfo();
	void* result = SDMMD_AFCSendStatusExtended(a, b, c, arg);
	if (arg) {
		CFRelease(arg);
	}
	return result;
}

void* SDMMD_AFCSendDataPacket(CFTypeRef a, void*b, uint32_t *dataBytePtr, uint32_t dataLength) {
	void* result = 0x0;
	SDMMD_AFCLog(0x5, "Writing data packet with data length %u\n",dataLength);
	if ((a+0x44) != 0x0) {
		result = (a+0x40);
	} else {
		uint64_t header = 0x4141504c36414643;
		uint32_t packetLength = dataLength + 0x28;
		uint32_t data32 = 0x0;
		if (b != 0x0) {
			data32 = (b+0x18);
		} else {
			data32 = 0xffffffff;
		}
		result = SDMMD_AFCSendHeader(a, &header);
		if (result == 0x0) {
			result = SDMMD_AFCSendData(a, dataBytePtr, dataLength);
		}
	}
	return result;
}

void* SDMMD_AFCSendHeader(CFTypeRef a, void*b) {
	void* result = 0x0;
	if ((a+0x44) != 0x0) {
		result = (a+0x40);
	} else {
		SDMMD_AFCLogPacketInfo(0x4, "AFCSendHeader", b);
		result = SDMMD_AFCSendData(a, b, 0x10);
	}
	return result;
}

void* SDMMD_AFCReadPacket(CFTypeRef a, CFTypeRef* b, CFTypeRef* c, CFTypeRef* d) {
	void* result = 0xe800400b;
	CFTypeRef packetHeader = NULL;
	CFTypeRef packetBody0 = 0x0;
	uint32_t packetBody1 = 0x0;
	SDMMD_AFCLockLock((a+0x90));
	if ((a+0x10) == 0x1) {
		result = SDMMD_AFCReadPacketHeader(a, , 0x80, packetHeader);
		if (result == 0x0) {
			result = SDMMD_AFCReadPacketBody(a, packetHeader, &packetBody0, &packetBody1);
			if (result == 0x0) {
				*b = packetHeader;
				*c = packetBody0;
				*d = packetBody1;
				result = 0x0;
			} else {
				if (packetBody0 != 0x0) {
					CFTypeRef allocRef = CFGetAllocator(a);
					CFAllocatorDeallocate(allocRef, packetBody0);
				}
			}
		} else {
			if (packetHeader != 0x0) {
				CFTypeRef allocRef = CFGetAllocator(a);
				CFAllocatorDeallocate(allocRef, packetHeader);
			}
		}
	}
	SDMMD_AFCLockUnlock((a+0x90));
	return result;
}

void* SDMMD_AFCReadPacketBody(CFTypeRef a,void*b, CFDataRef* c, uint32_t *readLength) {
	void* result = 0x0;
	if ((a+0x44) == 0x0) {
		uint32_t dataLength = (b+0x8);
		CFDataRef data = NULL;
		if (dataLength-(b+0x10) != 0x0) {
			CFTypeRef allocRef = CFGetAllocator(a);
			data = CFAllocatorAllocate(allocRef, dataLength, 0x0);
			if (result == 0x0) {
				result = SDMMD___AFCSetErrorResult(0x0, 0xe8004003, 0x24d, "CFAllocatorAllocate");
			}
			result = SDMMD_AFCReadData(a, data, dataLength);
			if (result == 0x0) {
				
			} else {
				result = (a+0x40);
			}
		}
		*c = data;
		*readLength = dataLength;
	} else {
		result = (a+0x40);
	}
	return result;
}

void* SDMMD_AFCSendPacket(CFTypeRef a, CFTypeRef b,void*c, uint32_t size) {
	void* result = 0xe800400b;
	SDMMD_AFCLockLock((a+0x90));
	if ((a+0x10) == 0x1) {
		if (c == 0x0)
			result = 0xe8004007;
		if (((char*)b+0x10) + size > 0x2000) {
			result = SDMMD_AFCSendHeader(a, b);
			if (result == 0x0) {
				result = SDMMD_AFCSendData(a, c, size);
			}
		} else {
			char *data = malloc(0x2000);
			if (data != 0x0) {
				memcpy((b+0x10) + data, c, size);
				SDMMD_AFCLogPacketInfo(0x4, "AFCSendHeader", b);
				result = SDMMD_AFCSendData(a, data, size);
				free(data);
			} else {
				result = SDMMD_AFCSendHeader(a, b);
				if (result == 0x0) {
					result = SDMMD_AFCSendData(a, c, size);
				}
			}
		}
	}
	SDMMD_AFCLockUnlock((a+0x90));
	return result;
}

uint32_t SDMMD_AFCHeaderInit(SDMMD_AFCHeaderRef header,void*b,void*c,void*d,void*e) {
	uint32_t result = 0x4141504c36414643;
	header->header = result;
	header->b = c;
	header->a = (d+c);
	header->d = b;
	if (e != 0x0) {
		header->c = e + 0x18;
	} else {
		header->c = 0xffffffff;
	}
	return result;
}

#endif