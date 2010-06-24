#include <kernel/utils/ElfInformation.h>
#include <lib/string.h>
#include <kernel/Memory/Virtual/VirtualMemoryManager.h>
#include <kernel/Memory/Virtual/Regions/PreallocatedMemoryRegion.h>
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
    
    announceElfRegions();
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

void ElfInformation::announceElfRegions()
{
    Elf32SectionHeader *text = GetSection(".text");
    VirtualMemoryManager::GetInstance()->KernelSpace()->AnnounceRegionWithPreallocatedMemory(
        ((Address)text->addr) & PAGEALIGN_MASK,
        (size_t)PAGE_ALIGN(text->size),
        ".text",
        ALLOCFLAG_EXECUTABLE
    );
    
    Elf32SectionHeader *data = GetSection(".data");
    VirtualMemoryManager::GetInstance()->KernelSpace()->AnnounceRegionWithPreallocatedMemory(
        ((Address)data->addr) & PAGEALIGN_MASK,
        (size_t)PAGE_ALIGN(data->size),
        ".data",
        ALLOCFLAG_WRITABLE
    );
    
    Elf32SectionHeader *rodata = GetSection(".rodata");
    VirtualMemoryManager::GetInstance()->KernelSpace()->AnnounceRegionWithPreallocatedMemory(
        ((Address)rodata->addr) & PAGEALIGN_MASK,
        (size_t)PAGE_ALIGN(rodata->size),
        ".rodata",
        ALLOCFLAG_NONE
    );
    
    Elf32SectionHeader *bss = GetSection(".bss");
    VirtualMemoryManager::GetInstance()->KernelSpace()->AnnounceRegionWithPreallocatedMemory(
        ((Address)bss->addr) & PAGEALIGN_MASK,
        (size_t)PAGE_ALIGN(bss->size),
        ".bss",
        ALLOCFLAG_WRITABLE
    );
    
    //Announce region for placement allocation
    VirtualMemoryManager::GetInstance()->KernelSpace()->AnnounceRegionWithPreallocatedMemory(
        GetPlacementBeginning(),
        KERNEL_PLACEMENT_SIZE,
        "Placement region",
        ALLOCFLAG_WRITABLE
    );
}

void ElfInformation::announceStringTables()
{
    /*FIXME: You may call me a dirty whore, but for now I'm going to reserve the whole space between
      placement stuff and code/data as ELF-Metadata for symbol resulution. Going to fix this later...
      I definitely need a dedicated debugging protocol in the kernel with external symbol files. This is crap! */
    extern uint8_t end;
    extern uint8_t placement;
    size_t metadata_size = &placement - &end;
    
    ELF_INFORMATION_DEBUG_MSG("Announcing ELF metadata Address " << hex << (unsigned)&end << " Size: " << metadata_size);
    VirtualMemoryManager::GetInstance()->KernelSpace()->AnnounceRegionWithPreallocatedMemory(
        PAGE_ALIGN((Address)&end),
        PAGE_ALIGN(metadata_size) - PAGE_SIZE,
        "ELF Metadata",
        ALLOCFLAG_NONE
    );
    
    //These regions can possibly overlap each other
    //So be careful, if you don't specify the same flags on these regions
    //They could possibly be overwritten.
    /*ELF_INFORMATION_DEBUG_MSG("Announcing section header string table. Address " << hex << shstrtab->addr + KERNEL_BASE << " Size: " << shstrtab->size);
    VirtualMemoryManager::GetInstance()->KernelSpace()->AnnounceRegionWithPreallocatedMemory((((Address)shstrtab->addr) & PAGEALIGN_MASK) + KERNEL_BASE, (size_t)PAGE_ALIGN(shstrtab->size), ".shstrtab", ALLOCFLAG_NONE);
    
    Elf32SectionHeader* symtab = GetSection(".symtab");
    ELF_INFORMATION_DEBUG_MSG("Announcing symbol table. Address " << hex << symtab->addr + KERNEL_BASE << " Size: " << symtab->size);
    VirtualMemoryManager::GetInstance()->KernelSpace()->AnnounceRegionWithPreallocatedMemory((((Address)symtab->addr) & PAGEALIGN_MASK) + KERNEL_BASE, (size_t)PAGE_ALIGN(symtab->size), ".symtab", ALLOCFLAG_NONE);
    
    Elf32SectionHeader* strtab = GetSection(".strtab");
    ELF_INFORMATION_DEBUG_MSG("Announcing string table. Address " << hex << strtab->addr + KERNEL_BASE << " Size: " << strtab->size);
    VirtualMemoryManager::GetInstance()->KernelSpace()->AnnounceRegionWithPreallocatedMemory((((Address)strtab->addr) & PAGEALIGN_MASK) + KERNEL_BASE, (size_t)PAGE_ALIGN(strtab->size), ".strtab", ALLOCFLAG_NONE);
    
    ELF_INFORMATION_DEBUG_MSG("Foo: " << (addr & PAGEALIGN_MASK) << " Size: " << (PAGE_ALIGN(num * size)));
    VirtualMemoryManager::GetInstance()->KernelSpace()->AnnounceRegionWithPreallocatedMemory(((Address)addr) & PAGEALIGN_MASK, (size_t)PAGE_ALIGN(num * size), "ELF Header", ALLOCFLAG_NONE);*/
}

char* ElfInformation::FindSymbol(Address address, Address *symbolStart)
{   
    Elf32SectionHeader* symtab = GetSection(".symtab");
    Elf32SectionHeader* strtab = GetSection(".strtab");
    
    Elf32Symbol *symbol = (Elf32Symbol*)(symtab->addr + KERNEL_BASE);
    for(unsigned int i = 0; i < symtab->size / sizeof(Elf32Symbol); i++)
    {
        if( (address >= symbol->value) && (address < (symbol->value + symbol->size)) )
        {
            char *c = (char *)(symbol->name + strtab->addr + KERNEL_BASE);
            if(symbolStart)
                *symbolStart = symbol->value;
            
            return c;
        }
        
        symbol++;
    }
 
    return NULL;
}
