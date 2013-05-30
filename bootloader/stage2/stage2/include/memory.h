#ifndef _GXLDR_MEMORY_H_
#define _GXLDR_MEMORY_H_

#include <arch.h>

#define PAGENUM(addr)           (((uint64_t)addr) / arch_pagesize)
#define PAGE_MASK               (~(arch_pagesize-1ULL))
#define PAGE_OFFSET(x)          ((x) & ~PAGE_MASK)
#define PAGE_START(x)           ((x) & PAGE_MASK)
#define PAGE_END(x)             PAGE_START((x) + (arch_pagesize-1ULL))

typedef enum {
    MemoryTypeFree,
    MemoryTypeBad,
    MemoryTypeUnusable,
    MemoryTypeSpecial,
    MemoryTypeLoaderExecutable,
    MemoryTypeLoaderTemporary,
    MemoryTypeLoaderStack,
    MemoryTypeLoaderHeap,
    MemoryTypeFirmware,
    MemoryTypePageLookupTable,
    MemoryTypeGeexOSPageStructures,
    MemoryTypeGeexOSPageDirectory,
    MemoryTypeGeexOSKernelEnvironmentInformation,
    MemoryTypeGeexOSKernelExecutable,
    MemoryTypeGeexOSKernelStack,
    MemoryTypeGeexOSKernelLibrary
} MemoryType;

typedef struct {
    MemoryType Type;
    uint64_t BasePage;
    uint64_t PageCount;
} FirmwareMemoryMapItem;

typedef struct {
    MemoryType type;            //Type of memory allocation
    uint64_t length;          //Continous length of allocated pages
} PageLookupTableItem;

//Firmware memory map items
#define MAX_MEMORY_MAP_ENTRIES 32
extern FirmwareMemoryMapItem FirmwareMemoryMap[MAX_MEMORY_MAP_ENTRIES];

//Firmware memory map handling
uint32_t memory_add_map_entry(FirmwareMemoryMapItem* map, uint32_t maxEntries, uint64_t base, uint64_t size, MemoryType type);
void memory_print_map(FirmwareMemoryMapItem* map);

//Loader memory map handling
void memory_init(void);
uint64_t memory_count_usable_pages(FirmwareMemoryMapItem* map);
uintptr_t memory_find_page_lookup_table_location(uint64_t TotalPageCount, FirmwareMemoryMapItem* map);
void memory_mark_pages(uint64_t start, uint64_t count, MemoryType type);
uint64_t memory_find_available_pages(uint64_t count);
void* memory_allocate(size_t s, MemoryType type);
uint64_t memory_getHighestPhysicalPage(void);

PageLookupTableItem* memory_getMemoryMap(size_t* noEntries);

void memory_print_alloc_map(void);
const char* memory_getTypeName(MemoryType t);

#endif
