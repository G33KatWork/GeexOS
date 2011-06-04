#ifndef _ELF32_H
#define _ELF32_H

#include <types.h>

#define SHF_WRITE         0x1
#define SHF_ALLOC         0x2
#define SHF_EXECINSTR     0x4
#define SHF_MASKPROC      0xf0000000

#define ELF32_R_SYM(val)  ((val) >> 8)
#define ELF32_R_TYPE(val) ((val) & 0xff)

typedef struct
{
    uint8_t  ident[ELF_IDENT_LEN];
    uint16_t type;
    uint16_t machine;
    uint32_t version;
    uint32_t entry;
    uint32_t phoff;
    uint32_t shoff;
    uint32_t flags;
    uint16_t ehsize;
    uint16_t phentsize;
    uint16_t phnum;
    uint16_t shentsize;
    uint16_t shnum;
    uint16_t shstrndx;
} Elf32Header;

typedef struct
{
    uint32_t type;
    uint32_t offset;
    uint32_t vaddr;
    uint32_t paddr;
    uint32_t filesz;
    uint32_t memsz;
    uint32_t flags;
    uint32_t align;
} Elf32ProgramHeader;

typedef struct
{
    uint32_t name;
    uint32_t type;
    uint32_t flags;
    uint32_t addr;
    uint32_t offset;
    uint32_t size;
    uint32_t link;
    uint32_t info;
    uint32_t addralign;
    uint32_t entsize;
} Elf32SectionHeader;

typedef struct
{
    uint32_t name;
    uint32_t value;
    uint32_t size;
    uint8_t  info;
    uint8_t  other;
    uint16_t shndx;
} Elf32Symbol;

typedef struct
{
    int tag;
    union
    {
        int32_t  val;
        uint32_t ptr;
    } un;
} Elf32Dyn;

typedef struct
{
    uint32_t offset;
    uint32_t info;
} Elf32Rel;


int elf32_isElf(void* buffer);
int elf32_getProgramHeaders(void* buffer, Elf32ProgramHeader** firstHeader);
Address elf32_getEntryPoint(void* buffer);
Address elf32_getSectionAddress(void* buffer, const char* sectionName, size_t* sectionSize);

#endif
