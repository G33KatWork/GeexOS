#include <arch/x86BootEnvironment.h>
#include <string.h>
#include <kernel/global.h>
#include <kernel/debug.h>

using namespace Arch;
using namespace Debug;

static const char* MemoryTypeNames[] = {
    "MemoryTypeFree",
    "MemoryTypeBad",
    "MemoryTypeSpecial",
    "MemoryTypeLoaderExecutable",
    "MemoryTypeLoaderTemporary",
    "MemoryTypeLoaderStack",
    "MemoryTypeLoaderHeap",
    "MemoryTypeFirmware",
    "MemoryTypePageLookupTable",
    "MemoryTypeGeexOSPageStructures",
    "MemoryTypeGeexOSPageDirectory",
    "MemoryTypeGeexOSKernelEnvironmentInformation",
    "MemoryTypeGeexOSKernelExecutable",
    "MemoryTypeGeexOSKernelStack",
    "MemoryTypeGeexOSKernelLibrary"
};

x86BootEnvironment::x86BootEnvironment(KernelInformation* info)
{
    HAL_DEBUG_MSG("Got " << dec << info->MemoryDescriptorCount << " memory descriptors from bootloader");
    for(uint16_t i = 0; i < info->MemoryDescriptorCount; i++)
    {
        MEMORY_DESCRIPTOR desc = info->MemoryDescriptors[i];
        HAL_DEBUG_MSG("Descriptor " << dec << i <<
                      ": Start: " << hex << desc.Start <<
                      " - Length: " << desc.Length <<
                      " - Type: " << MemoryTypeNames[desc.Type]
        );
    }

    HAL_DEBUG_MSG("Got " << dec << info->LoadedImageCount << " loaded images from bootloader");
    for(uint16_t i = 0; i < info->LoadedImageCount; i++)
    {
        LOADED_IMAGE img = info->LoadedImages[i];
        HAL_DEBUG_MSG("Image " << dec << i <<
                      ": Name: " << img.Name <<
                      " - PhysicalBase: " << hex << img.PhysicalBase <<
                      " - VirtualBase: " << img.VirtualBase <<
                      " - VirtualEntryPoint: " << img.VirtualEntryPoint <<
                      " - SizeOfImage: " << img.SizeOfImage <<
                      " - IsKernelImage: " << img.IsKernelImage
        );
    }

    HAL_DEBUG_MSG("Upper memory boundary is at " << hex << info->UpperMemoryBoundary);
    memory = info->UpperMemoryBoundary;

    //Copy cmdline
    /*cmdLine = (char*)kmalloc(strlen(i->cmdLine) + 1);
    memcpy(cmdLine, i->cmdLine, strlen(i->cmdLine) + 1);
    
    //Memory
    memory = i->memoryLo + i->memoryHi;
    
    //Modules
    moduleLocation = (Address)kmalloc(i->moduleLength);
    memcpy((void*)moduleLocation, (void*)i->moduleStartAddress, i->moduleLength);
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
    memcpy(strtab, (void*)i->strtabAddr, strtabSize);*/
}
