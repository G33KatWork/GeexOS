#ifndef _GEEXLDR_PELOADER_H_
#define _GEEXLDR_PELOADER_H_

#include <exeformats/pe.h>
#include <memory.h>

bool loadPeFile(const char* filename, MemoryType memType, Address* baseAddress, size_t* totalLength);

#endif
