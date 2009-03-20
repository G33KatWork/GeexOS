#ifndef _BOOTINFO_H
#define _BOOTINFO_H
//****************************************************************************
//**
//**    bootinfo.h
//**
//****************************************************************************

#include <stdint.h>

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

// format of a memory region
struct memory_region {
	uint32_t	size;
	uint32_t	startLo;
	uint32_t	startHi;
	uint32_t	sizeLo;
	uint32_t	sizeHi;
	uint32_t	type;
}  __attribute__((packed));
typedef struct memory_region memory_region_t;

struct multiboot_info {
	uint32_t	flags;
	uint32_t	mem_lower;
	uint32_t	mem_upper;
	uint32_t	boot_device;
	uint32_t	cmdline;
	uint32_t	mods_count;
	uint32_t	mods_addr;
	uint32_t	syms1;		// -|
	uint32_t	syms2;		//  |--> syms
	uint32_t	syms3;		//  |
	uint32_t	syms4;		// -|
	uint32_t	mmap_length;
	uint32_t	mmap_addr;
	uint32_t	drives_length;
	uint32_t	drives_addr;
	uint32_t	config_table;
	uint32_t	boot_loader_name;
	uint32_t	apm_table;
	uint32_t	vbe_control_info;
	uint32_t	vbe_mode_info;
	uint16_t	vbe_mode;
	uint32_t	vbe_interface_addr;
	uint16_t	vbe_interface_len;
}  __attribute__((packed));


//****************************************************************************
//**
//**    END [bootinfo.h]
//**
//****************************************************************************

#endif

