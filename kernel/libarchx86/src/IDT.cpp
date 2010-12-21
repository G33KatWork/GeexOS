#include <arch/internal/IDT.h>
#include <arch/internal/GDT.h>
#include <string.h>

using namespace Arch;

struct idt_entry idt[I86_IDT_MAX_ENTRY_COUNT];
struct idt_ptr ip;


void Arch::idt_flush()
{
	asm("lidt	%0" : "=m" (ip));
}

void Arch::idt_set_gate(uint8_t i, uint32_t base, uint16_t sel, uint8_t flags)
{
	idt[i].offset_low = base & 0xFFFF;
	idt[i].offset_high = (base >> 16) & 0xFFFF;
	
	idt[i].selector = sel;
	idt[i].zero = 0;
	
	idt[i].type_attr = flags;
}

void Arch::idt_setup(void)
{
    memset(&idt, 0, sizeof(struct idt_entry) * I86_IDT_MAX_ENTRY_COUNT);
	
	idt_set_gate(0, (uint32_t)exception0, GDT_KERNEL_CODE, I86_IDT_ATTR_PRESENT | I86_IDT_ATTR_32BIT_INT_GATE | I86_IDT_ATTR_PRIV_KERNEL);
	idt_set_gate(1, (uint32_t)exception1, GDT_KERNEL_CODE, I86_IDT_ATTR_PRESENT | I86_IDT_ATTR_32BIT_INT_GATE | I86_IDT_ATTR_PRIV_KERNEL);
	idt_set_gate(2, (uint32_t)exception2, GDT_KERNEL_CODE, I86_IDT_ATTR_PRESENT | I86_IDT_ATTR_32BIT_INT_GATE | I86_IDT_ATTR_PRIV_KERNEL);
	idt_set_gate(3, (uint32_t)exception3, GDT_KERNEL_CODE, I86_IDT_ATTR_PRESENT | I86_IDT_ATTR_32BIT_INT_GATE | I86_IDT_ATTR_PRIV_KERNEL);
	idt_set_gate(4, (uint32_t)exception4, GDT_KERNEL_CODE, I86_IDT_ATTR_PRESENT | I86_IDT_ATTR_32BIT_INT_GATE | I86_IDT_ATTR_PRIV_KERNEL);
	idt_set_gate(5, (uint32_t)exception5, GDT_KERNEL_CODE, I86_IDT_ATTR_PRESENT | I86_IDT_ATTR_32BIT_INT_GATE | I86_IDT_ATTR_PRIV_KERNEL);
	idt_set_gate(6, (uint32_t)exception6, GDT_KERNEL_CODE, I86_IDT_ATTR_PRESENT | I86_IDT_ATTR_32BIT_INT_GATE | I86_IDT_ATTR_PRIV_KERNEL);
	idt_set_gate(7, (uint32_t)exception7, GDT_KERNEL_CODE, I86_IDT_ATTR_PRESENT | I86_IDT_ATTR_32BIT_INT_GATE | I86_IDT_ATTR_PRIV_KERNEL);
	idt_set_gate(8, (uint32_t)exception8, GDT_KERNEL_CODE, I86_IDT_ATTR_PRESENT | I86_IDT_ATTR_32BIT_INT_GATE | I86_IDT_ATTR_PRIV_KERNEL);
	idt_set_gate(9, (uint32_t)exception9, GDT_KERNEL_CODE, I86_IDT_ATTR_PRESENT | I86_IDT_ATTR_32BIT_INT_GATE | I86_IDT_ATTR_PRIV_KERNEL);
	idt_set_gate(10, (uint32_t)exception10, GDT_KERNEL_CODE, I86_IDT_ATTR_PRESENT | I86_IDT_ATTR_32BIT_INT_GATE | I86_IDT_ATTR_PRIV_KERNEL);
	idt_set_gate(11, (uint32_t)exception11, GDT_KERNEL_CODE, I86_IDT_ATTR_PRESENT | I86_IDT_ATTR_32BIT_INT_GATE | I86_IDT_ATTR_PRIV_KERNEL);
	idt_set_gate(12, (uint32_t)exception12, GDT_KERNEL_CODE, I86_IDT_ATTR_PRESENT | I86_IDT_ATTR_32BIT_INT_GATE | I86_IDT_ATTR_PRIV_KERNEL);
	idt_set_gate(13, (uint32_t)exception13, GDT_KERNEL_CODE, I86_IDT_ATTR_PRESENT | I86_IDT_ATTR_32BIT_INT_GATE | I86_IDT_ATTR_PRIV_KERNEL);
	idt_set_gate(14, (uint32_t)exception14, GDT_KERNEL_CODE, I86_IDT_ATTR_PRESENT | I86_IDT_ATTR_32BIT_INT_GATE | I86_IDT_ATTR_PRIV_KERNEL);
	idt_set_gate(15, (uint32_t)exception15, GDT_KERNEL_CODE, I86_IDT_ATTR_PRESENT | I86_IDT_ATTR_32BIT_INT_GATE | I86_IDT_ATTR_PRIV_KERNEL);
	idt_set_gate(16, (uint32_t)exception16, GDT_KERNEL_CODE, I86_IDT_ATTR_PRESENT | I86_IDT_ATTR_32BIT_INT_GATE | I86_IDT_ATTR_PRIV_KERNEL);
	idt_set_gate(17, (uint32_t)exception17, GDT_KERNEL_CODE, I86_IDT_ATTR_PRESENT | I86_IDT_ATTR_32BIT_INT_GATE | I86_IDT_ATTR_PRIV_KERNEL);
	idt_set_gate(18, (uint32_t)exception18, GDT_KERNEL_CODE, I86_IDT_ATTR_PRESENT | I86_IDT_ATTR_32BIT_INT_GATE | I86_IDT_ATTR_PRIV_KERNEL);
	idt_set_gate(19, (uint32_t)exception19, GDT_KERNEL_CODE, I86_IDT_ATTR_PRESENT | I86_IDT_ATTR_32BIT_INT_GATE | I86_IDT_ATTR_PRIV_KERNEL);
	idt_set_gate(20, (uint32_t)exception20, GDT_KERNEL_CODE, I86_IDT_ATTR_PRESENT | I86_IDT_ATTR_32BIT_INT_GATE | I86_IDT_ATTR_PRIV_KERNEL);
	idt_set_gate(21, (uint32_t)exception21, GDT_KERNEL_CODE, I86_IDT_ATTR_PRESENT | I86_IDT_ATTR_32BIT_INT_GATE | I86_IDT_ATTR_PRIV_KERNEL);
	idt_set_gate(22, (uint32_t)exception22, GDT_KERNEL_CODE, I86_IDT_ATTR_PRESENT | I86_IDT_ATTR_32BIT_INT_GATE | I86_IDT_ATTR_PRIV_KERNEL);
	idt_set_gate(23, (uint32_t)exception23, GDT_KERNEL_CODE, I86_IDT_ATTR_PRESENT | I86_IDT_ATTR_32BIT_INT_GATE | I86_IDT_ATTR_PRIV_KERNEL);
	idt_set_gate(24, (uint32_t)exception24, GDT_KERNEL_CODE, I86_IDT_ATTR_PRESENT | I86_IDT_ATTR_32BIT_INT_GATE | I86_IDT_ATTR_PRIV_KERNEL);
	idt_set_gate(25, (uint32_t)exception25, GDT_KERNEL_CODE, I86_IDT_ATTR_PRESENT | I86_IDT_ATTR_32BIT_INT_GATE | I86_IDT_ATTR_PRIV_KERNEL);
	idt_set_gate(26, (uint32_t)exception26, GDT_KERNEL_CODE, I86_IDT_ATTR_PRESENT | I86_IDT_ATTR_32BIT_INT_GATE | I86_IDT_ATTR_PRIV_KERNEL);
	idt_set_gate(27, (uint32_t)exception27, GDT_KERNEL_CODE, I86_IDT_ATTR_PRESENT | I86_IDT_ATTR_32BIT_INT_GATE | I86_IDT_ATTR_PRIV_KERNEL);
	idt_set_gate(28, (uint32_t)exception28, GDT_KERNEL_CODE, I86_IDT_ATTR_PRESENT | I86_IDT_ATTR_32BIT_INT_GATE | I86_IDT_ATTR_PRIV_KERNEL);
	idt_set_gate(29, (uint32_t)exception29, GDT_KERNEL_CODE, I86_IDT_ATTR_PRESENT | I86_IDT_ATTR_32BIT_INT_GATE | I86_IDT_ATTR_PRIV_KERNEL);
	idt_set_gate(30, (uint32_t)exception30, GDT_KERNEL_CODE, I86_IDT_ATTR_PRESENT | I86_IDT_ATTR_32BIT_INT_GATE | I86_IDT_ATTR_PRIV_KERNEL);
	idt_set_gate(31, (uint32_t)exception31, GDT_KERNEL_CODE, I86_IDT_ATTR_PRESENT | I86_IDT_ATTR_32BIT_INT_GATE | I86_IDT_ATTR_PRIV_KERNEL);
	
	
	
	idt_set_gate(32, (uint32_t)irq32, GDT_KERNEL_CODE, I86_IDT_ATTR_PRESENT | I86_IDT_ATTR_32BIT_INT_GATE | I86_IDT_ATTR_PRIV_KERNEL);
	idt_set_gate(33, (uint32_t)irq33, GDT_KERNEL_CODE, I86_IDT_ATTR_PRESENT | I86_IDT_ATTR_32BIT_INT_GATE | I86_IDT_ATTR_PRIV_KERNEL);
	idt_set_gate(34, (uint32_t)irq34, GDT_KERNEL_CODE, I86_IDT_ATTR_PRESENT | I86_IDT_ATTR_32BIT_INT_GATE | I86_IDT_ATTR_PRIV_KERNEL);
	idt_set_gate(35, (uint32_t)irq35, GDT_KERNEL_CODE, I86_IDT_ATTR_PRESENT | I86_IDT_ATTR_32BIT_INT_GATE | I86_IDT_ATTR_PRIV_KERNEL);
	idt_set_gate(36, (uint32_t)irq36, GDT_KERNEL_CODE, I86_IDT_ATTR_PRESENT | I86_IDT_ATTR_32BIT_INT_GATE | I86_IDT_ATTR_PRIV_KERNEL);
	idt_set_gate(37, (uint32_t)irq37, GDT_KERNEL_CODE, I86_IDT_ATTR_PRESENT | I86_IDT_ATTR_32BIT_INT_GATE | I86_IDT_ATTR_PRIV_KERNEL);
	idt_set_gate(38, (uint32_t)irq38, GDT_KERNEL_CODE, I86_IDT_ATTR_PRESENT | I86_IDT_ATTR_32BIT_INT_GATE | I86_IDT_ATTR_PRIV_KERNEL);
	idt_set_gate(39, (uint32_t)irq39, GDT_KERNEL_CODE, I86_IDT_ATTR_PRESENT | I86_IDT_ATTR_32BIT_INT_GATE | I86_IDT_ATTR_PRIV_KERNEL);
	idt_set_gate(40, (uint32_t)irq40, GDT_KERNEL_CODE, I86_IDT_ATTR_PRESENT | I86_IDT_ATTR_32BIT_INT_GATE | I86_IDT_ATTR_PRIV_KERNEL);
	idt_set_gate(41, (uint32_t)irq41, GDT_KERNEL_CODE, I86_IDT_ATTR_PRESENT | I86_IDT_ATTR_32BIT_INT_GATE | I86_IDT_ATTR_PRIV_KERNEL);
	idt_set_gate(42, (uint32_t)irq42, GDT_KERNEL_CODE, I86_IDT_ATTR_PRESENT | I86_IDT_ATTR_32BIT_INT_GATE | I86_IDT_ATTR_PRIV_KERNEL);
	idt_set_gate(43, (uint32_t)irq43, GDT_KERNEL_CODE, I86_IDT_ATTR_PRESENT | I86_IDT_ATTR_32BIT_INT_GATE | I86_IDT_ATTR_PRIV_KERNEL);
	idt_set_gate(44, (uint32_t)irq44, GDT_KERNEL_CODE, I86_IDT_ATTR_PRESENT | I86_IDT_ATTR_32BIT_INT_GATE | I86_IDT_ATTR_PRIV_KERNEL);
	idt_set_gate(45, (uint32_t)irq45, GDT_KERNEL_CODE, I86_IDT_ATTR_PRESENT | I86_IDT_ATTR_32BIT_INT_GATE | I86_IDT_ATTR_PRIV_KERNEL);
	idt_set_gate(46, (uint32_t)irq46, GDT_KERNEL_CODE, I86_IDT_ATTR_PRESENT | I86_IDT_ATTR_32BIT_INT_GATE | I86_IDT_ATTR_PRIV_KERNEL);
	idt_set_gate(47, (uint32_t)irq47, GDT_KERNEL_CODE, I86_IDT_ATTR_PRESENT | I86_IDT_ATTR_32BIT_INT_GATE | I86_IDT_ATTR_PRIV_KERNEL);
	idt_set_gate(48, (uint32_t)irq48, GDT_KERNEL_CODE, I86_IDT_ATTR_PRESENT | I86_IDT_ATTR_32BIT_INT_GATE | I86_IDT_ATTR_PRIV_KERNEL);
	idt_set_gate(49, (uint32_t)irq49, GDT_KERNEL_CODE, I86_IDT_ATTR_PRESENT | I86_IDT_ATTR_32BIT_INT_GATE | I86_IDT_ATTR_PRIV_KERNEL);
	idt_set_gate(50, (uint32_t)irq50, GDT_KERNEL_CODE, I86_IDT_ATTR_PRESENT | I86_IDT_ATTR_32BIT_INT_GATE | I86_IDT_ATTR_PRIV_KERNEL);
	idt_set_gate(51, (uint32_t)irq51, GDT_KERNEL_CODE, I86_IDT_ATTR_PRESENT | I86_IDT_ATTR_32BIT_INT_GATE | I86_IDT_ATTR_PRIV_KERNEL);
	idt_set_gate(52, (uint32_t)irq52, GDT_KERNEL_CODE, I86_IDT_ATTR_PRESENT | I86_IDT_ATTR_32BIT_INT_GATE | I86_IDT_ATTR_PRIV_KERNEL);
	idt_set_gate(53, (uint32_t)irq53, GDT_KERNEL_CODE, I86_IDT_ATTR_PRESENT | I86_IDT_ATTR_32BIT_INT_GATE | I86_IDT_ATTR_PRIV_KERNEL);
	idt_set_gate(54, (uint32_t)irq54, GDT_KERNEL_CODE, I86_IDT_ATTR_PRESENT | I86_IDT_ATTR_32BIT_INT_GATE | I86_IDT_ATTR_PRIV_KERNEL);
	idt_set_gate(55, (uint32_t)irq55, GDT_KERNEL_CODE, I86_IDT_ATTR_PRESENT | I86_IDT_ATTR_32BIT_INT_GATE | I86_IDT_ATTR_PRIV_KERNEL);
	idt_set_gate(56, (uint32_t)irq56, GDT_KERNEL_CODE, I86_IDT_ATTR_PRESENT | I86_IDT_ATTR_32BIT_INT_GATE | I86_IDT_ATTR_PRIV_KERNEL);
	idt_set_gate(57, (uint32_t)irq57, GDT_KERNEL_CODE, I86_IDT_ATTR_PRESENT | I86_IDT_ATTR_32BIT_INT_GATE | I86_IDT_ATTR_PRIV_KERNEL);
	idt_set_gate(58, (uint32_t)irq58, GDT_KERNEL_CODE, I86_IDT_ATTR_PRESENT | I86_IDT_ATTR_32BIT_INT_GATE | I86_IDT_ATTR_PRIV_KERNEL);
	idt_set_gate(59, (uint32_t)irq59, GDT_KERNEL_CODE, I86_IDT_ATTR_PRESENT | I86_IDT_ATTR_32BIT_INT_GATE | I86_IDT_ATTR_PRIV_KERNEL);
	idt_set_gate(60, (uint32_t)irq60, GDT_KERNEL_CODE, I86_IDT_ATTR_PRESENT | I86_IDT_ATTR_32BIT_INT_GATE | I86_IDT_ATTR_PRIV_KERNEL);
}

void Arch::idt_install()
{
	ip.size = sizeof(struct idt_entry) * I86_IDT_MAX_ENTRY_COUNT - 1;
	ip.offset = (uint32_t)&idt;
	
	idt_flush();
}
