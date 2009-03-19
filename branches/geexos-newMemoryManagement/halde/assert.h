#ifndef _ASSERT_H
#define _ASSERT_H

#include <stdint.h>

#define ASSERT(b) ((b) ? (void)0 : panic_assert(__FILE__, __LINE__, #b))
void panic_assert(const char *file, unsigned line, const char *desc);

#endif
