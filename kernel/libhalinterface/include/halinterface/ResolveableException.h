#ifndef _HAL_INTERFACES_RESOLVEABLEEXCEPTION_H
#define _HAL_INTERFACES_RESOLVEABLEEXCEPTION_H

#include <types.h>
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
                PANIC("Recovery from exception " << IO::dec << regs->int_no << " failed. EIP: " << IO::hex << regs->eip << ": Errorcode: " << IO::dec << (unsigned)regs->err_code);
        }
    };
}

#endif
