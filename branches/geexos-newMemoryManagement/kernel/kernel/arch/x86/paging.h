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
 * This functions removes the lowest 4MB from the pagetable
 * These were only needed as long our temporary GDT was active
**/
void paging_remove_lowest4MB(void);

#endif