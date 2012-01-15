#ifndef _ARCH_IDT_H
#define _ARCH_IDT_H

#include <types.h>

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

struct idt_entry
{
	uint16_t offset_low;
	uint16_t selector;
	uint8_t zero;
	uint8_t type_attr;
	uint16_t offset_high;
} __attribute__((packed));

struct idt_ptr
{
	uint16_t size;
	uint32_t offset;
} __attribute__((packed));

void idt_init(void);

//Exception handler
void ex0(void);
void ex1(void);
void ex2(void);
void ex3(void);
void ex4(void);
void ex5(void);
void ex6(void);
void ex7(void);
void ex8(void);
void ex9(void);
void ex10(void);
void ex11(void);
void ex12(void);
void ex13(void);
void ex14(void);
void ex16(void);
void ex17(void);
void ex18(void);
void ex19(void);


#endif
