#ifndef PROCESSES_THREAD_H_
#define PROCESSES_THREAD_H_

#include <lib/types.h>
#include <kernel/Memory/Paging.h>
#include <arch/hal.h>

using namespace Memory;
using namespace Arch;

#include <kernel/global.h>
using namespace IO;

namespace Processes
{
    class Thread
    {
    public:
		Thread(unsigned int id, PageDirectory* pd);
        
        int GetId() { return tid; }
        PageDirectory* GetPageDirectory() { return page_directory; }
        void SetPageDirectory(PageDirectory* p) { page_directory = p; }
        int GetPriority() { return priority; }
        void SetPriority(unsigned char p) { priority = p; }
        unsigned long GetTimeslice() { return timeslice; }
        void SetTimeslice(unsigned long t) { timeslice = t; }
        Address GetStackPointer() { return stackPointer; }
        void SetStackPointer(Address s) { stackPointer = s; }
         Address GetBasePointer() { return basePointer; }
        void SetBasePointer(Address b) { basePointer = b; }
        Address GetInstructionPointer() { return instructionPointer; }
        void SetInstructionPointer(Address i) { instructionPointer = i; }
        
        void Sleep();
        void Wakeup();
		void SwitchTo();
        
    private:
        int tid;
        unsigned char priority;
        unsigned long timeslice;
        PageDirectory* page_directory;
		Address stackPointer;
		Address basePointer;
		Address instructionPointer;
    };
}
#endif
