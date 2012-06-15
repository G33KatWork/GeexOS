#include <arch/i386/trapframe.h>
#include <print.h>
#include <arch/i386/print.h>
#include <arch/i386/gdbstub.h>

static const char *i386ExceptionDescriptionText[] =
{
    "Exception 00: DIVIDE BY ZERO",
	"Exception 01: DEBUG EXCEPTION",
	"Exception 02: NON-MASKABLE INTERRUPT EXCEPTION",
	"Exception 03: BREAKPOINT (INT 3)",
	"Exception 04: OVERFLOW",
	"Exception 05: BOUND EXCEPTION",
	"Exception 06: INVALID OPCODE",
	"Exception 07: FPU NOT AVAILABLE",
	"Exception 08: DOUBLE FAULT",
	"Exception 09: COPROCESSOR SEGMENT OVERRUN",
	"Exception 0A: INVALID TSS",
	"Exception 0B: SEGMENT NOT PRESENT",
	"Exception 0C: STACK EXCEPTION",
	"Exception 0D: GENERAL PROTECTION FAULT",
	"Exception 0E: PAGE FAULT",
	"Exception 0F: Reserved",
	"Exception 10: COPROCESSOR ERROR",
	"Exception 11: ALIGNMENT CHECK",
	"Exception 12: MACHINE CHECK"
};

void printException(trapframe ctx);

void printException(trapframe ctx)
{
	if(gdbstub_enabled)
		gdbstub_i386_handle_exception(&ctx);

	if(ctx.ex_no == 3 || ctx.ex_no == 1)
		return;

	print_i386_setBackgroundColor(Red);
    print_i386_clear();
    
    printf("Exception occured in GXLDR:\r\n%s\r\n\r\n", i386ExceptionDescriptionText[ctx.ex_no]);
    printf("Registers:\r\n");
    
    printf("EAX: 0x%x\tESP: 0x%x\r\n", ctx.eax, ctx.esp);
    printf("EBX: 0x%x\tEBP: 0x%x\r\n", ctx.ebx, ctx.ebp);
    printf("ECX: 0x%x\tESI: 0x%x\r\n", ctx.ecx, ctx.esi);
    printf("EDX: 0x%x\tEDI: 0x%x\r\n\r\n", ctx.edx, ctx.edi);
    printf("CS:  0x%x\tEIP:  0x%x\r\n", ctx.cs, ctx.eip);
    printf("DS:  0x%x\tERROR CODE:  0x%x\r\n", ctx.ds, ctx.err_code);
    printf("ES:  0x%x\tEFLAGS:  0x%x\r\n", ctx.es, ctx.eflags);
    printf("FS:  0x%x\r\n", ctx.fs);
    printf("GS:  0x%x\r\n", ctx.gs);

    while(1)
    	asm volatile("hlt");
}
