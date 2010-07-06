#ifndef _HAL_INTERFACES_THREADCONTEXT_H
#define _HAL_INTERFACES_THREADCONTEXT_H

#include <types.h>
#include <halinterface/BasePaging.h>

namespace Arch
{
    template<typename DervivedContext>
    class BaseThreadContext
    {
    public:
        virtual ~BaseThreadContext() {}
        
        virtual void SetPageDirectory(BasePageDirectory* pdir) = 0;
        virtual BasePageDirectory* GetPageDirectory() = 0;
        
        virtual void SaveThreadContextFromRegisters(registers_t* state) = 0;
        virtual void SwitchToThisContext() = 0;
        
        
        virtual Address GetStackPointer() = 0;
        virtual void SetStackPointer(Address s) = 0;
        virtual Address GetFramePointer() = 0;
        virtual void SetFramePointer(Address b) = 0;
        virtual Address GetInstructionPointer() = 0;
        virtual void SetInstructionPointer(Address i) = 0;
        
        
        virtual void initializeForKernelmode(Address InitialIP, Address InitialSP, Address InitialFP, BasePageDirectory* PageDirectory) = 0;
        virtual void initializeForUsermode(Address InitialIP, Address InitialSP, Address InitialFP, BasePageDirectory* PageDirectory) = 0;
        
        
        /* Initialization helper */
        static DervivedContext* BuildForKernelmode(Address InitialIP, Address InitialSP, Address InitialFP, BasePageDirectory* PageDirectory)
        {
            DervivedContext* con = new DervivedContext();
            con->initializeForKernelmode(InitialIP, InitialSP, InitialFP, PageDirectory);
            return con;
        }
        
        static DervivedContext* BuildForUsermode(Address InitialIP, Address InitialSP, Address InitialFP, BasePageDirectory* PageDirectory)
        {
            DervivedContext* con = new DervivedContext();
            con->initializeForUsermode(InitialIP, InitialSP, InitialFP, PageDirectory);
            return con;
        }
    };
}

#endif
