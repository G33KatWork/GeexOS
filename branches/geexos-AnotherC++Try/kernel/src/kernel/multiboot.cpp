#include <kernel/global.h>
#include <kernel/multiboot.h>
#include <lib/string.h>
#include <kernel/elf32.h>

using namespace Kernel;
using namespace Lib;

Multiboot::Multiboot(MultibootHeader *h)
{
    header = h;
    symtab = NULL;
    strtab = NULL;
	
    if(!IsElf()) PANIC("Kernel is not ELF-compatible!");
    
    //Find symtab/strtab sections
    Elf32SectionHeader *shstrtab = (Elf32SectionHeader *)(header->elf_addr + header->elf_shndx * header->elf_size);
    
    for(unsigned int i = 0; i < header->elf_num; i++)
    {
        Elf32SectionHeader *sh = (Elf32SectionHeader *)(header->elf_addr + i * header->elf_size);
        
        if(sh->type == SHT_SYMTAB)
            symtab = sh;
        else if(sh->type == SHT_STRTAB)
        {
            char *c = (char *)(shstrtab->addr + sh->name);

            if(!strcmp(c, ".strtab"))
                strtab = sh;
        }
    }
}
