#ifndef _MULTIBOOT_H
#define _MULTIBOOT_H

#include <types.h>

#define MULTIBOOT_FLAG_MEM     0x001
#define MULTIBOOT_FLAG_DEVICE  0x002
#define MULTIBOOT_FLAG_CMDLINE 0x004
#define MULTIBOOT_FLAG_MODS    0x008
#define MULTIBOOT_FLAG_AOUT    0x010
#define MULTIBOOT_FLAG_ELF     0x020
#define MULTIBOOT_FLAG_MMAP    0x040
#define MULTIBOOT_FLAG_DRIVES  0x080
#define MULTIBOOT_FLAG_CONFIG  0x100
#define MULTIBOOT_FLAG_LOADER  0x200
#define MULTIBOOT_FLAG_APM     0x400
#define MULTIBOOT_FLAG_VBE     0x800

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
    // format of a module
    struct multiboot_module
    {
      uint32_t          modStart;
      uint32_t          modRnd;
      uint32_t          cmdLine;
      uint32_t          pad;
    }  __attribute__((packed));
    typedef struct multiboot_module multiboot_module_region_t;

    // format of a memory region
    struct multiboot_memory_region {
    	uint32_t    	size;
    	uint64_t    	addr;
    	uint64_t    	len;
    	uint32_t    	type;
    }  __attribute__((packed));
    typedef struct multiboot_memory_region multiboot_memory_region_t;
    
    // format of a drive
    struct multiboot_drive_region {
    	uint32_t    	size;
    /*	        +-------------------+
        0       | size              |
                +-------------------+
        4       | drive_number      |
                +-------------------+
        5       | drive_mode        |
                +-------------------+
        6       | drive_cylinders   |
        8       | drive_heads       |
        9       | drive_sectors     |
                +-------------------+
        10 - xx | drive_ports       |
                +-------------------+*/
    }  __attribute__((packed));
    typedef struct multiboot_drive_region multiboot_drive_region_t;
    
    // format for apm table
    struct multiboot_apm_region {
        uint16_t        version;
        uint16_t        cseg;
        uint32_t        offset;
        uint16_t        cseg_16;
        uint16_t        dseg;
        uint16_t        flags;
        uint16_t        cseg_len;
        uint16_t        cseg_16_len;
        uint16_t        dseg_len;
    }  __attribute__((packed));
    typedef struct multiboot_apm_region multiboot_apm_region_t;

    struct multiboot_info {
    	uint32_t    	            flags;
    	uint32_t	                memoryLo;
    	uint32_t    	            memoryHi;
    	uint32_t	                bootDevice;
    	char*                   	cmdLine;
    	uint32_t    	            modsCount;
    	multiboot_module_region_t*  modsAddr;
    	uint32_t    	            elf_num;
    	uint32_t    	            elf_size;
    	uint32_t    	            elf_addr;
    	uint32_t    	            elf_shndx;
    	uint32_t    	            mmap_length;
    	multiboot_memory_region_t*	mmap_addr;
    	uint32_t    	            drives_length;
    	multiboot_drive_region_t* 	drives_addr;
    	uint32_t    	            config_table;
    	uint32_t    	            bootloader_name;
    	multiboot_apm_region_t*    	apm_table;
    	uint32_t    	            vbe_control_info;
    	uint32_t    	            vbe_mode_info;
    	uint16_t	                vbe_mode;
        uint16_t                    vbe_interface_seg;
    	uint16_t    	            vbe_interface_off;
    	uint16_t      	            vbe_interface_len;
    }  __attribute__((packed));

    typedef struct multiboot_info MultibootInfo;

    class Multiboot
    {
    public:
        Multiboot(MultibootInfo *i);
        
        Address GetAddress() { return (Address)&info; }
        uint32_t GetLowerMemory() { return info.memoryLo; }
        uint32_t GetUpperMemory() { return info.memoryHi; }
        const char* GetKernelCommandline() { return cmdLine; }
        uint32_t GetELFNum() { return info.elf_num; }
        uint32_t GetELFSize() { return info.elf_size; }
        uint32_t GetELFAddress() { return info.elf_addr; }
        uint32_t GetELFshndx() { return info.elf_shndx; }
        uint32_t GetMemregionCount() { return info.mmap_length / sizeof(multiboot_memory_region_t); }
        multiboot_memory_region_t* GetMemregions() { return memregions; }
        
        bool IsElf() { return info.flags & MULTIBOOT_FLAG_ELF; }
        
    private:
        MultibootInfo info;
        char* cmdLine;
        multiboot_memory_region_t* memregions;
    };

}

#endif
