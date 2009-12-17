#ifndef _MULTIBOOT_H
#define _MULTIBOOT_H

#include <lib/types.h>
#include <kernel/elf32.h>
#include <kernel/ElfInformation.h>

#define MULTIBOOT_FLAG_MEM     0x001
#define MULTIBOOT_FLAG_DEVICE  0x002
#define MULTIBOOT_FLAG_CMDLINE 0x004
#define MULTIBOOT_FLAG_MODS    0x008
#define MULTIBOOT_FLAG_AOUT    0x010
#define MULTIBOOT_FLAG_ELF     0x020
#define MULTIBOOT_FLAG_MMAP    0x040
#define MULTIBOOT_FLAG_CONFIG  0x080
#define MULTIBOOT_FLAG_LOADER  0x100
#define MULTIBOOT_FLAG_APM     0x200
#define MULTIBOOT_FLAG_VBE     0x400

// multiboot info structure passed from boot loader
/*The format of the Multiboot information structure (as defined so far) follows:

             +-------------------+
     0       | flags             |    (required)
             +-------------------+
     4       | mem_lower         |    (present if flags[0] is set)
     8       | mem_upper         |    (present if flags[0] is set)
             +-------------------+
     12      | boot_device       |    (present if flags[1] is set)
             +-------------------+
     16      | cmdline           |    (present if flags[2] is set)
             +-------------------+
     20      | mods_count        |    (present if flags[3] is set)
     24      | mods_addr         |    (present if flags[3] is set)
             +-------------------+
     28 - 40 | syms              |    (present if flags[4] or
             |                   |                flags[5] is set)
             +-------------------+
     44      | mmap_length       |    (present if flags[6] is set)
     48      | mmap_addr         |    (present if flags[6] is set)
             +-------------------+
     52      | drives_length     |    (present if flags[7] is set)
     56      | drives_addr       |    (present if flags[7] is set)
             +-------------------+
     60      | config_table      |    (present if flags[8] is set)
             +-------------------+
     64      | boot_loader_name  |    (present if flags[9] is set)
             +-------------------+
     68      | apm_table         |    (present if flags[10] is set)
             +-------------------+
     72      | vbe_control_info  |    (present if flags[11] is set)
     76      | vbe_mode_info     |
     80      | vbe_mode          |
     82      | vbe_interface_seg |
     84      | vbe_interface_off |
     86      | vbe_interface_len |
             +-------------------+										*/

namespace Kernel
{

    // format of a memory region
    struct memory_region {
    	unsigned int    	size;
    	unsigned int    	startLo;
    	unsigned int    	startHi;
    	unsigned int    	sizeLo;
    	unsigned int    	sizeHi;
    	unsigned int    	type;
    }  __attribute__((packed));
    typedef struct memory_region memory_region_t;

    struct multiboot_header {
    	unsigned int    	flags;
    	unsigned int	    memoryLo;
    	unsigned int    	memoryHi;
    	unsigned int	    bootDevice;
    	unsigned int    	cmdLine;
    	unsigned int    	modsCount;
    	unsigned int    	modsAddr;
    	unsigned int    	elf_num;
    	unsigned int    	elf_size;
    	unsigned int    	elf_addr;
    	unsigned int    	elf_shndx;
    	unsigned int    	mmap_length;
    	memory_region_t*	mmap_addr;
    	unsigned int    	drives_length;
    	unsigned int    	drives_addr;
    	unsigned int    	config_table;
    	unsigned int    	bootloader_name;
    	unsigned int    	apm_table;
    	unsigned int    	vbe_control_info;
    	unsigned int    	vbe_mode_info;
    	unsigned short	    vbe_mode;
    	unsigned int    	vbe_interface_addr;
    	unsigned short  	vbe_interface_len;
    }  __attribute__((packed));

    typedef struct multiboot_header MultibootHeader;

    class Multiboot
    {
    public:
        Multiboot(MultibootHeader *h);
        
        unsigned int GetLowerMemory() { return header->memoryLo; }
        unsigned int GetUpperMemory() { return header->memoryHi; }
        char* GetKernelCommandline() { return (char *)header->cmdLine; }
        bool IsElf() { return header->flags & MULTIBOOT_FLAG_ELF; }
        
        ElfInformation *elfInfo;
        
    private:
        MultibootHeader *header;
    };

}

#endif
