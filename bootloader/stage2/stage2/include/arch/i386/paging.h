#ifndef _ARCH_I386_PAGING_H
#define _ARCH_I386_PAGING_H

#include <stdint.h>
#include <stdbool.h>

#include <arch/i386/memory.h>

#define     PAGE_SIZE       4096

struct page {
    unsigned int present        : 1;    // Page present in memory
    unsigned int rw             : 1;    // Read-only if clear, readwrite if set
    unsigned int user           : 1;    // Supervisor level only if clear
    unsigned int writethrough   : 1;    // Write-Through caching or Write-Back
    unsigned int cachedisabled  : 1;    // Disable caching for this page
    unsigned int accessed       : 1;    // Has the page been accessed since last refresh?
    unsigned int dirty          : 1;    // Has the page been written to since last refresh?
    unsigned int zero           : 1;    // Must be zero
    unsigned int global         : 1;    // Don't update cache in TLB if CR3 is reset
    unsigned int unused         : 3;    // Amalgamation of unused bits
    unsigned int frame          : 20;   // Frame address (shifted right 12 bits)
}  __attribute__((packed));
typedef struct page page_t;

struct page_table {
    page_t pages[1024];
} __attribute__((packed));
typedef struct page_table page_table_t;

struct page_directory {
    page_table_t* page_tables[1024];
} __attribute__((packed));
typedef struct page_directory page_directory_t;


void paging_allocateNonPAE(void);
void paging_mapVirtualMemoryNonPAE(uint64_t physical, uint64_t virtual, bool usermode, bool writable);
void paging_mapRangeNonPAE(uint64_t physical, uint64_t virtual, size_t len, bool usermode, bool writable);
bool paging_isAddressPresentNonPAE(uint64_t virtual);
bool paging_isRangeFreeNonPAE(uint64_t virtual, size_t len);
uint64_t paging_findFreeRangeNonPAE(uint64_t base, size_t size);

extern page_directory_t* pageDirectory;

static inline void paging_set_page_directory(void* pagedirectoryPhysical)
{
    asm volatile (  "mov %0, %%eax\n"
        "mov %%eax, %%cr3\n"
        "mov %%cr0, %%eax\n"
        "orl $0x80000000, %%eax\n"
        "mov %%eax, %%cr0\n" :: "m" (pagedirectoryPhysical));
}

#endif
