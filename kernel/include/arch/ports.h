#ifndef _ARCH_PORTS_H
#define _ARCH_PORTS_H

#include <lib/types.h>

namespace Arch
{
    inline void outb(unsigned short port, unsigned char value)
    {
        asm volatile ("outb %1, %0" : : "dN" (port), "a" (value));
    }

    inline unsigned char inb(unsigned short port)
    {
        unsigned char ret;
        asm volatile ("inb %1, %0" : "=a" (ret) : "dN" (port));
        return ret;
    }
    
    inline void outw(unsigned short port, unsigned short value)
    {
        asm volatile ("outw %1, %0" : : "dN" (port), "a" (value));
    }

    inline unsigned short inw(unsigned short port)
    {
        unsigned short ret;
        asm volatile ("inw %1, %0" : "=a" (ret) : "dN" (port));
        return ret;
    }
}

#endif
