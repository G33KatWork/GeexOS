#ifndef _ARCHX86_THREADCONTEXT_H
#define _ARCHX86_THREADCONTEXT_H

#include <types.h>
#include <halinterface/BaseThreadContext.h>
#include <arch/x86Paging.h>
#include <arch/internal/SchedulingHelper.h>

namespace Arch
{   
    class x86ThreadContext : public BaseThreadContext<x86ThreadContext>
    {
    public:
        virtual void SetPageDirectory(BasePageDirectory* pd);
        virtual BasePageDirectory* GetPageDirectory();
        
        virtual void SaveThreadContextFromRegisters(registers_t* state);
        virtual void SwitchToThisContext();
        
        
        virtual Address GetStackPointer() { return threadInfo.esp; }
        virtual void SetStackPointer(Address s) { threadInfo.esp = s; }
        virtual Address GetFramePointer() { return threadInfo.ebp; }
        virtual void SetFramePointer(Address b) { threadInfo.ebp = b; }
        virtual Address GetInstructionPointer() { return threadInfo.eip; }
        virtual void SetInstructionPointer(Address i) { threadInfo.eip = i; }
        
        
        virtual void initializeForKernelmode(Address InitialIP, Address InitialSP, Address InitialFP, BasePageDirectory* PageDirectory);
        virtual void initializeForUsermode(Address InitialIP, Address InitialSP, Address InitialFP, BasePageDirectory* PageDirectory);
    
    private:
        bool comingFromUsermode(uint32_t cs) { return (cs & 0x3) == 0x3; }
    
    protected:
        x86ThreadInfo threadInfo;
        x86PageDirectory* pdir;
    };
}

#endif
