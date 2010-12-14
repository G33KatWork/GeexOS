#include <kernel/Memory/Virtual/VirtualMemoryManager.h>
#include <arch/HAL.h>
#include <kernel/debug.h>
#include <string.h>
#include <kernel/Memory/IO/IOMemoryManager.h>
#include <arch/AddressLayout.h>

using namespace Arch;
using namespace Memory;
using namespace Debug;

/* We initialize the VirtualMemoryManager statically, because otherwise the new
 * operator would again call GetInstance() to see if the slab allocator was
 * initialized. And again, and again, and again. And everything breaks due to
 * an overflown stack.
 */

VirtualMemoryManager VirtualMemoryManager::instance = VirtualMemoryManager();

VirtualMemoryManager* VirtualMemoryManager::GetInstance() { return &instance; }

VirtualMemoryManager::VirtualMemoryManager()
    : phys(NULL),
    iomanager(NULL),
    currentSpace(NULL),
    kernelSpace(NULL),
    kernelStack(NULL),
    kernelElf(NULL),
    kernelThreadStacks(NULL),
    slabAllocator(NULL),
    SpaceListHead(NULL)
{}

void VirtualMemoryManager::Init(size_t MemorySize)
{
    VIRTUAL_MEMORY_MANAGER_DEBUG_MSG("Initializing virtual kernel memory subsystem. Memory size: " << dec << (unsigned int)MemorySize << "KB");
    
    phys = new BitfieldPhysicalMemoryManager(MemorySize);
    iomanager = new IOMemoryManager(KERNEL_IO_MEMORY_START, KERNEL_IO_MEMORY_END - KERNEL_IO_MEMORY_START);
    VIRTUAL_MEMORY_MANAGER_DEBUG_MSG("Physical memory manager initialized...");
}

void VirtualMemoryManager::AddSpace(VirtualMemorySpace* space)
{
    VIRTUAL_MEMORY_MANAGER_DEBUG_MSG("Adding VirtualMemorySpace " << space->name);
    
    space->Next = SpaceListHead;
    SpaceListHead = space;
}

void VirtualMemoryManager::RemoveSpace(VirtualMemorySpace* space)
{
    VIRTUAL_MEMORY_MANAGER_DEBUG_MSG("Removing VirtualMemorySpace " << space->name);
    
    if(SpaceListHead == space)
    {
        SpaceListHead = space->Next;
        return;
    }
    
    VirtualMemorySpace* curItem = SpaceListHead;
    
    while(curItem->Next != space && curItem->Next != NULL)
        	curItem = curItem->Next;
    
    if(curItem->Next == space)
        curItem->Next = curItem->Next->Next;
}

VirtualMemorySpace* VirtualMemoryManager::FindSpaceByName(const char* spaceName)
{
    for(VirtualMemorySpace* curSpace = SpaceListHead; curSpace != NULL; curSpace = curSpace->Next)
    {
        if(!strcmp(curSpace->name, spaceName))
            return curSpace;
    }
    
    return NULL;
}

void VirtualMemoryManager::SetCurrentMemorySpace(VirtualMemorySpace* newSpace)
{
    VIRTUAL_MEMORY_MANAGER_DEBUG_MSG("Setting new current VirtualMemorySpace " << newSpace->name);
    
    CurrentHAL->GetPaging()->SwitchCurrentPageDirectory(newSpace->pageDirectory);
    currentSpace = newSpace;
}
