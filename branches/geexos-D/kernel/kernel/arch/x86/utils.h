#ifndef _UTILS_H
#define _UTILS_H

#include <stdint.h>

/**
 * get cpu vender
**/
char* get_cpu_vendor (void);

/**
 * read a byte from the given portid
**/
uint8_t inportb (uint16_t portid);

/**
 * write a given byte to a given port
**/
void outportb (uint16_t portid, uint8_t value);

#endif