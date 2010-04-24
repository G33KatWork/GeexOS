#ifndef _ARCH_IDT_H
#define _ARCH_IDT_H

#include <lib/types.h>

// count of gdt entries
#define I86_IDT_MAX_ENTRY_COUNT			255

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

namespace Arch
{
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

    void idt_flush(void);
    void idt_set_gate(uint8_t i, uint32_t base, uint16_t sel, uint8_t flags);
    void idt_setup(void);
    void idt_install(void);
    
    
    /**
     * ISRs for Exceptions (IRQs 0 to 31) (see interrupt.S)
    **/
    extern "C" void isr0(void);
    extern "C" void isr1(void);
    extern "C" void isr2(void);
    extern "C" void isr3(void);
    extern "C" void isr4(void);
    extern "C" void isr5(void);
    extern "C" void isr6(void);
    extern "C" void isr7(void);
    extern "C" void isr8(void);
    extern "C" void isr9(void);
    extern "C" void isr10(void);
    extern "C" void isr11(void);
    extern "C" void isr12(void);
    extern "C" void isr13(void);
    extern "C" void isr14(void);
    extern "C" void isr15(void);
    extern "C" void isr16(void);
    extern "C" void isr17(void);
    extern "C" void isr18(void);
    extern "C" void isr19(void);
    extern "C" void isr20(void);
    extern "C" void isr21(void);
    extern "C" void isr22(void);
    extern "C" void isr23(void);
    extern "C" void isr24(void);
    extern "C" void isr25(void);
    extern "C" void isr26(void);
    extern "C" void isr27(void);
    extern "C" void isr28(void);
    extern "C" void isr29(void);
    extern "C" void isr30(void);
    extern "C" void isr31(void);

    /**
     * ISRs for Hardware-IRQs (IRQs 32 to 47) (see interrupt.S)
    **/
    extern "C" void irq0(void);
    extern "C" void irq1(void);
    extern "C" void irq2(void);
    extern "C" void irq3(void);
    extern "C" void irq4(void);
    extern "C" void irq5(void);
    extern "C" void irq6(void);
    extern "C" void irq7(void);
    extern "C" void irq8(void);
    extern "C" void irq9(void);
    extern "C" void irq10(void);
    extern "C" void irq11(void);
    extern "C" void irq12(void);
    extern "C" void irq13(void);
    extern "C" void irq14(void);
    extern "C" void irq15(void);
}

#endif
