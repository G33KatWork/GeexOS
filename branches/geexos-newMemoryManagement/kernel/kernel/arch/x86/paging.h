#ifndef _PAGING_H
#define _PAGING_H
//****************************************************************************
//**
//**    paging.h
//**
//****************************************************************************

#include <stdint.h>

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
 * This function maps a page into a frame
**/
void paging_map_address(uint32_t physAddr, uint32_t virtualAddr, uint16_t flags);

#endif