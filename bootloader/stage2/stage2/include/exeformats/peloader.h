#ifndef _GEEXLDR_PELOADER_H_
#define _GEEXLDR_PELOADER_H_

#include <exeformats/loader.h>
#include <exeformats/pe.h>
#include <memory.h>
#include <list.h>

bool pe_loadFile(const char* filename, MemoryType memType, LoadedImage** imageInformation);

#endif
