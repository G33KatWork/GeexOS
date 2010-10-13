#ifndef _X86EMU_X86EMUANDINSTRUCTION_H
#define _X86EMU_X86EMUANDINSTRUCTION_H

#include <x86emu/X86EmulatorEnvironment.h>
#include <x86emu/X86EmulatorInstructionDecoder.h>
#include <x86emu/x86EmulatorDebug.h>
#include <x86emu/instructions/x86primitiveInstructions.h>

namespace X86Emu
{
    // opcode 0x24
    // and a byte with AL
    void X86AndByteALRegister(X86EmulatorEnvironment* env, X86EmulatorInstructionDecoder* decoder, u8 opcode)
    {
        u8 value = env->Memory()->FetchByte();
        X86EMU_DEBUG_MSG("AND AL, " << hex << (unsigned)value);

        u32 newValue = AndByte(env->Registers()->GetRegister(AL), value, env);

        env->Registers()->SetRegister(AL, newValue);
        env->Overrides()->Clear();
    }
    
    // opcode 0x25
    // and a word or dword with (E)AX
    void X86AndWordAXRegister(X86EmulatorEnvironment* env, X86EmulatorInstructionDecoder* decoder, u8 opcode)
    {
        u32 value;
        u32 newValue;
        
        if(env->Overrides()->GetOverride(PREFIX_DATA32))
        {
            value = env->Memory()->FetchDword();
            newValue = AndDword(env->Registers()->GetRegister(AX), value, env);
        }
        else
        {
            value = env->Memory()->FetchWord();
            newValue = AndWord(env->Registers()->GetRegister(AX), value, env);
        }
        
        X86EMU_DEBUG_MSG("AND " << env->Registers()->GetRegisterName(AX) << ", " << hex << (unsigned)value);
        
        env->Registers()->SetRegister(AX, newValue);
        
        env->Overrides()->Clear();
    }
}

#endif
