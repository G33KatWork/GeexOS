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
    Address Start;
    Address Length;
    MemoryType Type;
} MEMORY_DESCRIPTOR, *PMEMORY_DESCRIPTOR;

typedef struct _LDRBLK_LOADED_IMAGE {
    char    Name[32];
    Address PhysicalBase;
    Address VirtualBase;
    Address VirtualEntryPoint;
    size_t  SizeOfImage;
    int     IsKernelImage;
} LOADED_IMAGE, *PLOADED_IMAGE;

typedef struct _LOADER_BLOCK
{
    uint16_t LoadedImageCount;
    LOADED_IMAGE LoadedImages[32];
    uint16_t MemoryDescriptorCount;
    MEMORY_DESCRIPTOR MemoryDescriptors[32];
    Address ACPIRDSPDescriptor;
    Address UpperMemoryBoundary;
    //TODO: more?
} KernelInformation;

// format of a memory region
/*struct kerninfo_memory_region {
	uint32_t    	size;
	uint64_t    	addr;
	uint64_t    	len;
	uint32_t    	type;
}  __attribute__((packed));
typedef struct kerninfo_memory_region KernelInformationMemoryRegion;

// format of a mapped program region of the kernel binary
struct kerninfo_program_region {
    uint64_t        vaddr;
    uint64_t        len;
    uint32_t        flags;  //see elf flags of program headers
} __attribute__((packed));
typedef struct kerninfo_program_region KernelInformationProgramRegion;*/

/**
 * This structure is passed to the kernel by the loader stub
 * for several information like the amount of physical memory,
 * current virtual memory map, reserved physical memory by the
 * BIOS, the kernel commandline and the location of the modules
**/
/*struct kernel_information {
	uint32_t	                memoryLo;
	uint32_t    	            memoryHi;
    char*                       cmdLine;
    uint32_t                    moduleStartAddress;
    uint32_t                    moduleLength;
    uint32_t                    memoryRegionCount;
    KernelInformationMemoryRegion* memoryRegions;
    uint32_t                    programRegionCount;
    KernelInformationProgramRegion* programRegions;
    uint32_t                    symtabAddr;
    uint32_t                    symtabSize;
    uint32_t                    strtabAddr;
    uint32_t                    strtabSize;
}  __attribute__((packed));

typedef struct kernel_information KernelInformation;*/

#endif
