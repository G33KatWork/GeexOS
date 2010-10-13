#ifndef _X86EMU_X86EMUADDINSTRUCTION_H
#define _X86EMU_X86EMUADDINSTRUCTION_H

#include <x86emu/X86EmulatorEnvironment.h>
#include <x86emu/X86EmulatorInstructionDecoder.h>
#include <x86emu/x86EmulatorDebug.h>
#include <x86emu/instructions/x86primitiveInstructions.h>

namespace X86Emu
{
    // opcode 0x04
    // add a byte to AL
    void X86AddByteALRegister(X86EmulatorEnvironment* env, X86EmulatorInstructionDecoder* decoder, u8 opcode)
    {
        u8 value = env->Memory()->FetchByte();
        X86EMU_DEBUG_MSG("ADD AL, " << hex << (unsigned)value);

        u32 newValue = AddByte(env->Registers()->GetRegister(AL), value, env);

        env->Registers()->SetRegister(AL, newValue);
        env->Overrides()->Clear();
    }
    
    // opcode 0x05
    // add a word or dword to (E)AX
    void X86AddWordAXRegister(X86EmulatorEnvironment* env, X86EmulatorInstructionDecoder* decoder, u8 opcode)
    {
        u32 value;
        u32 newValue;
        
        if(env->Overrides()->GetOverride(PREFIX_DATA32))
        {
            value = env->Memory()->FetchDword();
            newValue = AddDword(env->Registers()->GetRegister(AX), value, env);
        }
        else
        {
            value = env->Memory()->FetchWord();
            newValue = AddWord(env->Registers()->GetRegister(AX), value, env);
        }
        
        X86EMU_DEBUG_MSG("ADD " << env->Registers()->GetRegisterName(AX) << ", " << hex << (unsigned)value);
        env->Registers()->SetRegister(AX, newValue);
        
        env->Overrides()->Clear();
    }
    
    // opcode 0x14
    // add a byte to AL with carry
    void X86AddCarryByteALRegister(X86EmulatorEnvironment* env, X86EmulatorInstructionDecoder* decoder, u8 opcode)
    {
        u8 value = env->Memory()->FetchByte();
        X86EMU_DEBUG_MSG("ADC AL, " << hex << (unsigned)value);

        u32 newValue = AddCarryByte(env->Registers()->GetRegister(AL), value, env);

        env->Registers()->SetRegister(AL, newValue);
        env->Overrides()->Clear();
    }
    
    // opcode 0x15
    // add a word or dword to (E)AX with carry
    void X86AddCarryWordAXRegister(X86EmulatorEnvironment* env, X86EmulatorInstructionDecoder* decoder, u8 opcode)
    {
        u32 value;
        u32 newValue;
        
        if(env->Overrides()->GetOverride(PREFIX_DATA32))
        {
            value = env->Memory()->FetchDword();
            newValue = AddCarryDword(env->Registers()->GetRegister(AX), value, env);
        }
        else
        {
            value = env->Memory()->FetchWord();
            newValue = AddCarryWord(env->Registers()->GetRegister(AX), value, env);
        }
        
        X86EMU_DEBUG_MSG("ADC " << env->Registers()->GetRegisterName(AX) << ", " << hex << (unsigned)value);
        env->Registers()->SetRegister(AX, newValue);
        
        env->Overrides()->Clear();
    }
}

#endif
