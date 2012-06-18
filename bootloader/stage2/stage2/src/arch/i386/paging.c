#include <arch/i386/paging.h>
#include <memory.h>
#include <print.h>
#include <lib.h>

page_directory_t* pageDirectory;

void paging_allocateNonPAE()
{
    pageDirectory = (page_directory_t*)memory_allocate(sizeof(page_directory_t), MemoryTypeGeexOSPageStructures);
    memset(pageDirectory, 0, sizeof(page_directory_t));

    debug_printf("ARCH PAGING: PD is at %x\n", pageDirectory);
}

void paging_mapVirtualMemoryNonPAE(Address physical, Address virtual, bool usermode, bool writable)
{
    uint32_t pdirIndex = (virtual >> 22) & 0x3FF;
    uint32_t ptblIndex = (virtual >> 12) & 0x3FF;
    
    //Allocate and assign new page table if necessary
    if(!((Address)(pageDirectory->page_tables[pdirIndex]) & 0x1))
    {
        debug_printf("PAGING: Allocating new page table for index 0x%x\n", pdirIndex);
        
        void* newPageTbl = memory_allocate(sizeof(page_table_t), MemoryTypeGeexOSPageStructures);
        memset(newPageTbl, 0, sizeof(page_table_t));

        pageDirectory->page_tables[pdirIndex] = (page_table_t*)((Address)newPageTbl | 3);
    }
    
    //NOTE: This only works, because paging is still switched off
    //and we are working with physical addresses here
    page_table_t* ptbl = (page_table_t*)((Address)pageDirectory->page_tables[pdirIndex] & 0xFFFFF000);
    
    ptbl->pages[ptblIndex].frame = (physical >> 12) & 0xFFFFF;
    ptbl->pages[ptblIndex].present = 1;
    ptbl->pages[ptblIndex].rw = writable ? 1 : 0;
    ptbl->pages[ptblIndex].user = usermode ? 1 : 0;
}

void paging_mapRangeNonPAE(Address physical, Address virtual, size_t len, bool usermode, bool writable)
{
    physical = PAGE_ALIGN_DOWN(physical);
    virtual = PAGE_ALIGN_DOWN(virtual);
    len = IS_PAGE_ALIGNED(len) ? len : PAGE_ALIGN(len);

    debug_printf("PAGING: Mapping from physical 0x%x to virtual 0x%x with len 0x%x\n", physical, virtual, len);

    for(Address p = physical, v = virtual; p < physical + len; p += PAGE_SIZE, v += PAGE_SIZE)
        paging_mapVirtualMemoryNonPAE(p, v, usermode, writable);
}

bool paging_isAddressPresentNonPAE(Address virtual)
{
    uint32_t pdirIndex = (virtual >> 22) & 0x3FF;
    uint32_t ptblIndex = (virtual >> 12) & 0x3FF;
    
    if(!((Address)(pageDirectory->page_tables[pdirIndex]) & 0x1))
        return false;

    page_table_t* ptbl = (page_table_t*)((Address)pageDirectory->page_tables[pdirIndex] & 0xFFFFF000);

    return ptbl->pages[ptblIndex].present == 1;
}

bool paging_isRangeFreeNonPAE(Address virtual, size_t len)
{
    virtual = PAGE_ALIGN_DOWN(virtual);
    len = IS_PAGE_ALIGNED(len) ? len : PAGE_ALIGN(len);

    debug_printf("PAGING: Checking if range from 0x%x with length 0x%x is in use\n", virtual, len);

    for(Address v = virtual; v < virtual + len; v += PAGE_SIZE)
        if(paging_isAddressPresentNonPAE(v))
        {
            debug_printf("PAGING: 0x%x is in use\n", v);
            return false;
        }

    return true;
}

Address paging_findFreeRangeNonPAE(Address base, size_t size)
{
    size_t continousFreeSpace = 0;

    for(Address curAddr = base; curAddr != 0/*implicit overflow*/; curAddr += PAGE_SIZE)
    {
        if(continousFreeSpace >= size)
            return curAddr-size;

        if(!paging_isAddressPresentNonPAE(curAddr))
            continousFreeSpace += PAGE_SIZE;
        else
            continousFreeSpace = 0;
    }
    
    return 0;
}
