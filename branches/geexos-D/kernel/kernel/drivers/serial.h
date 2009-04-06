#ifndef _SERIAL_H
#define _SERIAL_H

#include <stdint.h>

#define COM1 0x3f8

void init_serial(uint16_t portnum, uint32_t baudrate);
int printf_serial(uint16_t portnum, const char* str, ...);
void putc_serial(uint16_t portnum, char c);
void puts_serial (uint16_t portnum, const char* str);

#endif