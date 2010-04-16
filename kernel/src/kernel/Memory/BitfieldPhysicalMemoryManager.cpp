#include <kernel/Memory/BitfieldPhysicalMemoryManager.h>
#include <arch/Paging.h>
#include <lib/string.h>
#include <kernel/global.h>
#include <kernel/debug.h>

using namespace IO;
using namespace Memory;
using namespace Arch;

#define INDEX_FROM_BIT(a) (a/(8*4))
#define OFFSET_FROM_BIT(a) (a%(8*4))

BitfieldPhysicalMemoryManager::BitfieldPhysicalMemoryManager(size_t memorySize)
{
    //setup bitmap
	nFrames = memorySize * 1024 / PAGE_SIZE;
	frames = (unsigned int*)kmalloc(INDEX_FROM_BIT(nFrames) * sizeof(uint32_t));
	memset(frames, 0, INDEX_FROM_BIT(nFrames) * sizeof(uint32_t));
	
	//mark 0 to 4MB for kernel as used
	for(unsigned int i = 0; i < 1024; i++)
		bitmap_set_frame(i*PAGE_SIZE);
}

unsigned int BitfieldPhysicalMemoryManager::bitmap_get_first_free(void)
{
	for(unsigned int i = 0; i < INDEX_FROM_BIT(nFrames); i++)
	{
		if(frames[i] != 0xFFFFFFFF)	//nothing is free here
		{
			// at least one bit is free here
			for(unsigned int j = 0; j < 32; j++)
			{
				unsigned int toTest = 0x1 << j;
				if( !(frames[i]&toTest) )
					return (i*4*8+j);	//32 bit (4*8) per entry;
			}
		}
	}
	
	PANIC("Desperately out of memory: No more frames left to allocate");
	
	//make compiler shut up... we will never get here in case of error
	return (unsigned int)-1;
}

void BitfieldPhysicalMemoryManager::bitmap_set_frame(Address physAddr)
{
	Address frame = physAddr / PAGE_SIZE;
	unsigned int idx = INDEX_FROM_BIT(frame);
	unsigned int off = OFFSET_FROM_BIT(frame);
	frames[idx] |= (0x1 << off);
}

void BitfieldPhysicalMemoryManager::bitmap_clear_frame(Address physAddr)
{
	Address frame = physAddr / PAGE_SIZE;
	unsigned int idx = INDEX_FROM_BIT(frame);
	unsigned int off = OFFSET_FROM_BIT(frame);
	frames[idx] &= ~(0x1 << off);
}

Address BitfieldPhysicalMemoryManager::AllocateFrame()
{
    Address page = (bitmap_get_first_free() * PAGE_SIZE);
	bitmap_set_frame(page);
	PHYS_BITFIELD_DEBUG_MSG("Page allocated: " << hex << (unsigned)page);
	return page;
}

void BitfieldPhysicalMemoryManager::DeallocateFrame(Address physAddr)
{    
	PHYS_BITFIELD_DEBUG_MSG("Page deallocated: " << hex << (unsigned)physAddr);
    bitmap_clear_frame(physAddr);
}
