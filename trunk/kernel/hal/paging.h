#ifndef _PAGING_H
#define _PAGING_H
//****************************************************************************
//**
//**    paging.h
//**    - Paging support
//**
//****************************************************************************
//============================================================================
//    INTERFACE REQUIRED HEADERS
//============================================================================

#include <stdint.h>
#include "regs.h"

//============================================================================
//    INTERFACE DEFINITIONS / ENUMERATIONS / SIMPLE TYPEDEFS
//============================================================================
//============================================================================
//    INTERFACE CLASS PROTOTYPES / EXTERNAL CLASS REFERENCES
//============================================================================
//============================================================================
//    INTERFACE STRUCTURES / UTILITY CLASSES
//============================================================================

struct page
{
   unsigned present    : 1;   // Page present in memory
   unsigned rw         : 1;   // Read-only if clear, readwrite if set
   unsigned user       : 1;   // Supervisor level only if clear
   unsigned accessed   : 1;   // Has the page been accessed since last refresh?
   unsigned dirty      : 1;   // Has the page been written to since last refresh?
   unsigned unused     : 7;   // Amalgamation of unused and reserved bits
   unsigned frame      : 20;  // Frame address (shifted right 12 bits)
};

struct page_table
{
   page pages[1024];
};

struct page_directory
{
   // Array of pointers to pagetables.
   page_table *tables[1024];

   /**
	  Array of pointers to the pagetables above, but gives their
	  *physical* location, for loading into the CR3 register.
   **/
   unsigned tablesPhysical[1024];

   /**
      The physical address of tablesPhysical. This comes into play
      when we get our kernel heap allocated and the directory
      may be in a different location in virtual memory.
   **/
   unsigned physicalAddr;
};

//============================================================================
//    INTERFACE DATA DECLARATIONS
//============================================================================
//============================================================================
//    INTERFACE FUNCTION PROTOTYPES
//============================================================================

// Sets up the environment, page directories etc and enables paging.
void initialise_paging();

// Causes the specified page directory to be loaded into the CR3 register.
void switch_page_directory(page_directory *new);

/**
  Retrieves a pointer to the page required.
  If make == 1, if the page-table in which this page should
  reside isn't created, create it!
**/
page *get_page(unsigned address, int make, page_directory *dir);

// Handler for page faults.
void page_fault(registers regs); 
//============================================================================
//    INTERFACE OBJECT CLASS DEFINITIONS
//============================================================================
//============================================================================
//    INTERFACE TRAILING HEADERS
//============================================================================
//****************************************************************************
//**
//**    END paging.h
//**
//****************************************************************************
#endif
