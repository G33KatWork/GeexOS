#ifndef _LDR_GEEXOS_H_
#define _LDR_GEEXOS_H_

#include <stdint.h>
#include <memory.h>

#define	GEEXOS_KERNEL_STACK_ADDRESS	0xE0000000
#define	GEEXOS_KERNEL_STACK_SIZE	0x00004000
#define GEEXOS_ENV_INFO_ADDRESS		0xE0004000

#define GEEXOS_MAX_IMAGE_DESCRIPTORS	32
#define	GEEXOS_MAX_MEMORY_DESCRIPTORS	32

typedef struct _MEMORY_DESCRIPTOR
{
	uint64_t start;
	uint64_t length;
	MemoryType type;
} MEMORY_DESCRIPTOR, *PMEMORY_DESCRIPTOR;

typedef struct _LDRBLK_LOADED_IMAGE {
	char 	Name[32];
	uint64_t PhysicalBase;
	uint64_t VirtualBase;
	uint64_t VirtualEntryPoint;
	size_t 	SizeOfImage;
    int     IsKernelImage;
    //int     ImageType;
} LOADED_IMAGE, *PLOADED_IMAGE;

typedef struct _LOADER_BLOCK
{
	size_t LoaderBlockSize;
	uint16_t LoadedImageCount;
	LOADED_IMAGE LoadedImages[GEEXOS_MAX_IMAGE_DESCRIPTORS];
	uint16_t MemoryDescriptorCount;
	MEMORY_DESCRIPTOR MemoryDescriptors[GEEXOS_MAX_MEMORY_DESCRIPTORS];
	uint64_t ACPIRDSPDescriptor;
	uint64_t UpperMemoryBoundary;
	//TODO: more?
} LOADER_BLOCK, *PLOADER_BLOCK;

void loader_loadGeexOS(void);
void loader_buildPhysicalMemoryMap(PLOADER_BLOCK loaderBlock);
void loader_addPhysicalMemoryBlock(PLOADER_BLOCK loaderBlock, uint64_t start, uint64_t len, MemoryType type);
PLOADER_BLOCK loader_allocateAndPopulateLoaderBlock(void);

uint64_t loader_findAndMapACPITables(void);

#endif
