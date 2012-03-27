#include <memory.h>
#include <arch.h>
#include <lib.h>
#include <print.h>

#define min(a, b) \
    (a < b) ? (a) : (b)

static PageNumber highestPhysicalPage, lowestPhysicalPage;
static PageLookupTableItem* pageLookupTable;

static const char* MemoryTypeNames[] = {
    "MemoryTypeFree",
    "MemoryTypeBad",
    "MemoryTypeSpecial",
    "MemoryTypeLoaderExecutable",
    "MemoryTypeLoaderTemporary",
    "MemoryTypeLoaderStack",
    "MemoryTypeFirmware"
};

uint32_t memory_add_map_entry(MemoryDescriptor* map, uint32_t maxEntries, PageNumber base, PageNumber size, MemoryType type)
{
    uint32_t i, c;
    
    //printf("Adding to map. Base: 0x%x Size: 0x%x Type: %s\r\n", base, size, MemoryTypeNames[type]);
    
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

void memory_print_map(MemoryDescriptor* map)
{
    for(MemoryDescriptor* cur = map; cur->PageCount > 0; cur++)
        printf("Start: 0x%x Size: 0x%x Type: %s\r\n", cur->BasePage, cur->PageCount, MemoryTypeNames[cur->Type]);
}

void memory_init()
{
    PageNumber usablePages = memory_count_usable_pages(FirmwareMemoryMap);
    printf("MM: Having %d usable physical pages\n", usablePages);
    void* tableLocation = memory_find_page_lookup_table_location(usablePages, FirmwareMemoryMap);
    printf("MM: Page lookup table location: 0x%x\n", tableLocation);
    pageLookupTable = (PageLookupTableItem*)tableLocation;
    
    memory_mark_pages(lowestPhysicalPage, usablePages, MemoryTypeFirmware);
    
    memory_print_alloc_map();
}

PageNumber memory_count_usable_pages(MemoryDescriptor* map)
{
    //Find upper and lower memory boundaries
    //Holes are not considered here
    highestPhysicalPage = lowestPhysicalPage = 0;
    
    for(MemoryDescriptor* cur = map; cur->PageCount > 0; cur++)
    {
        if(cur->BasePage + cur->PageCount > highestPhysicalPage)
            if(cur->Type == MemoryTypeFree)
                highestPhysicalPage = cur->BasePage + cur->PageCount;
        
        if(cur->BasePage < lowestPhysicalPage)
            lowestPhysicalPage = cur->BasePage;
    }
    
    return highestPhysicalPage - lowestPhysicalPage;
}

void* memory_find_page_lookup_table_location(PageNumber totalPageCount, MemoryDescriptor* map)
{
    //Search in memory for uppermost region which is big enough to hold array of all available pages
    
    size_t pageSize = arch_get_page_size();
    
    void* curAddr = NULL;
    PageNumber curStart = 0;
    
    size_t totalTableSize = totalPageCount * sizeof(PageLookupTableItem);
    PageNumber totalTableSizeInPages = totalTableSize / pageSize;            //FIXME: align up to pagesize?
    
    for(MemoryDescriptor* cur = map; cur->PageCount > 0; cur++)
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
    start -= lowestPhysicalPage;
    
    PageNumber i = 0;
    for(PageLookupTableItem* cur = pageLookupTable + start; i < count; i++, cur++) {
        cur->type = type;
        cur->length = type == MemoryTypeFree ? 0 : 1;
    }
}

void memory_print_alloc_map()
{
    printf("Page lookup table at 0x%x\n", pageLookupTable);
    
    for(PageNumber i = 0; i < highestPhysicalPage - lowestPhysicalPage; i++)
    {
        if(i % 32 == 0)
            printf("\n0x%x:\t", i * arch_get_page_size());
        else if(i % 4 == 0)
            printf(" ");
        
        switch(pageLookupTable[i].type)
        {
            case MemoryTypeFree:
                printf("*");
                break;
            case MemoryTypeBad:
                printf("-");
                break;
            case MemoryTypeSpecial:
                printf("S");
                break;
            case MemoryTypeLoaderExecutable:
                printf("X");
                break;
            case MemoryTypeLoaderTemporary:
                printf("T");
                break;
            case MemoryTypeLoaderStack:
                printf("S");
                break;
            case MemoryTypeFirmware:
                printf("F");
                break;
            default:
                printf("?");
                break;
        }
    }
}



