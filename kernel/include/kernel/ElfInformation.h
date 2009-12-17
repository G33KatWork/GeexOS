#ifndef ELFINFORMATION_H_
#define ELFINFORMATION_H_

#include <lib/types.h>
#include <kernel/elf32.h>
#include <lib/string.h>

#include <kernel/global.h>
using namespace IO;

class ElfInformation
{
public:
    ElfInformation(unsigned int elfAddr, unsigned int elfShndx, unsigned int elfSize, unsigned int elfNum)
    {
        //Set section header string table
        shstrtab = (Elf32SectionHeader *)(elfAddr + elfShndx * elfSize);
        
        addr = elfAddr;
        shndx = elfShndx;
        size = elfSize;
        num = elfNum;
    }
    
    Elf32SectionHeader* GetSection(const char* name)
    {
        for(unsigned int i = 0; i < num; i++)
        {
            Elf32SectionHeader *sh = (Elf32SectionHeader *)(addr + i * size);
            char *c = (char *)(shstrtab->addr + sh->name);
            
            if(!strcmp(c, name))
                return sh;
        }
        
        return NULL;
    }
    
    char* GetSectionName(Elf32SectionHeader* section)
    {
        return (char *)(shstrtab->addr + section->name);
    }
    
private:
    Elf32SectionHeader *shstrtab;
    unsigned int addr;
    unsigned int shndx;
    unsigned int size;
    unsigned int num;
};
    
#endif
