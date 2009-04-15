#include <kernel/global.h>

//Support for pure virtual functions
extern "C" void __cxa_pure_virtual()
{
    PANIC("Error while calling pure virtual function!");
}
