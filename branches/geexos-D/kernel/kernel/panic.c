#include <stdio.h>
#include "DebugDisplay.h"
#include "panic.h"

static const char* panic="KERNEL PANIC:\n";

void kernel_panic (const char* fmt, ...) {

	asm volatile("cli");

	DebugClrScr (0x13);
	DebugGotoXY (0,0);
	DebugSetColor (0x17);
	DebugPuts (panic);

	va_list		args;
	char buf[1024];
	
	va_start (args, fmt);
	vsprintf (buf, fmt, args);
	va_end (args);

	DebugPrintf ("*** STOP: %s", buf);

	//for (;;);
	asm volatile("hlt");
}