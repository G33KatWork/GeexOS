#ifndef _ARCHX86_PAGEFAULTHANDLER_H
#define _ARCHX86_PAGEFAULTHANDLER_H

#include <types.h>
#include <kernel/global.h>

#include <arch/internal/x86ResolveableException.h>
#include <kernel/Memory/Virtual/VirtualMemoryManager.h>

namespace Arch
{
    class x86PageFaultHandler : public x86ResolveableException
    {
    private:
        Address faultingInstruction;
        Address faultingAddress;
        bool NonPresent;
        bool WriteOperation;
        bool Usermode;
        bool ReservedBit;
        bool InstructionFetch;
    
    public:
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
    
    protected:
        virtual bool Resolve()
        {
            if(!Memory::VirtualMemoryManager::GetInstance()->GetCurrentMemorySpace())
                PANIC("Memory manager isn't initialized yet and a pagefault occured ( " << (NonPresent ? "not-present " : "")
                    << (WriteOperation ? "write-operation " : "") << (Usermode ? "user-mode " : "")
                    << (ReservedBit ? "reserved-bit " : "") << (InstructionFetch ? "instruction-fetch " : "")
                    << ") at " << Debug::hex << faultingAddress << " EIP: " << faultingInstruction
                );
            
            if(!Memory::VirtualMemoryManager::GetInstance()->GetCurrentMemorySpace()->HandlePageFault(this->faultingAddress))
                PANIC("Unresolveable page fault! ( " << (NonPresent ? "not-present " : "")
                    << (WriteOperation ? "write-operation " : "") << (Usermode ? "user-mode " : "")
                    << (ReservedBit ? "reserved-bit " : "") << (InstructionFetch ? "instruction-fetch " : "")
                    << ") at " << Debug::hex << faultingAddress << " EIP: " << faultingInstruction
                );
            
			// either we resolved it or we panicked already, so return true
            return true;
        }
    };
}

#endif
