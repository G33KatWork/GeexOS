#ifndef PROCESSES_THREAD_H_
#define PROCESSES_THREAD_H_

#include <arch/types.h>
#include <arch/HAL.h>

using namespace Arch;

namespace Processes
{
    enum ThreadState {
        THREAD_SLEEPING,
        THREAD_RUNNING
    };
    
    class Thread
    {
        friend class Scheduler;
        
    public:
		Thread(unsigned int threadId, Address initialIP, Address initialSP, Address initialBP, const char* threadName, bool umode, BasePageDirectory* pd);
        
        int GetId() { return tid; }
        const char* GetName() { return name; }
        BasePageDirectory* GetPageDirectory() { return threadContext->GetPageDirectory(); }
        void SetPageDirectory(BasePageDirectory* p) { threadContext->SetPageDirectory(p); }
        /*int GetPriority() { return priority; }
        void SetPriority(unsigned char p) { priority = p; }*/
        unsigned long GetTimeslice() { return timeslice; }
        void SetTimeslice(unsigned long t) { timeslice = t; }
        Address GetStackPointer() { return threadContext->GetStackPointer(); }
        void SetStackPointer(Address s) { threadContext->SetStackPointer(s); }
         Address GetFramePointer() { return threadContext->GetFramePointer(); }
        void SetFramePointer(Address f) { threadContext->SetFramePointer(f); }
        Address GetInstructionPointer() { return threadContext->GetInstructionPointer(); }
        void SetInstructionPointer(Address i) { threadContext->SetInstructionPointer(i); }
        ThreadState GetThreadState() { return state; }
        bool IsUsermode() { return usermode; }
        
        void SaveThreadContext(registers_t* regs) { threadContext->SaveThreadContextFromRegisters(regs); }
        void SwitchTo() { threadContext->SwitchToThisContext(); }
        
        void Sleep() { state = THREAD_SLEEPING; }
        void Wakeup() { state = THREAD_RUNNING; }
        
    protected:
        int tid;
        //unsigned char priority;
        ThreadState state;
        ThreadContext* threadContext;
        unsigned long timeslice;
        const char* name;
        bool usermode;
        
    private:
        Thread* next;
    };
}
#endif
