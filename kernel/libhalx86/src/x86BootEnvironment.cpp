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
    kernelInformation = (KernelInformation*)kmalloc(info->LoaderBlockSize);
    memcpy(kernelInformation, info, info->LoaderBlockSize);

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
    memory = kernelInformation->UpperMemoryBoundary;
}

PMEMORY_DESCRIPTOR x86BootEnvironment::GetMemoryRegions(size_t* totalCount)
{
    if(totalCount)
        *totalCount = kernelInformation->MemoryDescriptorCount;

    return kernelInformation->MemoryDescriptors;
}

PMEMORY_DESCRIPTOR x86BootEnvironment::GetMemoryFirstRegionByType(MemoryType type)
{
    for(uint16_t i = 0; i < kernelInformation->MemoryDescriptorCount; i++)
    {
        PMEMORY_DESCRIPTOR desc = &kernelInformation->MemoryDescriptors[i];
        if(desc->Type == type)
            return desc;
    }

    return NULL;
}
