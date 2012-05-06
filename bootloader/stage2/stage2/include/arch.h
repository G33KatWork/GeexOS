#ifndef _ARCH_H_
#define _ARCH_H_

#include <types.h>

void arch_machine_setup(void);
void arch_disksystem_setup(void);
void arch_dbg_print(char* str);
void arch_panic(const char* format, ...) __attribute__((noreturn));
size_t arch_get_page_size(void);

#endif
