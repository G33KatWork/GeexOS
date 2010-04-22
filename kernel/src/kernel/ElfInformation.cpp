#include <kernel/ElfInformation.h>
#include <lib/string.h>
#include <kernel/Memory/Virtual/VirtualMemoryManager.h>
#include <arch/Paging.h>
#include <kernel/Memory/PlacementAllocator.h>
#include <kernel/debug.h>
#include <arch/AddressLayout.h>

using namespace Memory;
using namespace IO;

ElfInformation::ElfInformation(unsigned int elfAddr, unsigned int elfShndx, unsigned int elfSize, unsigned int elfNum)
{
    addr = elfAddr + KERNEL_BASE;
    shndx = elfShndx + KERNEL_BASE;
    size = elfSize;
    num = elfNum;
    
    //Set section header string table
    shstrtab = (Elf32SectionHeader *)(addr + shndx * size);
    
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

    announceStringTables();
}

Elf32SectionHeader* ElfInformation::GetSection(const char* name)
{
    ELF_INFORMATION_DEBUG_MSG("Request for ELF-Section with name " << name);
    
    for(unsigned int i = 0; i < num; i++)
    {
        Elf32SectionHeader *sh = (Elf32SectionHeader *)(addr + i * size);
        ELF_INFORMATION_DEBUG_MSG("sh: " << hex << (unsigned)sh);
        char *c = (char *)(shstrtab->addr + KERNEL_BASE + sh->name);
        ELF_INFORMATION_DEBUG_MSG("c: " << hex << (unsigned)c);
        
        if(!strcmp(c, name))
        {
            ELF_INFORMATION_DEBUG_MSG("ELF-Section found at " << hex << (unsigned)sh);
            return sh;
        }
    }
    
    return NULL;
}

char* ElfInformation::GetSectionName(Elf32SectionHeader* section)
{
    return (char *)(shstrtab->addr + KERNEL_BASE + section->name);
}

void ElfInformation::announceStringTables()
{
    //These regions can possibly overlap each other
    //So be careful, if you don't specify the same flags on these regions
    //They could possibly be overwritten.
    ELF_INFORMATION_DEBUG_MSG("Announcing section header string table. Address " << hex << (unsigned)shstrtab->addr + KERNEL_BASE << " Size: " << (unsigned)shstrtab->size);
    VirtualMemoryManager::GetInstance()->KernelSpace()->AnnounceRegion((((Address)shstrtab->addr) & IDENTITY_POSITION) + KERNEL_BASE, (size_t)PAGE_ALIGN(shstrtab->size), ".shstrtab", ALLOCFLAG_NONE);
    
    Elf32SectionHeader* symtab = GetSection(".symtab");
    ELF_INFORMATION_DEBUG_MSG("Announcing symbol table. Address " << hex << (unsigned)symtab->addr + KERNEL_BASE << " Size: " << (unsigned)symtab->size);
    VirtualMemoryManager::GetInstance()->KernelSpace()->AnnounceRegion((((Address)symtab->addr) & IDENTITY_POSITION) + KERNEL_BASE, (size_t)PAGE_ALIGN(symtab->size), ".symtab", ALLOCFLAG_NONE);
    
    Elf32SectionHeader* strtab = GetSection(".strtab");
    ELF_INFORMATION_DEBUG_MSG("Announcing string table. Address " << hex << (unsigned)strtab->addr + KERNEL_BASE << " Size: " << (unsigned)strtab->size);
    VirtualMemoryManager::GetInstance()->KernelSpace()->AnnounceRegion((((Address)strtab->addr) & IDENTITY_POSITION) + KERNEL_BASE, (size_t)PAGE_ALIGN(strtab->size), ".strtab", ALLOCFLAG_NONE);
    
    ELF_INFORMATION_DEBUG_MSG("Foo: " << (unsigned)(addr & IDENTITY_POSITION) << " Size: " << (unsigned)(PAGE_ALIGN(num * size)));
    VirtualMemoryManager::GetInstance()->KernelSpace()->AnnounceRegion(((Address)addr) & IDENTITY_POSITION, (size_t)PAGE_ALIGN(num * size), "ELF Header", ALLOCFLAG_NONE);
}
