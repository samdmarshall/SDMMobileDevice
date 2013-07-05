/*
 *  SDMSymbolTable.c
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

#ifndef _SDMSYMBOLTABLE_C_
#define _SDMSYMBOLTABLE_C_

#pragma mark -
#pragma mark Includes
#include "SDMSymbolTable.h"
#include <dlfcn.h>
#include <mach/mach.h>
#include <mach-o/dyld.h>
#include <mach-o/nlist.h>
#include <mach-o/ldsyms.h>
#include "disasm.h"

#pragma mark -
#pragma mark Internal Types

typedef struct SDMSTSymbolTableListEntry {
	union {
	   uint32_t n_strx;
	} n_un;
	uint8_t n_type;
	uint8_t n_sect;
	uint16_t n_desc;
} __attribute__ ((packed)) SDMSTSymbolTableListEntry;

typedef struct SDMSTSeg32Data {
	uint32_t vmaddr;
	uint32_t vmsize;
	uint32_t fileoff;
} __attribute__ ((packed)) SDMSTSeg32Data;

typedef struct SDMSTSeg64Data {
	uint64_t vmaddr;
	uint64_t vmsize;
	uint64_t fileoff;
} __attribute__ ((packed)) SDMSTSeg64Data;

#pragma mark -
#pragma mark Declarations

void SDMSTBuildLibraryInfo(SDMSTLibrarySymbolTable *libTable);
int SDMSTCompareTableEntries(const void *entry1, const void *entry2);
void SDMSTGenerateSortedSymbolTable(struct SDMSTLibrarySymbolTable *libTable);
bool SMDSTSymbolDemangleAndCompare(char *symFromTable, char *symbolName);
uint32_t SDMSTGetFunctionLength(struct SDMSTLibrarySymbolTable *libTable, void* functionPointer);
uint32_t SDMSTGetArgumentCount(struct SDMSTLibrarySymbolTable *libTable, void* functionPointer);
SDMSTFunctionCall SDMSTSymbolLookup(struct SDMSTLibrarySymbolTable *libTable, char *symbolName);

#pragma mark -
#pragma mark Functions

void SDMSTBuildLibraryInfo(SDMSTLibrarySymbolTable *libTable) {
	if (libTable->libInfo == NULL) {
		libTable->libInfo = (struct SDMSTLibraryTableInfo *)calloc(0x1, sizeof(struct SDMSTLibraryTableInfo));
		uint32_t count = _dyld_image_count();
		for (uint32_t i = 0x0; i < count; i++) {
			if (strcmp(_dyld_get_image_name(i), libTable->libraryPath) == 0x0) {
				libTable->libInfo->imageNumber = i;
				break;
			}
		}
		const struct mach_header *imageHeader = _dyld_get_image_header(libTable->libInfo->imageNumber);
		libTable->libInfo->headerMagic = imageHeader->magic;
		libTable->libInfo->arch = (struct SDMSTLibraryArchitecture){imageHeader->cputype, imageHeader->cpusubtype};
		libTable->libInfo->is64bit = ((imageHeader->cputype == CPU_TYPE_X86_64 || imageHeader->cputype == CPU_TYPE_POWERPC64) && (libTable->libInfo->headerMagic == MH_MAGIC_64 || libTable->libInfo->headerMagic == MH_CIGAM_64) ? true : false);
		libTable->libInfo->mhOffset = (uintptr_t*)imageHeader;
	}
	struct mach_header *libHeader = (struct mach_header *)((char*)libTable->libInfo->mhOffset);
	if (libTable->libInfo->headerMagic == libHeader->magic) {
		if (libTable->libInfo->symtabCommands == NULL) {
			struct load_command *loadCmd = (struct load_command *)((char*)libTable->libInfo->mhOffset + (libTable->libInfo->is64bit ? sizeof(struct mach_header_64) : sizeof(struct mach_header)));
			libTable->libInfo->symtabCommands = (struct symtab_command *)calloc(0x1, sizeof(struct symtab_command));
			libTable->libInfo->symtabCount = 0x0;
			for (uint32_t i = 0x0; i < libHeader->ncmds; i++) {
				if (loadCmd->cmd == LC_SYMTAB) {
					libTable->libInfo->symtabCommands = realloc(libTable->libInfo->symtabCommands, (libTable->libInfo->symtabCount+1)*sizeof(struct symtab_command));
					libTable->libInfo->symtabCommands[libTable->libInfo->symtabCount] = *(struct symtab_command *)loadCmd;
					libTable->libInfo->symtabCount++;
				}
				if (loadCmd->cmd == (libTable->libInfo->is64bit ? LC_SEGMENT_64 : LC_SEGMENT)) {
					struct SDMSTSegmentEntry *seg = (struct SDMSTSegmentEntry *)loadCmd;
					if ((libTable->libInfo->textSeg == NULL) && !strncmp(SEG_TEXT,seg->segname,sizeof(seg->segname))) {
						libTable->libInfo->textSeg = (struct SDMSTSegmentEntry *)seg;
					} else if ((libTable->libInfo->linkSeg == NULL) && !strncmp(SEG_LINKEDIT,seg->segname,sizeof(seg->segname))) {
						libTable->libInfo->linkSeg = (struct SDMSTSegmentEntry *)seg;
					}
				}
				loadCmd = (struct load_command *)((char*)loadCmd + loadCmd->cmdsize);
			}
		}
	}
}

int SDMSTCompareTableEntries(const void *entry1, const void *entry2) {
	if (((struct SDMSTMachOSymbol *)entry1)->offset < ((struct SDMSTMachOSymbol *)entry2)->offset) return -1;
	if (((struct SDMSTMachOSymbol *)entry1)->offset == ((struct SDMSTMachOSymbol *)entry2)->offset) return 0;
	if (((struct SDMSTMachOSymbol *)entry1)->offset > ((struct SDMSTMachOSymbol *)entry2)->offset) return 1;
	return -0;
}

void SDMSTGenerateSortedSymbolTable(struct SDMSTLibrarySymbolTable *libTable) {
	if (libTable->table == NULL) {
		void* symbolAddress = 0x0;
		libTable->table = (struct SDMSTMachOSymbol *)calloc(0x1, sizeof(struct SDMSTMachOSymbol));
		libTable->symbolCount = 0x0;
		if (libTable->libInfo == NULL)
			SDMSTBuildLibraryInfo(libTable);
		for (uint32_t i = 0x0; i < libTable->libInfo->symtabCount; i++) {
			struct symtab_command *cmd = (struct symtab_command *)(&(libTable->libInfo->symtabCommands[i]));
			uint64_t fslide = 0x0, mslide = 0x0;
			if (libTable->libInfo->is64bit) {
				struct SDMSTSeg64Data *textData = (struct SDMSTSeg64Data *)((char*)libTable->libInfo->textSeg + sizeof(struct SDMSTSegmentEntry));
				struct SDMSTSeg64Data *linkData = (struct SDMSTSeg64Data *)((char*)libTable->libInfo->linkSeg + sizeof(struct SDMSTSegmentEntry));
				fslide = (uint64_t)(linkData->vmaddr - textData->vmaddr) - linkData->fileoff;
				mslide = (uint64_t)((char*)libTable->libInfo->mhOffset - textData->vmaddr);
			} else {
				struct SDMSTSeg32Data *textData = (struct SDMSTSeg32Data *)((char*)libTable->libInfo->textSeg + sizeof(struct SDMSTSegmentEntry));
				struct SDMSTSeg32Data *linkData = (struct SDMSTSeg32Data *)((char*)libTable->libInfo->linkSeg + sizeof(struct SDMSTSegmentEntry));
				fslide = (uint64_t)(linkData->vmaddr - textData->vmaddr) - linkData->fileoff;
				mslide = (uint64_t)((char*)libTable->libInfo->mhOffset - textData->vmaddr);
			}
			struct SDMSTSymbolTableListEntry *entry = (struct SDMSTSymbolTableListEntry *)((char*)libTable->libInfo->mhOffset + cmd->symoff + fslide);
			for (uint32_t j = 0x0; j < cmd->nsyms; j++) {
				if (!(entry->n_type & N_STAB) && ((entry->n_type & N_TYPE) == N_SECT)) {
					char *strTable = (char*)libTable->libInfo->mhOffset + cmd->stroff + fslide;
					if (libTable->libInfo->is64bit) {
						uint64_t *n_value = (uint64_t*)((char*)entry + sizeof(struct SDMSTSymbolTableListEntry));
						symbolAddress = (void*)*n_value;
					} else {
						uint32_t *n_value = (uint32_t*)((char*)entry + sizeof(struct SDMSTSymbolTableListEntry));
						symbolAddress = (void*)*n_value;
					}
					libTable->table = realloc(libTable->table, sizeof(struct SDMSTMachOSymbol)*(libTable->symbolCount+0x1));
					struct SDMSTMachOSymbol *aSymbol = (struct SDMSTMachOSymbol *)calloc(0x1, sizeof(struct SDMSTMachOSymbol));
					aSymbol->tableNumber = i;
					aSymbol->symbolNumber = j;
					aSymbol->offset = (void*)symbolAddress + _dyld_get_image_vmaddr_slide(libTable->libInfo->imageNumber);
					aSymbol->name = ((char *)strTable + entry->n_un.n_strx);
					libTable->table[libTable->symbolCount] = *aSymbol;
					libTable->symbolCount++;
				}
				entry = (struct SDMSTSymbolTableListEntry *)((char*)entry + (sizeof(struct SDMSTSymbolTableListEntry) + (libTable->libInfo->is64bit ? sizeof(uint64_t) : sizeof(uint32_t))));
			}
		}
		qsort(libTable->table, libTable->symbolCount, sizeof(struct SDMSTMachOSymbol), SDMSTCompareTableEntries);
	}
}

struct SDMSTLibrarySymbolTable* SDMSTLoadLibrary(char *path) {
	struct SDMSTLibrarySymbolTable *table = (struct SDMSTLibrarySymbolTable *)calloc(0x1, sizeof(struct SDMSTLibrarySymbolTable));
	void* handle = dlopen(path, RTLD_LOCAL);
	if (handle) {
		table->libraryPath = path;
		table->libraryHandle = handle;
		table->libInfo = NULL;
		table->table = NULL;
		table->symbolCount = 0x0;
		SDMSTBuildLibraryInfo(table);
		SDMSTGenerateSortedSymbolTable(table);
	}
	return table;
}

bool SMDSTSymbolDemangleAndCompare(char *symFromTable, char *symbolName) {
	bool matchesName = false;
	if (symFromTable && symbolName) {
		uint32_t tabSymLength = strlen(symFromTable);
		uint32_t symLength = strlen(symbolName);
		if (symLength <= tabSymLength) {
			char *offset = strstr(symFromTable, symbolName);
			if (offset) {
				uint32_t originOffset = offset - symFromTable;
				if (tabSymLength-originOffset == symLength)
					matchesName = (strcmp(&symFromTable[originOffset], symbolName) == 0x0);
			} 
		}
	}
	return matchesName;
}

uint32_t SDMSTGetFunctionLength(struct SDMSTLibrarySymbolTable *libTable, void* functionPointer) {
	uint32_t nextOffset = 0xffffffff;
	for (uint32_t i = 0x0; i < libTable->symbolCount; i++)
		if (functionPointer < libTable->table[i].offset)
			if ((uint32_t)libTable->table[i].offset < nextOffset)
				nextOffset = (uint32_t)libTable->table[i].offset;
	return (nextOffset - (uint32_t)functionPointer);
}

SDMSTParsedLine* SDMSTParse(char *code) {
	SDMSTParsedLine *line = calloc(1, sizeof(SDMSTParsedLine));
	line->instructionLen = strcspn(code, " ");
	line->instruction = calloc(1, sizeof(char)*line->instructionLen);
	line->instruction = strncpy(line->instruction, code, line->instructionLen);
	line->value0Len = strcspn(code+(line->instructionLen+1), ", ");
	line->value0 = calloc(1, sizeof(char)*line->value0Len);
	line->value0 = strncpy(line->value0, code+(line->instructionLen+1), line->value0Len);
	line->value1Len = strcspn(code+(line->instructionLen+1+line->value0Len+2), " ");
	line->value1 = calloc(1, sizeof(char)*line->value1Len);
	line->value1 = strncpy(line->value1, code+(line->instructionLen+1+line->value0Len+2), line->value1Len);
	return line;
}

uint32_t SDMSTFindInputRegisters(SDMDisasm disasm, char *code) {
	uint32_t result = 0;
	SDMSTInputRegisterType *inputRegArray = (disasm.arch == i386Arch || disasm.arch == x86_64Arch ? kIntelInputRegs : kARMInputRegs);
	uint32_t inputRegArrayCount = (disasm.arch == i386Arch || disasm.arch == x86_64Arch ? kIntelInputRegsCount : kARMInputRegsCount);
	SDMSTParsedLine *line = SDMSTParse(code);
	if (line->value1Len) {
		for (uint32_t i = 0; i < inputRegArrayCount; i++) {
			if (strstr(line->value1, inputRegArray[i].name)) {
				result = inputRegArray[i].number+1;
			}
		}
	}
	free(line);
	return result;
}

uint32_t SDMSTGetArgumentCount(struct SDMSTLibrarySymbolTable *libTable, void* functionPointer) {
	uint32_t argumentCount = 0x0;
	if (functionPointer) {
		uint32_t functionLength = SDMSTGetFunctionLength(libTable, functionPointer);
			struct SDMDisasm disasm = SDM_disasm_init((struct mach_header *)(libTable->libInfo->mhOffset));
			SDM_disasm_setbuffer(&disasm, functionPointer, functionLength);
			ud_t obj;
			ud_init(&obj);
			ud_set_mode(&obj, (disasm.is64Bit? 0x40 : 0x20));
			ud_set_syntax(&obj, UD_SYN_INTEL);
			ud_set_input_buffer(&obj, functionPointer, functionLength);
			bool *inputRegisters;
			while (ud_disassemble(&obj)) {
				if (disasm.arch == i386Arch || disasm.arch == x86_64Arch) {
					char *code = (char*)ud_insn_asm(&obj);
					uint32_t count = SDMSTFindInputRegisters(disasm, code);
					if (count > argumentCount)
						argumentCount = count;
					if (strcmp(code, "ret")==0x0)
						break;
				} else if (disasm.arch == ARMv6Arch || disasm.arch == ARMv7Arch) {

				}
			}
		if (libTable->libInfo->arch.type == CPU_TYPE_ARM) {
			bool inputRegisters[kARMInputRegsCount] = {false, false, false, false};
			if (libTable->libInfo->arch.subtype == CPU_SUBTYPE_ARM_V6) {
				
			} else if (libTable->libInfo->arch.subtype == CPU_SUBTYPE_ARM_V7) {
				
			}
			for (uint32_t i = 0x0; i < kARMInputRegsCount; i++)
				if (inputRegisters[i])
					argumentCount = kARMInputRegs[i].number+1;
		}
	}
	return argumentCount;
}

SDMSTFunctionCall SDMSTSymbolLookup(struct SDMSTLibrarySymbolTable *libTable, char *symbolName) {
	void* symbolAddress = 0x0;
	for (uint32_t i = 0x0; i < libTable->symbolCount; i++)
		if (SMDSTSymbolDemangleAndCompare(libTable->table[i].name, symbolName)) {
			symbolAddress = libTable->table[i].offset;
			break;
		}
	return symbolAddress;
}

struct SDMSTFunction* SDMSTCreateFunction(struct SDMSTLibrarySymbolTable *libTable, char *name) {
	struct SDMSTFunction *function = (struct SDMSTFunction*)calloc(0x1, sizeof(struct SDMSTFunction));
	function->name = name;
	function->offset = SDMSTSymbolLookup(libTable, name);
	function->argc = SDMSTGetArgumentCount(libTable, function->offset);
	function->argv = (uintptr_t*)calloc(0xe, sizeof(uintptr_t));
	return function;
}

struct SDMSTFunctionReturn* SDMSTCallFunction(struct SDMSTLibrarySymbolTable *libTable, struct SDMSTFunction *function) {
	struct SDMSTFunctionReturn *result = (struct SDMSTFunctionReturn*)calloc(0x1, sizeof(struct SDMSTFunctionReturn));
	result->function = function;
	result->value = NULL;
	switch (function->argc) {
		case 0: result->value = function->offset(); break;
		case 1: result->value = function->offset(function->argv[0]); break;
		case 2: result->value = function->offset(function->argv[0], function->argv[1]); break;
		case 3: result->value = function->offset(function->argv[0], function->argv[1], function->argv[2]); break;
		case 4: result->value = function->offset(function->argv[0], function->argv[1], function->argv[2], function->argv[3]); break;
		case 5: result->value = function->offset(function->argv[0], function->argv[1], function->argv[2], function->argv[3], function->argv[4]); break;
		case 6: result->value = function->offset(function->argv[0], function->argv[1], function->argv[2], function->argv[3], function->argv[4], function->argv[5]); break;
		case 7: result->value = function->offset(function->argv[0], function->argv[1], function->argv[2], function->argv[3], function->argv[4], function->argv[5], function->argv[6]); break;
		case 8: result->value = function->offset(function->argv[0], function->argv[1], function->argv[2], function->argv[3], function->argv[4], function->argv[5], function->argv[6], function->argv[7]); break;
		case 9: result->value = function->offset(function->argv[0], function->argv[1], function->argv[2], function->argv[3], function->argv[4], function->argv[5], function->argv[6], function->argv[7], function->argv[8]); break;
		case 10: result->value = function->offset(function->argv[0], function->argv[1], function->argv[2], function->argv[3], function->argv[4], function->argv[5], function->argv[6], function->argv[7], function->argv[8], function->argv[9]); break;
		case 11: result->value = function->offset(function->argv[0], function->argv[1], function->argv[2], function->argv[3], function->argv[4], function->argv[5], function->argv[6], function->argv[7], function->argv[8], function->argv[9], function->argv[10]); break;
		case 12: result->value = function->offset(function->argv[0], function->argv[1], function->argv[2], function->argv[3], function->argv[4], function->argv[5], function->argv[6], function->argv[7], function->argv[8], function->argv[9], function->argv[10], function->argv[11]); break;
		case 13: result->value = function->offset(function->argv[0], function->argv[1], function->argv[2], function->argv[3], function->argv[4], function->argv[5], function->argv[6], function->argv[7], function->argv[8], function->argv[9], function->argv[10], function->argv[11], function->argv[12]); break;
		case 14: result->value = function->offset(function->argv[0], function->argv[1], function->argv[2], function->argv[3], function->argv[4], function->argv[5], function->argv[6], function->argv[7], function->argv[8], function->argv[9], function->argv[10], function->argv[11], function->argv[12], function->argv[13]); break;
		default: break;
	}
	return result;
}

void SDMSTFunctionRelease(struct SDMSTFunction *function) {
	free(function->argv);
	free(function);
}

void SDMSTFunctionReturnRelease(struct SDMSTFunctionReturn *functionReturn) {
	SDMSTFunctionRelease(functionReturn->function);
	free(functionReturn);
}

void SDMSTLibraryRelease(struct SDMSTLibrarySymbolTable *libTable) {
	free(libTable->libInfo);
	free(libTable->table);
	dlclose(libTable->libraryHandle);
	free(libTable);
}

#endif