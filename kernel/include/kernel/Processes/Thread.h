#ifndef PROCESSES_THREAD_H_
#define PROCESSES_THREAD_H_

#include <lib/types.h>
#include <arch/Paging.h>
#include <arch/hal.h>
#include <arch/scheduling.h>

using namespace Arch;

namespace Processes
{
    class Thread
    {
        friend class Scheduler;
        
    public:
		Thread(unsigned int id, Address initialIP, Address initialSP, Address initialBP, const char* threadName);
        
        int GetId() { return tid; }
        const char* GetName() { return name; }
        /*PageDirectory* GetPageDirectory() { return page_directory; }
        void SetPageDirectory(PageDirectory* p) { page_directory = p; }
        int GetPriority() { return priority; }
        void SetPriority(unsigned char p) { priority = p; }*/
        unsigned long GetTimeslice() { return timeslice; }
        void SetTimeslice(unsigned long t) { timeslice = t; }
        Address GetStackPointer() { return threadInfo.esp; }
        void SetStackPointer(Address s) { threadInfo.esp = s; }
         Address GetBasePointer() { return threadInfo.ebp; }
        void SetBasePointer(Address b) { threadInfo.ebp = b; }
        Address GetInstructionPointer() { return threadInfo.eip; }
        void SetInstructionPointer(Address i) { threadInfo.eip = i; }
        
        void Sleep();
        void Wakeup();
        
    private:
        int tid;
        //unsigned char priority;
        ThreadInfo threadInfo;
        unsigned long timeslice;
        const char* name;
    };
}
#endif
