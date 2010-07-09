#include <kernel/Memory/Virtual/Regions/KernelThreadStackRegion.h>

using namespace Memory;
using namespace Debug;

KernelThreadStack* KernelThreadStackMemoryRegion::CreateStack(size_t InitialSize, size_t MaxSize)
{
    //WARNING: This function assumes that the list of allocated stacks is ordered by their start adresses!
    //This is normally assured by the AddToList and RemoveFromList methods. USE THEM!
    
    KERNEL_THREAD_STACK_DEBUG_MSG("Creating new kernel thread stack with"
                                  << " initial size " << (unsigned)InitialSize
                                  << " and max size " << (unsigned)MaxSize
                                  );
    
    ASSERT(MaxSize >= InitialSize, "Initial size cannot be bigger than max size");
    ASSERT(IS_PAGE_ALIGNED(InitialSize) && IS_PAGE_ALIGNED(MaxSize), "Initial size and max size must be page aligned");
    
    //Initialize the list if neccessary
    if(!stackList)
    {
        KERNEL_THREAD_STACK_DEBUG_MSG("Allocation of first stack, this one is easy.");
        
        //Stack grows down, so it starts on the upper boundary
        MapFreshPages(startAddress + MaxSize - InitialSize, InitialSize);
        stackList = new KernelThreadStack(startAddress, InitialSize, MaxSize, NULL);
        return stackList;
    }
    
    KERNEL_THREAD_STACK_DEBUG_MSG("Not the first stack, finding a hole...");
    
    //we need to find a hole in the space if we are not the first stack to be allocated
    for(KernelThreadStack* cur = stackList; cur != NULL; cur = cur->next)
    {
        size_t holeSize;
        
        if(cur->next != NULL) //check if we are at the end of the list
            holeSize = cur->next->beginning - (cur->beginning + cur->maxSize);
        else
            holeSize = (startAddress + size) - (cur->beginning + cur->maxSize);
            
        KERNEL_THREAD_STACK_DEBUG_MSG("Hole between two stacks: " << hex << (unsigned)holeSize);
        
        if(holeSize >= MaxSize)
        {
            KERNEL_THREAD_STACK_DEBUG_MSG("Hole is big enough...");
            KernelThreadStack* s = new KernelThreadStack(cur->beginning + cur->maxSize, InitialSize, MaxSize, NULL);
            MapFreshPages(cur->beginning + cur->maxSize + MaxSize - InitialSize, InitialSize);
            AddToList(s);
            return s;
        }
    }
    
    return NULL;
}

void KernelThreadStackMemoryRegion::DestroyStack(KernelThreadStack* stack)
{
    UnmapPages(stack->beginning + stack->maxSize - stack->curSize, stack->curSize);
    RemoveFromList(stack);
    delete stack;
}

bool KernelThreadStackMemoryRegion::HandlePageFault()
{
    //TODO: Determine the corresponding thread stack and expand it appropriately
    //      alternatively we need to panic here. We can't hand this fault up
    //      because LazyMemoryRegion doesn't know about our stacks here
    PANIC("Dynamic growing of kernel thread stacks not yet implemented");
    
    //We handle this stuff here or we are in kernel panic land, so return true
    return true;
}

void KernelThreadStackMemoryRegion::DumpStacks(BaseDebugOutputDevice* c)
{
    for(KernelThreadStack* cur = stackList; cur != NULL; cur = cur->next)
    {
        *c << "KTSTACK: " << "\tBeginning: " << cur->beginning << endl;
        *c << "KTSTACK: " << "\tMax Size: " << (unsigned)cur->maxSize << endl;
        *c << "KTSTACK: " << "\tCur Size: " << (unsigned)cur->curSize << endl;
        *c << endl;
    }
}



void KernelThreadStackMemoryRegion::AddToList(KernelThreadStack* toAdd)
{
    ASSERT(stackList, "stackList should not be NULL anymore when AddToList is called");
    
    KERNEL_THREAD_STACK_DEBUG_MSG("Adding a stack to the list...");
    
    for(KernelThreadStack* cur = stackList; cur != NULL; cur = cur->next)
    {
        if(cur->next == NULL) //end of list?
        {
            KERNEL_THREAD_STACK_DEBUG_MSG("We reached the end of the list, just appending...");
            
            toAdd->next = NULL;
            cur->next = toAdd;
            break;
        }
        else
        {
            if(toAdd->beginning > cur->beginning && toAdd->beginning < cur->next->beginning)
            {
                KERNEL_THREAD_STACK_DEBUG_MSG("Found the proper place between a: " << hex << cur->beginning << " and b: " << cur->next->beginning);
                
                toAdd->next = cur->next;
                cur->next = toAdd;
                break;
            }
        }
    }
}

void KernelThreadStackMemoryRegion::RemoveFromList(KernelThreadStack* toRemove)
{
    ASSERT(stackList, "stackList should not be NULL anymore when RemoveFromList is called");
    
    if(stackList == toRemove)
    {
        stackList = toRemove->next;
        return;
    }
    
    for(KernelThreadStack* cur = stackList; cur != NULL; cur = cur->next)
    {
        if(cur->next != NULL)
        {
            if(cur->next == toRemove)
            {
                cur->next = toRemove->next;
                break;
            }
        }
    }
}

