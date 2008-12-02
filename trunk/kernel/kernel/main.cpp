/*
======================================
	main.cpp
		-kernel startup code
======================================
*/

#include "DebugDisplay.h"

int kmain () {

	int i=0x12;

	DebugClrScr (0x18);

	DebugGotoXY (4,4);
	DebugSetColor (0x17);
	DebugPrintf ("+----------------------------------------+\n");
	DebugPrintf ("|  GeexOS 32 Bit C++ Kernel Executing!   |\n");
	DebugPrintf ("+----------------------------------------+\n\n");
	DebugSetColor (0x12);

	DebugSetColor (0x12);
	DebugPrintf ("\ni as integer ........................");
	DebugPrintf ("\ni in hex ............................");

	DebugGotoXY (25,8);
	DebugSetColor (0x1F);
	DebugPrintf ("\n[%i]",i);
	DebugPrintf ("\n[0x%x]",i);

	DebugGotoXY (4,16);
	DebugSetColor (0x1F);
	DebugPrintf ("\n\nI am preparing to load... Hold on, please... :)");

	return 0;
}
