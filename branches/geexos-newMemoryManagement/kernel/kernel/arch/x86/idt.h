#ifndef _IDT_H
#define _IDT_H

#include <stdint.h>

// count of idt entries
#define I86_IDT_MAX_ENTRY_COUNT			256

// idt descriptor type attribute flags
#define I86_IDT_ATTR_TASK_GATE			0x05		//00000101
#define I86_IDT_ATTR_16BIT_INT_GATE		0x06		//00000110
#define I86_IDT_ATTR_16BIT_TRAP_GATE	0x07		//00000111
#define I86_IDT_ATTR_32BIT_INT_GATE		0x0E		//00001110
#define I86_IDT_ATTR_32BIT_TRAP_GATE	0x0F		//00001111

#define I86_IDT_ATTR_SYSTEM_SEGMENT		0x10		//00010000

#define I86_IDT_ATTR_PRIV_KERNEL		0x00		//00000000
#define I86_IDT_ATTR_PRIV_RING1			0x20		//00100000
#define I86_IDT_ATTR_PRIV_RING2			0x40		//01000000
#define I86_IDT_ATTR_PRIV_USER			0x60		//01100000

#define I86_IDT_ATTR_PRESENT			0x80		//10000000


/**
 * struct for each entry in idt
**/
struct idt_entry
{
	uint16_t offset_low;
	uint16_t selector;
	uint8_t zero;
	uint8_t type_attr;
	uint16_t offset_high;
} __attribute__((packed));

/**
 * struct for the pointer to the idt
**/
struct idt_ptr
{
	uint16_t size;
	uint32_t offset;
} __attribute__((packed));


/**
 * all the idt entries itself
**/
struct idt_entry idt[I86_IDT_MAX_ENTRY_COUNT];

/**
 * the idt pointer itself (referenced by idt_flush() in idt.S)
**/
struct idt_ptr ip;


/**
 * activates the idt in *ip, defined in idt.S
**/
void idt_flush(void);

/**
 * sets an idt interrupt gate
**/
void idt_set_gate(uint8_t i, uint32_t base, uint16_t sel, uint8_t flags);

/**
 * loads the idt
**/
void idt_install(void);

/**
 * ISRs for IRQs 0 to 31 (see interrupt.S)
**/
extern void isr0(void);
extern void isr1(void);
extern void isr2(void);
extern void isr3(void);
extern void isr4(void);
extern void isr5(void);
extern void isr6(void);
extern void isr7(void);
extern void isr8(void);
extern void isr9(void);
extern void isr10(void);
extern void isr11(void);
extern void isr12(void);
extern void isr13(void);
extern void isr14(void);
extern void isr15(void);
extern void isr16(void);
extern void isr17(void);
extern void isr18(void);
extern void isr19(void);
extern void isr20(void);
extern void isr21(void);
extern void isr22(void);
extern void isr23(void);
extern void isr24(void);
extern void isr25(void);
extern void isr26(void);
extern void isr27(void);
extern void isr28(void);
extern void isr29(void);
extern void isr30(void);
extern void isr31(void);
extern void irq0(void);
extern void irq1(void);
extern void irq2(void);
extern void irq3(void);
extern void irq4(void);
extern void irq5(void);
extern void irq6(void);
extern void irq7(void);
extern void irq8(void);
extern void irq9(void);
extern void irq10(void);
extern void irq11(void);
extern void irq12(void);
extern void irq13(void);
extern void irq14(void);
extern void irq15(void);

#endif