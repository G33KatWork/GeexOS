#ifndef _ARCH_H_
#define _ARCH_H_

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <disk.h>
#include <debug.h>

extern size_t arch_pagesize;

void arch_early_machine_setup(void);
void arch_machine_setup(void);
void arch_disksystem_setup(AddDiskDeviceCallback cb);

void arch_panic(const char* format, ...) __attribute__((noreturn));

void arch_screen_putchar(char c);

void arch_serial_setup(DBGPORT_RS232_PORTS port);
void arch_serial_putchar(DBGPORT_RS232_PORTS port, char c);
char arch_serial_getchar(DBGPORT_RS232_PORTS port);

void arch_gdbstub_enable(void);

void arch_initialize_virtual_memory(void);
void arch_map_virtual_memory(uint64_t physical, uint64_t virtual, bool writable, bool executable);
void arch_map_virtual_memory_range(uint64_t physical, uint64_t virtual, size_t len, bool writable, bool executable);
bool arch_is_virtual_memory_range_free(uint64_t virtual, size_t len);
uint64_t arch_find_free_virtual_memory_region(uint64_t base, size_t len);
void arch_enable_paging(void);

void arch_execute_at_address_with_stack(uint32_t entrypoint, uint32_t stackpointer, void* context);

#endif
