//#include <kernel/global.h>
#include <string.h>
#include <cxxabi.h>

void *__dso_handle;

//C++ runtime shit
//See: http://wiki.osdev.org/C%2B%2B
//TODO: Implement this

//Support for pure virtual functions
extern "C" void __cxa_pure_virtual()
{
    //PANIC("Error while calling pure virtual function!");
}

extern "C" int __cxa_atexit(void (*destructor) (void *), void *arg, void *dso)
{

}

extern "C" void __cxa_finalize(void *f)
{

}
