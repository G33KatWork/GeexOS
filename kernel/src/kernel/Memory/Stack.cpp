#include <kernel/Memory/Stack.h>
#include <kernel/global.h>
#include <arch/Paging.h>
#include <arch/scheduling.h>
#include <lib/string.h>

using namespace Memory;
using namespace Arch;
using namespace IO;

//TODO: check if this code is broken

Stack::Stack(Address end, size_t s)
{
    DEBUG_MSG("Creating stack at " << hex << (unsigned)end << " with size of " << dec <<( unsigned)(s/1024) << "KB");
    
    ASSERT(end % PAGE_SIZE == 0, "Stack end (lowermost address) must be page aligned");
    ASSERT((end + s) % PAGE_SIZE == 0, "Stack beginning (uppermost address) must be page aligned");
    
    this->endAddr = end;
    this->size = s;
    
    //Assign space to our defined addresses
    size_t i = 0;
    while(i < s)
    {
        Address newFrame = memoryManager.AllocateFrame();
        Paging::GetInstance()->MapAddress(end + i, newFrame, true, false);
        i += PAGE_SIZE;
    }
}

Address Stack::GetEndAddress()
{
    return endAddr;
}

unsigned int Stack::GetSize()
{
    return size;
}

Address Stack::GetStartAddress()
{
    return endAddr + size;
}

void Stack::MoveCurrentStackHere(Address initialESP)
{
    Address oldStackPointer = readStackPointer();
    Address oldBasePointer = readBasePointer();
    
    Address offset = GetStartAddress() - initialESP;
    
    Address newStackPointer = oldStackPointer + offset;
    Address newBasePointer = oldBasePointer + offset;
    
    //Copy the stack
    memcpy((void*)newStackPointer, (const void*)oldStackPointer, initialESP - oldStackPointer);

    //Correct base pointers
    for(Address i = GetStartAddress() - 0x4; i > GetEndAddress(); i -= 4)
    {
        Address tmp = * (Address*)i;
        
        if (( oldStackPointer < tmp) && (tmp < initialESP))
        {
            tmp = tmp + offset;
            Address *tmp2 = (Address*)i;
            *tmp2 = tmp;
        }
    }

    writeStackPointer(newStackPointer);
    writeBasePointer(newBasePointer);
}

