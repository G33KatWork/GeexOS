#include <kernel/Memory/Virtual/VirtualMemoryManager.h>
#include <arch/HAL.h>
#include <kernel/debug.h>
#include <string.h>
#include <kernel/Memory/IO/IOMemoryManager.h>
#include <arch/AddressLayout.h>

using namespace Arch;
using namespace Memory;
using namespace Debug;

VirtualMemoryManager* VirtualMemoryManager::instance = NULL;

VirtualMemoryManager* VirtualMemoryManager::GetInstance()
{
    if(instance == NULL)
        instance = new VirtualMemoryManager();

    return instance;
}

VirtualMemoryManager::VirtualMemoryManager()
    : kernelStack(NULL)
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
