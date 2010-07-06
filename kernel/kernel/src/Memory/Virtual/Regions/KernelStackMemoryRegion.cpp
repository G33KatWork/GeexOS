#include <kernel/Memory/Virtual/Regions/KernelStackMemoryRegion.h>
#include <arch/HAL.h>
#include <kernel/Memory/Virtual/VirtualMemoryManager.h>
#include <kernel/global.h>
#include <kernel/debug.h>
#include <string.h>

using namespace Memory;
using namespace Arch;
using namespace IO;

KernelStackMemoryRegion::KernelStackMemoryRegion(Address RegionStart, size_t MaxRegionSize, size_t InitialSize, const char* RegionName)
: LazyMemoryRegion(RegionStart, MaxRegionSize, RegionName, ALLOCFLAG_WRITABLE)
{
    STACK_DEBUG_MSG("Creating kernel stack region " << RegionName);
    STACK_DEBUG_MSG("Region start: " << hex << RegionStart
                 << " Max stack size: " << hex << MaxRegionSize
                 << " Initial stack size: " << hex << InitialSize);
    
    curSize = initialSize = InitialSize;
    
    //map initial stack space
    for(Address i = startAddress + MaxRegionSize - PAGE_SIZE; i > (startAddress + MaxRegionSize - PAGE_SIZE) - initialSize; i -= PAGE_SIZE)
    {
        STACK_DEBUG_MSG("Initial mapping " << hex << i);
        CurrentHAL->GetPaging()->MapAddress(
            i,
            VirtualMemoryManager::GetInstance()->PhysicalAllocator()->AllocateFrame(),
            true,   /* writable */
            false   /* usermode */
            );
    }
}

void KernelStackMemoryRegion::MoveCurrentStackHere(Address initialESP)
{
    STACK_DEBUG_MSG("Moving stack from " << initialESP);
    
    Address oldStackPointer = readStackPointer();
    Address oldBasePointer = readBasePointer();
    
    STACK_DEBUG_MSG("Old Stackpointer: " << oldStackPointer);
    STACK_DEBUG_MSG("Old Basepointer: " << oldBasePointer);
    
    Address offset = GetStartAddress() - initialESP;
    
    STACK_DEBUG_MSG("Offset between old and new Stack: " << offset);
    
    Address newStackPointer = oldStackPointer + offset;
    Address newBasePointer = oldBasePointer + offset;
    
    STACK_DEBUG_MSG("New Stackpointer: " << newStackPointer);
    STACK_DEBUG_MSG("New Basepointer: " << newBasePointer);
    
    //Copy the stack
    memcpy((void*)newStackPointer, (const void*)oldStackPointer, initialESP - oldStackPointer);

    //Correct base pointers
    STACK_DEBUG_MSG("Correcting basepointers from " << GetStartAddress() - 0x4 << " to " << GetStartAddress() - (initialESP - oldStackPointer));
    for(Address i = GetStartAddress() - 0x4; i > GetStartAddress() - (initialESP - oldStackPointer); i -= 4)
    {
        Address tmp = * (Address*)i;
        
        if (( oldStackPointer < tmp) && (tmp <= initialESP))
        {
            tmp = tmp + offset;
            Address *tmp2 = (Address*)i;
            *tmp2 = tmp;
        }
    }

    STACK_DEBUG_MSG("Stack moved. Switching...");

    writeStackPointer(newStackPointer);
    writeBasePointer(newBasePointer);
    
    STACK_DEBUG_MSG("Now living on new stack.");
}

void KernelStackMemoryRegion::PrintStacktrace(unsigned int n)
{
    // Stack contains:
    //  function argument (n)
    //  ebp of calling function (pointed to by current ebp)
    //  Return address in calling function
    unsigned int * ebp = &n + 1;
    kdbg << "Stack trace:" << endl;
    
    for(unsigned int frame = 0; frame < n; ++frame)
    {
        if((Address)ebp == GetStartAddress() || ebp[1] == 0) // No caller on stack
            break;
        
        unsigned int eip = ebp[1];
            
        // Unwind to previous stack frame
        ebp = reinterpret_cast<unsigned int *>(ebp[0]);
        //unsigned int * arguments = &ebp[2];
        
        //get symbol name
        Address symStart;
        char *symName = VirtualMemoryManager::GetInstance()->KernelElf()->FindSymbol(eip, &symStart);
        if(symName == NULL)
            kdbg << "\t" << "<" << hex << eip << ">\t" << "<unknown symbol>" << endl;
        else
            kdbg << "\t" << "<" << hex << symStart << ">\t" << symName << endl;
    }
}
