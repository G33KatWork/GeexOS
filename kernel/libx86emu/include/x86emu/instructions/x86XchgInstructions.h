#ifndef _X86EMU_X86EMUXCHGINSTRUCTIONS_H
#define _X86EMU_X86EMUXCHGINSTRUCTIONS_H

#include <x86emu/X86EmulatorEnvironment.h>
#include <x86emu/X86EmulatorInstructionDecoder.h>
#include <x86emu/x86EmulatorDebug.h>

namespace X86Emu
{
    // opcode 0x91 to 0x93
    // exchange contents of (E)AX and (E)CX, (E)DX or (E)BX
    // target register is described by lower byte
    void X86XchgAXWordRegisterInstruction(X86EmulatorEnvironment* env, X86EmulatorInstructionDecoder* decoder, u8 opcode)
    {
        u32 tmp;
        X86Register reg = (X86Register)(REGISTER(opcode) | 0x8);    //or with 0x8 to specify REGISTER_TYPE_WORD
        
        if(env->Overrides()->GetOverride(PREFIX_DATA32)) {
            X86EMU_DEBUG_MSG("XCHG EAX, " << env->Registers()->GetRegisterName(reg));
        } else {
            X86EMU_DEBUG_MSG("XCHG AX, " << env->Registers()->GetRegisterName(reg));
        }
        
        tmp = env->Registers()->GetRegister(AX);
        env->Registers()->SetRegister(AX, env->Registers()->GetRegister(reg));
        env->Registers()->SetRegister(reg, tmp);
        
        env->Overrides()->Clear();
    }
    
    // opcode 0x94 to 0x97
    // exchange contents of (E)AX and (E)SP, (E)BP, (E)SI or (E)DI
    // target register is described by lower byte
    void X86XchgAXIndexRegisterInstruction(X86EmulatorEnvironment* env, X86EmulatorInstructionDecoder* decoder, u8 opcode)
    {
        u32 tmp;
        X86Register reg = (X86Register)(REGISTER(opcode) | 0xC);    //or with 0xC to specify REGISTER_TYPE_INDEX
        
        if(env->Overrides()->GetOverride(PREFIX_DATA32)) {
            X86EMU_DEBUG_MSG("XCHG EAX, " << env->Registers()->GetRegisterName(reg));
        } else {
            X86EMU_DEBUG_MSG("XCHG AX, " << env->Registers()->GetRegisterName(reg));
        }
        
        tmp = env->Registers()->GetRegister(AX);
        env->Registers()->SetRegister(AX, env->Registers()->GetRegister(reg));
        env->Registers()->SetRegister(reg, tmp);
        
        env->Overrides()->Clear();
    }
}

#endif
