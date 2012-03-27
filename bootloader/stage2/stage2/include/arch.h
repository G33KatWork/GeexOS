#ifndef _ARCH_H_
#define _ARCH_H_

#include <types.h>

void arch_machine_setup(void);
void arch_dbg_print(char* str);
size_t arch_get_page_size(void);

#endif
