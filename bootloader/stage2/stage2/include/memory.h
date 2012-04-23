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
    MemoryTypeFirmware,
    MemoryTypePageLookupTable
} MemoryType;

typedef struct {
    MemoryType Type;
    PageNumber BasePage;
    PageNumber PageCount;
} FirmwareMemoryMapItem;

typedef struct {
    MemoryType type;            //Type of memory allocation
    PageNumber length;          //Continous length of allocated pages
} PageLookupTableItem;

#define MAX_MEMORY_MAP_ENTRIES 32
extern FirmwareMemoryMapItem FirmwareMemoryMap[MAX_MEMORY_MAP_ENTRIES];

uint32_t memory_add_map_entry(FirmwareMemoryMapItem* map, uint32_t maxEntries, PageNumber base, PageNumber size, MemoryType type);
void memory_print_map(FirmwareMemoryMapItem* map);

void memory_init(void);
PageNumber memory_count_usable_pages(FirmwareMemoryMapItem* map);
void* memory_find_page_lookup_table_location(PageNumber TotalPageCount, FirmwareMemoryMapItem* map);
void memory_mark_pages(PageNumber start, PageNumber count, MemoryType type);
void memory_print_alloc_map(void);

#endif
