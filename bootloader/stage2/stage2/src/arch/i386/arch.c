#include <arch.h>
#include <lib.h>
#include <debug.h>
#include <print.h>
#include <disk.h>
#include <arch/i386/gdt.h>
#include <arch/i386/idt.h>
#include <arch/i386/print.h>
#include <arch/i386/serial.h>
#include <arch/i386/memory.h>
#include <arch/i386/biosdisk.h>
#include <arch/i386/paging.h>
#include <arch/i386/cpuid.h>
#include <arch/i386/gdbstub.h>

static uint16_t serial_ports[] = {0, 0x3F8, 0x2F8, 0x3E8, 0x2E8};

void arch_early_machine_setup()
{
    gdt_init();
    idt_init();
}

void arch_machine_setup()
{
    print_i386_clear();
    
    mem_i386_build_memory_map();

    debug_printf("ARCH: GXLDR machine setup done.\n");
    char cpuvendor[17] = {0};
    char cpubrand[49] = {0};

    cpuid_getVendorString(cpuvendor);
    cpuid_getBrandString(cpubrand);

    printf("Running on %s - %s\n", cpuvendor, cpubrand);
}

void arch_disksystem_setup(AddDiskDeviceCallback cb)
{
    biosdisk_i386_initialize(cb);
}

void arch_serial_setup(DBGPORT_RS232_PORTS port)
{
    if(port < 1 || port > 5)
        return;

    arch_i386_serial_setup(serial_ports[port]);
}

void arch_serial_putchar(DBGPORT_RS232_PORTS port, char c)
{
    if(port < 1 || port > 5)
        return;
    
    arch_i386_serial_put_char(serial_ports[port], c);
}

char arch_serial_getchar(DBGPORT_RS232_PORTS port)
{
    if(port < 1 || port > 5)
        return ' ';
    
    return arch_i386_serial_get_char(serial_ports[port]);
}

void arch_gdbstub_enable()
{
    gdbstub_i386_enable();
    printf("GDB stub enabled, waiting for debugger to be attached...\n");
    asm ("int $3");
}

void arch_screen_putchar(char c)
{
    print_i386_char(c);
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

void arch_initialize_virtual_memory()
{
    if(cpuid_hasfeature(CPUID_FLAG_PAE, false))
    {
        debug_printf("ARCH: CPU has PAE support\n");
        //TODO: implement PAE paging
    }

    paging_allocateNonPAE();
    
}

void arch_map_virtual_memory(Address physical, Address virtual, bool writable, bool UNUSED(executable))
{
    //TDOO: if PAE is enabled, call PAE function

    paging_mapVirtualMemoryNonPAE(physical, virtual, false, writable);
}

void arch_map_virtual_memory_range(Address physical, Address virtual, size_t len, bool writable, bool UNUSED(executable))
{
    //TDOO: if PAE is enabled, call PAE function

    paging_mapRangeNonPAE(physical, virtual, len, false, writable);
}

void arch_enable_paging()
{
    paging_set_page_directory(pageDirectory);
}
