#include <kernel/Memory/Heap.h>
#include <arch/Paging.h>
#include <kernel/global.h>

using namespace Memory;
using namespace Arch;
using namespace IO;

static bool header_t_less_predicate(header_t* a, header_t* b)
{
    return a->size < b->size;
}

void Heap::expand(size_t newSize)
{
    DEBUG_MSG("Heap expanding to " << dec << (unsigned)newSize);
    ASSERT(newSize > endAddress - startAddress, "This is just impossible... No, do it yourself. I'm outta here!");
    
    //get nearest following page boundary
    if(newSize & IDENTITY_POSITION != 0)
    {
        newSize &= IDENTITY_POSITION;
        newSize += PAGE_SIZE;
    }
    
    ASSERT(startAddress + newSize <= maxAddress, "Heap got too big... Can't expand anymore.");
    
    size_t oldSize = endAddress - startAddress;
    size_t i = oldSize;
    while(i < newSize)
    {
        Address newFrame = memoryManager.AllocateFrame();
        Paging::GetInstance()->MapAddress(startAddress + i, newFrame, true, false);
        DEBUG_MSG("Mapped frame " << hex << (unsigned)newFrame << " to: " << hex << (unsigned)(startAddress + i));
        i += PAGE_SIZE;
    }
    
    endAddress = startAddress + newSize;
}

int Heap::findSmallestHole(size_t size, bool pageAlign)
{
    unsigned int i = 0;
    while(i < index->GetSize())
    {
        header_t *header = this->index->ItemAt(i);
        
        if(pageAlign)
        {
            Address location = (Address)header;
            int offset = 0;
            
            if ((((location+sizeof(header_t))) & IDENTITY_POSITION) != 0)
				offset = PAGE_SIZE - (location+sizeof(header_t)) % PAGE_SIZE;
			
			int hole_size = (int)header->size - offset;
            
            //Do we fit?
			if (hole_size >= (int)size)
				break;
        }
        else if (header->size >= size)
			break;
		
        i++;
    }
    
    if (i == index->GetSize())
		return -1; // We got to the end and didn't find anything.
	else
		return i;
}

Heap::Heap(Address location, size_t size, size_t initialSize)
{
    //Check for heap location to be page aligned and that the supplied parameters are fine
    ASSERT(location % PAGE_SIZE == 0, "Heap is not page aligned");
    ASSERT((location + size) % PAGE_SIZE == 0, "Heap is not page aligned");
    //ASSERT(initialSize > HEAP_INDEX_SIZE * sizeof(void*), "Initial size must be big enough to contain all indices");
    
    //Set some basic data
    this->startAddress = location;
    this->endAddress = startAddress;
    this->maxAddress = location + size;
    
    //Acquire some initial space from the paging subsystem
    this->expand(initialSize);
    
    //Create array for our indices
    //FIXME: This stuff here still uses the placement allocator, perhaps we can move it to the beap beginning?
    this->index = new OrderedArray<header_t*, HEAP_INDEX_SIZE>();
    this->index->SetPredicate(&header_t_less_predicate);
    
    //Setup the first hole
    header_t *hole = (header_t*)this->startAddress;
    hole->size = (unsigned)(endAddress - startAddress); //Why?
    hole->magic = HEAP_MAGIC;
    hole->isHole = true;
    this->index->Insert(hole);
    
    //TODO: 1. expand to initial size -> done
    //TODO: 2. place indices array at the beginning -> not really possible
    //TODO: 3. set start address AFTER indices array
    //TODO: 4. setup one biiiig hole over all the memory we have in our heap, since it's free :) -> done
}

void* Heap::Allocate(size_t len, bool pageAlign)
{
    //Every block gets header and footer - they want some space, too
    size_t newSize = len + sizeof(header_t) + sizeof(footer_t);
    
    int i = this->findSmallestHole(newSize, pageAlign);
    DEBUG_MSG("Found hole #" << dec << i);
    
    if(i == -1)     //no hole found ;(
    {
        //save some stuff
        size_t oldLength = this->endAddress - this->startAddress;
        Address oldEndAddress = this->endAddress;
        
        //expand, since we have no suitable holes
        this->expand(oldLength + newSize);
        size_t newLength = this->endAddress - this->startAddress;
        
        i = 0;
        int idx = -1;
        uint32_t value = 0;
        
        //Look for the endmost header
        while (i < (signed int) index->GetSize())
		{
			uint32_t tmp = (uint32_t)index->ItemAt(i);
			if (tmp > value)
			{
				value = tmp;
				idx = i;
			}
			i++;
		}
		
		//we found no header...
		if(idx == -1)
        {
            header_t *header = (header_t *)oldEndAddress;
			header->magic = HEAP_MAGIC;
			header->size = newLength - oldLength;
			header->isHole = true;
			footer_t *footer = (footer_t *) (oldEndAddress + header->size - sizeof(footer_t));
			footer->magic = HEAP_MAGIC;
			footer->header = header;
            index->Insert(header);
        }
        else
        {
            // The last header needs adjusting.
			header_t *header = index->ItemAt(idx);
			header->size += newLength - oldLength;
            // Rewrite the footer.
			footer_t *footer = (footer_t *) ( (uint32_t)header + header->size - sizeof(footer_t) );
			footer->header = header;
			footer->magic = HEAP_MAGIC;
		}
		
		// We now have enough space. Recurse, and call the function again.
		return this->Allocate(len, pageAlign);
    }
    
    header_t *origHoleHeader = index->ItemAt(i);
	uint32_t origHolePos = (uint32_t)origHoleHeader;
	uint32_t origHoleSize = origHoleHeader->size;
    // Here we work out if we should split the hole we found into two parts.
    // Is the original hole size - requested hole size less than the overhead for adding a new hole?
	if (origHoleSize - newSize < sizeof(header_t) + sizeof(footer_t))
	{
        DEBUG_MSG("No, we don't the hole...");
        // Then just increase the requested size to the size of the hole we found.
		len += origHoleSize - newSize;
		newSize = origHoleSize;
	}

    // If we need to page-align the data, do it now and make a new hole in front of our block.
	if (pageAlign && origHolePos & IDENTITY_POSITION)
	{
		uint32_t newLocation  = origHolePos + PAGE_SIZE - (origHolePos & 0xFFF) - sizeof(header_t);
		header_t *holeHeader  = (header_t *)origHolePos;
		holeHeader->size      = PAGE_SIZE - (origHolePos & 0xFFF) - sizeof(header_t);
		holeHeader->magic     = HEAP_MAGIC;
		holeHeader->isHole    = true;
		footer_t *holeFooter  = (footer_t *) ( (uint32_t)newLocation - sizeof(footer_t) );
		holeFooter->magic     = HEAP_MAGIC;
		holeFooter->header    = holeHeader;
		origHolePos           = newLocation;
		origHoleSize          = origHoleSize - holeHeader->size;
	}
	else
	{
        // Else we don't need this hole any more, delete it from the index.
		index->RemoveAt(i);
	}

    DEBUG_MSG("OrigHolePos: " << hex << origHolePos);
    // Overwrite the original header...
	header_t *blockHeader   = (header_t *)origHolePos;
	blockHeader->magic      = HEAP_MAGIC;
	blockHeader->isHole     = false;
	blockHeader->size       = newSize;
    // ...And the footer
	footer_t *blockFooter   = (footer_t *) (origHolePos + sizeof(header_t) + len);
	DEBUG_MSG("Setting new footer to: " << hex << (unsigned)blockFooter);
	blockFooter->magic      = HEAP_MAGIC;
	blockFooter->header     = blockHeader;

    // We may need to write a new hole after the allocated block.
    // We do this only if the new hole would have positive size...
	if (origHoleSize - newSize > 0)
	{
        DEBUG_MSG("We need to write a new hole after our just allocated block...");
		header_t *holeHeader  = (header_t *) (origHolePos + sizeof(header_t) + len + sizeof(footer_t));
        DEBUG_MSG("Header of this new block will be at: " << hex << (unsigned)holeHeader);
		holeHeader->magic     = HEAP_MAGIC;
		holeHeader->isHole    = true;
		holeHeader->size      = origHoleSize - newSize;
		DEBUG_MSG("Size of this block: " << dec << (unsigned)holeHeader->size);
		footer_t *holeFooter  = (footer_t *) ( (uint32_t)holeHeader + origHoleSize - newSize - sizeof(footer_t) );
        DEBUG_MSG("Footer of this new block will be at: " << hex << (unsigned)holeFooter);
		if (((uint32_t)holeFooter + sizeof(footer_t)) <= this->endAddress)
		{
			holeFooter->magic = HEAP_MAGIC;
			holeFooter->header = holeHeader;
		}
        // Put the new hole in the index;
		index->Insert(holeHeader);
	}
    
    // ...And we're done!
	return (void *) ( (uint32_t)blockHeader + sizeof(header_t) );
}

void Heap::Deallocate(void* p)
{
    
}
