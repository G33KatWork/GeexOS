#include <arch/i386/paging.h>
#include <memory.h>
#include <print.h>
#include <lib.h>

page_directory_t* pageDirectory;

void paging_allocateNonPAE()
{
    pageDirectory = (page_directory_t*)memory_allocate(sizeof(page_directory_t), MemoryTypeGeexOSPageStructures);
    memset(pageDirectory, 0, sizeof(page_directory_t));

    printf("ARCH PAGING: PD is at %x\n", pageDirectory);
}

void paging_mapVirtualMemoryNonPAE(Address physical, Address virtual, bool usermode, bool writable)
{
    uint32_t pdirIndex = (virtual >> 22) & 0x3FF;
    uint32_t ptblIndex = (virtual >> 12) & 0x3FF;
    
    //Allocate and assign new page table if necessary
    if(!((Address)(pageDirectory->page_tables[pdirIndex]) & 0x1))
    {
        pageDirectory->page_tables[pdirIndex] = (page_table_t*)(((Address)(memory_allocate(sizeof(page_table_t), MemoryTypeGeexOSPageStructures))) | 3);
        memset(pageDirectory->page_tables[pdirIndex], 0, sizeof(page_table_t));
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

    printf("PAGING: Mapping from physical 0x%x to virtual 0x%x with len 0x%x\n", physical, virtual, len);

    for(Address p = physical, v = virtual; p < physical + len; p += PAGE_SIZE, v += PAGE_SIZE)
        paging_mapVirtualMemoryNonPAE(p, v, usermode, writable);
}
