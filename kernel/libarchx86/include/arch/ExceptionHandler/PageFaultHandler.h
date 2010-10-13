#ifndef _ARCHX86_PAGEFAULTHANDLER_H
#define _ARCHX86_PAGEFAULTHANDLER_H

#include <types.h>
#include <halinterface/ResolveableException.h>
#include <kernel/global.h>

namespace Arch
{
    class PageFaultHandler : public ResolveableException
    {
    private:
        Address faultingInstruction;
        Address faultingAddress;
        bool NonPresent;
        bool WriteOperation;
        bool Usermode;
        bool ReservedBit;
        bool InstructionFetch;
        
    protected:
        virtual void Prepare(registers_t* regs)
        {
            //A page fault has occurred.
            //The faulting address is stored in the CR2 register.
            asm volatile("mov %%cr2, %0" : "=r" (faultingAddress));
            
            faultingInstruction = regs->eip;

            //The error code gives us details of what happened.
            NonPresent = !(regs->err_code & 0x1);       // Page not present
            WriteOperation = regs->err_code & 0x2;      // Write operation?
            Usermode = regs->err_code & 0x4;            // Processor was in user-mode?
            ReservedBit = regs->err_code & 0x8;         // Overwritten CPU-reserved bits of page entry?
            InstructionFetch = regs->err_code & 0x10;   // Caused by an instruction fetch?
        }
        
        virtual bool Resolve()
        {
            PANIC("Unresolveable page fault! ( " << (NonPresent ? "not-present " : "")
                << (WriteOperation ? "write-operation " : "") << (Usermode ? "user-mode " : "")
                << (ReservedBit ? "reserved-bit " : "") << (InstructionFetch ? "instruction-fetch " : "")
                << ") at " << Debug::hex << faultingAddress << " EIP: " << faultingInstruction
            );
            
            return false;
        }
        
    public:
    };
}

#endif
