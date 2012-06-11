#ifndef _ARCH_I386_BIOSCALL_H
#define _ARCH_I386_BIOSCALL_H

typedef struct
{
  uint32_t eax;
  uint16_t es;
  uint16_t ds;
  uint16_t flags;
  uint16_t dummy;
  uint32_t ebx;
  uint32_t ecx;
  uint32_t edi;
  uint32_t esi;
  uint32_t edx;
} bios_int_registers;

#define EFLAGS_CARRY            (1<<0)
#define EFLAGS_PARITY           (1<<2)
#define EFLAGS_ADJUST           (1<<4)
#define EFLAGS_ZERO             (1<<6)
#define EFLAGS_SIGN             (1<<7)
#define EFLAGS_TRAP             (1<<8)
#define EFLAGS_INTERRUPT        (1<<9)
#define EFLAGS_DIRECTION        (1<<10)
#define EFLAGS_OVERFLOW         (1<<11)

#define EFLAGS_IS_SET(flags, flag) ((flags & flag) != 0)

//Also defined in realmode.S!
#define GXLDR_RMODE_STACKTOP    0x7000
#define GXLDR_RMODE_STACKSIZE   0x1000

#define BIOSCALLBUFFER          0x1000
#define BIOSCALLBUFFER_SIZE     6*0x1000
#define BIOSCALLBUF_SEGMENT     0x100
#define BIOSCALLBUF_OFFSET      0

#define REALMODE_PTR(segment, offset)   ((void*)((((uint16_t)segment) << 4) + ((uint16_t)offset)))

extern void callInt(uint8_t intNum, bios_int_registers *registers);

#endif
