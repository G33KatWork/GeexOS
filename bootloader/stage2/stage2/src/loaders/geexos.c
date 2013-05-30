#include <loaders/geexos.h>
#include <print.h>
#include <arch.h>
#include <exeformats/elfloader.h>

void loader_loadGeexOS()
{
	printf("GXLDR: Loading GeexOS...\n");

	//build up paging infrastructure
	arch_initialize_virtual_memory();
	debug_printf("GXLDR: Paging-related datastructures initialized\n");
	
	//allocate and map a kernel stack in virtual memory
	void* kernelStack = memory_allocate(GEEXOS_KERNEL_STACK_SIZE, MemoryTypeGeexOSKernelStack);
	arch_map_virtual_memory_range((uintptr_t)kernelStack, GEEXOS_KERNEL_STACK_ADDRESS, GEEXOS_KERNEL_STACK_SIZE, true, false);

	LoadedImage* kernelImageInfo;

	loader_setLibrarySearchPath("hd(0,0)/system/");

	//TODO: read kernel location from a config-file
	// if(!pe_loadFile("hd(0,0)/system/gxoskrnl.exe", MemoryTypeGeexOSKernelExecutable, &kernelImageInfo))
	// 	arch_panic("Error loading kernel PE file");
	if(!elf_loadFile("hd(0,0)/system/gxoskrnl.elf", MemoryTypeGeexOSKernelExecutable, &kernelImageInfo))
		arch_panic("Error loading kernel ELF file");

	loader_printLoadedImages();

	//Map bottom 4 MB, the code we are running currently hopefully is located here
	arch_map_virtual_memory_range(0x0, 0x0, 4*1024*1024, true, true);

	//TODO: build loader block with memory map, loaded images, etc.
	/*PLOADER_BLOCK loaderBlock = */loader_allocateAndPopulateLoaderBlock();

	debug_printf("GXLDR: Enabling paging\n");
	arch_enable_paging();

	//while(1);
	printf("Jumping into GeexOS kernel\n");
	arch_execute_at_address_with_stack(kernelImageInfo->VirtualEntryPoint, GEEXOS_KERNEL_STACK_ADDRESS + GEEXOS_KERNEL_STACK_SIZE, (void*)GEEXOS_ENV_INFO_ADDRESS);
}

void loader_buildPhysicalMemoryMap(PLOADER_BLOCK loaderBlock)
{
	size_t mmap_elementCount;
	PageLookupTableItem* mmap = memory_getMemoryMap(&mmap_elementCount);

	if(!mmap)
		arch_panic("GXLDR: Unable to retrieve memory map\n");

	//try to make regions out of the allocation blocks
	uint64_t pagesCount = 0;
	uint64_t lastPageIndex = 0;
	MemoryType lastPageType = mmap[0].type;
	for(size_t i = 0; i < mmap_elementCount; i++)
	{
		if(mmap[i].type == lastPageType && i != mmap_elementCount-1)
			pagesCount++;
		else
		{
			debug_printf("GXLDR: Built physical memory region from 0x%X with size 0x%X of type %s\n", lastPageIndex, pagesCount, memory_getTypeName(lastPageType));
			loader_addPhysicalMemoryBlock(loaderBlock, lastPageIndex, pagesCount, lastPageType);

			//go to next entry in allocation map
			lastPageIndex = i;
			lastPageType = mmap[i].type;
			pagesCount = 1;
		}
	}
}

void loader_addPhysicalMemoryBlock(PLOADER_BLOCK loaderBlock, uint64_t start, uint64_t len, MemoryType type)
{
	if(loaderBlock->MemoryDescriptorCount >= GEEXOS_MAX_MEMORY_DESCRIPTORS)
		arch_panic("GXLDR: No more space left for memory descriptors");

	uint16_t curMemDescIndex = loaderBlock->MemoryDescriptorCount;

	switch(type)
	{
		case MemoryTypeFree:
		case MemoryTypeLoaderExecutable:
		case MemoryTypeLoaderTemporary:
		case MemoryTypeLoaderStack:
		case MemoryTypeLoaderHeap:
		case MemoryTypePageLookupTable:
			break;

		case MemoryTypeBad:
		case MemoryTypeSpecial:
		case MemoryTypeUnusable:
		case MemoryTypeFirmware:
		case MemoryTypeGeexOSPageStructures:
		case MemoryTypeGeexOSPageDirectory:
		case MemoryTypeGeexOSKernelEnvironmentInformation:
		case MemoryTypeGeexOSKernelExecutable:
		case MemoryTypeGeexOSKernelStack:
		case MemoryTypeGeexOSKernelLibrary:
			loaderBlock->MemoryDescriptors[curMemDescIndex].start = start;
			loaderBlock->MemoryDescriptors[curMemDescIndex].length = len;
			loaderBlock->MemoryDescriptors[curMemDescIndex].type = type;
			loaderBlock->MemoryDescriptorCount++;

			break;
	}
}

PLOADER_BLOCK loader_allocateAndPopulateLoaderBlock()
{
	int totalSize = sizeof(LOADER_BLOCK);
	debug_printf("GXLDR: Total loader block size is 0x%x\n", totalSize);
	PLOADER_BLOCK loaderBlock = memory_allocate(totalSize, MemoryTypeGeexOSKernelEnvironmentInformation);
	memset(loaderBlock, 0, totalSize);
	arch_map_virtual_memory((uintptr_t)loaderBlock, GEEXOS_ENV_INFO_ADDRESS, false, false);
	loaderBlock->LoaderBlockSize = totalSize;

	int i = 0;
	LoadedImage *img;
	list_for_each_entry(img, &loader_loadedImages, Link) {
		if(i >= GEEXOS_MAX_IMAGE_DESCRIPTORS)
			arch_panic("GXLDR: No more space left for loaded image descriptors");

		memcpy(loaderBlock->LoadedImages[i].Name, img->Name, 32);
		loaderBlock->LoadedImages[i].PhysicalBase = img->PhysicalBase;
		loaderBlock->LoadedImages[i].VirtualBase = img->VirtualBase;
		loaderBlock->LoadedImages[i].VirtualEntryPoint = img->VirtualEntryPoint;
		loaderBlock->LoadedImages[i].SizeOfImage = img->SizeOfImage;
		loaderBlock->LoadedImages[i].IsKernelImage = i==0 ? true : false;
		//loaderBlock->LoadedImages[i].ImageType = img->ImageType;
		loaderBlock->LoadedImageCount++;
		
		i++;
	}

	//find acpi tables
	loaderBlock->ACPIRDSPDescriptor = loader_findAndMapACPITables();

	//build physical memory map
	loader_buildPhysicalMemoryMap(loaderBlock);
	
	//We want the memory amount, so add one to the highest usable page
	//FIXME: this is crap, get it directly from the bios memory map!
	loaderBlock->UpperMemoryBoundary = (memory_getHighestPhysicalPage()+1) * arch_pagesize;

	//TODO: virtual memory map?

	return loaderBlock;
}

struct RSDPDescriptor {
	char Signature[8];
	uint8_t Checksum;
	char OEMID[6];
	uint8_t Revision;
	uint32_t RsdtAddress;
};

//TODO: move to arch?
uint64_t loader_findAndMapACPITables()
{
	//TODO: get from a config?
	uintptr_t searchStart = 0;
	uintptr_t searchEnd = 0x100000;

	struct RSDPDescriptor* d;
	bool found = false;
	uintptr_t curAddr = searchStart;

	while(!found && curAddr < searchEnd)
	{
		d = (struct RSDPDescriptor*)curAddr;
		curAddr += 16;

		if(memcmp("RSD PTR ", d->Signature, 8) == 0)
			found = true;
	}

	if(found)
	{
		debug_printf("GXLDR: Found ACPI RDSP at 0x%x\n", d);
		return (uint64_t)((uintptr_t)d);
	}
	else
		return -1;
}
