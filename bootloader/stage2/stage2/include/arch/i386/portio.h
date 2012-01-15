#ifndef _ARCH_I386_PORTIO_H
#define _ARCH_I386_PORTIO_H

static inline void portio_outb(unsigned short port, unsigned char value)
{
    asm volatile ("outb %1, %0" : : "dN" (port), "a" (value));
}

static inline unsigned char portio_inb(unsigned short port)
{
    unsigned char ret;
    asm volatile ("inb %1, %0" : "=a" (ret) : "dN" (port));
    return ret;
}

static inline void portio_outw(unsigned short port, unsigned short value)
{
    asm volatile ("outw %1, %0" : : "dN" (port), "a" (value));
}

static inline unsigned short portio_inw(unsigned short port)
{
    unsigned short ret;
    asm volatile ("inw %1, %0" : "=a" (ret) : "dN" (port));
    return ret;
}

#endif
