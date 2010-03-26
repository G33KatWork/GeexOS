#include <kernel/ElfInformation.h>
#include <lib/string.h>
#include <kernel/Memory/Virtual/VirtualMemoryManager.h>
#include <arch/Paging.h>
#include <kernel/Memory/PlacementAllocator.h>

using namespace Memory;

ElfInformation::ElfInformation(unsigned int elfAddr, unsigned int elfShndx, unsigned int elfSize, unsigned int elfNum)
{
    //Set section header string table
    shstrtab = (Elf32SectionHeader *)(elfAddr + elfShndx * elfSize);
    
    addr = elfAddr;
    shndx = elfShndx;
    size = elfSize;
    num = elfNum;
    
    Elf32SectionHeader *text = GetSection(".text");
    VirtualMemoryManager::GetInstance()->KernelSpace()->AnnounceRegion(((Address)text->addr) & IDENTITY_POSITION, (size_t)PAGE_ALIGN(text->size), ".text", ALLOCFLAG_EXECUTABLE);
    
    Elf32SectionHeader *data = GetSection(".data");
    VirtualMemoryManager::GetInstance()->KernelSpace()->AnnounceRegion(((Address)data->addr) & IDENTITY_POSITION, (size_t)PAGE_ALIGN(data->size), ".data", ALLOCFLAG_WRITABLE);
    
    Elf32SectionHeader *rodata = GetSection(".rodata");
    VirtualMemoryManager::GetInstance()->KernelSpace()->AnnounceRegion(((Address)rodata->addr) & IDENTITY_POSITION, (size_t)PAGE_ALIGN(rodata->size), ".rodata", ALLOCFLAG_NONE);
    
    Elf32SectionHeader *bss = GetSection(".bss");
    VirtualMemoryManager::GetInstance()->KernelSpace()->AnnounceRegion(((Address)bss->addr) & IDENTITY_POSITION, (size_t)PAGE_ALIGN(bss->size), ".bss", ALLOCFLAG_WRITABLE);
    
    //Announce region for placement allocation
    VirtualMemoryManager::GetInstance()->KernelSpace()->AnnounceRegion(GetPlacementBeginning(), PLACEMENT_SIZE, "Placement region", ALLOCFLAG_WRITABLE);
}

Elf32SectionHeader* ElfInformation::GetSection(const char* name)
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

char* ElfInformation::GetSectionName(Elf32SectionHeader* section)
{
    return (char *)(shstrtab->addr + section->name);
}
