/*
=========================================================================
	main.cpp

	main kernel program
=========================================================================
*/

#include <hal.h>
#include "DebugDisplay.h"
#include "exception.h"

int kmain () {

	hal_initialize ();
	
	//! install our exception handlers
	setvect (0,(void (&)(void))divide_by_zero_fault);
	setvect (1,(void (&)(void))single_step_trap);
	setvect (2,(void (&)(void))nmi_trap);
	setvect (3,(void (&)(void))breakpoint_trap);
	setvect (4,(void (&)(void))overflow_trap);
	setvect (5,(void (&)(void))bounds_check_fault);
	setvect (6,(void (&)(void))invalid_opcode_fault);
	setvect (7,(void (&)(void))no_device_fault);
	setvect (8,(void (&)(void))double_fault_abort);
	setvect (10,(void (&)(void))invalid_tss_fault);
	setvect (11,(void (&)(void))no_segment_fault);
	setvect (12,(void (&)(void))stack_fault);
	setvect (13,(void (&)(void))general_protection_fault);
	setvect (14,(void (&)(void))page_fault);
	setvect (16,(void (&)(void))fpu_fault);
	setvect (17,(void (&)(void))alignment_check_fault);
	setvect (18,(void (&)(void))machine_check_abort);
	setvect (19,(void (&)(void))simd_fpu_fault);

	DebugClrScr (0x18);

	DebugGotoXY (0,0);
	DebugSetColor (0x70);
	DebugPrintf (" GeexOs Kernel preparing to load...                                             ");
	DebugGotoXY (0,1);
	DebugSetColor (0x19);
	DebugPrintf (" GeexOS Starting Up...\n");
	
	DebugGotoXY(0, 10);
	DebugPrintf(" Processor vendor: ");
	//DebugPrintf(get_cpu_vendor());
	
	/*for(;;) {
		DebugGotoXY (0,14);
		DebugPrintf ("Current tick count: %i", get_tick_count());
	}*/

	return 0;
}



