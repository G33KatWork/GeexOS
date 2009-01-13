//****************************************************************************
//**
//**    paging.cpp
//**    - Paging support
//**
//****************************************************************************
//============================================================================
//    IMPLEMENTATION HEADERS
//============================================================================

#include <string.h>
#include <hal.h>
#include "DebugDisplay.h"
#include "paging.h"
#include "kmalloc.h"

//============================================================================
//    IMPLEMENTATION PRIVATE DEFINITIONS / ENUMERATIONS / SIMPLE TYPEDEFS
//============================================================================

// Macros used in the bitset algorithms.
#define INDEX_FROM_BIT(a) (a/(8*4))
#define OFFSET_FROM_BIT(a) (a%(8*4))

//============================================================================
//    IMPLEMENTATION PRIVATE CLASS PROTOTYPES / EXTERNAL CLASS REFERENCES
//============================================================================
//============================================================================
//    IMPLEMENTATION PRIVATE STRUCTURES / UTILITY CLASSES
//============================================================================
//============================================================================
//    IMPLEMENTATION REQUIRED EXTERNAL REFERENCES (AVOID)
//============================================================================

// Defined in kmalloc.c
extern unsigned placement_address;

//============================================================================
//    IMPLEMENTATION PRIVATE DATA
//============================================================================

// The kernel's page directory
page_directory_t *kernel_directory=0;

// The current page directory;
page_directory_t *current_directory=0;

// A bitset of frames - used or free.
unsigned *frames;
unsigned nframes;

//============================================================================
//    INTERFACE DATA
//============================================================================
//============================================================================
//    IMPLEMENTATION PRIVATE FUNCTION PROTOTYPES
//============================================================================
//============================================================================
//    IMPLEMENTATION PRIVATE FUNCTIONS
//============================================================================

// Static function to set a bit in the frames bitset
void set_frame(unsigned frame_addr)
{
   unsigned frame = frame_addr/0x1000;
   unsigned idx = INDEX_FROM_BIT(frame);
   unsigned off = OFFSET_FROM_BIT(frame);
   frames[idx] |= (0x1 << off);
}

// Static function to clear a bit in the frames bitset
void clear_frame(unsigned frame_addr)
{
   unsigned frame = frame_addr/0x1000;
   unsigned idx = INDEX_FROM_BIT(frame);
   unsigned off = OFFSET_FROM_BIT(frame);
   frames[idx] &= ~(0x1 << off);
}

// Static function to test if a bit is set.
unsigned test_frame(unsigned frame_addr)
{
   unsigned frame = frame_addr/0x1000;
   unsigned idx = INDEX_FROM_BIT(frame);
   unsigned off = OFFSET_FROM_BIT(frame);
   return (frames[idx] & (0x1 << off));
}

// Static function to find the first free frame.
unsigned first_frame()
{
   unsigned i, j;
   for (i = 0; i < INDEX_FROM_BIT(nframes); i++)
   {
       if (frames[i] != 0xFFFFFFFF) // nothing free, exit early.
       {
           // at least one bit is free here.
           for (j = 0; j < 32; j++)
           {
               unsigned toTest = 0x1 << j;
               if ( !(frames[i]&toTest) )
               {
                   return i*4*8+j;
               }
           }
       }
   }

   //Make the compiler shut up
   return 0;
} 

// Function to allocate a frame.
void alloc_frame(page_t *page, int is_kernel, int is_writeable)
{
    if (page->frame != 0)
    {
        return;
    }
    else
    {
        unsigned idx = first_frame();
        if (idx == (unsigned)-1)
        {
            // PANIC! no free frames!!
        }
        set_frame(idx*0x1000);
        page->present = 1;
        page->rw = (is_writeable)?1:0;
        page->user = (is_kernel)?0:1;
        page->frame = idx;
    }
}

//============================================================================
//    INTERFACE FUNCTIONS
//============================================================================

void paging_initialize()
{
    // The size of physical memory. For the moment we 
    // assume it is 16MB big.
    unsigned mem_end_page = 0x1000000;
    
    nframes = mem_end_page / 0x1000;
    frames = (unsigned*)kmalloc(INDEX_FROM_BIT(nframes));
    memset(frames, 0, INDEX_FROM_BIT(nframes));
    
    // Let's make a page directory.
    kernel_directory = (page_directory_t*)kmalloc_a(sizeof(page_directory_t));
    current_directory = kernel_directory;

    // We need to identity map (phys addr = virt addr) from
    // 0x0 to the end of used memory, so we can access this
    // transparently, as if paging wasn't enabled.
    // NOTE that we use a while loop here deliberately.
    // inside the loop body we actually change placement_address
    // by calling kmalloc(). A while loop causes this to be
    // computed on-the-fly rather than once at the start.
    unsigned i = 0;
    while (i < placement_address)
    {
        // Kernel code is readable but not writeable from userspace.
        alloc_frame( get_page(i, 1, kernel_directory), 0, 0);
        i += 0x1000;
    }
    // Before we enable paging, we must register our page fault handler.
    setvect (14,(void (&)(void))page_fault);

    // Now, enable paging!
    switch_page_directory(kernel_directory);
}

void switch_page_directory(page_directory_t *dir)
{
    current_directory = dir;
    asm volatile("mov %0, %%cr3":: "r"(&dir->tablesPhysical));
    unsigned cr0;
    asm volatile("mov %%cr0, %0": "=r"(cr0));
    cr0 |= 0x80000000; // Enable paging!
    asm volatile("mov %0, %%cr0":: "r"(cr0));
}

page_t *get_page(unsigned address, int make, page_directory_t *dir)
{
    // Turn the address into an index.
    address /= 0x1000;
    // Find the page table containing this address.
    unsigned table_idx = address / 1024;
    if (dir->tables[table_idx]) // If this table is already assigned
    {
        return &dir->tables[table_idx]->pages[address%1024];
    }
    else if(make)
    {
        unsigned tmp;
        dir->tables[table_idx] = (page_table_t*)kmalloc_ap(sizeof(page_table_t), &tmp);
        dir->tablesPhysical[table_idx] = tmp | 0x7; // PRESENT, RW, US.
        return &dir->tables[table_idx]->pages[address%1024];
    }
    else
    {
        return 0;
    }
}

//void page_fault(void)
//{
    // A page fault has occurred.
    // The faulting address is stored in the CR2 register.
    /*unsigned faulting_address;
    asm volatile("mov %%cr2, %0" : "=r" (faulting_address));
    
    // The error code gives us details of what happened.
    int present   = !(regs.err_code & 0x1); // Page not present
    int rw = regs.err_code & 0x2;           // Write operation?
    int us = regs.err_code & 0x4;           // Processor was in user-mode?
    int reserved = regs.err_code & 0x8;     // Overwritten CPU-reserved bits of page entry?
    int id = regs.err_code & 0x10;          // Caused by an instruction fetch?

    // Output an error message.
    DebugPrintf("Page fault! ( ");
    if (present) {DebugPrintf("present ");}
    if (rw) {DebugPrintf("read-only ");}
    if (us) {DebugPrintf("user-mode ");}
    if (reserved) {DebugPrintf("reserved ");}
    DebugPrintf(") at 0x");
    DebugPrintf(faulting_address);
    DebugPrintf("\n");*/
/*    DebugPrintf("Page fault");
	for(;;) ;
    //TODO add real panic support
}*/

//============================================================================
//    INTERFACE CLASS BODIES
//============================================================================
//****************************************************************************
//**
//**    END[paging.cpp]
//**
//****************************************************************************
