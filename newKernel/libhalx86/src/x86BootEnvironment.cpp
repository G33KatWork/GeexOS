#include <arch/x86BootEnvironment.h>
#include <string.h>
#include <kernel/global.h>

using namespace Arch;

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
    programRegions = (KernelInformationProgramRegion*)kmalloc(sizeof(KernelInformationProgramRegion) * programRegionCount);
    memcpy(programRegions, i->programRegions, sizeof(KernelInformationProgramRegion) * programRegionCount);
    
    symtabSize = i->symtabSize;
    strtabSize = i->strtabSize;
    symtab = (uint8_t*)kmalloc(symtabSize);
    strtab = (uint8_t*)kmalloc(strtabSize);
    
    memcpy(symtab, (void*)i->symtabAddr, symtabSize);
    memcpy(strtab, (void*)i->strtabAddr, strtabSize);
}
