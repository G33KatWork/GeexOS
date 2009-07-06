#ifndef _ARCH_TSS_H
#define _ARCH_TSS_H

#include <lib/types.h>

namespace Arch
{
    struct tss_entry
    {
    	u32int prev_tss;
    	u32int esp0;
    	u32int ss0;
    	u32int esp1;
    	u32int ss1;
    	u32int esp2;
    	u32int ss2;
    	u32int cr3;
    	u32int eip;
    	u32int eflags;
    	u32int eax;
    	u32int ecx;
    	u32int edx;
    	u32int ebx;
    	u32int esp;
    	u32int ebp;
    	u32int esi;
    	u32int edi;
    	u32int es;
    	u32int cs;
    	u32int ss;
    	u32int ds;
    	u32int fs;
    	u32int gs;
    	u32int ldt;
    	u16int trap;
    	u16int iomap_base;
    } __attribute__((packed));

    typedef struct tss_entry TssEntry;

    void tss_set_kernel_stack(Address stack);
}

#endif
