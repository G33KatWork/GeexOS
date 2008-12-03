/*
====================================================
	entry.cpp
		-This is the kernel entry point. This is called
		from the boot loader
====================================================
*/

extern int kmain ();// __attribute__ ((cdecl));
extern void InitializeConstructors();// __attribute__ ((cdecl));
extern void Exit ();// __attribute__ ((cdecl));

int main () {

#ifdef ARCH_X86
	asm (
		"cli				\n\t"	// clear interrupts--Do not enable them yet
		"mov $0x10, %ax		\n\t"	// offset 0x10 in gdt for data selector, remember?
		"mov %ax, %ds		\n\t"
		"mov %ax, %es		\n\t"
		"mov %ax, %fs		\n\t"
		"mov %ax, %gs		\n\t"
		"mov %ax, %ss		\n\t"	// Set up base stack
		"mov $0x90000, %esp	\n\t"
		"mov %esp, %ebp		\n\t"	// store current stack pointer
		"push %ebp				"
		);
#endif

	InitializeConstructors();
	kmain ();
	Exit();

#ifdef ARCH_X86
	asm("cli				\n\t");
#endif

	for (;;) ;
}
