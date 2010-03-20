#include <kernel/Memory/Virtual/Allocator/StaticMemoryAllocator.h>
#include <kernel/debug.h>

using namespace Memory;
using namespace Arch;
using namespace IO;

StaticMemoryAllocator::StaticMemoryAllocator()
{
    SwappingAllowed = false;
}

void* StaticMemoryAllocator::Allocate(VirtualMemoryRegion* region, Address address, size_t size, AllocationFlags flags)
{
    return NULL;
}

void StaticMemoryAllocator::Deallocate(VirtualMemoryRegion* region, Address address)
{
    
}

void StaticMemoryAllocator::CorrectAllocationFlags(VirtualMemoryRegion* region, AllocationFlags flags)
{
    STATIC_ALLOC_DEBUG_MSG("Correcting alloc flags for " << region->Name);
    
    Address virtualAddress = region->StartAddress;
    size_t size = region->Size;
    
    STATIC_ALLOC_DEBUG_MSG("Starting address: " << hex << (unsigned)region->StartAddress);
    STATIC_ALLOC_DEBUG_MSG("Size: " << hex << (unsigned)region->Size);
    STATIC_ALLOC_DEBUG_MSG("New flags: " << ((flags.writable == 1)?"rw":"ro") << " " << ((flags.usermode == 1)?"umode":"kmode") << " " << ((flags.executable == 1)?"exec":"noexec"));
    
    for(Address i = virtualAddress; i < PAGE_ALIGN(virtualAddress + size); i += PAGE_SIZE)
    {
        Paging::GetInstance()->MapAddress(
            i,
            Paging::GetInstance()->GetPhysicalAddress(i),
            flags.writable == 1,
            flags.usermode == 1
        );
    }
}
