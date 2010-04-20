#ifndef ELFINFORMATION_H_
#define ELFINFORMATION_H_

#include <lib/types.h>
#include <kernel/elf32.h>

class ElfInformation
{
public:
    ElfInformation(unsigned int elfAddr, unsigned int elfShndx, unsigned int elfSize, unsigned int elfNum);
    
    Elf32SectionHeader* GetSection(const char* name);
    char* GetSectionName(Elf32SectionHeader* section);
    
private:
    void announceStringTables(void);
    
    Elf32SectionHeader *shstrtab;
    unsigned int addr;
    unsigned int shndx;
    unsigned int size;
    unsigned int num;
};
    
#endif
