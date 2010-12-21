#ifndef _ARCHX86_IDT_H
#define _ARCHX86_IDT_H

#include <types.h>
#include <arch/types.h>

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
    extern "C" void exception0(void);
    extern "C" void exception1(void);
    extern "C" void exception2(void);
    extern "C" void exception3(void);
    extern "C" void exception4(void);
    extern "C" void exception5(void);
    extern "C" void exception6(void);
    extern "C" void exception7(void);
    extern "C" void exception8(void);
    extern "C" void exception9(void);
    extern "C" void exception10(void);
    extern "C" void exception11(void);
    extern "C" void exception12(void);
    extern "C" void exception13(void);
    extern "C" void exception14(void);
    extern "C" void exception15(void);
    extern "C" void exception16(void);
    extern "C" void exception17(void);
    extern "C" void exception18(void);
    extern "C" void exception19(void);
    extern "C" void exception20(void);
    extern "C" void exception21(void);
    extern "C" void exception22(void);
    extern "C" void exception23(void);
    extern "C" void exception24(void);
    extern "C" void exception25(void);
    extern "C" void exception26(void);
    extern "C" void exception27(void);
    extern "C" void exception28(void);
    extern "C" void exception29(void);
    extern "C" void exception30(void);
    extern "C" void exception31(void);

    /**
     * exceptions for Hardware-IRQs (IRQs 32 to 47) (see interrupt.S)
    **/
    extern "C" void irq32(void);
    extern "C" void irq33(void);
    extern "C" void irq34(void);
    extern "C" void irq35(void);
    extern "C" void irq36(void);
    extern "C" void irq37(void);
    extern "C" void irq38(void);
    extern "C" void irq39(void);
    extern "C" void irq40(void);
    extern "C" void irq41(void);
    extern "C" void irq42(void);
    extern "C" void irq43(void);
    extern "C" void irq44(void);
    extern "C" void irq45(void);
    extern "C" void irq46(void);
    extern "C" void irq47(void);
    extern "C" void irq48(void);
    extern "C" void irq49(void);
    extern "C" void irq50(void);
    extern "C" void irq51(void);
    extern "C" void irq52(void);
    extern "C" void irq53(void);
    extern "C" void irq54(void);
    extern "C" void irq55(void);
    extern "C" void irq56(void);
    extern "C" void irq57(void);
    extern "C" void irq58(void);
    extern "C" void irq59(void);
    extern "C" void irq60(void);
}

#endif
