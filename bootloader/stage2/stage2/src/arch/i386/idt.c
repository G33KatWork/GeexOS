#include <arch/i386/idt.h>
#include <lib.h>

#define I86_IDT_ENTRY_COUNT     32

struct idt_entry idt[I86_IDT_ENTRY_COUNT];
struct idt_ptr ip;

void idt_set_gate(uint8_t i, uint32_t base, uint16_t sel, uint8_t flags);

void idt_init()
{
    ip.size = sizeof(struct idt_entry) * I86_IDT_ENTRY_COUNT - 1;
    ip.offset = (uint32_t)&idt;
    
    memset(&idt, 0, sizeof(struct idt_entry) * I86_IDT_ENTRY_COUNT);
    
    idt_set_gate(0, (uint32_t)ex0, 0x08, I86_IDT_ATTR_PRESENT | I86_IDT_ATTR_32BIT_INT_GATE | I86_IDT_ATTR_PRIV_KERNEL);
    idt_set_gate(1, (uint32_t)ex1, 0x08, I86_IDT_ATTR_PRESENT | I86_IDT_ATTR_32BIT_INT_GATE | I86_IDT_ATTR_PRIV_KERNEL);
    idt_set_gate(2, (uint32_t)ex2, 0x08, I86_IDT_ATTR_PRESENT | I86_IDT_ATTR_32BIT_INT_GATE | I86_IDT_ATTR_PRIV_KERNEL);
    idt_set_gate(3, (uint32_t)ex3, 0x08, I86_IDT_ATTR_PRESENT | I86_IDT_ATTR_32BIT_INT_GATE | I86_IDT_ATTR_PRIV_KERNEL);
    idt_set_gate(4, (uint32_t)ex4, 0x08, I86_IDT_ATTR_PRESENT | I86_IDT_ATTR_32BIT_INT_GATE | I86_IDT_ATTR_PRIV_KERNEL);
    idt_set_gate(5, (uint32_t)ex5, 0x08, I86_IDT_ATTR_PRESENT | I86_IDT_ATTR_32BIT_INT_GATE | I86_IDT_ATTR_PRIV_KERNEL);
    idt_set_gate(6, (uint32_t)ex6, 0x08, I86_IDT_ATTR_PRESENT | I86_IDT_ATTR_32BIT_INT_GATE | I86_IDT_ATTR_PRIV_KERNEL);
    idt_set_gate(7, (uint32_t)ex7, 0x08, I86_IDT_ATTR_PRESENT | I86_IDT_ATTR_32BIT_INT_GATE | I86_IDT_ATTR_PRIV_KERNEL);
    idt_set_gate(8, (uint32_t)ex8, 0x08, I86_IDT_ATTR_PRESENT | I86_IDT_ATTR_32BIT_INT_GATE | I86_IDT_ATTR_PRIV_KERNEL);
    idt_set_gate(9, (uint32_t)ex9, 0x08, I86_IDT_ATTR_PRESENT | I86_IDT_ATTR_32BIT_INT_GATE | I86_IDT_ATTR_PRIV_KERNEL);
    idt_set_gate(10, (uint32_t)ex10, 0x08, I86_IDT_ATTR_PRESENT | I86_IDT_ATTR_32BIT_INT_GATE | I86_IDT_ATTR_PRIV_KERNEL);
    idt_set_gate(11, (uint32_t)ex11, 0x08, I86_IDT_ATTR_PRESENT | I86_IDT_ATTR_32BIT_INT_GATE | I86_IDT_ATTR_PRIV_KERNEL);
    idt_set_gate(12, (uint32_t)ex12, 0x08, I86_IDT_ATTR_PRESENT | I86_IDT_ATTR_32BIT_INT_GATE | I86_IDT_ATTR_PRIV_KERNEL);
    idt_set_gate(13, (uint32_t)ex13, 0x08, I86_IDT_ATTR_PRESENT | I86_IDT_ATTR_32BIT_INT_GATE | I86_IDT_ATTR_PRIV_KERNEL);
    idt_set_gate(14, (uint32_t)ex14, 0x08, I86_IDT_ATTR_PRESENT | I86_IDT_ATTR_32BIT_INT_GATE | I86_IDT_ATTR_PRIV_KERNEL);
    idt_set_gate(16, (uint32_t)ex16, 0x08, I86_IDT_ATTR_PRESENT | I86_IDT_ATTR_32BIT_INT_GATE | I86_IDT_ATTR_PRIV_KERNEL);
    idt_set_gate(17, (uint32_t)ex17, 0x08, I86_IDT_ATTR_PRESENT | I86_IDT_ATTR_32BIT_INT_GATE | I86_IDT_ATTR_PRIV_KERNEL);
    idt_set_gate(18, (uint32_t)ex18, 0x08, I86_IDT_ATTR_PRESENT | I86_IDT_ATTR_32BIT_INT_GATE | I86_IDT_ATTR_PRIV_KERNEL);
    idt_set_gate(19, (uint32_t)ex19, 0x08, I86_IDT_ATTR_PRESENT | I86_IDT_ATTR_32BIT_INT_GATE | I86_IDT_ATTR_PRIV_KERNEL);
    
    asm("lidt	%0" : "=m" (ip));
}

void idt_set_gate(uint8_t i, uint32_t base, uint16_t sel, uint8_t flags)
{
    idt[i].offset_low = base & 0xFFFF;
    idt[i].offset_high = (base >> 16) & 0xFFFF;
    
    idt[i].selector = sel;
    idt[i].zero = 0;
    
    idt[i].type_attr = flags;
}