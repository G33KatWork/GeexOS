#ifndef _GEEXLDR_ELFLOADER_H_
#define _GEEXLDR_ELFLOADER_H_

#include <exeformats/loader.h>
#include <exeformats/elf.h>
#include <memory.h>

bool elf_loadFile(const char* filename, MemoryType memType, LoadedImage** imageInformation);

#endif
