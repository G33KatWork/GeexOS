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
    ASSERT(sizeof(KernelInformation) == info->LoaderBlockSize, "Size missmatch of KernelInformation struct. Incompatible loader?");

    kernelInformation = (KernelInformation*)kmalloc(info->LoaderBlockSize);
    memcpy(kernelInformation, info, info->LoaderBlockSize);

#ifdef EN_HAL_DEBUG_MSG
    HAL_DEBUG_MSG("Got " << dec << kernelInformation->MemoryDescriptorCount << " memory descriptors from bootloader");
    for(uint16_t i = 0; i < kernelInformation->MemoryDescriptorCount; i++)
    {
        MEMORY_DESCRIPTOR desc = kernelInformation->MemoryDescriptors[i];
        HAL_DEBUG_MSG("Descriptor " << dec << i <<
                      ": Start: " << hex << desc.Start <<
                      " - Length: " << desc.Length <<
                      " - Type: " << MemoryTypeNames[desc.Type]
        );
    }

    HAL_DEBUG_MSG("Got " << dec << kernelInformation->LoadedImageCount << " loaded images from bootloader");
    for(uint16_t i = 0; i < kernelInformation->LoadedImageCount; i++)
    {
        LOADED_IMAGE img = kernelInformation->LoadedImages[i];
        HAL_DEBUG_MSG("Image " << dec << i <<
                      ": Name: " << img.Name <<
                      " - PhysicalBase: " << hex << img.PhysicalBase <<
                      " - VirtualBase: " << img.VirtualBase <<
                      " - VirtualEntryPoint: " << img.VirtualEntryPoint <<
                      " - SizeOfImage: " << img.SizeOfImage <<
                      " - IsKernelImage: " << img.IsKernelImage
        );
    }

    HAL_DEBUG_MSG("Upper memory boundary is at " << hex << kernelInformation->UpperMemoryBoundary);
#endif
    
    memory = kernelInformation->UpperMemoryBoundary;
}

PMEMORY_DESCRIPTOR x86BootEnvironment::GetMemoryRegions(size_t* totalCount)
{
    if(totalCount)
        *totalCount = kernelInformation->MemoryDescriptorCount;

    return kernelInformation->MemoryDescriptors;
}

PMEMORY_DESCRIPTOR x86BootEnvironment::GetMemoryRegionByType(PMEMORY_DESCRIPTOR start, MemoryType type)
{
    ASSERT(
        start == NULL ||
        (
            (((Address)start) > ((Address)kernelInformation->MemoryDescriptors))
            && 
            (((Address)start) < (((Address)kernelInformation->MemoryDescriptors) + kernelInformation->MemoryDescriptorCount*sizeof(MEMORY_DESCRIPTOR)))
        ),
        "start parameter out of bounds"
    );

    uint16_t i;

    if(!start)
    {
        start = kernelInformation->MemoryDescriptors;
        i = 0;
    }
    else
    {
        start++;
        i = (((Address)start) - ((Address)kernelInformation->MemoryDescriptors)) / sizeof(MEMORY_DESCRIPTOR);
    }

    while(i < kernelInformation->MemoryDescriptorCount)
    {
        if(start->Type == type)
            return start;

        start++;
        i++;
    }

    return NULL;
}

void x86BootEnvironment::DumpBootEnvironment(BaseOutputDevice* c)
{
    *c << hex;

    *c << "x86 boot environment passed from stage 2 of bootloader: " << endl;
    *c << "Loader Block:" << endl;
    *c << "\tLoaderBlockSize: " << this->kernelInformation->LoaderBlockSize << endl;
    *c << endl;

    *c << "\tLoadedImageCount: " << this->kernelInformation->LoadedImageCount << endl;
    *c << "\tLoadedImages:" << endl;
    for(int i = 0; i < this->kernelInformation->LoadedImageCount; i++)
    {
        *c << "\t\tImage " << i << ":" << endl;
        *c << "\t\t\tName: " << this->kernelInformation->LoadedImages[i].Name << endl;
        *c << "\t\t\tPhysicalBase: " << this->kernelInformation->LoadedImages[i].PhysicalBase << endl;
        *c << "\t\t\tVirtualBase: " << this->kernelInformation->LoadedImages[i].VirtualBase << endl;
        *c << "\t\t\tVirtualEntryPoint: " << this->kernelInformation->LoadedImages[i].VirtualEntryPoint << endl;
        *c << "\t\t\tSizeOfImage: " << this->kernelInformation->LoadedImages[i].SizeOfImage << endl;
        *c << "\t\t\tIsKernelImage: " << this->kernelInformation->LoadedImages[i].IsKernelImage << endl;
    }
    *c << endl;

    *c << "\tMemoryDescriptorCount: " << this->kernelInformation->MemoryDescriptorCount << endl;
    *c << "\tMemoryDescriptors:" << endl;
    for(int i = 0; i < this->kernelInformation->MemoryDescriptorCount; i++)
    {
        *c << "\t\tMemoryDescriptor " << i << ":" << endl;
        *c << "\t\t\tStart: " << this->kernelInformation->MemoryDescriptors[i].Start << endl;
        *c << "\t\t\tLength: " << this->kernelInformation->MemoryDescriptors[i].Length << endl;
        *c << "\t\t\tType: " << MemoryTypeNames[this->kernelInformation->MemoryDescriptors[i].Type] << endl;
    }
    *c << endl;

    *c << "\tACPIRDSPDescriptor: " << this->kernelInformation->ACPIRDSPDescriptor << endl;
    *c << "\tUpperMemoryBoundary: " << this->kernelInformation->UpperMemoryBoundary << endl;
}
