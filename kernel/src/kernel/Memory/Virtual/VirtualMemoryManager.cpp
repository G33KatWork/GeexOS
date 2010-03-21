#include <kernel/Memory/Virtual/VirtualMemoryManager.h>
#include <arch/Paging.h>
#include <kernel/debug.h>
#include <lib/string.h>

using namespace Arch;
using namespace Memory;
using namespace IO;

VirtualMemoryManager::VirtualMemoryManager(size_t MemorySize)
{
    VIRTUAL_MEMORY_MANAGER_DEBUG_MSG("Initializing virtual kernel memory subsystem. Memory size: " << dec << (unsigned int)MemorySize << "KB");

    Paging::GetInstance()->Init();
    VIRTUAL_MEMORY_MANAGER_DEBUG_MSG("Paging initialized...");
    
    phys = new BitfieldPhysicalMemoryManager(MemorySize);
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
