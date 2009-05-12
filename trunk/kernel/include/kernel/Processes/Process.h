#ifndef PROCESSES_PROCESS_H_
#define PROCESSES_PROCESS_H_

#include <lib/types.h>
#include <kernel/Memory/Paging.h>
#include <arch/hal.h>

using namespace Memory;
using namespace Arch;

namespace Processes
{
    class Process
    {
    public:
        Process(unsigned int id, unsigned int sp, PageDirectory* pd);
        
        int GetPid() { return pid; }
        
        void UpdateLastRun() { lastRun = 0; }
        unsigned long GetLastRun() { return lastRun; }
        
        unsigned int GetBasePointer() { return basePointer; }
        void SetBasePointer(unsigned int p) { basePointer = p; }
        unsigned int GetStackPointer() { return stackPointer; }
        void SetStackPointer(unsigned int s) { stackPointer = s; }
        unsigned int GetInstructionPointer() { return instructionPointer; }
        void SetInstructionPointer(unsigned int i) { instructionPointer = i; }
        
        PageDirectory* GetPageDirectory() { return page_directory; }
        void SetPageDirectory(PageDirectory* p) { page_directory = p; }
        
    private:
        int pid;
        unsigned int stackPointer, basePointer;
        unsigned int instructionPointer;
        
        long lastRun;
        
        PageDirectory* page_directory;
    };
}
#endif
