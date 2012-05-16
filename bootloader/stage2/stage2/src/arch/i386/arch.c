#include <arch.h>
#include <lib.h>
#include <print.h>
#include <disk.h>
#include <arch/i386/gdt.h>
#include <arch/i386/idt.h>
#include <arch/i386/print.h>
#include <arch/i386/serial.h>
#include <arch/i386/memory.h>
#include <arch/i386/biosdisk.h>

void arch_machine_setup()
{
    arch_i386_serial_setup();
    print_i386_clear();
    gdt_init();
    idt_init();
    
    mem_i386_build_memory_map();
}

void arch_disksystem_setup(AddDiskDeviceCallback cb)
{
    biosdisk_i386_initialize(cb);
}

//TODO: make configurable which devices are used
void arch_dbg_print(char* str)
{
    while(*str)
    {
        print_i386_char(*str);
        arch_i386_serial_print_char(*str);
        str++;
    }
}

void arch_panic(const char* format, ...)
{
    print_i386_setBackgroundColor(Red);
    print_i386_clear();
    printf("PANIC: \n");
    
    va_list params;
    
    va_start(params, format);
    vprintf(format, params);
    va_end(params);
    
    for(;;);
}

size_t arch_get_page_size()
{
    return ARCH_PAGE_SIZE;
}

