//****************************************************************************
//**
//**    panic.cpp
//**
//****************************************************************************

#include <hal.h>
#include <stdio.h>
#include "DebugDisplay.h"

const char* sickpc = " \
                               _______      \n\
                               |.-----.|    \n\
                               ||x . x||    \n\
                               ||_.-._||    \n\
                               `--)-(--`    \n\
                              __[=== o]___  \n\
                             |:::::::::::|\\ \n\
                             `-=========-`()\n\
                                G. O. S.\n\n";
const char* panic="Kernel panic:\n\n";

void kernel_panic (const char* fmt, ...) {

	disable ();

	DebugClrScr (0x13);
	DebugGotoXY (0,0);
	DebugSetColor (0x17);
	DebugPuts (sickpc);
	DebugPuts (panic);

	va_list		args;
	char buf[1024];
	
	va_start (args, fmt);
	vsprintf (buf, fmt, args);
	va_end (args);

	DebugPrintf ("*** STOP: %s", buf);

	for (;;);
}

