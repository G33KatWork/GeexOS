#ifndef _X86EMU_X86EMUSUBINSTRUCTION_H
#define _X86EMU_X86EMUSUBINSTRUCTION_H

#include <x86emu/X86EmulatorEnvironment.h>
#include <x86emu/X86EmulatorInstructionDecoder.h>
#include <x86emu/x86EmulatorDebug.h>
#include <x86emu/instructions/x86primitiveInstructions.h>

namespace X86Emu
{
    // opcode 0x2c
    // subtract a byte from AL
    void X86SubByteALRegister(X86EmulatorEnvironment* env, X86EmulatorInstructionDecoder* decoder, u8 opcode)
    {
        u8 value = env->Memory()->FetchByte();
        X86EMU_DEBUG_MSG("SUB AL, " << hex << (unsigned)value);

        u32 newValue = SubByte(env->Registers()->GetRegister(AL), value, env);

        env->Registers()->SetRegister(AL, newValue);
        env->Overrides()->Clear();
    }
    
    // opcode 0x2d
    // add a word or dword to (E)AX
    void X86SubWordAXRegister(X86EmulatorEnvironment* env, X86EmulatorInstructionDecoder* decoder, u8 opcode)
    {
        u32 value;
        u32 newValue;
        
        if(env->Overrides()->GetOverride(PREFIX_DATA32))
        {
            value = env->Memory()->FetchDword();
            newValue = SubDword(env->Registers()->GetRegister(AX), value, env);
        }
        else
        {
            value = env->Memory()->FetchWord();
            newValue = SubWord(env->Registers()->GetRegister(AX), value, env);
        }
        
        X86EMU_DEBUG_MSG("SUB " << env->Registers()->GetRegisterName(AX) << ", " << hex << (unsigned)value);
        env->Registers()->SetRegister(AX, newValue);
        
        env->Overrides()->Clear();
    }
    
    // opcode 0x1c
    // subtract a byte from AL
    void X86SbbByteALRegister(X86EmulatorEnvironment* env, X86EmulatorInstructionDecoder* decoder, u8 opcode)
    {
        u8 value = env->Memory()->FetchByte();
        X86EMU_DEBUG_MSG("SBB AL, " << hex << (unsigned)value);

        u32 newValue = SbbByte(env->Registers()->GetRegister(AL), value, env);

        env->Registers()->SetRegister(AL, newValue);
        env->Overrides()->Clear();
    }
    
    // opcode 0x1d
    // add a word or dword to (E)AX
    void X86SbbWordAXRegister(X86EmulatorEnvironment* env, X86EmulatorInstructionDecoder* decoder, u8 opcode)
    {
        u32 value;
        u32 newValue;
        
        if(env->Overrides()->GetOverride(PREFIX_DATA32))
        {
            value = env->Memory()->FetchDword();
            newValue = SbbDword(env->Registers()->GetRegister(AX), value, env);
        }
        else
        {
            value = env->Memory()->FetchWord();
            newValue = SbbWord(env->Registers()->GetRegister(AX), value, env);
        }
        
        X86EMU_DEBUG_MSG("SBB " << env->Registers()->GetRegisterName(AX) << ", " << hex << (unsigned)value);
        env->Registers()->SetRegister(AX, newValue);
        
        env->Overrides()->Clear();
    }
}

#endif
