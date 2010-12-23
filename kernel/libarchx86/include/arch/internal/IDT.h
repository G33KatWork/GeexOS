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
    
    
    //Exceptions
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

    //Interrupts
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
    extern "C" void irq61(void);
    extern "C" void irq62(void);
    extern "C" void irq63(void);
    extern "C" void irq64(void);
    extern "C" void irq65(void);
    extern "C" void irq66(void);
    extern "C" void irq67(void);
    extern "C" void irq68(void);
    extern "C" void irq69(void);
    extern "C" void irq70(void);
    extern "C" void irq71(void);
    extern "C" void irq72(void);
    extern "C" void irq73(void);
    extern "C" void irq74(void);
    extern "C" void irq75(void);
    extern "C" void irq76(void);
    extern "C" void irq77(void);
    extern "C" void irq78(void);
    extern "C" void irq79(void);
    extern "C" void irq80(void);
    extern "C" void irq81(void);
    extern "C" void irq82(void);
    extern "C" void irq83(void);
    extern "C" void irq84(void);
    extern "C" void irq85(void);
    extern "C" void irq86(void);
    extern "C" void irq87(void);
    extern "C" void irq88(void);
    extern "C" void irq89(void);
    extern "C" void irq90(void);
    extern "C" void irq91(void);
    extern "C" void irq92(void);
    extern "C" void irq93(void);
    extern "C" void irq94(void);
    extern "C" void irq95(void);
    extern "C" void irq96(void);
    extern "C" void irq97(void);
    extern "C" void irq98(void);
    extern "C" void irq99(void);
    extern "C" void irq100(void);
    extern "C" void irq101(void);
    extern "C" void irq102(void);
    extern "C" void irq103(void);
    extern "C" void irq104(void);
    extern "C" void irq105(void);
    extern "C" void irq106(void);
    extern "C" void irq107(void);
    extern "C" void irq108(void);
    extern "C" void irq109(void);
    extern "C" void irq110(void);
    extern "C" void irq111(void);
    extern "C" void irq112(void);
    extern "C" void irq113(void);
    extern "C" void irq114(void);
    extern "C" void irq115(void);
    extern "C" void irq116(void);
    extern "C" void irq117(void);
    extern "C" void irq118(void);
    extern "C" void irq119(void);
    extern "C" void irq120(void);
    extern "C" void irq121(void);
    extern "C" void irq122(void);
    extern "C" void irq123(void);
    extern "C" void irq124(void);
    extern "C" void irq125(void);
    extern "C" void irq126(void);
    extern "C" void irq127(void);
    extern "C" void irq128(void);
    extern "C" void irq129(void);
    extern "C" void irq130(void);
    extern "C" void irq131(void);
    extern "C" void irq132(void);
    extern "C" void irq133(void);
    extern "C" void irq134(void);
    extern "C" void irq135(void);
    extern "C" void irq136(void);
    extern "C" void irq137(void);
    extern "C" void irq138(void);
    extern "C" void irq139(void);
    extern "C" void irq140(void);
    extern "C" void irq141(void);
    extern "C" void irq142(void);
    extern "C" void irq143(void);
    extern "C" void irq144(void);
    extern "C" void irq145(void);
    extern "C" void irq146(void);
    extern "C" void irq147(void);
    extern "C" void irq148(void);
    extern "C" void irq149(void);
    extern "C" void irq150(void);
    extern "C" void irq151(void);
    extern "C" void irq152(void);
    extern "C" void irq153(void);
    extern "C" void irq154(void);
    extern "C" void irq155(void);
    extern "C" void irq156(void);
    extern "C" void irq157(void);
    extern "C" void irq158(void);
    extern "C" void irq159(void);
    extern "C" void irq160(void);
    extern "C" void irq161(void);
    extern "C" void irq162(void);
    extern "C" void irq163(void);
    extern "C" void irq164(void);
    extern "C" void irq165(void);
    extern "C" void irq166(void);
    extern "C" void irq167(void);
    extern "C" void irq168(void);
    extern "C" void irq169(void);
    extern "C" void irq170(void);
    extern "C" void irq171(void);
    extern "C" void irq172(void);
    extern "C" void irq173(void);
    extern "C" void irq174(void);
    extern "C" void irq175(void);
    extern "C" void irq176(void);
    extern "C" void irq177(void);
    extern "C" void irq178(void);
    extern "C" void irq179(void);
    extern "C" void irq180(void);
    extern "C" void irq181(void);
    extern "C" void irq182(void);
    extern "C" void irq183(void);
    extern "C" void irq184(void);
    extern "C" void irq185(void);
    extern "C" void irq186(void);
    extern "C" void irq187(void);
    extern "C" void irq188(void);
    extern "C" void irq189(void);
    extern "C" void irq190(void);
    extern "C" void irq191(void);
    extern "C" void irq192(void);
    extern "C" void irq193(void);
    extern "C" void irq194(void);
    extern "C" void irq195(void);
    extern "C" void irq196(void);
    extern "C" void irq197(void);
    extern "C" void irq198(void);
    extern "C" void irq199(void);
    extern "C" void irq200(void);
    extern "C" void irq201(void);
    extern "C" void irq202(void);
    extern "C" void irq203(void);
    extern "C" void irq204(void);
    extern "C" void irq205(void);
    extern "C" void irq206(void);
    extern "C" void irq207(void);
    extern "C" void irq208(void);
    extern "C" void irq209(void);
    extern "C" void irq210(void);
    extern "C" void irq211(void);
    extern "C" void irq212(void);
    extern "C" void irq213(void);
    extern "C" void irq214(void);
    extern "C" void irq215(void);
    extern "C" void irq216(void);
    extern "C" void irq217(void);
    extern "C" void irq218(void);
    extern "C" void irq219(void);
    extern "C" void irq220(void);
    extern "C" void irq221(void);
    extern "C" void irq222(void);
    extern "C" void irq223(void);
    extern "C" void irq224(void);
    extern "C" void irq225(void);
    extern "C" void irq226(void);
    extern "C" void irq227(void);
    extern "C" void irq228(void);
    extern "C" void irq229(void);
    extern "C" void irq230(void);
    extern "C" void irq231(void);
    extern "C" void irq232(void);
    extern "C" void irq233(void);
    extern "C" void irq234(void);
    extern "C" void irq235(void);
    extern "C" void irq236(void);
    extern "C" void irq237(void);
    extern "C" void irq238(void);
    extern "C" void irq239(void);
    extern "C" void irq240(void);
    extern "C" void irq241(void);
    extern "C" void irq242(void);
    extern "C" void irq243(void);
    extern "C" void irq244(void);
    extern "C" void irq245(void);
    extern "C" void irq246(void);
    extern "C" void irq247(void);
    extern "C" void irq248(void);
    extern "C" void irq249(void);
    extern "C" void irq250(void);
    extern "C" void irq251(void);
    extern "C" void irq252(void);
    extern "C" void irq253(void);
    extern "C" void irq254(void);
    extern "C" void irq255(void);
}

#endif
