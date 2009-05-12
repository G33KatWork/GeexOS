#ifndef _ARCH_SCHEDULING_H
#define _ARCH_SCHEDULING_H

namespace Arch
{
    extern "C"  void    writeStackPointer(Address a);
    extern "C"  Address readStackPointer();
    extern "C"  void    writeBasePointer(Address a);
    extern "C"  Address readBasePointer();
}

#endif
