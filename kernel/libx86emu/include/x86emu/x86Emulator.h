#ifndef _X86EMU_X86EMU_H
#define _X86EMU_X86EMU_H

#include <x86emu/x86EmulatorEnvironment.h>
#include <x86emu/x86EmulatorInstructionDecoder.h>

namespace X86Emu
{
    class X86Emulator
    {
    public:
        X86Emulator(X86EmulatorEnvironment* EmulatorEnvironment)
        {
            env = EmulatorEnvironment;
        }
        
        ~X86Emulator() {}
        void Run();
    
    private:
        X86EmulatorEnvironment* env;
        X86EmulatorInstructionDecoder decoder;
    };
}

#endif
