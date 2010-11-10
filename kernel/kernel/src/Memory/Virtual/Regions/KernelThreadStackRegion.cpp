#include <kernel/Memory/Virtual/Regions/KernelThreadStackRegion.h>

using namespace Memory;
using namespace Debug;

KernelThreadStack* KernelThreadStackMemoryRegion::CreateStack(size_t StackSize)
{
    //WARNING: This function assumes that the list of allocated stacks is ordered by their start adresses!
    //This is normally assured by the AddToList and RemoveFromList methods. USE THEM!
    
    KERNEL_THREAD_STACK_DEBUG_MSG("Creating new kernel thread stack with size " << StackSize);
    
    ASSERT(IS_PAGE_ALIGNED(StackSize), "Stack size must be page aligned");
    
    //Initialize the list if neccessary
    if(!stackList)
    {
        KERNEL_THREAD_STACK_DEBUG_MSG("Allocation of first stack, this one is easy.");
        
        //Stack grows down, so it starts on the upper boundary
        MapFreshPages(startAddress, StackSize);
        stackList = new KernelThreadStack(startAddress, StackSize, NULL);
        return stackList;
    }
    
    KERNEL_THREAD_STACK_DEBUG_MSG("Not the first stack, finding a hole...");
    
    //we need to find a hole in the space if we are not the first stack to be allocated
    for(KernelThreadStack* cur = stackList; cur != NULL; cur = cur->next)
    {
        size_t holeSize;
        
        if(cur->next != NULL) //check if we are at the end of the list
            holeSize = cur->next->beginning - (cur->beginning + cur->stackSize);
        else
            holeSize = (startAddress + size) - (cur->beginning + cur->stackSize);
            
        KERNEL_THREAD_STACK_DEBUG_MSG("Hole between two stacks: " << hex << (unsigned)holeSize);
        
        if(holeSize >= StackSize)
        {
            KERNEL_THREAD_STACK_DEBUG_MSG("Hole is big enough...");
            KernelThreadStack* s = new KernelThreadStack(cur->beginning + cur->stackSize, StackSize, NULL);
            MapFreshPages(cur->beginning + cur->stackSize, StackSize);
            AddToList(s);
            return s;
        }
    }
    
    return NULL;
}

void KernelThreadStackMemoryRegion::DestroyStack(KernelThreadStack* stack)
{
    UnmapPages(stack->beginning, stack->stackSize);
    RemoveFromList(stack);
    delete stack;
}

bool KernelThreadStackMemoryRegion::HandlePageFault(Address faultingAddress)
{
    PANIC("Stack overflow in kernel thread stack. Faulting address: " << faultingAddress);
    return false;
}

void KernelThreadStackMemoryRegion::DumpStacks(BaseDebugOutputDevice* c)
{
    for(KernelThreadStack* cur = stackList; cur != NULL; cur = cur->next)
    {
        *c << "KTSTACK: " << "\tBeginning: " << cur->beginning << endl;
        *c << "KTSTACK: " << "\tSize: " << cur->stackSize << endl;
        *c << endl;
    }
}



void KernelThreadStackMemoryRegion::AddToList(KernelThreadStack* toAdd)
{
    ASSERT(stackList, "stackList should not be NULL anymore when AddToList is called");
    
    KERNEL_THREAD_STACK_DEBUG_MSG("Adding a stack to the list...");
	KERNEL_THREAD_STACK_DEBUG_MSG("beginning: " << toAdd->beginning);    

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

