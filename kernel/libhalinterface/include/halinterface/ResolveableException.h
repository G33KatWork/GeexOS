#ifndef _HAL_INTERFACES_RESOLVEABLEEXCEPTION_H
#define _HAL_INTERFACES_RESOLVEABLEEXCEPTION_H

#include <halinterface/InterruptServiceRoutine.h>
#include <kernel/global.h>

namespace Arch
{
    class ResolveableException : public InterruptServiceRoutine
    {
    protected:
        virtual void Prepare(registers_t* regs) = 0;
        virtual bool Resolve() = 0;
        
    public:
        virtual void Execute(registers_t* regs)
        {
            Prepare(regs);
            
            if(!Resolve())
                PANIC("Recovery from exception " << Debug::dec << regs->int_no << " failed. EIP: " << Debug::hex << regs->eip << ": Errorcode: " << Debug::dec << (unsigned)regs->err_code);
        }
    };
}

#endif
