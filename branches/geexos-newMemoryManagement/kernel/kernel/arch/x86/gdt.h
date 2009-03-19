#ifndef _GDT_H
#define _GDT_H
//****************************************************************************
//**
//**    gdt.h
//**
//**	global descriptor table (gdt) for i86 processors. This handles
//**	the basic memory map for the system and permission levels
//**
//****************************************************************************

#include <stdint.h>

// count of gdt entries
#define I86_GDT_ENTRY_COUNT			3

// gdt descriptor access bit flags
#define I86_GDT_DESC_ACCESS			0x0001			//00000001
#define I86_GDT_DESC_READWRITE		0x0002			//00000010
#define I86_GDT_DESC_EXPANSION		0x0004			//00000100
#define I86_GDT_DESC_EXEC_CODE		0x0008			//00001000
#define I86_GDT_DESC_CODEDATA		0x0010			//00010000
#define I86_GDT_DESC_DPL			0x0060			//01100000
#define I86_GDT_DESC_MEMORY			0x0080			//10000000

// gdt descriptor granularity bit flags
#define I86_GDT_GRAND_LIMITHI_MASK	0x0f			//00001111
#define I86_GDT_GRAND_OS			0x10			//00010000
#define I86_GDT_GRAND_32BIT			0x40			//01000000
#define I86_GDT_GRAND_4K			0x80			//10000000


/**
 * struct for each entry in gdt
**/
struct gdt_entry
{
	uint16_t limit_low;
	uint16_t base_low;
	uint8_t base_middle;
	uint8_t access;
	uint8_t granularity;
	uint8_t base_high;
} __attribute__((packed));

/**
 * struct for the pointer to the gdt
**/
struct gdt_ptr
{
	uint16_t size;
	uint32_t offset;
} __attribute__((packed));


/**
 * activates the gdt in *gp, defined in start.S
**/
extern void gdt_flush(void);

/**
 * sets a gdt entry
**/
void gdt_set_descriptor(uint16_t i, uint64_t base, uint64_t limit, uint8_t access, uint8_t grand);

/**
 * loads the gdt
**/
void gdt_install(void);

#endif