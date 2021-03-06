#include <kernel/Memory/Virtual/VirtualMemoryManager.h>
#include <arch/HAL.h>
#include <kernel/debug.h>
#include <string.h>

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
    : iomanager(NULL),
    currentSpace(NULL),
    kernelSpace(NULL)
{}

void VirtualMemoryManager::Init()
{
    iomanager = new IOMemoryManager(KERNEL_IO_MEMORY_START, KERNEL_IO_MEMORY_END - KERNEL_IO_MEMORY_START);
    VIRTUAL_MEMORY_MANAGER_DEBUG_MSG("IO memory manager initialized...");
}

void VirtualMemoryManager::AddSpace(VirtualMemorySpace* space)
{
    VIRTUAL_MEMORY_MANAGER_DEBUG_MSG("Adding VirtualMemorySpace " << space->name);
    
    memorySpaceList.Append(space);
}

void VirtualMemoryManager::RemoveSpace(VirtualMemorySpace* space)
{
    VIRTUAL_MEMORY_MANAGER_DEBUG_MSG("Removing VirtualMemorySpace " << space->name);
    
    memorySpaceList.Remove(space);
}

VirtualMemorySpace* VirtualMemoryManager::FindSpaceByName(const char* spaceName)
{
    for(VirtualMemorySpace* curSpace = memorySpaceList.Head(); curSpace != NULL; curSpace = memorySpaceList.GetNext(curSpace))
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
