#ifndef _PAGING_H
#define _PAGING_H
//****************************************************************************
//**
//**    paging.h
//**
//****************************************************************************

#include <stdint.h>

/**
 * The page directory for 1024 pagetables
**/
unsigned long kernelpagedir[1024] __attribute__ ((aligned (4096)));

/**
 * The first pagetable which will be used by the kernel
**/
unsigned long lowpagetable[1024] __attribute__ ((aligned (4096)));

/**
 * This function initalises the page directory and the first
 * pagetable for our kernel to map it to 0xC00000000
**/
void init_paging();

#endif