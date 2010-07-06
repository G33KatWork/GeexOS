#ifndef _ARCHX86_FATALEXCEPTION_H
#define _ARCHX86_FATALEXCEPTION_H

#include <types.h>
#include <halinterface/InterruptServiceRoutine.h>
#include <kernel/debug.h>

namespace Arch
{
    class FatalExceptionHandler : public InterruptServiceRoutine
    {   
    public:
        virtual void Execute(registers_t* regs)
        {
            PANIC("Exception " << IO::dec << regs->int_no << ": EIP: " << IO::hex << regs->eip << ": Errorcode: " << IO::dec << (unsigned)regs->err_code);
        }
    };
}

#endif
