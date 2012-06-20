#include <memory.h>
#include <lib.h>
#include <print.h>
#include <debug.h>

#define min(a, b) \
    (a < b) ? (a) : (b)

static PageNumber highestPhysicalPage = 0xFFFFFFFF, lowestPhysicalPage = 0x0;
static PageLookupTableItem* pageLookupTable;

static const char* MemoryTypeNames[] = {
    "MemoryTypeFree",
    "MemoryTypeBad",
    "MemoryTypeSpecial",
    "MemoryTypeLoaderExecutable",
    "MemoryTypeLoaderTemporary",
    "MemoryTypeLoaderStack",
    "MemoryTypeLoaderHeap",
    "MemoryTypeFirmware",
    "MemoryTypePageLookupTable",
    "MemoryTypeGeexOSPageStructures",
    "MemoryTypeGeexOSKernelEnvironmentInformation",
    "MemoryTypeGeexOSKernelExecutable",
    "MemoryTypeGeexOSKernelStack",
    "MemoryTypeGeexOSKernelLibrary"
};

uint32_t memory_add_map_entry(FirmwareMemoryMapItem* map, uint32_t maxEntries, PageNumber base, PageNumber size, MemoryType type)
{
    uint32_t i, c;
    
    //debug_printf("Adding to map. Base: 0x%x Size: 0x%x Type: %s\r\n", base, size, MemoryTypeNames[type]);
    
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
        PageNumber NextBase = map[i].BasePage + map[i].PageCount;
        
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
        debug_printf("Start: 0x%x Size: 0x%x Type: %s\r\n", cur->BasePage, cur->PageCount, MemoryTypeNames[cur->Type]);
}

void memory_init()
{
    PageNumber usablePages = memory_count_usable_pages(FirmwareMemoryMap);
    debug_printf("MM: Having %d usable physical pages. highest: 0x%x, lowest: 0x%x\n", usablePages, highestPhysicalPage, lowestPhysicalPage);
    void* tableLocation = memory_find_page_lookup_table_location(usablePages, FirmwareMemoryMap);
    debug_printf("MM: Page lookup table location: 0x%x\n", tableLocation);
    pageLookupTable = (PageLookupTableItem*)tableLocation;
    
    memory_mark_pages(lowestPhysicalPage, usablePages, MemoryTypeFirmware);
    
    for(FirmwareMemoryMapItem* cur = FirmwareMemoryMap; cur->PageCount > 0; cur++)
        //We won't allocate any special MMIO pages at the top of the address space, so we won't consider them
        if(cur->BasePage < highestPhysicalPage)
            memory_mark_pages(cur->BasePage, cur->PageCount, cur->Type);
    
    PageNumber lookupTableStartPage = PAGENUM(pageLookupTable);
    PageNumber lookupTablePageLen = PAGENUM((usablePages * sizeof(PageLookupTableItem)) + arch_get_page_size());
    debug_printf("MM: startpage: 0x%x plen: 0x%x\n", lookupTableStartPage, lookupTablePageLen);
    memory_mark_pages(lookupTableStartPage, lookupTablePageLen, MemoryTypePageLookupTable);
}

PageNumber memory_count_usable_pages(FirmwareMemoryMapItem* map)
{
    //Find upper and lower memory boundaries
    //Holes are not considered here
    highestPhysicalPage = lowestPhysicalPage = 0;
    
    for(FirmwareMemoryMapItem* cur = map; cur->PageCount > 0; cur++)
    {
        if(cur->BasePage + cur->PageCount > highestPhysicalPage)
            if(cur->Type == MemoryTypeFree)
                highestPhysicalPage = cur->BasePage + cur->PageCount;
        
        if(cur->BasePage < lowestPhysicalPage)
            lowestPhysicalPage = cur->BasePage;
    }
    
    return highestPhysicalPage - lowestPhysicalPage;
}

void* memory_find_page_lookup_table_location(PageNumber totalPageCount, FirmwareMemoryMapItem* map)
{
    //Search in memory for uppermost region which is big enough to hold array of all available pages
    
    size_t pageSize = arch_get_page_size();
    
    void* curAddr = NULL;
    PageNumber curStart = 0;
    
    size_t totalTableSize = totalPageCount * sizeof(PageLookupTableItem);
    PageNumber totalTableSizeInPages = totalTableSize / pageSize;            //FIXME: align up to pagesize?
    
    for(FirmwareMemoryMapItem* cur = map; cur->PageCount > 0; cur++)
    {
        if(cur->Type != MemoryTypeFree) continue;
        if(cur->PageCount < totalTableSizeInPages) continue;
        if(cur->BasePage < curStart) continue;

        curStart = cur->BasePage;
        curAddr = (void*)((cur->BasePage + cur->PageCount) * pageSize - totalTableSize);
    }

    return curAddr;
}

void memory_mark_pages(PageNumber start, PageNumber count, MemoryType type)
{
    if(start < lowestPhysicalPage || start + count > highestPhysicalPage)
        arch_panic(
            "MM: memory_mark_pages() start or count out of bounds:\n"
            "start: 0x%x, count: 0x%x, type: %s\n"
            "lowestPhysicalPage: 0x%x, highestPhysicalPage: 0x%x",
            start, count, type, lowestPhysicalPage, highestPhysicalPage
        );
    
    start -= lowestPhysicalPage;
    
    PageNumber i = 0;
    for(PageLookupTableItem* cur = pageLookupTable + start; i < count; i++, cur++) {
        cur->type = type;
        cur->length = type == MemoryTypeFree ? 0 : 1;
    }
}

PageNumber memory_find_available_pages(PageNumber count)
{
    PageNumber currentFree = 0;

    //FIXME: slow, save hints and search from there... otherwise, we are just a fscking bootloader
    for(PageNumber i = 0; i < highestPhysicalPage - lowestPhysicalPage; i++)
    {
        if(pageLookupTable[i].type == MemoryTypeFree)
            currentFree++;
        else
            currentFree = 0;

        if(currentFree >= count)
            return lowestPhysicalPage + (i - currentFree + 1);
    }

    arch_panic("MM: memory_find_available_pages() tried to find 0x%x free pages, can't find enough", count);
}

void* memory_allocate(size_t s, MemoryType type)
{
    if(s <= 0)
        arch_panic("MM: memory_allocate() size was <= 0");

    PageNumber pagesNeeded = PAGEALIGN_UP(s) / arch_get_page_size();
    //printf("%x pages needed for memalloc\n", pagesNeeded);

    PageNumber firstFreePage = memory_find_available_pages(pagesNeeded);
    //printf("allocating 0x%x pages beginning at 0x%x\n", pagesNeeded, firstFreePage);

    memory_mark_pages(firstFreePage, pagesNeeded, type);

    return (void*)(firstFreePage * arch_get_page_size());
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
    
    for(PageNumber i = 0; i < highestPhysicalPage - lowestPhysicalPage; i++)
    {
        if(i % 32 == 0)
            debug_printf("\n0x%x:\t", i * arch_get_page_size());
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


