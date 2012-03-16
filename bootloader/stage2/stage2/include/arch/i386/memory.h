#ifndef _GXLDR_ARCH_I386_MEMORY_H_
#define _GXLDR_ARCH_I386_MEMORY_H_

#include <types.h>
#include <memory.h>

#define ARCH_PAGE_SIZE          0x1000

#define GXLDR_STACKTOP          0x8000
#define GXLDR_STACKSIZE         0x1000
#define GXLDR_EXEC_BASE         0x8000

typedef enum {
    BiosMemoryUsable = 1,
    BiosMemoryReserved,
    BiosMemoryAcpiReclaim,
    BiosMemoryAcpiNvs,
    BiosMemoryBad
} BiosMemoryType;

typedef struct {
    uint64_t BaseAddress;
    uint64_t Length;
    uint32_t Type;      //BiosMemoryType
    uint32_t Reserved;
} BiosMemoryMap;

void mem_i386_build_memory_map(void);
void mem_i386_e820_detect(void);

#endif
