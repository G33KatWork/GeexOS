#include <arch/BitfieldPhysicalMemoryAllocator.h>
#include <arch/HAL.h>
#include <string.h>
#include <kernel/global.h>

using namespace Debug;
using namespace Arch;
using namespace DataStructures;

BitfieldPhysicalMemoryAllocator::BitfieldPhysicalMemoryAllocator(size_t memorySize)
{
    //setup bitmap
	nFrames = memorySize * 1024 / PAGE_SIZE;
	PHYS_BITFIELD_DEBUG_MSG("Initialization allocator with memsize of " << dec << memorySize << "KB which results in " << nFrames << " possible frames");
	
    frames = new Bitfield(nFrames);
}

Address BitfieldPhysicalMemoryAllocator::AllocateFrame()
{
    uint32_t pageIndex = frames->GetFirstClearedIndex();
    if(pageIndex == (uint32_t)-1)
        PANIC("Desperately out of memory: No more frames left to allocate");
    
	frames->SetBit(pageIndex);
	PHYS_BITFIELD_DEBUG_MSG("Page allocated: " << hex << pageIndex * PAGE_SIZE);
	return (Address)(pageIndex * PAGE_SIZE);
}

void BitfieldPhysicalMemoryAllocator::DeallocateFrame(Address physAddr)
{    
	PHYS_BITFIELD_DEBUG_MSG("Page deallocated: " << hex << physAddr);
    frames->ClearBit(physAddr / PAGE_SIZE);
}

void BitfieldPhysicalMemoryAllocator::MarkAsUsed(Address physAddr)
{
    PHYS_BITFIELD_DEBUG_MSG("Marking frame as used: " << hex << physAddr);
    if((physAddr / PAGE_SIZE) <= nFrames)
    {
        if(frames->IsBitSet(physAddr / PAGE_SIZE))
            PANIC("Frame at " << hex << physAddr << " already was marked as used");
        
        frames->SetBit(physAddr / PAGE_SIZE);
    }
}


bool BitfieldPhysicalMemoryAllocator::IsFree(Address physAddr)
{
    PHYS_BITFIELD_DEBUG_MSG("Checking if frame is allocated: " << hex << physAddr);
    uint32_t frameIndex = physAddr / PAGE_SIZE;
	if((physAddr / PAGE_SIZE) <= nFrames)
	    return !frames->IsBitSet(frameIndex);
	else
        return false;
}

void BitfieldPhysicalMemoryAllocator::DumpUsed(BaseOutputDevice* c)
{
    //Weird and slow code, just to make it look pretty... Anyway, it's just for debugging
    uint32_t printed = 0;
    bool firstPrint = false;
    
    for(uint32_t i = 0; i < frames->GetNumElements(); i++)
    {
        if(frames->IsBitSet(i))
        {
            if(!firstPrint)
                *c << "BFPHYSMEM: ";
            *c << hex << i * 4096 << "\t";
            printed++;
            firstPrint = true;
        }
        
        if(firstPrint && printed % 8 == 0)
            *c << endl << "BFPHYSMEM: ";
    }
    
    *c << endl << endl;
}
