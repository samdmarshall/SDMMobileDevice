/*
 *  disasm.h
 *  SDMSymbolTable
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

#ifndef _SDMDISASM_H_
#define _SDMDISASM_H_

#include <stdbool.h>
#include <mach-o/loader.h>
#include "udis86.h"
#include "arm_decode.h"

typedef struct SDMSTParsedLine {
	char *instruction;
	uint32_t instructionLen;
	char *value0;
	uint32_t value0Len;
	char *value1;
	uint32_t value1Len;
} __attribute__ ((packed)) SDMSTParsedLine;

typedef struct SDMSTInputRegisterType {
	char *name;
	uint32_t number;
} __attribute__ ((packed)) SDMSTInputRegisterType;

typedef struct IntelInstruction {
	char *data;
} __attribute__ ((packed)) IntelInstruction;

#pragma mark -
#pragma mark Constants

#define kIntelInputRegsCount 0xe

#define kARMInputRegsCount 0x4

static SDMSTInputRegisterType kIntelInputRegs[kIntelInputRegsCount] = {
	{"rdi\0", 0x0},
	{"rsi\0", 0x1},
	{"rdx\0", 0x2},
	{"rcx\0", 0x3},
	{"r8\0", 0x4},
	{"r9\0", 0x5},
	{"xmm0\0", 0x6},
	{"xmm1\0", 0x7},
	{"xmm2\0", 0x8},
	{"xmm3\0", 0x9},
	{"xmm4\0", 0xa},
	{"xmm5\0", 0xb},
	{"xmm6\0", 0xc},
	{"xmm7\0", 0xd}
};

static SDMSTInputRegisterType kARMInputRegs[kARMInputRegsCount] = {
	{"r0\0", 0x0},
	{"r1\0", 0x1},
	{"r2\0", 0x2},
	{"r3\0", 0x3}
};

typedef enum CPUArchitecture {
	InvalidArch = -1,
	i386Arch = 1,
	x86_64Arch = 2,
	ARMv6Arch = 3,
	ARMv7Arch = 4
} CPUArchitecture;

typedef enum Endianness {
	InvalidEndian = -1,
	LittleEndian = 1,
	BigEndian = 2,
	BiEndian = 3
} Endianness;

typedef struct SDMDisasmObject {
	uint32_t *buffer;
	uint32_t length;
	uint32_t remainder;
} __attribute__ ((packed)) SDMDisasmObject;

typedef struct SDMDisasm {
	CPUArchitecture arch;
	bool is64Bit;
	Endianness endian;
	ud_t obj;
	union {
		SDMDisasmObject arm;
	} handler;
} SDMDisasm;

SDMDisasm SDM_disasm_init(struct mach_header *header);
void SDM_disasm_setbuffer(SDMDisasm *disasm, uint32_t *buffer, uint32_t length);
uint32_t SDM_disasm_parse(SDMDisasm disasm);

#endif