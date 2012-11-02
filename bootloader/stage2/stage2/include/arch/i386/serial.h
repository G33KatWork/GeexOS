#ifndef _SERIAL_H_
#define _SERIAL_H_

#include <stdint.h>

void arch_i386_serial_setup(uint16_t port);
void arch_i386_serial_put_char(uint16_t port, char c);
char arch_i386_serial_get_char(uint16_t port);

#endif
