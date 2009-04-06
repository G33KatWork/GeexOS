#include "paging.h"
#include "../../FrameAllocator.h"
#include "../../DebugDisplay.h"
#include "../../MemoryManager.h"

/**
 * The first pagetable which will be used by the kernel
 * It will hold the first 4 physical MiB (0 Mib to 4 MiB)
**/
page_table_t lowpagetable __attribute__ ((aligned (4096)));

/**
 * The page directory used by the kernel
**/
page_directory_t *kernel_directory = 0;

/**
 * The page directory of the current context
**/
page_directory_t *current_directory = 0;

/**
 * This function fills the page directory and the page table,
 * then enables paging by putting the address of the page directory
 * into the CR3 register and setting the 31st bit into the CR0 one
**/
void init_paging()
{
	// Pointers to the page directory and the page table
	void *kernelpagedirPtr = 0;
	void *lowpagetablePtr = 0;
	int k = 0;

	kernel_directory = (page_directory_t*)kmalloc_a(sizeof(page_directory_t));
	current_directory = kernel_directory;
	
	kernelpagedirPtr = (uint8_t *)kernel_directory + 0x40000000;	// Translate the page directory from
																	// virtual address to physical address by provocating an overflow
																	// 0x40000000 + 0xC0000000 = 0x0
	lowpagetablePtr = (uint8_t *)(&lowpagetable) + 0x40000000;		// Same for the page table

	for (k = 0; k < 1024; k++)
	{
		lowpagetable.pages[k].frame = (k * 4096) >> 12;				// ...map the first 4MB of memory into the page table...
		lowpagetable.pages[k].present = 1;
		lowpagetable.pages[k].rw = 1;
	}
	
	for(k = 0; k < 1023; k++)
		kernel_directory->pagetables[k] = 0;								// ...and clear the page directory entries...
	kernel_directory->identitytable = ((uint32_t)kernelpagedirPtr) | 0x3;	// ...as well as setting up the identity pagetable

	// Fills the addresses 0...4MB and 3072MB...3076MB of the page directory
	// with the same page table
	kernel_directory->pagetables[0] = (uint32_t)lowpagetablePtr | 0x3;
	kernel_directory->pagetables[768] = (uint32_t)lowpagetablePtr | 0x3;

	// Copy the address of the page directory into the CR3 register and enable paging!
	asm volatile (	"mov %0, %%eax\n"
			"mov %%eax, %%cr3\n"
			"mov %%cr0, %%eax\n"
			"orl $0x80000000, %%eax\n"
			"mov %%eax, %%cr0\n" :: "m" (kernelpagedirPtr));
}

void paging_remove_lowest4MB()
{
	kernel_directory->pagetables[0] = (unsigned long)0;
}

void paging_map_address(uint32_t* physAddr, uint32_t* virtualAddr, uint16_t flags)
{
	// Make sure that both addresses are page-aligned.
	uint32_t pdindex = (uint32_t)(virtualAddr) >> 22;
	uint32_t ptindex = (uint32_t)(virtualAddr) >> 12 & 0x03FF;

	page_directory_t *pd = (page_directory_t *)IDENTITY_POSITION;

	//check if pagetable exists, if not create one
	if(pd->pagetables[pdindex] == 0)
	{
		//allocate a page for holding the table (1024*4B = 4096B = exactly one frame)
		page_table_t* physicalPageAddr = (page_table_t*) allocate_frame();
		pd->pagetables[pdindex] = (((uint32_t)physicalPageAddr) | 0x03);

		//Get a pointer for our new PT
		//see http://wiki.osdev.org/Paging#Manipulation
		page_table_t *newPT = (page_table_t *)( ((uint32_t *)0xFFC00000) + (0x400 * pdindex) );
		
		//initialize all entries in it with 0
		for(int i = 0; i < 1024; i++)
		{
			newPT->pages[i].frame = 0;
			newPT->pages[i].present = 0;
		}
	}

	//get pointer to pagetable
	//see http://wiki.osdev.org/Paging#Manipulation
	page_table_t *pt = (page_table_t *)( ((uint32_t *)0xFFC00000) + (0x400 * pdindex) );

	//check if table is already added
	if(pt->pages[ptindex].frame != 0)
		return;		//TODO: Add proper error handling

	pt->pages[ptindex].frame = (uint32_t)(physAddr) >> 12;
	pt->pages[ptindex].present = flags & 0x1;
	pt->pages[ptindex].rw = (flags & 0x2) >> 1;
	pt->pages[ptindex].user = (flags & 0x4) >> 2;
	pt->pages[ptindex].accessed = (flags & 0x8) >> 3;
	pt->pages[ptindex].dirty = (flags & 0x10) >> 4;
	pt->pages[ptindex].unused = (flags & 0x20) >> 5;

	//TODO: Invalidate page in TLB
}

bool paging_page_has_frame(uint32_t* virtualAddr)
{
    uint32_t pdindex = (uint32_t)(virtualAddr) >> 22;
	uint32_t ptindex = (uint32_t)(virtualAddr) >> 12 & 0x03FF;

    page_directory_t* pd = (page_directory_t*) IDENTITY_POSITION;
    uint8_t presentBit = (uint8_t) (pd->pagetables[pdindex] & 0x1);
    if(presentBit == 0)
        return false;

    page_table_t* pt = (page_table_t*)(((unsigned long *)0xFFC00000) + (0x400 * pdindex));
    if(pt->pages[ptindex].present == 0)
        return false;

    return true;
}

//TODO: Add function for unmapping addresses
