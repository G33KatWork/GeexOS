#ifndef _PANIC_H
#define _PANIC_H

#include <stdint.h>

void kernel_panic (const char* fmt, ...);

#endif
