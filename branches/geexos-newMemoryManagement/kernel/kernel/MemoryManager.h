#ifndef _MEMORYMANAGER_H
#define _MEMORYMANAGER_H

#include <stdint.h>

uint32_t kmalloc(uint32_t size);
uint32_t kmalloc_a(uint32_t size);

#endif