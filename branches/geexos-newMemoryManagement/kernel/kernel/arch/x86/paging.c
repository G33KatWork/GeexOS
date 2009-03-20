#include "paging.h"

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
	kernelpagedir[1023] = (unsigned long)kernelpagedir | 0x3;

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

uint32_t create_empty_pagetable(void)
{
	
}

void paging_map_address(uint32_t frameNumber, uint32_t pageNumber)
{
	uint16_t dirIndex = pageNumber / 1024;
	uint16_t tableIndex = pageNumber % 1024;
	
	uint32_t addr = frameNumber * 4096;
	
	if(kernelpagedir[dirIndex] == 0)
	{
		unsigned long *newTable = kmalloc(1024);
	}
}
