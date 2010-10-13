#ifndef _GDBSTUB_H
#define _GDBSTUB_H

#include <lib/types.h>
#include <kernel/IO/SerialConsole.h>
#include <kernel/IInterruptServiceRoutine.h>
#include <arch/interrupts.h>

#define BREAKPOINT  asm("int $3")

#define BUFMAX      400

namespace Debug
{
    class GDBStub
    {
    public:
        GDBStub(IO::SerialConsole* serialLine)
        {
            serial = serialLine;
            enabled = false;
            
            curThreadIDGeneral = 0;
            curThreadIDStepping = 0;
        }
        
        void Enable()
        {
            enabled = true;
            Arch::InterruptDispatcher::GetInstance()->RegisterHandler(3, new DebugInterruptHandler(this));
        }
        
        bool IsEnabled() { return enabled; }
        void TrapFromException(registers_t* regs);
    
    private:
        IO::SerialConsole* serial;
        bool enabled;
        
        char inBuffer[BUFMAX];
        char outBuffer[BUFMAX];
        
        int curThreadIDGeneral;
        int curThreadIDStepping;
        
        void getpacket(char* buffer);
        void putpacket(char* buffer);
        
        int hexchar2int(char ch);
        char computeSignal(char exceptionVector);
        int hexToInt(char **ptr, int *intValue);
        
        //Handler class for debugging interrupt
        class DebugInterruptHandler : public IInterruptServiceRoutine
        {
        private:
            GDBStub* stub;
        public:
            DebugInterruptHandler(GDBStub *gdbstub)
                : stub(gdbstub)
            { }
            
            void Execute(registers_t* regs) { stub->TrapFromException(regs); }
        };
    };
}

#endif
