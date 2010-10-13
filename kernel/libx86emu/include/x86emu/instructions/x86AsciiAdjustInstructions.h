#ifndef _X86EMU_X86EMUAAINSTRUCTION_H
#define _X86EMU_X86EMUAAINSTRUCTION_H

#include <x86emu/X86EmulatorEnvironment.h>
#include <x86emu/X86EmulatorInstructionDecoder.h>
#include <x86emu/x86EmulatorDebug.h>
#include <x86emu/instructions/x86primitiveInstructions.h>

namespace X86Emu
{
    // opcode 0xD5
    // ASCII Adjust AX Before Division
    void X86AadWordAXRegister(X86EmulatorEnvironment* env, X86EmulatorInstructionDecoder* decoder, u8 opcode)
    {
        u8 value = env->Memory()->FetchByte();
        X86EMU_DEBUG_MSG("AAD " << hex << (unsigned)value << ", AX");
        
        if(value != 10)
        {
            //FIXME: add base decoding
            X86EMU_DEBUG_MSG("No support for base decoding in AAD instruction. Halting...");
            env->HaltMachine();
        }

        u16 newValue = AadWord(env->Registers()->GetRegister(AX), env);

        env->Registers()->SetRegister(AX, newValue);
        env->Overrides()->Clear();
    }
    
    // opcode 0xD4
    // ASCII Adjust AX After Multiplication
    void X86AamWordAXRegister(X86EmulatorEnvironment* env, X86EmulatorInstructionDecoder* decoder, u8 opcode)
    {
        u8 value = env->Memory()->FetchByte();
        X86EMU_DEBUG_MSG("AAM " << hex << (unsigned)value << ", AX");
        
        if(value != 10)
        {
            //FIXME: add base decoding
            X86EMU_DEBUG_MSG("No support for base decoding in AAM instruction. Halting...");
            env->HaltMachine();
        }

        u16 newValue = AamWord(env->Registers()->GetRegister(AX), env);

        env->Registers()->SetRegister(AX, newValue);
        env->Overrides()->Clear();
    }
}

#endif
