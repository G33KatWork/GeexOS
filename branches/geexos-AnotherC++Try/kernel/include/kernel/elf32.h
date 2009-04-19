#ifndef _ELF32_H
#define _ELF32_H

#include <lib/types.h>

#define SHT_PROGBITS      0x1     // The data is contained in the program file.
#define SHT_SYMTAB	      0x2	  // Symbol table
#define SHT_STRTAB	      0x3	  // String table
#define SHT_RELA          0x4
#define SHT_HASH          0x5     // Symbol hash table
#define SHT_DYNAMIC       0x6     // Dynamic linking information
#define SHT_NOTE          0x7
#define SHT_NOBITS        0x8     // The data is not contained in the program file.
#define SHT_REL           0x9
#define SHT_DYNSYM        0xb
#define SHT_INIT_ARRAY    0xe
#define SHT_FINI_ARRAY    0xf
#define SHT_PREINIT_ARRAY 0x10

#define SHF_WRITE         0x1
#define SHF_ALLOC         0x2
#define SHF_EXECINSTR     0x4
#define SHF_MASKPROC      0xf0000000

#define ELF32_R_SYM(val)  ((val) >> 8)
#define ELF32_R_TYPE(val) ((val) & 0xff)

typedef struct
{
    unsigned char  ident[16];
    unsigned short type;
    unsigned short machine;
    unsigned int version;
    unsigned int entry;
    unsigned int phoff;
    unsigned int shoff;
    unsigned int flags;
    unsigned short ehsize;
    unsigned short phentsize;
    unsigned short phnum;
    unsigned short shentsize;
    unsigned short shnum;
    unsigned short shstrndx;
} Elf32Header;

typedef struct
{
    unsigned int type;
    unsigned int offset;
    unsigned int vaddr;
    unsigned int paddr;
    unsigned int filesz;
    unsigned int memsz;
    unsigned int flags;
    unsigned int align;
} Elf32ProcessHeader;

typedef struct
{
    unsigned int name;
    unsigned int type;
    unsigned int flags;
    unsigned int addr;
    unsigned int offset;
    unsigned int size;
    unsigned int link;
    unsigned int info;
    unsigned int addralign;
    unsigned int entsize;
} Elf32SectionHeader;

typedef struct
{
    unsigned int name;
    unsigned int value;
    unsigned int size;
    unsigned char  info;
    unsigned char  other;
    unsigned short shndx;
} Elf32Symbol;

typedef struct
{
    int tag;
    union
    {
        int val;
        unsigned int ptr;
    } un;
} Elf32Dyn;

typedef struct
{
    unsigned int offset;
    unsigned int info;
} Elf32Rel;

#endif
