/*
====================================================
	entry.c
		-This is the kernel entry point. This is called
		from the boot loader
====================================================
*/

#include <bootinfo.h>

extern int kmain (struct multiboot_info* bootinfo);
int entry (struct multiboot_info* bootinfo);

int entry (struct multiboot_info* bootinfo)
{
	kmain (bootinfo);

#ifdef ARCH_X86
	asm("cli");
	asm("hlt");
#endif

	for (;;) ;
}
