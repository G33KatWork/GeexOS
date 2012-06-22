//#include <kernel/global.h>
#include <string.h>
#include <cxxabi.h>

extern "C" void atexit(void* f)
{
    //FIXME: implement...    
}

//C++ runtime shit
//See: http://wiki.osdev.org/C_PlusPlus

//Support for pure virtual functions
extern "C" void __cxa_pure_virtual()
{
    //PANIC("Error while calling pure virtual function!");
}

