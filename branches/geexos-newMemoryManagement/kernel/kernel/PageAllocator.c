#include "PageAllocator.h"
#include "MemoryManager.h"
#include "arch/x86/paging.h"
#include <string.h>

uint32_t *frames;
uint32_t nFrames;

#define INDEX_FROM_BIT(a) (a/(8*4))
#define OFFSET_FROM_BIT(a) (a%(8*4))

static uint32_t bitmap_get_first_free(void)
{
	for(uint32_t i = 0; i < INDEX_FROM_BIT(nFrames); i++)
	{
		if(frames[i] != 0xFFFFFFFF)	//nothing is free, now we have a problem!
		{
			// at least one bit is free here
			for(uint32_t j = 0; j < 32; j++)
			{
				uint32_t toTest = 0x1 << j;
				if( !(frames[i]&toTest) )
					return (i*4*8+j) * 0x1000;	//32 bit (4*8) per entry; *0x1000 for physical address
			}
		}
	}
	
	//TODO: Add proper error handling
	return -1;
}

static void bitmap_set_frame(uint32_t physAddr)
{
	uint32_t frame = physAddr / 0x1000;
	uint32_t idx = INDEX_FROM_BIT(frame);
	uint32_t off = OFFSET_FROM_BIT(frame);
	frames[idx] |= (0x1 << off);
}

static void bitmap_clear_frame(uint32_t physAddr)
{
	uint32_t frame = physAddr / 0x1000;
	uint32_t idx = INDEX_FROM_BIT(frame);
	uint32_t off = OFFSET_FROM_BIT(frame);
	frames[idx] &= ~(0x1 << off);
}


void init_allocator(uint32_t memorySize)
{
	//setup bitmap
	nFrames = memorySize / 4;
	frames = (uint32_t*)kmalloc(INDEX_FROM_BIT(nFrames));
	memset(frames, 0, INDEX_FROM_BIT(nFrames));
	
	//mark 0 to 4MB for kernel as used
	for(int i = 0; i < 1024; i++)
		bitmap_set_frame(i*0x1000);
}

uint32_t allocate_page()
{
	uint32_t page = bitmap_get_first_free();
	bitmap_set_frame(page);
	return page;
}

void free_page(uint32_t physAddr)
{
	bitmap_clear_frame(physAddr);
}
