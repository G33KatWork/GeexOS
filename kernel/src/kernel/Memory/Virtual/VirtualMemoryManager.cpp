#include <kernel/Memory/Virtual/VirtualMemoryManager.h>
#include <arch/Paging.h>
#include <kernel/debug.h>

using namespace Arch;
using namespace Memory;
using namespace IO;

VirtualMemoryManager::VirtualMemoryManager(size_t memsize)
{
    VIRTUAL_MEMORY_MANAGER_DEBUG_MSG("Initializing virtual kernel memory subsystem. Memory size: " << dec << (unsigned int)memsize << "KB");

    Paging::GetInstance()->Init();
    VIRTUAL_MEMORY_MANAGER_DEBUG_MSG("Paging initialized...");
}
