/*
 *  SDMSymbolTable.h
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


#ifndef _SDMSYMBOLTABLE_H_
#define _SDMSYMBOLTABLE_H_

#pragma mark -
#pragma mark Includes
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <mach-o/loader.h>

#pragma mark -
#pragma mark Types

typedef void* (*SDMSTFunctionCall)();

typedef struct SDMSTFunction {
	char *name;
	SDMSTFunctionCall offset;
	uint32_t argc;
	uintptr_t *argv;
} __attribute__ ((packed)) SDMSTFunction;

struct SDMSTFunctionReturn {
	struct SDMSTFunction *function;
	void* value;
} __attribute__ ((packed)) SDMSTFunctionReturn;

typedef struct SDMSTSegmentEntry {
	uint32_t cmd;
	uint32_t cmdsize;
	char segname[0x10];
} __attribute__ ((packed)) SDMSTSegmentEntry;

typedef struct SDMSTLibraryArchitecture {
	cpu_type_t type;
	cpu_subtype_t subtype;
} __attribute__ ((packed)) SDMSTLibraryArchitecture;

typedef struct SDMSTLibraryTableInfo {
	uint32_t imageNumber;
	uintptr_t *mhOffset;
	struct SDMSTSegmentEntry *textSeg;
	struct SDMSTSegmentEntry *linkSeg;
	struct symtab_command *symtabCommands;
	uint32_t symtabCount;
	uint32_t headerMagic;
	bool is64bit;
	struct SDMSTLibraryArchitecture arch;
} __attribute__ ((packed)) SDMSTLibraryTableInfo;

typedef struct SDMSTMachOSymbol {
	uint32_t tableNumber;
	uint32_t symbolNumber;
	void* offset;
	char *name;
} __attribute__ ((packed)) SDMSTMachOSymbol;

typedef struct SDMSTLibrarySymbolTable {
	char *libraryPath;
	uintptr_t* libraryHandle;
	struct SDMSTLibraryTableInfo *libInfo;
	struct SDMSTMachOSymbol *table;
	uint32_t symbolCount;
} __attribute__ ((packed)) SDMSTLibrarySymbolTable;

#pragma mark -
#pragma mark Declarations

struct SDMSTLibrarySymbolTable* SDMSTLoadLibrary(char *path);
struct SDMSTFunction* SDMSTCreateFunction(struct SDMSTLibrarySymbolTable *libTable, char *name);
struct SDMSTFunctionReturn* SDMSTCallFunction(struct SDMSTLibrarySymbolTable *libTable, struct SDMSTFunction *function);
void SDMSTFunctionRelease(struct SDMSTFunction *function);
void SDMSTFunctionReturnRelease(struct SDMSTFunctionReturn *functionReturn);
void SDMSTLibraryRelease(struct SDMSTLibrarySymbolTable *libTable);

#endif