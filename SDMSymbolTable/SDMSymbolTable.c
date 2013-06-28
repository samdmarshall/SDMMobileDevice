/*
 *  SDMSymbolTable.c
 *  SDM_MD_Demo
 *
 *  Created by sam on 6/18/13.
 *  Copyright 2013 Sam Marshall. All rights reserved.
 *
 */

#ifndef _SDMSYMBOLTABLE_C_
#define _SDMSYMBOLTABLE_C_

#include "SDMSymbolTable.h"
#include <dlfcn.h>
#include <mach/mach.h>
#include <mach-o/dyld.h>
#include <mach-o/nlist.h>
#include <mach-o/loader.h>
#include <mach-o/ldsyms.h>

typedef struct SymbolTableListEntry {
	union {
	   uint32_t n_strx;
	} n_un;
	uint8_t n_type;
	uint8_t n_sect;
	uint16_t n_desc;
} __attribute__ ((packed)) SymbolTableListEntry;

typedef struct Seg32Data {
	uint32_t vmaddr;
	uint32_t vmsize;
	uint32_t fileoff;
} __attribute__ ((packed)) Seg32Data;

typedef struct Seg64Data {
	uint64_t vmaddr;
	uint64_t vmsize;
	uint64_t fileoff;
} __attribute__ ((packed)) Seg64Data;

struct LibrarySymbolTable* SDMSTLoadLibrary(char *path) {
	struct LibrarySymbolTable *table = (struct LibrarySymbolTable *)calloc(0x1, sizeof(struct LibrarySymbolTable));
	void* handle = dlopen(path, RTLD_LOCAL);
	if (handle) {
		table->libraryPath = path;
		table->libraryHandle = handle;
		table->libInfo = NULL;
		table->symbolCount = 0x0;
		table->table = (struct MachOSymbol *)calloc(0x1, sizeof(struct MachOSymbol));
	}
	return table;
}

bool SMDSTSymbolDemangleAndCompare(char *symFromTable, char *symbolName) {
	bool matchesName = false;
	if (symFromTable) {
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

void* SDMSTSymbolLookup(struct LibrarySymbolTable *libTable, char *symbolName) {
	void* symbolAddress = 0x0;
	for (uint32_t i = 0x0; i < libTable->symbolCount; i++) {
		if (strcmp(libTable->table[i].symbolName, symbolName) == 0x0) {
			symbolAddress = libTable->table[i].functionPointer;
			break;
		}
	}
	if (symbolAddress == 0x0) {
		symbolAddress = dlsym(libTable->libraryHandle, symbolName);
		if (symbolAddress) {
			libTable->table = realloc(libTable->table, (libTable->symbolCount+0x1)*sizeof(struct MachOSymbol));
			struct MachOSymbol *newSymbol = (struct MachOSymbol *)malloc(sizeof(struct MachOSymbol));
			newSymbol->functionPointer = symbolAddress;
			newSymbol->symbolName = malloc(strlen(symbolName)+1);
			strcpy(newSymbol->symbolName, symbolName);
			libTable->table[libTable->symbolCount] = *newSymbol;
			libTable->symbolCount++;
		} else {
			if (libTable->libInfo == NULL) {
				libTable->libInfo = (struct LibraryTableInfo *)calloc(0x1, sizeof(struct LibraryTableInfo));
				uint32_t count = _dyld_image_count();
				for (uint32_t i = 0x0; i < count; i++) {
					if (strcmp(_dyld_get_image_name(i), libTable->libraryPath) == 0x0) {
						libTable->libInfo->imageNumber = i;
						break;
					}
				}
				const struct mach_header *imageHeader = _dyld_get_image_header(libTable->libInfo->imageNumber);
				libTable->libInfo->headerMagic = imageHeader->magic;
				libTable->libInfo->is64bit = ((imageHeader->cputype == CPU_TYPE_X86_64 || imageHeader->cputype == CPU_TYPE_POWERPC64) && (libTable->libInfo->headerMagic == MH_MAGIC_64 || libTable->libInfo->headerMagic == MH_CIGAM_64)  ? true : false);
				libTable->libInfo->mhOffset = (char*)imageHeader;
			}
			if (libTable->libInfo) {
				struct mach_header *libHeader = (char*)libTable->libInfo->mhOffset;
				if (libTable->libInfo->headerMagic == libHeader->magic) {
					struct load_command *loadCmd = (char*)libTable->libInfo->mhOffset + (libTable->libInfo->is64bit ? sizeof(struct mach_header_64) : sizeof(struct mach_header));
					struct symtab_command *symtabCommands = (struct symtab_command *)calloc(0x1, sizeof(struct symtab_command));
					uint32_t symtabCount = 0x0;
					for (uint32_t i = 0x0; i < libHeader->ncmds; i++) {
						if (loadCmd->cmd == LC_SYMTAB) {
							symtabCommands = realloc(symtabCommands, (symtabCount+1)*sizeof(struct symtab_command));
							symtabCommands[symtabCount] = *(struct symtab_command *)loadCmd;
							symtabCount++;
						}
						if (loadCmd->cmd == (libTable->libInfo->is64bit ? LC_SEGMENT_64 : LC_SEGMENT)) {
							struct SegmentEntry *seg = loadCmd;
							if ((libTable->libInfo->textSeg == NULL) && !strncmp(SEG_TEXT,seg->segname,sizeof(seg->segname))) {
								libTable->libInfo->textSeg = (char*)seg;
							} else if ((libTable->libInfo->linkSeg == NULL) && !strncmp(SEG_LINKEDIT,seg->segname,sizeof(seg->segname))) {
								libTable->libInfo->linkSeg = (char*)seg;
							}
						}
						loadCmd = (char*)loadCmd + loadCmd->cmdsize;
					}
					bool foundSymbol = false;
					for (uint32_t i = 0x0; i < symtabCount; i++) {
						struct symtab_command *cmd = &symtabCommands[i];
						uint64_t fslide = 0x0, mslide = 0x0;
						if (libTable->libInfo->is64bit) {
							struct Seg64Data *textData = (char*)libTable->libInfo->textSeg + sizeof(struct SegmentEntry);
							struct Seg64Data *linkData = (char*)libTable->libInfo->linkSeg + sizeof(struct SegmentEntry);
							fslide = (linkData->vmaddr - textData->vmaddr) - linkData->fileoff;
							mslide = (char*)libTable->libInfo->mhOffset - textData->vmaddr;
						} else {
							struct Seg32Data *textData = (char*)libTable->libInfo->textSeg + sizeof(struct SegmentEntry);
							struct Seg32Data *linkData = (char*)libTable->libInfo->linkSeg + sizeof(struct SegmentEntry);
							fslide = (linkData->vmaddr - textData->vmaddr) - linkData->fileoff;
							mslide = (char*)libTable->libInfo->mhOffset - textData->vmaddr;
						}
						struct SymbolTableListEntry *entry = (char*)libTable->libInfo->mhOffset + cmd->symoff + fslide;
						for (uint32_t j = 0x0; j < cmd->nsyms; j++) {
							if (!(entry->n_type & N_STAB) && ((entry->n_type & N_TYPE) == N_SECT)) {
								char *strTable = (char*)libTable->libInfo->mhOffset + cmd->stroff + fslide;
								if (SMDSTSymbolDemangleAndCompare(((char *)strTable + entry->n_un.n_strx), symbolName)) {
									if (libTable->libInfo->is64bit) {
										uint64_t *n_value = ((char*)entry + sizeof(struct SymbolTableListEntry));
										symbolAddress = *n_value;
									} else {
										uint32_t *n_value = ((char*)entry + sizeof(struct SymbolTableListEntry));
										symbolAddress = *n_value;
									}
									libTable->table = realloc(libTable->table, libTable->symbolCount+0x1);
									struct MachOSymbol *newSymbol = (struct MachOSymbol *)malloc(sizeof(struct MachOSymbol));
									newSymbol->functionPointer = symbolAddress + mslide + _dyld_get_image_vmaddr_slide(libTable->libInfo->imageNumber);
									newSymbol->symbolName = malloc(strlen(symbolName)+1);
									strcpy(newSymbol->symbolName, symbolName);
									libTable->table[libTable->symbolCount] = *newSymbol;
									libTable->symbolCount++;
									foundSymbol = true;
									break;
								}
							}
							entry = (char*)entry + (sizeof(struct SymbolTableListEntry) + (libTable->libInfo->is64bit ? sizeof(uint64_t) : sizeof(uint32_t)));
						}
						if (foundSymbol)
							break;
					}
					free(symtabCommands);
				}
			}
		}
	}
	return symbolAddress;
}

void SDMSTLibraryRelease(struct LibrarySymbolTable *libTable) {
	free(libTable->libInfo);
	for (uint32_t i = 0x0; i < libTable->symbolCount; i++) {
		free(libTable->table[i].symbolName);
	}
	free(libTable->table);
	dlclose(libTable->libraryHandle);
	free(libTable);
}

#endif