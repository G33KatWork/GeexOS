#include <kernel/Memory/MemoryManager.h>
#include <arch/Paging.h>

using namespace Arch;

#include <kernel/global.h>
using namespace IO;

#define DEBUG_MEMORY_MSG(x) DEBUG_MSG(x)
//#define DEBUG_MEMORY_MSG(x)

using namespace Memory;

MemoryManager::MemoryManager(unsigned int memsize)
{
    DEBUG_MEMORY_MSG("Initializing memory subsystem. Memory size: " << dec << memsize/1024 << "KB");

    Paging::GetInstance()->Init();
    DEBUG_MEMORY_MSG("Paging initialized...");
}
