/*
 *  disasm.c
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

#ifndef _SDMDISASM_C_
#define _SDMDISASM_C_

#include "disasm.h"

CPUArchitecture SDM_disasm_get_arch(cpu_type_t type, cpu_subtype_t subtype) {
	switch (type) {
		case CPU_TYPE_I386: {
			return i386Arch;
		};
		case CPU_TYPE_X86_64: {
			return x86_64Arch;
		};
		case CPU_TYPE_ARM: {
			if (subtype == CPU_SUBTYPE_ARM_V6)
				return ARMv6Arch;
			if (subtype == CPU_SUBTYPE_ARM_V7)
				return ARMv7Arch;
		};
		default: {
			return InvalidArch;
		};
	}
}

Endianness SDM_disasm_get_endian(CPUArchitecture arch) {
	switch (arch) {
		case i386Arch:
		case x86_64Arch:
		case ARMv6Arch:
		case ARMv7Arch: {
			return LittleEndian;
		};
		default: {
			return InvalidEndian;
		};
	}
}

SDMDisasm SDM_disasm_init(struct mach_header *header) {
	SDMDisasm disasm;
	disasm.arch = SDM_disasm_get_arch(header->cputype, header->cpusubtype);
	disasm.is64Bit = ((header->magic == MH_MAGIC_64 || header->magic == MH_CIGAM_64) ? true : false);
	disasm.endian = SDM_disasm_get_endian(disasm.arch);
	if (disasm.arch == i386Arch || disasm.arch == x86_64Arch) {
		ud_init(&disasm.obj);
		ud_set_mode(&disasm.obj, (disasm.is64Bit? 0x40 : 0x20));
		ud_set_syntax(&disasm.obj, UD_SYN_INTEL);
	} else if (disasm.arch == ARMv6Arch || disasm.arch == ARMv7Arch) {
		
	}
	return disasm;
}

void SDM_disasm_setbuffer(SDMDisasm *disasm, uint32_t *buffer, uint32_t length) {
	if (disasm->arch == i386Arch || disasm->arch == x86_64Arch) {
		ud_set_input_buffer(&disasm->obj, (uint8_t*)buffer, length);
	} else if (disasm->arch == ARMv6Arch || disasm->arch == ARMv7Arch) {
		disasm->handler.arm = (SDMDisasmObject){buffer, length, 0x0};
	}
}

ARMInstruction SDM_disasm_arm(uint32_t instruction) {
	ARMInstruction in;
	in.cc = instruction & 0xF0000000;
	return in;
}

uint32_t SDM_disasm_parse(SDMDisasm disasm) {
	if (disasm.arch == i386Arch || disasm.arch == x86_64Arch) {
		return ud_disassemble(&disasm.obj);
	} else if (disasm.arch == ARMv6Arch || disasm.arch == ARMv7Arch) {
		if (disasm.handler.arm.remainder - 1 > 0) {
			uint32_t data = disasm.handler.arm.buffer[disasm.handler.arm.length-disasm.handler.arm.remainder];
			ARMInstruction instruction = SDM_disasm_arm(data);
			disasm.handler.arm.remainder--;
		}
		return disasm.handler.arm.remainder;
	}
}

#endif