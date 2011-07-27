#include <arch/x86BootEnvironment.h>
#include <string.h>
#include <kernel/global.h>

using namespace Arch;

#define     ELF_SECTION_HEADER_FLAG_WRITE           (1 << 0)
#define     ELF_SECTION_HEADER_FLAG_EXEC            (1 << 2)

x86BootEnvironment::x86BootEnvironment(KernelInformation* i)
{
    //Copy cmdline
    cmdLine = (char*)kmalloc(strlen(i->cmdLine) + 1);
    memcpy(cmdLine, i->cmdLine, strlen(i->cmdLine) + 1);
    
    //Memory
    memory = i->memoryLo + i->memoryHi;
    
    //Modules
    moduleLocation = i->moduleStartAddress;
    moduleSize = i->moduleLength;
    
    //memory regions
    biosMemregionCount = i->memoryRegionCount;
    biosMemregions = (KernelInformationMemoryRegion*)kmalloc(sizeof(KernelInformationMemoryRegion) * biosMemregionCount);
    memcpy(biosMemregions, i->memoryRegions, sizeof(KernelInformationMemoryRegion) * biosMemregionCount);
    
    //program region
    programRegionCount = i->programRegionCount;
    programRegions = (KernelProgramRegion*)kmalloc(sizeof(KernelProgramRegion) * programRegionCount);
    for(size_t j = 0; j < programRegionCount; j++)
    {
        programRegions[j].VirtualStart = (Address)i->programRegions[j].vaddr;
        programRegions[j].Length = i->programRegions[j].len;
        programRegions[j].Readable = true;
        programRegions[j].Writable = (i->programRegions[j].flags & ELF_SECTION_HEADER_FLAG_WRITE) ? true : false;
        programRegions[j].Executable = (i->programRegions[j].flags & ELF_SECTION_HEADER_FLAG_EXEC) ? true : false;
    }
    
    symtabSize = i->symtabSize;
    strtabSize = i->strtabSize;
    symtab = (uint8_t*)kmalloc(symtabSize);
    strtab = (uint8_t*)kmalloc(strtabSize);
    
    memcpy(symtab, (void*)i->symtabAddr, symtabSize);
    memcpy(strtab, (void*)i->strtabAddr, strtabSize);
}
