#include <memory.h>
#include <arch.h>
#include <lib.h>
#include <print.h>

#define min(a, b) \
    (a < b) ? (a) : (b)

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
    for(MemoryDescriptor cur = *map; cur.PageCount > 0;)
    {
        printf("Start: 0x%x Size: 0x%x Type: %s\r\n", cur.BasePage, cur.PageCount, MemoryTypeNames[cur.Type]);
        map++;
        cur = *map;
    }
}

