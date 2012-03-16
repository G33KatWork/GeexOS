#ifndef _GXLDR_MEMORY_H_
#define _GXLDR_MEMORY_H_

#include <types.h>

typedef enum {
    MemoryTypeFree,
    MemoryTypeBad,
    MemoryTypeSpecial,
    MemoryTypeLoaderExecutable,
    MemoryTypeLoaderTemporary,
    MemoryTypeLoaderStack,
    MemoryTypeFirmware
} MemoryType;

typedef struct {
    MemoryType Type;
    PageNumber BasePage;
    PageNumber PageCount;
} MemoryDescriptor;

#define MAX_MEMORY_MAP_ENTRIES 32
extern MemoryDescriptor MemoryMap[MAX_MEMORY_MAP_ENTRIES];

uint32_t memory_add_map_entry(MemoryDescriptor* map, uint32_t maxEntries, PageNumber base, PageNumber size, MemoryType type);
void memory_print_map(MemoryDescriptor* map);

#endif
