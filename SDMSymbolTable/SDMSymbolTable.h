/*
 *  SDMSymbolTable.h
 *  SDM_MD_Demo
 *
 *  Created by sam on 6/18/13.
 *  Copyright 2013 Sam Marshall. All rights reserved.
 *
 */

#ifndef _SDMSYMBOLTABLE_H_
#define _SDMSYMBOLTABLE_H_

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct SegmentEntry {
	uint32_t cmd;
	uint32_t cmdsize;
	char segname[16];
} __attribute__ ((packed)) SegmentEntry;

typedef struct LibraryTableInfo {
	uint32_t imageNumber;
	uintptr_t *mhOffset;
	struct SegmentEntry *textSeg;
	struct SegmentEntry *linkSeg; 
	uint32_t headerMagic;
	bool is64bit;
} __attribute__ ((packed)) LibraryTableInfo;

typedef struct MachOSymbol {
	void* functionPointer;
	char *symbolName;
} __attribute__ ((packed)) MachOSymbol;

typedef struct LibrarySymbolTable {
	char *libraryPath;
	uintptr_t* libraryHandle;
	struct LibraryTableInfo *libInfo;
	uint32_t symbolCount;
	struct MachOSymbol *table;
} __attribute__ ((packed)) LibrarySymbolTable;

struct LibrarySymbolTable* SDMSTLoadLibrary(char *path);
void* SDMSTSymbolLookup(struct LibrarySymbolTable *libTable, char *symbolName);
void SDMSTLibraryRelease(struct LibrarySymbolTable *libTable);

#endif