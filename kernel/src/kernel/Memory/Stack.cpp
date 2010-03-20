#include <kernel/Memory/Stack.h>
#include <kernel/global.h>
#include <arch/Paging.h>
#include <arch/scheduling.h>
#include <lib/string.h>
#include <kernel/utils/DebuggingSymbols.h>

using namespace Memory;
using namespace Arch;
using namespace IO;

Stack::Stack(Address end, size_t s)
{
    ASSERT(end % PAGE_SIZE == 0, "Stack end (lowermost address) must be page aligned");
    ASSERT((end + s) % PAGE_SIZE == 0, "Stack beginning (uppermost address) must be page aligned");
    
    this->endAddr = end;
    this->size = s;
}

void Stack::AllocateSpace()
{
    //Assign space to our defined addresses
    size_t i = 0;
    while(i < size)
    {
        //FIXME: make this work again, if we use this class!!1!
        //Address newFrame = memoryManager.AllocateFrame();
        Address newFrame = NULL;
        Paging::GetInstance()->MapAddress(endAddr + i, newFrame, true, false);
        i += PAGE_SIZE;
    }
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
        
        if (( oldStackPointer < tmp) && (tmp <= initialESP))
        {
            tmp = tmp + offset;
            Address *tmp2 = (Address*)i;
            *tmp2 = tmp;
        }
    }

    writeStackPointer(newStackPointer);
    writeBasePointer(newBasePointer);
}

void Stack::PrintStacktrace(unsigned int n)
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
        char *symName = Debug::FindSymbol(eip, &symStart);
        if(symName == NULL)
            kdbg << "\t" << "<" << hex << eip << ">\t" << "<unknown symbol>" << endl;
        else
            kdbg << "\t" << "<" << hex << symStart << ">\t" << symName << endl;
    }
}

//debug version...
/*void Stack::PrintStacktrace(unsigned int n)
{
    unsigned int *stack = &n;
    for(int i = 0; i < 60; i++)
        kdbg << hex << (unsigned)(((unsigned)stack) + (i*sizeof(unsigned int))) << ": " << stack[i] << endl;
    
    // Stack contains:
    //  function argument (n)
    //  ebp of calling function (pointed to by current ebp)
    //  Return address in calling function
    unsigned int * ebp = &n + 1;
    DEBUG_MSG("address of n: " << hex << (unsigned)&n);
    DEBUG_MSG("initial ebp: " << hex << (unsigned)ebp);
    kdbg << "Stack trace:" << endl;
    
    for(unsigned int frame = 0; frame < n; ++frame)
    {
        DEBUG_MSG("frame " << dec << frame);
        
        if((Address)ebp == GetStartAddress() || ebp[1] == 0) // No caller on stack
            break;
        
        unsigned int eip = ebp[1];
        DEBUG_MSG("location of EIP on stack: " << hex << (unsigned)(ebp + 1));
        DEBUG_MSG("eip of caller: " << hex << eip);
            
        // Unwind to previous stack frame
        ebp = reinterpret_cast<unsigned int *>(ebp[0]);
        DEBUG_MSG("ebp of caller: " << hex << (unsigned)ebp);
        //unsigned int * arguments = &ebp[2];
        
        //get symbol name
        Address symStart;
        char *symName = Debug::FindSymbol(eip, &symStart);
        if(symName == NULL)
            kdbg << "\t" << "<" << hex << eip << ">\t" << "<unknown symbol>" << endl;
        else
        {
            //LargeStaticString demangled = LargeStaticString();
            //LargeStaticString toDemangle = LargeStaticString((const char*)symName);
            //demangle_full(toDemangle, demangled);
            //kdbg << "\t" << "<" << hex << symStart << ">\t" << demangled << endl;
            kdbg << "\t" << "<" << hex << symStart << ">\t" << symName << endl;
        }
        
        DEBUG_MSG("next stackframe...");
        //kout << "\t" << hex << eip << endl;
    }
}*/
