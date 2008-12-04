/*
====================================================
	entry.cpp
		-This is the kernel entry point. This is called
		from the boot loader
====================================================
*/

#include <bootinfo.h>

extern int kmain (multiboot_info* bootinfo);
extern void InitializeConstructors();
extern void Exit ();

extern "C" int entry (multiboot_info* bootinfo) {

	InitializeConstructors();
	kmain (bootinfo);
	Exit();

#ifdef ARCH_X86
	asm("cli");
	asm("hlt");
#endif

	for (;;) ;
}
