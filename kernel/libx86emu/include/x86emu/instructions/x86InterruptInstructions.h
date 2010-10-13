#ifndef _X86EMU_X86EMUINTERRUPTINSTRUCTION_H
#define _X86EMU_X86EMUINTERRUPTINSTRUCTION_H

#include <x86emu/X86EmulatorEnvironment.h>
#include <x86emu/X86EmulatorInstructionDecoder.h>
#include <x86emu/x86EmulatorDebug.h>
#include <x86emu/instructions/x86primitiveInstructions.h>

namespace X86Emu
{
    // opcode 0xcf
    // pop IP, CS and flags and return there
    void X86IretInstruction(X86EmulatorEnvironment* env, X86EmulatorInstructionDecoder* decoder, u8 opcode)
    {
        X86EMU_DEBUG_MSG("IRET");
        
        env->Overrides()->Clear();
    }
}

#endif
