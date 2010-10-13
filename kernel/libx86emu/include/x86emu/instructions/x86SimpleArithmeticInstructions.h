#ifndef _X86EMU_X86EMUSIMPLEARITHINSTRUCTION_H
#define _X86EMU_X86EMUSIMPLEARITHINSTRUCTION_H

#include <x86emu/X86EmulatorEnvironment.h>
#include <x86emu/X86EmulatorInstructionDecoder.h>
#include <x86emu/x86EmulatorDebug.h>
#include <x86emu/instructions/x86primitiveInstructions.h>

namespace X86Emu
{
    /*Opcode    Always0 Operation   Register    Mnemonic
     *
     * 0x4      00      0001        00          ADD AL
     * 0x5      00      0001        01          ADD AX
     *
     * 0xC      00      0011        00          OR AL
     * 0xD      00      0011        01          OR AX
     *
     * 0x14     00      0101        00          ADC AL
     * 0x15     00      0101        01          ADC AX
     *
     * And so on..
     *
     */
    
    /*enum SimpleArithmeticRegister
    {
        SIM_ARITH_AL = 0x0,
        SIM_ARITH_AX = 0x1
    };
    
    enum SimpleArithmeticOperation
    {
        SIM_ARITH_ADD = 0x1,
        SIM_ARITH_OR = 0x3,
        SIM_ARITH_ADC = 0x5,
        SIM_ARITH_SBB = 0x7,
        SIM_ARITH_AND = 0x9,
        SIM_ARITH_SUB = 0xB,
        SIM_ARITH_XOR = 0xD,
        SIM_ARITH_CMP = 0xF,
    };
    
    void X86SimpleArithmeticRegisterImmediateInstruction(X86EmulatorEnvironment* env, X86EmulatorInstructionDecoder* decoder, u8 opcode)
    {
        X86Register targetRegister = opcode & 0x3 == SIM_ARITH_AL ? AL : AX;
        u8 operation = (opcode & 0x3C) >> 2;
        
        
        u32 value;
        if(targetRegister == AL)
            value = env->Memory()->FetchByte();
        else if(env->Overrides()->GetOverride(PREFIX_DATA32))
            value = env->Memory()->FetchDword();
        else
            value = env->Memory()->FetchWord();
        
        switch(operation)
        {
            case SIM_ARITH_ADD:
            break;
            case SIM_ARITH_OR:
            break;
            case SIM_ARITH_ADC:
            break;
            case SIM_ARITH_SBB:
            break;
            case SIM_ARITH_AND:
            break;
            case SIM_ARITH_SUB:
            break;
            case SIM_ARITH_XOR:
            break;
            case SIM_ARITH_CMP:
            break;
        }
        X86EMU_DEBUG_MSG("XOR AL, " << hex << (unsigned)value);

        u32 newValue = XorByte(env->Registers()->GetRegister(AL), value, env);

        env->Registers()->SetRegister(AL, newValue);
        env->Overrides()->Clear();
    }*/
}

#endif
