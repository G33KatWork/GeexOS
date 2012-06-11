#ifndef _GXLDR_MEMORY_H_
#define _GXLDR_MEMORY_H_

#include <types.h>
#include <arch.h>

#define PAGENUM(addr)           ((PageNumber)(((Address)addr) / arch_get_page_size()))
#define PAGEALIGN_DOWN(addr)    (((Address)addr) & ~((arch_get_page_size()) - 1))
#define PAGEALIGN_UP(addr)      PAGEALIGN_DOWN(((Address)addr) + (arch_get_page_size()) - 1)

typedef enum {
    MemoryTypeFree,
    MemoryTypeBad,
    MemoryTypeSpecial,
    MemoryTypeLoaderExecutable,
    MemoryTypeLoaderTemporary,
    MemoryTypeLoaderStack,
    MemoryTypeLoaderHeap,
    MemoryTypeFirmware,
    MemoryTypePageLookupTable,
    MemoryTypeGeexOSPageStructures,
    MemoryTypeGeexOSKernelEnvironmentInformation,
    MemoryTypeGeexOSKernelExecutable,
    MemoryTypeGeexOSKernelStack,
    MemoryTypeGeexOSKernelLibrary
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
PageNumber memory_find_available_pages(PageNumber count);
void* memory_allocate(size_t s, MemoryType type);

void memory_print_alloc_map(void);


#endif
