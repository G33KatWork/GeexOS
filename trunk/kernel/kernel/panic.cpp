//****************************************************************************
//**
//**    panic.cpp
//**
//****************************************************************************

#include <hal.h>
#include <stdarg.h>
#include "DebugDisplay.h"

//! something is wrong--bail out
void kernel_panic (const char* fmt, ...) {

	disable ();

	va_list		args;
	va_start (args, fmt);

// We will need a vsprintf() here. I will see if I can write
// one before the tutorial release

	va_end (args);

	const char* disclamer="We apologize, GOS has encountered a problem and has been shut down\n\
to prevent damage to your computer. Any unsaved work might be lost.\n\
We are sorry for the inconvenience this might have caused.\n\n\
Please report the following information and restart your computer.\n\
The system has been halted.\n\n";

	DebugClrScr (0x1f);
	DebugGotoXY (0,0);
	DebugSetColor (0x1f);
	DebugPuts (disclamer);

	DebugPrintf ("*** STOP: %s", fmt);

	for (;;);
}

