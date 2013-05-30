#include <memory.h>
#include <lib.h>
#include <print.h>
#include <debug.h>

//FIXME: the whole memory management is designed for continous memory -> NUMA is not possible
//FIXME: memory manager runs into an endless loop if mmio space collides with memory map
//FIXME: handle more than 4GB of memory

#define min(a, b) \
    (a < b) ? (a) : (b)

static uint64_t highestPhysicalPage = 0xFFFFFFFFFFFFFFFF, lowestPhysicalPage = 0x0;
static PageLookupTableItem* pageLookupTable;

static const char* MemoryTypeNames[] = {
    "MemoryTypeFree",
    "MemoryTypeBad",
    "MemoryTypeUnusable",
    "MemoryTypeSpecial",
    "MemoryTypeLoaderExecutable",
    "MemoryTypeLoaderTemporary",
    "MemoryTypeLoaderStack",
    "MemoryTypeLoaderHeap",
    "MemoryTypeFirmware",
    "MemoryTypePageLookupTable",
    "MemoryTypeGeexOSPageStructures",
    "MemoryTypeGeexOSPageDirectory",
    "MemoryTypeGeexOSKernelEnvironmentInformation",
    "MemoryTypeGeexOSKernelExecutable",
    "MemoryTypeGeexOSKernelStack",
    "MemoryTypeGeexOSKernelLibrary"
};

uint32_t memory_add_map_entry(FirmwareMemoryMapItem* map, uint32_t maxEntries, uint64_t base, uint64_t size, MemoryType type)
{
    uint32_t i, c;
    
    debug_printf("Adding to map. Base: 0x%X Size: 0x%X Type: %s\r\n", base, size, MemoryTypeNames[type]);
    
    //search for end of list and place to insert new entry
    //c will contain the amount of entries currently in the memory map
    //i will contain the place for insertion of the new entry
    for(i = 0, c = 0; (c < maxEntries) && (map[c].PageCount != 0); c++)
        if(map[i].BasePage + map[i].PageCount <= base) i++;
    
    //is there enough space in the table left for a new entry?
    if(c >= maxEntries) return c;
    
    //printf("Memmap has %d entries, inserting at %d\r\n", c, i);
    
    while(i < c && map[i].BasePage <= base)
    {
        uint64_t NextBase = map[i].BasePage + map[i].PageCount;
        
        if((base + size) <= NextBase) return c;
        
        size -= (NextBase - base);
        base = NextBase;
        
        i++;
    }
    
    if(i < c)
    {
        //Move the following entries down and make room for our new entry
        memmove(&map[i+1], &map[i], (c-i)*sizeof(map[0]));
        
        map[i].BasePage = base;
        map[i].PageCount = min(size, map[i+1].BasePage - base);
        map[i].Type = type;
        c++;
        
        if(size > map[i].PageCount)
            c = memory_add_map_entry(map, maxEntries, base + map[i].PageCount, size - map[i].PageCount, type);
    }
    else
    {
        //we reached the end of the list, we can just append here
        map[i].BasePage = base;
        map[i].PageCount = size;
        map[i].Type = type;
        c++;
    }
    
    return c;
}

void memory_print_map(FirmwareMemoryMapItem* map)
{
    for(FirmwareMemoryMapItem* cur = map; cur->PageCount > 0; cur++)
        debug_printf("Start: 0x%X Size: 0x%X Type: %x %s\r\n", cur->BasePage, cur->PageCount, cur->Type, MemoryTypeNames[cur->Type]);
}

void memory_init()
{
    uint64_t usablePages = memory_count_usable_pages(FirmwareMemoryMap);
    debug_printf("MM: Having 0x%X usable physical pages. highest: 0x%X, lowest: 0x%X\n", usablePages, highestPhysicalPage, lowestPhysicalPage);
    
    pageLookupTable = (PageLookupTableItem*)memory_find_page_lookup_table_location(usablePages, FirmwareMemoryMap);
    //debug_printf("MM: Page lookup table location: 0x%X\n", (uint64_t)pageLookupTable);
    
    //Mark all pages as unusable memory
    memory_mark_pages(lowestPhysicalPage, usablePages, MemoryTypeUnusable);
    
    //Now iterate over memory map and mark memory with their correct types
    for(FirmwareMemoryMapItem* cur = FirmwareMemoryMap; cur->PageCount > 0; cur++)
        //We won't allocate any special MMIO pages at the top of the address space, so we won't consider them
        if(cur->BasePage < highestPhysicalPage)
            memory_mark_pages(cur->BasePage, cur->PageCount, cur->Type);
    
    //And finally mark the page lookup table
    uint64_t lookupTableStartPage = PAGENUM(PAGE_START((uintptr_t)pageLookupTable));
    uint64_t lookupTablePageLen = PAGENUM(PAGE_END((uintptr_t)(usablePages * sizeof(PageLookupTableItem))));
    debug_printf("MM: startpage: 0x%X plen: 0x%X\n", lookupTableStartPage, lookupTablePageLen);
    memory_mark_pages(lookupTableStartPage, lookupTablePageLen, MemoryTypePageLookupTable);
}

uint64_t memory_count_usable_pages(FirmwareMemoryMapItem* map)
{
    //Find upper and lower memory boundaries
    //Holes are not considered here
    highestPhysicalPage = lowestPhysicalPage = 0;
    
    for(FirmwareMemoryMapItem* cur = map; cur->PageCount > 0; cur++)
    {
        if(cur->BasePage + cur->PageCount > highestPhysicalPage)
            if(cur->Type == MemoryTypeFree)
                highestPhysicalPage = cur->BasePage + cur->PageCount - 1;
        
        if(cur->BasePage < lowestPhysicalPage)
            lowestPhysicalPage = cur->BasePage;
    }
    
    return highestPhysicalPage - lowestPhysicalPage;
}

uintptr_t memory_find_page_lookup_table_location(uint64_t totalPageCount, FirmwareMemoryMapItem* map)
{
    //Search in memory for uppermost region which is big enough to hold array of all available pages,
    //but is lower than 4GB for compatibility with 32 bit system
    
    uint64_t totalTableSize = totalPageCount * sizeof(PageLookupTableItem);
    uint64_t totalTableSizeInPages = PAGE_END(totalTableSize) / arch_pagesize;

    debug_printf("MM: Looking for a suitbale place for the page look up table. Size needed in pages: 0x%X\n", totalTableSizeInPages);
    
    for(FirmwareMemoryMapItem* cur = map; cur->PageCount > 0; cur++)
    {
        if(cur->Type != MemoryTypeFree) continue;
        if(cur->PageCount < totalTableSizeInPages) continue;

        uint64_t loc = cur->BasePage * arch_pagesize;

        if(loc + totalTableSize > 0xFFFFFFFF)
            arch_panic("MM: Page lookup table location is bigger than 32 bit");

        return (uintptr_t)loc;
    }

    return 0;
}







void memory_mark_pages(uint64_t start, uint64_t count, MemoryType type)
{
    if(start < lowestPhysicalPage || start + count - 1 > highestPhysicalPage)
        arch_panic(
            "MM: memory_mark_pages() start or count out of bounds:\n"
            "start: 0x%X, count: 0x%X, type: %s\n"
            "lowestPhysicalPage: 0x%X, highestPhysicalPage: 0x%X",
            start, count, type, lowestPhysicalPage, highestPhysicalPage
        );
    
    start -= lowestPhysicalPage;
    
    uint64_t i = 0;
    for(PageLookupTableItem* cur = pageLookupTable + start; i < count; i++, cur++) {
        cur->type = type;
        cur->length = type == MemoryTypeFree ? 0 : 1;
    }
}

uint64_t memory_find_available_pages(uint64_t count)
{
    uint64_t currentFree = 0;

    //FIXME: slow, save hints and search from there... otherwise, we are just a fscking bootloader
    for(uint64_t i = 0; i < highestPhysicalPage - lowestPhysicalPage; i++)
    {
        if(pageLookupTable[i].type == MemoryTypeFree)
            currentFree++;
        else
            currentFree = 0;

        if(currentFree >= count)
            return lowestPhysicalPage + (i - currentFree + 1);
    }

    arch_panic("MM: memory_find_available_pages() tried to find 0x%X free pages, can't find enough", count);
}

void* memory_allocate(size_t s, MemoryType type)
{
    if(s <= 0)
        arch_panic("MM: memory_allocate() size was <= 0");

    uint64_t pagesNeeded = PAGE_START(s) / arch_pagesize;
    //printf("%x pages needed for memalloc\n", pagesNeeded);

    uint64_t firstFreePage = memory_find_available_pages(pagesNeeded);
    //printf("allocating 0x%x pages beginning at 0x%x\n", pagesNeeded, firstFreePage);

    memory_mark_pages(firstFreePage, pagesNeeded, type);

    if((firstFreePage + pagesNeeded) * arch_pagesize > 0xFFFFFFFF)
        arch_panic("MM: Trying to allocate memory at addresses bigger than 32 bit");

    return (void*)((uintptr_t)(firstFreePage * arch_pagesize));
}

uint64_t memory_getHighestPhysicalPage()
{
    return highestPhysicalPage;
}

PageLookupTableItem* memory_getMemoryMap(size_t* noEntries)
{
    if(noEntries)
        *noEntries = highestPhysicalPage - lowestPhysicalPage;

    return pageLookupTable;
}

void memory_print_alloc_map()
{
    debug_printf("Page lookup table at 0x%x\n", pageLookupTable);
    
    for(uint64_t i = 0; i < highestPhysicalPage - lowestPhysicalPage; i++)
    {
        if(i % 32 == 0)
            debug_printf("\n0x%X:\t", i * arch_pagesize);
        else if(i % 4 == 0)
            debug_printf(" ");
        
        switch(pageLookupTable[i].type)
        {
            case MemoryTypeFree:
                debug_printf("*");
                break;
            case MemoryTypeBad:
                debug_printf("-");
                break;
            case MemoryTypeUnusable:
                debug_printf("U");
                break;
            case MemoryTypeSpecial:
                debug_printf("+");
                break;
            case MemoryTypeLoaderExecutable:
                debug_printf("X");
                break;
            case MemoryTypeLoaderTemporary:
                debug_printf("T");
                break;
            case MemoryTypeLoaderStack:
                debug_printf("S");
                break;
            case MemoryTypeLoaderHeap:
                debug_printf("H");
                break;
            case MemoryTypeFirmware:
                debug_printf("F");
                break;
            case MemoryTypePageLookupTable:
                debug_printf("L");
                break;
            case MemoryTypeGeexOSPageStructures:
                debug_printf("P");
                break;
            case MemoryTypeGeexOSPageDirectory:
                debug_printf("Y");
                break;
            case MemoryTypeGeexOSKernelEnvironmentInformation:
                debug_printf("A");
                break;
            case MemoryTypeGeexOSKernelExecutable:
                debug_printf("K");
                break;
            case MemoryTypeGeexOSKernelStack:
                debug_printf("C");
                break;
            case MemoryTypeGeexOSKernelLibrary:
                debug_printf("D");
                break;
            default:
                debug_printf("?");
                break;
        }
    }
    debug_printf("\n");
}

const char* memory_getTypeName(MemoryType t)
{
    if(t >= sizeof(MemoryTypeNames) / sizeof(const char*))
        return "<<Index out of bounds>>";

    return MemoryTypeNames[t];
}
