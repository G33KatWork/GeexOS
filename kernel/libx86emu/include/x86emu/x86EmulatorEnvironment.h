#ifndef _X86EMU_X86EMUENVIRONMENT_H
#define _X86EMU_X86EMUENVIRONMENT_H

#include <x86emu/x86EmulatorTypes.h>
#include <x86emu/x86EmulatorDebug.h>
#include <x86emu/x86EmulatorMemory.h>
#include <x86emu/x86Registers.h>
#include <x86emu/x86Flags.h>
#include <x86emu/x86SegmentOverrides.h>
#include <string.h>

namespace X86Emu
{
    class X86EmulatorEnvironment
    {
    private:
        X86EmulatorMemory* memory;
        X86Registers* registers;
        X86Flags flags;
        X86SegmentOverrides overrides;
        
        bool interruptPending;
        u8 pendingInterrupt;
        bool machineHalted;
    
    public:
        X86EmulatorEnvironment(Address MemoryBase, size_t MemorySize)
        {
            memory = new X86EmulatorMemory(MemoryBase, MemorySize, this);
            registers = new X86Registers(this);
            interruptPending = false;
            pendingInterrupt = 0;
            machineHalted = false;
        }
        
        ~X86EmulatorEnvironment()
        {
            delete memory;
            delete registers;
        }
        
        inline bool MachineHalted() { return machineHalted; }
        inline void HaltMachine() { machineHalted = true;}
        inline bool InterruptPending() { return interruptPending; }
        inline void SetInterruptHandled() { interruptPending = false; }
        inline void InterruptOccured(u8 intNumber) { interruptPending = true; pendingInterrupt = intNumber; }
        
        inline X86EmulatorMemory* Memory() { return memory; }
        inline X86Registers* Registers() { return registers; }
        inline X86Flags* Flags() { return &flags; }
        inline X86SegmentOverrides* Overrides() { return &overrides; }
    };
}

#endif
