#ifndef _ARCH_I386_EXCEPTION_H
#define _ARCH_I386_EXCEPTION_H

#include <types.h>

typedef struct trapframe
{
	uint32_t gs, fs, es, ds;
	uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;	// Pushed by pusha.
	uint32_t ex_no, err_code;							// Interrupt number and error code (if applicable)
	uint32_t eip, cs, eflags, useresp, ss;				// Pushed by the processor automatically.
} registers_t;

#endif
