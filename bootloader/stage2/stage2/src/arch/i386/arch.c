#include <arch.h>
#include <arch/i386/gdt.h>
#include <arch/i386/idt.h>
#include <arch/i386/print.h>

void arch_machine_setup()
{
    print_i386_clear();
    gdt_init();
    idt_init();
}

void arch_dbg_print(char* str)
{
    print_i386_string(str);
}
