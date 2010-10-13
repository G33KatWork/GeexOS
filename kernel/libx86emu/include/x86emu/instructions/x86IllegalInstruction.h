#ifndef _X86EMU_X86EMUILLEGALINSTRUCTION_H
#define _X86EMU_X86EMUILLEGALINSTRUCTION_H

#include <x86emu/X86EmulatorEnvironment.h>
#include <x86emu/X86EmulatorInstructionDecoder.h>
#include <x86emu/x86EmulatorDebug.h>

namespace X86Emu
{
    void X86IllegalInstruction(X86EmulatorEnvironment* env, X86EmulatorInstructionDecoder* decoder, u8 opcode)
    {
        X86EMU_DEBUG_MSG("Illegal opcode " << hex << opcode << " at " << (unsigned)env->Registers()->GetSegmentRegister(CS) << ":" << (unsigned)env->Registers()->GetInstructionPointer());
        env->HaltMachine();
    }
}

#endif
