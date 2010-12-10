#include <kernel/Memory/Virtual/Regions/BuddyAllocatedMemoryRegion.h>


#include <kernel/global.h>
#include <string.h>

using namespace Memory;
using namespace Debug;
using namespace DataStructures;

BuddyAllocatedMemoryRegion::BuddyAllocatedMemoryRegion(Address RegionStart, size_t RegionSize, const char* RegionName, size_t MinimumOrder)
    : LazyMemoryRegion(RegionStart, RegionSize, RegionName, ALLOCFLAG_WRITABLE)
{
    ASSERT(IsPowerOf2(RegionSize), "RegionSize must be power of 2");

	BUDDY_ALLOC_DEBUG_MSG("Initializing buddy allocator region \"" << RegionName << 
	                      "\" with start " << hex << RegionStart <<
	                      ", size " << RegionSize);

    minOrder = MinimumOrder;

	//Determine how much splits we can have
	maxOrder = 0;
	while(RegionSize > 1)
	{
		RegionSize >>= 1;
		maxOrder++;
	}
	
	BUDDY_ALLOC_DEBUG_MSG("Maximum order is due to given size " << dec << maxOrder);
	
	//Initialize tag bitmap
    size_t numBlocks = (1 << maxOrder) / (1 << minOrder);
    BUDDY_ALLOC_DEBUG_MSG("Maximum number of blocks is " << dec << numBlocks);
    elementsAllocated = new Bitfield(numBlocks);
	
	//Allocate list arrays
	freeElements = new struct BuddyElement*[maxOrder + 1];
	memset(freeElements, 0, sizeof(struct BuddyElement*) * (maxOrder + 1));
	
	//Map page for first buddy element metadata
	MapFreshPages(RegionStart, PAGE_SIZE);
	
	//Initialize first buddy element
	struct BuddyElement* firstElement = (struct BuddyElement*)RegionStart;
	firstElement->order = maxOrder;
	firstElement->next = NULL;
	
	//Add first buddy element to free lists
	freeElements[maxOrder] = firstElement;
}

bool BuddyAllocatedMemoryRegion::HandlePageFault(Address faultingAddress)
{
    VIRTUAL_MEMORY_REGION_DEBUG_MSG("Pagefault handler in BuddyAllocatedMemoryRegion called");
    return LazyMemoryRegion::HandlePageFault(faultingAddress); //hand up
}

Address BuddyAllocatedMemoryRegion::AllocateBuddy(size_t order)
{
    BUDDY_ALLOC_DEBUG_MSG("Allocating buddy with order " << dec << order);
    
    //we count the minimum order from 0, so add minOrder here
    order += minOrder;
    
    ASSERT(order <= maxOrder, "Order must not be greater than maximum order");
    if(order > maxOrder)
        return NULL;
    
    Address returnAddress = NULL;
    
    for(size_t i = order; i <= maxOrder; i++)
    {
        if(freeElements[i] == NULL)
            continue;
            
        BUDDY_ALLOC_DEBUG_MSG("Found free element at order " << dec << i);
        
        //remove from list
        struct BuddyElement* ret = freeElements[i];
        freeElements[i] = ret->next;
        //mark as allocated in bitmap
        MarkElementAllocated((Address)ret);
        returnAddress = (Address)ret;
        
        //we allocated an element too big, we need to split it
        while(i > order)
        {
            BUDDY_ALLOC_DEBUG_MSG("Found element was too big, splitting...");
            i--;
            struct BuddyElement* buddy = (struct BuddyElement*)(returnAddress + (1 << i));
            BUDDY_ALLOC_DEBUG_MSG("Created new buddy at address " << hex << (Address)buddy << " with order " << dec << i);
            buddy->order = i;
            MarkElementFree((Address)buddy);
            buddy->next = freeElements[i];
            freeElements[i] = buddy;
        }
        
        //we allocated our element and splitted it, if it was too big, we are done
        break;
    }
    
    BUDDY_ALLOC_DEBUG_MSG("Returning address " << hex << returnAddress);
    return returnAddress;
}

void BuddyAllocatedMemoryRegion::FreeBuddy(Address addr, size_t order)
{
    BUDDY_ALLOC_DEBUG_MSG("Freeing element at " << hex << addr << " with order " << dec << order);
    
    ASSERT(addr != NULL, "Address to free must not be 0");
    if(addr == NULL)
        return;
    
    //we count the minimum order from 0, so add minOrder here
    order += minOrder;
    
    ASSERT(order <= maxOrder, "Order must not be greater than maximum order " << dec << maxOrder);
    if(order > maxOrder)
        return;
    
    if(IsElementFree(addr))
    {
        ASSERT(false, "Element already is free");
        return;
    }
    
    struct BuddyElement* block = (struct BuddyElement*)addr;
    while(order < maxOrder)
    {
        struct BuddyElement* buddy = (struct BuddyElement*)((((Address)block - this->startAddress) ^ (1 << order)) + this->startAddress);
        
        BUDDY_ALLOC_DEBUG_MSG("Buddy for element to free at " << hex << (Address)block << " is at " << hex << (Address)buddy << " with order " << dec << order);
        if(!IsElementFree((Address)buddy))
        {
            BUDDY_ALLOC_DEBUG_MSG("Buddy is not marked as free, breaking up");
            break;
        }
        
        if(IsElementFree((Address)(buddy)) && buddy->order != order)
        {
            BUDDY_ALLOC_DEBUG_MSG("Buddy is marked as free, but order (" << dec << buddy->order << ") doesn't match, breaking up");
            break;
        }
        
        //merge buddies
        RemoveFromList(&freeElements[order], buddy);
        if((Address)buddy < (Address)block)
            block = buddy;
        order++;
        block->order = order;
    }
    
    block->order = order;
    MarkElementFree((Address)block);
    block->next = freeElements[order];
    freeElements[order] = block;
    
    BUDDY_ALLOC_DEBUG_MSG("No more buddies to merge");
}

void BuddyAllocatedMemoryRegion::DumpBuddyInfo(BaseDebugOutputDevice* c)
{
    *c << "BUDDY: " << this->Name() << ": ";
    for(size_t order = minOrder; order <= maxOrder; order++)
    {
        size_t free = 0;
        for(struct BuddyElement* curElem = freeElements[order]; curElem != NULL; curElem = curElem->next)
            free++;
        
        *c << dec << free << "\t";
    }
    *c << endl;
}

void BuddyAllocatedMemoryRegion::RemoveFromList(struct BuddyElement** listHead, struct BuddyElement* toRemove)
{
   if(*listHead == toRemove)
   {
       *listHead = toRemove->next;
       return;
   }

   for(struct BuddyElement* cur = *listHead; cur->next != NULL; cur = cur->next)
   {
       if(cur->next != NULL)
       {
           if(cur->next == toRemove)
           {
               cur->next = toRemove->next;
               return;
           }
       }
   }
   
   BUDDY_ALLOC_DEBUG_MSG("Element to remove from list not found");
}
