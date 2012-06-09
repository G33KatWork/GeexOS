#include <loaders/geexos.h>
#include <print.h>
#include <arch.h>
#include <exeformats/peloader.h>

void loader_loadGeexOS()
{
	printf("GXLDR: Loading GeexOS...\n");

	//build up paging infrastructure
	arch_initialize_virtual_memory();
	printf("GXLDR: Paging-related datastructures initialized\n");
	
	LoadedImage* kernelImageInfo;

	//TODO: read kernel location from a config-file
	if(!pe_loadFile("hd(0,0)/system/gxoskrnl.exe", MemoryTypeGeexOSKernelExecutable, &kernelImageInfo))
		arch_panic("Error loading kernel pe file");

	pe_printLoadedImages();

	//Map bottom 4 MB, the code we are running currently hopefully is located here
	arch_map_virtual_memory_range(0x0, 0x0, 4*1024*1024, true, true);
	arch_enable_paging();

	//TODO: build loader block with memory map, loaded images, etc.

	//TODO: setup a stack for the kernel
	//FIXME: this is only a hack for testing, get the entry point address from the list of loaded images later
	typedef void (*entryPoint)(void);
	entryPoint p = (entryPoint)kernelImageInfo->VirtualEntryPoint;
	p();
}
