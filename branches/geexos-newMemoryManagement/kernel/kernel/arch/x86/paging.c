#include "paging.h"
#include "../../FrameAllocator.h"
#include "../../DebugDisplay.h"

/**
 * The page directory for 1024 pagetables
**/
unsigned long kernelpagedir[1024] __attribute__ ((aligned (4096)));

/**
 * The first pagetable which will be used by the kernel
**/
unsigned long lowpagetable[1024] __attribute__ ((aligned (4096)));


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

	kernelpagedirPtr = (char *)kernelpagedir + 0x40000000;	// Translate the page directory from
															// virtual address to physical address by provocating an overflow
															// 0x40000000 + 0xC0000000 = 0x0
	lowpagetablePtr = (char *)lowpagetable + 0x40000000;	// Same for the page table

	for (k = 0; k < 1024; k++)
	{
		lowpagetable[k] = (k * 4096) | 0x3;	// ...map the first 4MB of memory into the page table...
		kernelpagedir[k] = 0;				// ...and clear the page directory entries
	}

	// Fills the addresses 0...4MB and 3072MB...3076MB of the page directory
	// with the same page table
	kernelpagedir[0] = (unsigned long)lowpagetablePtr | 0x3;
	kernelpagedir[768] = (unsigned long)lowpagetablePtr | 0x3;

	// Let the last entry in the directory point to itself
	// 0xFFFFF000 to 0xFFFFFFFF refers to all the pages
	void *identityPtr = (char *)kernelpagedir - 0xC0000000;
	kernelpagedir[1023] = (unsigned long)identityPtr | 0x3;

	// Copies the address of the page directory into the CR3 register and, finally, enables paging!
	asm volatile (	"mov %0, %%eax\n"
			"mov %%eax, %%cr3\n"
			"mov %%cr0, %%eax\n"
			"orl $0x80000000, %%eax\n"
			"mov %%eax, %%cr0\n" :: "m" (kernelpagedirPtr));
}

void paging_remove_lowest4MB()
{
	kernelpagedir[0] = (unsigned long)0;
}

void paging_map_address(uint32_t physAddr, uint32_t virtualAddr, uint16_t flags)
{
	// Make sure that both addresses are page-aligned.
	uint32_t pdindex = virtualAddr >> 22;
	uint32_t ptindex = virtualAddr >> 12 & 0x03FF;

	uint32_t *pd = (uint32_t *)0xFFFFF000;

	//check if pagetable exists, if not create one
	if(pd[pdindex] == 0)
	{
		//allocate a page for holding the table (1024*4B = 4096B = exactly one frame)
		uint32_t* physicalPageAddr = allocate_frame();
		pd[pdindex] = (((uint32_t)physicalPageAddr) | 0x03);

		//Get a pointer for our new PT
		//see http://wiki.osdev.org/Paging#Manipulation
		uint32_t *newPT =((uint32_t *)0xFFC00000) + (0x400 * pdindex);
		for(int i = 0; i < 1024; i++) //initialize all entries in it with 0
			newPT[i] = 0;
	}

	//get pointer to pagetable
	//see http://wiki.osdev.org/Paging#Manipulation
	uint32_t *pt =((uint32_t *)0xFFC00000) + (0x400 * pdindex);

	//check if table is already added
	if(pt[ptindex] != 0)
		return;		//TODO: Add proper error handling

	pt[ptindex] = physAddr | (flags & 0xFFF) | 0x03; //Present

	//TODO: Invalidate page in TLB
}

//TODO: Add function for unmapping addresses
