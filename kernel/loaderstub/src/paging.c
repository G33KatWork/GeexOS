#include <paging.h>
#include <lib.h>

page_directory_t pageDirectory __attribute__ ((aligned (4096)));
page_table_t pageTables[MAX_NUM_PAGETABLES] __attribute__ ((aligned (4096)));

uint32_t nextFreePageTable = 0;

void paging_init()
{
    //clear page directory and page table
    memset(&pageDirectory, 0, sizeof(page_directory_t));
    memset(&pageTables, 0, sizeof(page_table_t) * MAX_NUM_PAGETABLES);
    
    //assign page table to 0MB
    pageDirectory.page_tables[0] = (page_table_t*)(((Address)(&pageTables[0])) | 3);      //0MB
    
    //identity map first physical 4MB
    for (uint16_t i = 0; i < 1024; i++)
    {
        pageTables[0].pages[i].frame = ((i * PAGE_SIZE) >> 12) & 0xFFFFF;
        pageTables[0].pages[i].present = 1;
        pageTables[0].pages[i].rw = 1;
    }
    
    //mark first pagetable as used
    nextFreePageTable++;
}

void paging_map(Address physical, Address virtual, bool writable, bool usermode, bool executable)
{
    (void)executable; //suppress unused-warning
    uint32_t pdirIndex = (virtual >> 22) & 0x3FF;
    uint32_t ptblIndex = (virtual >> 12) & 0x3FF;
    
    //Allocate and assign new page table if necessary
    if(!((Address)(pageDirectory.page_tables[pdirIndex]) & 0x1))
    {
        if(nextFreePageTable >= MAX_NUM_PAGETABLES)
            panic("ERROR: No more free page tables available.");
        
        pageDirectory.page_tables[pdirIndex] = (page_table_t*)(((unsigned int)(&pageTables[nextFreePageTable++])) | 3);
    }
    
    //NOTE: This only works, because the first 4MB (where the page tables actually are)
    //are identity mapped. Otherwise we would only get a physical address out
    //of the page directory which is not necessarily the same in the virtual space!
    page_table_t* ptbl = (page_table_t*)((Address)pageDirectory.page_tables[pdirIndex] & 0xFFFFF000);
    
    ptbl->pages[ptblIndex].frame = (physical >> 12) & 0xFFFFF;
    ptbl->pages[ptblIndex].present = 1;
    ptbl->pages[ptblIndex].rw = writable ? 1 : 0;
    ptbl->pages[ptblIndex].user = usermode ? 1 : 0;
}

void paging_setflags(Address virtual, bool writable, bool usermode, bool executable)
{
    (void)executable; //suppress unused-warning
    uint32_t pdirIndex = (virtual >> 22) & 0x3FF;
    uint32_t ptblIndex = (virtual >> 12) & 0x3FF;
    
    //check if page table is present
    if(!((Address)(pageDirectory.page_tables[pdirIndex]) & 0x1))
        panic("ERROR: Can't set flags for virtual address which apparently isn't mapped because of not present page directory.");
    
    //NOTE: This only works, because the first 4MB (where the page tables actually are)
    //are identity mapped. Otherwise we would only get a physical address out
    //of the page directory which is not necessarily the same in the virtual space!
    page_table_t* ptbl = (page_table_t*)((Address)pageDirectory.page_tables[pdirIndex] & 0xFFFFF000);
    
    if(!ptbl->pages[ptblIndex].present)
        panic("ERROR: Can't set flags for virtual address which apparently isn't mapped because of not present page table.");
    
    ptbl->pages[ptblIndex].rw = writable ? 1 : 0;
    ptbl->pages[ptblIndex].user = usermode ? 1 : 0;
    
    //invalidate TLB entry
    asm __volatile__ ("invlpg %0"::"m" (*(char *)virtual));
}
