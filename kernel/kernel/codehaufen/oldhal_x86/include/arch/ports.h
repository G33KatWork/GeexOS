#ifndef _ARCH_PORTS_H
#define _ARCH_PORTS_H

#include <types.h>

namespace Arch
{
    inline void outb(uint16_t port, uint8_t value)
    {
        asm volatile ("outb %1, %0" : : "dN" (port), "a" (value));
    }

    inline unsigned char inb(uint16_t port)
    {
        uint8_t ret;
        asm volatile ("inb %1, %0" : "=a" (ret) : "dN" (port));
        return ret;
    }
    
    inline void outw(uint16_t port, uint16_t value)
    {
        asm volatile ("outw %1, %0" : : "dN" (port), "a" (value));
    }

    inline uint16_t inw(uint16_t port)
    {
        uint16_t ret;
        asm volatile ("inw %1, %0" : "=a" (ret) : "dN" (port));
        return ret;
    }
}

#endif
