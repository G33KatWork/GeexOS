
//****************************************************************************
//**
//**    gdt.c
//**
//**	global descriptor table (gdt) for i86 processors. This handles
//**	the basic memory map for the system and permission levels
//**
//****************************************************************************

#include "gdt.h"
#include <string.h>

// processor gdtr register points to base of gdt. This helps
// us set up the pointer
struct gdtr {

	//! size of gdt
	uint16_t		m_limit;

	//! base address of gdt
	uint32_t		m_base;
} __attribute__((packed));
typedef struct gdtr gdtr_t;

// *********************************** Private ***********************************

// global descriptor table is an array of descriptors
static gdt_descriptor_t			_gdt [MAX_DESCRIPTORS];

// gdtr data
static gdtr_t					_gdtr;

// install gdtr
static void gdt_install ();

// install gdtr
static void gdt_install () {
	asm("lgdt	%0" : "=m" (_gdtr));
}

// ************************************ Public ***********************************

// Setup a descriptor in the Global Descriptor Table
void gdt_set_descriptor(uint32_t i, uint64_t base, uint64_t limit, uint8_t access, uint8_t grand)
{
	if (i > MAX_DESCRIPTORS)
		return;

	//! null out the descriptor
	memset ((void*)&_gdt[i], 0, sizeof (gdt_descriptor_t));

	//! set limit and base addresses
	_gdt[i].baseLo	= (uint16_t)(base & 0xffff);
	_gdt[i].baseMid	= (uint8_t)((base >> 16) & 0xff);
	_gdt[i].baseHi	= (uint8_t)((base >> 24) & 0xff);
	_gdt[i].limit	= (uint16_t)(limit & 0xffff);

	//! set flags and grandularity bytes
	_gdt[i].flags = access;
	_gdt[i].grand = (uint8_t)((limit >> 16) & 0x0f);
	_gdt[i].grand |= grand & 0xf0;
}


// returns descriptor in gdt
gdt_descriptor_t* i86_gdt_get_descriptor (int i) {

	if (i > MAX_DESCRIPTORS)
		return 0;

	return &_gdt[i];
}


// initialize gdt
int i86_gdt_initialize () {

	//! set up gdtr
	_gdtr.m_limit = (sizeof (gdt_descriptor_t) * MAX_DESCRIPTORS)-1;
	_gdtr.m_base = (uint32_t)&_gdt[0];

	//! set null descriptor
	gdt_set_descriptor(0, 0, 0, 0, 0);

	//! set default kernel code descriptor
	gdt_set_descriptor (1,0,0xffffffff,
		I86_GDT_DESC_READWRITE|I86_GDT_DESC_EXEC_CODE|I86_GDT_DESC_CODEDATA|I86_GDT_DESC_MEMORY,
		I86_GDT_GRAND_4K | I86_GDT_GRAND_32BIT | I86_GDT_GRAND_LIMITHI_MASK);

	//! set default kernel data descriptor
	gdt_set_descriptor (2,0,0xffffffff,
		I86_GDT_DESC_READWRITE|I86_GDT_DESC_CODEDATA|I86_GDT_DESC_MEMORY,
		I86_GDT_GRAND_4K | I86_GDT_GRAND_32BIT | I86_GDT_GRAND_LIMITHI_MASK);

	//! set default user mode code descriptor
	gdt_set_descriptor (3,0,0xffffffff,
		I86_GDT_DESC_READWRITE|I86_GDT_DESC_EXEC_CODE|I86_GDT_DESC_CODEDATA|I86_GDT_DESC_DPL|I86_GDT_DESC_MEMORY,
		I86_GDT_GRAND_4K | I86_GDT_GRAND_32BIT | I86_GDT_GRAND_LIMITHI_MASK);

	//! set default user mode data descriptor
	gdt_set_descriptor (4,0,0xffffffff,
		I86_GDT_DESC_READWRITE|I86_GDT_DESC_CODEDATA|I86_GDT_DESC_DPL|I86_GDT_DESC_MEMORY,
		I86_GDT_GRAND_4K | I86_GDT_GRAND_32BIT | I86_GDT_GRAND_LIMITHI_MASK);

	//! install gdtr
	gdt_install ();

	return 0;
}

//****************************************************************************
//**
//**    END[gdt.c]
//**
//**************************************************************************** 
