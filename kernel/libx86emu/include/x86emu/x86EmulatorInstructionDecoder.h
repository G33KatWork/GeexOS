#ifndef _X86EMU_X86EMUINSTRUCTIONDECODER_H
#define _X86EMU_X86EMUINSTRUCTIONDECODER_H

#include <x86emu/X86EmulatorEnvironment.h>
#include <x86emu/X86EmulatorInstructionDecoder.h>

namespace X86Emu
{
    class X86EmulatorInstructionDecoder;
    typedef void(*X86Instruction)(X86EmulatorEnvironment*, X86EmulatorInstructionDecoder*, u8 opcode);
    
    class X86EmulatorInstructionDecoder
    {
    private:
        static X86Instruction operations[256];
            
    public:
        X86EmulatorInstructionDecoder() {}
        ~X86EmulatorInstructionDecoder() {}
        
        void ExecuteNext(X86EmulatorEnvironment* env);
    };
}

#endif
