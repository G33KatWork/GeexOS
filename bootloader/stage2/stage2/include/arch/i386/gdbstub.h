#ifndef _GXLDR_GDBSTUB_H_
#define _GXLDR_GDBSTUB_H_

#include <arch/i386/trapframe.h>

#include <stdbool.h>

extern bool gdbstub_enabled;

void gdbstub_i386_enable(void);
void gdbstub_i386_handle_exception(trapframe* ctx);

#endif
