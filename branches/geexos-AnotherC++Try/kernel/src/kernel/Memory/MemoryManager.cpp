#include <kernel/Memory/MemoryManager.h>
#include <lib/string.h>
#include <kernel/global.h>
#include <kernel/Memory/Paging.h>

using namespace Memory;
using namespace Lib;

#define INDEX_FROM_BIT(a) (a/(8*4))
#define OFFSET_FROM_BIT(a) (a%(8*4))

extern unsigned int end;

MemoryManager::MemoryManager()
{
    //FIXME: Why +0x10000? if not, it would overwrite the ELF tables used by stacktraces
    placement_address = (unsigned int)&end + 0x20000;
    heapInitialized = false;
}

bool MemoryManager::HeapInitialized()
{
    return heapInitialized;
}

void* MemoryManager::kmalloc(size_t size, bool pageAlign, Address* physicalAddress)
{
    unsigned int tmp;
    
    if(pageAlign && (placement_address & 0xFFFFF000))
    {
        // Align the placement address;
        placement_address &= 0xFFFFF000;
        placement_address += 0x1000;
    }
    
    tmp = placement_address;
    
    if(physicalAddress != NULL)
        *physicalAddress = tmp;

    placement_address += size;
    return (void*)tmp;
}

void* MemoryManager::kcalloc(unsigned int n, size_t s, bool pageAlign, Address* physicalAddress)
{
    void* first = kmalloc(s, pageAlign, physicalAddress);
    for(unsigned int i = 1; i < n; i++)
        kmalloc(s, pageAlign);
    
    return first;
}

void MemoryManager::free(void* p)
{
    
}

unsigned int MemoryManager::bitmap_get_first_free(void)
{
	for(unsigned int i = 0; i < INDEX_FROM_BIT(nFrames); i++)
	{
		if(frames[i] != 0xFFFFFFFF)	//nothing is free, now we have a problem!
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

void MemoryManager::bitmap_set_frame(Address physAddr)
{
	Address frame = physAddr / PAGE_SIZE; //FIXME: Make this platform independent
	unsigned int idx = INDEX_FROM_BIT(frame);
	unsigned int off = OFFSET_FROM_BIT(frame);
	frames[idx] |= (0x1 << off);
}

void MemoryManager::InitializeFrameAllocator(unsigned int memorySize)
{
    //setup bitmap
	nFrames = memorySize / 4;
	frames = (unsigned int*)kmalloc(INDEX_FROM_BIT(nFrames), false, NULL);
	memset(frames, 0, INDEX_FROM_BIT(nFrames));
	
	//mark 0 to 4MB for kernel as used
	for(unsigned int i = 0; i < 1024; i++)
		bitmap_set_frame(i*PAGE_SIZE);
}

Address MemoryManager::AllocateFrame()
{
    Address page = (bitmap_get_first_free() * PAGE_SIZE);
	bitmap_set_frame(page);
	return page;
}
