#ifndef _X86EMU_X86EMUDEBUG_H
#define _X86EMU_X86EMUDEBUG_H

#ifdef INKERNEL
#include <kernel/debug.h>
#else
#include <iostream>
#include <iomanip>
using namespace std;
#define X86EMU_DEBUG_MSG(x)                 cout << "X86EMU: " << x << endl
//#define X86EMU_DEBUG_MSG(x)
#endif

#endif
