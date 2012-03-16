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

//Also defined in realmode.S!
#define GXLDR_RMODE_STACKTOP    0x7000
#define GXLDR_RMODE_STACKSIZE   0x1000

#define BIOSCALLBUFFER          0x1000
#define BIOSCALLBUF_SEGMENT     0x100
#define BIOSCALLBUF_OFFSET      0

#define REALMODE_PTR(segment, offset)   ((void*)((((uint16_t)segment) << 4) + ((uint16_t)offset)))

extern void callInt(uint8_t intNum, bios_int_registers *registers);

#endif
