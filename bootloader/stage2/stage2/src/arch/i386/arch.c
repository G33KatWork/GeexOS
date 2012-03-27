#include <arch.h>
#include <arch/i386/gdt.h>
#include <arch/i386/idt.h>
#include <arch/i386/print.h>
#include <arch/i386/memory.h>

void arch_machine_setup()
{
    print_i386_clear();
    gdt_init();
    idt_init();
    
    mem_i386_build_memory_map();
}

void arch_dbg_print(char* str)
{
    print_i386_string(str);
}

size_t arch_get_page_size()
{
    return ARCH_PAGE_SIZE;
}

