#include <elf.h>
#include <lib.h>
#include <print.h>

int elf32_isElf(void* buffer)
{
    Elf32Header* header = (Elf32Header*)buffer;
    
    return 
        header->ident[ELF_IDENT_MAGIC_OFFSET] == ELF_IDENT_MAGIC_0 &&
        header->ident[ELF_IDENT_MAGIC_OFFSET+1] == ELF_IDENT_MAGIC_1 &&
        header->ident[ELF_IDENT_MAGIC_OFFSET+2] == ELF_IDENT_MAGIC_2 &&
        header->ident[ELF_IDENT_MAGIC_OFFSET+3] == ELF_IDENT_MAGIC_3 &&
        header->ident[ELF_IDENT_CLASS_OFFSET] == ELF_IDENT_CLASS_32;
}

int elf32_getProgramHeaders(void* buffer, Elf32ProgramHeader** firstHeader)
{
    if(!buffer || !firstHeader)
        return 0;
    
    Elf32Header* header = (Elf32Header*)buffer;
    int numProgHeaders = header->phnum;
    
    if(numProgHeaders == 0)
        *firstHeader = NULL;
    else
        *firstHeader = (Elf32ProgramHeader*)(((uint8_t*)buffer) + header->phoff);
    
    return numProgHeaders;
}

Address elf32_getEntryPoint(void* buffer)
{
    Elf32Header* header = (Elf32Header*)buffer;
    return (Address)header->entry;
}

Address elf32_getSectionAddress(void* buffer, const char* sectionName, size_t* sectionSize)
{
    Elf32Header* header = (Elf32Header*)buffer;
    
    Elf32SectionHeader* sectionHeaders = (Elf32SectionHeader*)(header->shoff + (Address)buffer);
    Elf32SectionHeader sectionHeaderStringTable = sectionHeaders[header->shstrndx];
    
    for(unsigned int i = 0; i < header->shnum; i++)
    {
        Elf32SectionHeader sh = sectionHeaders[i];
        char *c = (char *)(sectionHeaderStringTable.offset + ((Address)buffer) + sh.name);
        
        if(!strcmp(c, sectionName))
        {
            if(sectionSize)
                *sectionSize = sh.size;
            
            return (Address)(((Address)buffer) + sh.offset);
        }
    }
    
    return 0;
}
