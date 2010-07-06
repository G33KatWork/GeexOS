#ifndef _ARCHX86_ADDRESSLAYOUT_H
#define _ARCHX86_ADDRESSLAYOUT_H

//Some constants describing the kernel address space layout are
//defined here. Use them whenever you can!

/*
4 GiB +-------------------------+ KERNEL_IO_MEMORY_END = 0xFFFFFFFF
      |                         |
      |                         |
      | I/O Memory, ACPI Tables |   = 512MB
      |                         |
      |                         |
      |_________________________| KERNEL_IO_MEMORY_START = 0xE0000000
      |                         | KERNEL_STACK_ADDRESS = 0xE0000000
      | Kernel Stack            |   = 64KB
      |_________________________| 
      |                         |
      |                         |
      | Unused                  |
      |                         |
      |_________________________| 
      |                         | KERNEL_THREAD_STACK_REGION_END = 0xC2000000
      | Kernel thread stacks    |   = 16MB
      |_________________________| KERNEL_THREAD_STACK_REGION_START = 0xC1000000
      |                         |
      | Unused                  |
      |_________________________|
      |                         | KERNEL_PLACEMENT_END = 0xC0400000
      | Placement region        |   = 4MB
      |_________________________| KERNEL_PLACEMENT_START = 0xC0200000
      |                         | KERNEL_ELF_END = 0xC0200000
      | Kernel ELF itself       |   = max. 2MB
      |_________________________| KERNEL_ELF_START = 0xC0100000
      |                         |
      | BIOS Stuff              |   = 1MB
3 GiB |_________________________| KERNEL_BASE = 0xC0000000
      |                         |
      | Unused                  |
      |                         |
      |                         |
      |                         |
      |                         |
      |                         |
      |                         |
      |                         |
      |                         |
      |                         |
      |                         |
      |                         |
0 GiB +_________________________+ 0x00000000
*/


#define     KERNEL_BASE                         0xC0000000

#define     KERNEL_VIDEO_MEMORY                 (KERNEL_BASE + 0xB8000)

#define     BIOS_ADDRESS                        (KERNEL_BASE + 0x0)
#define     BIOS_SIZE                           0x100000

#define     KERNEL_STACK_ADDRESS                0xE0000000
#define     KERNEL_STACK_SIZE                   0x10000             //64KByte

#define     KERNEL_THREAD_STACK_REGION_START    (KERNEL_BASE + 0x1000000)
#define     KERNEL_THREAD_STACK_REGION_END      (KERNEL_BASE + 0x2000000)
#define     KERNEL_THREAD_STACK_REGION_SIZE     (KERNEL_THREAD_STACK_REGION_END - KERNEL_THREAD_STACK_REGION_START)

#define     KERNEL_IO_MEMORY_START              0xE0000000          //Uppermost 512MB are for IO Memory
#define     KERNEL_IO_MEMORY_END                0xFFFFFFFF

#define     KERNEL_PLACEMENT_END                (KERNEL_BASE + 0x400000)
#define     KERNEL_PLACEMENT_START              (KERNEL_BASE + 0x200000)
#define     KERNEL_PLACEMENT_SIZE               (KERNEL_PLACEMENT_END - KERNEL_PLACEMENT_START)

#endif
