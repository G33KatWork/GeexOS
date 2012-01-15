#ifndef _PRINT_H_
#define _PRINT_H_

#include <lib.h>

size_t   printf(const char*, ... );
size_t   snprintf(char*, size_t, const char*, ...);

size_t   vprintf(const char*, va_list);
size_t   vsnprintf(char*, size_t, const char*, va_list);

#endif
