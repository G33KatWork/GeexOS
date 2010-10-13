#ifndef _X86EMU_X86EMUINCREMENTDECREMENTREGISTERS_H
#define _X86EMU_X86EMUINCREMENTDECREMENTREGISTERS_H

#include <x86emu/X86EmulatorEnvironment.h>
#include <x86emu/X86EmulatorInstructionDecoder.h>
#include <x86emu/x86EmulatorDebug.h>
#include <x86emu/instructions/x86primitiveInstructions.h>

namespace X86Emu
{
    // opcode 0x40 to 0x43
    // increment (E)AX, (E)BX, (E)CX, (E)DX
    void X86IncrementRegister(X86EmulatorEnvironment* env, X86EmulatorInstructionDecoder* decoder, u8 opcode)
    {
        u8 reg = (opcode & 0x3) | REGISTER_TYPE_WORD;
        X86EMU_DEBUG_MSG("Incrementing register " << hex << reg);
        
        u32 newValue = env->Registers()->GetRegister((X86Register)reg);
        if(env->Overrides()->GetOverride(PREFIX_DATA32))
            newValue = IncrementDword(newValue, env);
        else
            newValue = IncrementWord(newValue, env);
        
        env->Registers()->SetRegister((X86Register)reg, newValue);
        env->Overrides()->Clear();
    }
    
    // opcode 0x44 to 0x47
    // increment (E)SP, (E)BP, (E)SI, (E)DI
    void X86IncrementIndexRegister(X86EmulatorEnvironment* env, X86EmulatorInstructionDecoder* decoder, u8 opcode)
    {
        u8 reg = (opcode & 0x3) | REGISTER_TYPE_INDEX;
        X86EMU_DEBUG_MSG("Incrementing index register " << hex << reg);
        
        u32 newValue = env->Registers()->GetRegister((X86Register)reg);
        if(env->Overrides()->GetOverride(PREFIX_DATA32))
            newValue = IncrementDword(newValue, env);
        else
            newValue = IncrementWord(newValue, env);
        
        env->Registers()->SetRegister((X86Register)reg, newValue);
        env->Overrides()->Clear();
    }
    
    // opcode 0x48 to 0x4b
    // decrement (E)AX, (E)BX, (E)CX, (E)DX
    void X86DecrementRegister(X86EmulatorEnvironment* env, X86EmulatorInstructionDecoder* decoder, u8 opcode)
    {
        u8 reg = (opcode & 0x3) | REGISTER_TYPE_WORD;
        X86EMU_DEBUG_MSG("Decrementing register " << hex << reg);
        
        u32 newValue = env->Registers()->GetRegister((X86Register)reg);
        if(env->Overrides()->GetOverride(PREFIX_DATA32))
            newValue = DecrementDword(newValue, env);
        else
            newValue = DecrementWord(newValue, env);
        
        env->Registers()->SetRegister((X86Register)reg, newValue);
        env->Overrides()->Clear();
    }
    
    // opcode 0x4c to 0x4f
    // decrement (E)SP, (E)BP, (E)SI, (E)DI
    void X86DecrementIndexRegister(X86EmulatorEnvironment* env, X86EmulatorInstructionDecoder* decoder, u8 opcode)
    {
        u8 reg = (opcode & 0x3) | REGISTER_TYPE_INDEX;
        X86EMU_DEBUG_MSG("Decrementing index register " << hex << reg);
        
        u32 newValue = env->Registers()->GetRegister((X86Register)reg);
        if(env->Overrides()->GetOverride(PREFIX_DATA32))
            newValue = DecrementDword(newValue, env);
        else
            newValue = DecrementWord(newValue, env);
        
        env->Registers()->SetRegister((X86Register)reg, newValue);
        env->Overrides()->Clear();
    }
}

#endif
