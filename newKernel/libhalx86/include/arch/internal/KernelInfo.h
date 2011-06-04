#ifndef _ARCHX86_KERNELINFO_H_
#define _ARCHX86_KERNELINFO_H_

#include <types.h>

// format of a memory region
struct kerninfo_memory_region {
	uint32_t    	size;
	uint64_t    	addr;
	uint64_t    	len;
	uint32_t    	type;
}  __attribute__((packed));
typedef struct kerninfo_memory_region KernelInformationMemoryRegion;

/**
 * This structure is passed to the kernel by the loader stub
 * for several information like the amount of physical memory,
 * current virtual memory map, reserved physical memory by the
 * BIOS, the kernel commandline and the location of the modules
**/
struct kernel_information {
	uint32_t	                memoryLo;
	uint32_t    	            memoryHi;
    char*                       cmdLine;
    uint32_t                    moduleStartAddress;
    uint32_t                    moduleLength;
    uint32_t                    memoryRegionCount;
    KernelInformationMemoryRegion* memoryRegions;
    uint32_t                    symtabAddr;
    uint32_t                    symtabSize;
    uint32_t                    strtabAddr;
    uint32_t                    strtabSize;
}  __attribute__((packed));

typedef struct kernel_information KernelInformation;

#endif
