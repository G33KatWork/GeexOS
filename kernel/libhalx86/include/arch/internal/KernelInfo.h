#ifndef _ARCHX86_KERNELINFO_H_
#define _ARCHX86_KERNELINFO_H_

#include <types.h>

//These structs are passed by the loader to the kernel.
//If you change them here, be sure to change them in the loader, too.

typedef enum {
    MemoryTypeFree,
    MemoryTypeBad,
    MemoryTypeSpecial,
    MemoryTypeLoaderExecutable,
    MemoryTypeLoaderTemporary,
    MemoryTypeLoaderStack,
    MemoryTypeLoaderHeap,
    MemoryTypeFirmware,
    MemoryTypePageLookupTable,
    MemoryTypeGeexOSPageStructures,
    MemoryTypeGeexOSPageDirectory,
    MemoryTypeGeexOSKernelEnvironmentInformation,
    MemoryTypeGeexOSKernelExecutable,
    MemoryTypeGeexOSKernelStack,
    MemoryTypeGeexOSKernelLibrary
} MemoryType;

typedef struct _MEMORY_DESCRIPTOR
{
    uint64_t Start;
    uint64_t Length;
    MemoryType Type;
} MEMORY_DESCRIPTOR, *PMEMORY_DESCRIPTOR;

typedef struct _LDRBLK_LOADED_IMAGE {
    char    Name[32];
    uint64_t PhysicalBase;
    uint64_t VirtualBase;
    uint64_t VirtualEntryPoint;
    size_t  SizeOfImage;
    int     IsKernelImage;
} LOADED_IMAGE, *PLOADED_IMAGE;

typedef struct _LOADER_BLOCK
{
    size_t LoaderBlockSize;
    uint16_t LoadedImageCount;
    LOADED_IMAGE LoadedImages[32];
    uint16_t MemoryDescriptorCount;
    MEMORY_DESCRIPTOR MemoryDescriptors[32];
    uint64_t ACPIRDSPDescriptor;
    uint64_t UpperMemoryBoundary;
    //TODO: more?
} KernelInformation;

#endif
