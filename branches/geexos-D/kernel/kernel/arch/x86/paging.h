#ifndef _PAGING_H
#define _PAGING_H
//****************************************************************************
//**
//**    paging.h
//**
//****************************************************************************

#include <stdint.h>

/**
 * Position in virtual memory where we store the identity paging
**/
#define IDENTITY_POSITION   0xFFFFF000

/**
 * Size of one page
**/
#define PAGE_SIZE           0x1000

/**
 * Definition of a page entry in a page table
**/
typedef struct page
{
    uint32_t present    : 1;   // Page present in memory
    uint32_t rw         : 1;   // Read-only if clear, readwrite if set
    uint32_t user       : 1;   // Supervisor level only if clear
    uint32_t accessed   : 1;   // Has the page been accessed since last refresh?
    uint32_t dirty      : 1;   // Has the page been written to since last refresh?
    uint32_t unused     : 7;   // Amalgamation of unused and reserved bits
    uint32_t frame      : 20;  // Frame address (shifted right 12 bits)
} page_t;

/**
 * Definition of a page table
**/
typedef struct page_table
{
    page_t pages[1024];
} page_table_t;

/**
 * Definition of a page directory
 * We are not using page_table_t here as types, because
 * we would have to allocate the full 4MB for every page
 * directory. Even, if it is not fully used. We would not
 * have the advantages of space saving of the two-staged paging
**/
typedef struct page_directory
{
	uint32_t pagetables[1023];
	uint32_t identitytable;
} page_directory_t;

/**
 * This function initalises the page directory and the first
 * pagetable for our kernel to map it to 0xC00000000
**/
void init_paging(void);

/**
 * This function removes the lowest 4MB from the pagetable
 * These were only needed as long our temporary GDT was active
**/
void paging_remove_lowest4MB(void);

/**
 * This function maps a virtual address to the physical address
 * It does NOT handle the allocation of a page. If this is neccessary, the
 * caller is responsible for doing this.
 * The function does allocate a frame, if a new pagetable has to be created
 * for the given virtual address and registers it in the page directory
**/
void paging_map_address(uint32_t* physAddr, uint32_t* virtualAddr, uint16_t flags);

/**
 * This function checks wether a virtual address in the current page directory
 * has a frame mapped to it
**/
bool paging_page_has_frame(uint32_t* virtualAddr);

#endif