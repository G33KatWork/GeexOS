#include <kernel/Memory/Virtual/VirtualMemoryManager.h>
#include <arch/Paging.h>

using namespace Arch;

#include <kernel/global.h>
using namespace IO;

#define DEBUG_MEMORY_MSG(x) DEBUG_MSG(x)
//#define DEBUG_MEMORY_MSG(x)

using namespace Memory;

VirtualMemoryManager::VirtualMemoryManager(size_t memsize)
{
    DEBUG_MEMORY_MSG("Initializing virtual kernel memory subsystem. Memory size: " << dec << (unsigned int)memsize << "KB");

    Paging::GetInstance()->Init();
    DEBUG_MEMORY_MSG("Paging initialized...");
}
