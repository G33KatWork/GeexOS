#ifndef _X86EMU_X86EMUSTACKINSTRUCTIONS_H
#define _X86EMU_X86EMUSTACKINSTRUCTIONS_H

#include <x86emu/X86EmulatorEnvironment.h>
#include <x86emu/X86EmulatorInstructionDecoder.h>
#include <x86emu/x86EmulatorDebug.h>
#include <x86emu/instructions/x86primitiveInstructions.h>

namespace X86Emu
{
    // opcode 0x50 to 0x57
    // push value from (E)AX, (E)BX, (E)CX, (E)DX, (E)SP, (E)BP, (E)SI, (E)DI to stack
    void X86PushWordRegisterInstruction(X86EmulatorEnvironment* env, X86EmulatorInstructionDecoder* decoder, u8 opcode)
    {
        X86Register reg = (X86Register)((opcode & 0x0f) | 0x8);     //lowermost 4 bit describe target register
        
        X86EMU_DEBUG_MSG("PUSH " << hex << env->Registers()->GetRegisterName(reg));
        
        if(env->Overrides()->GetOverride(PREFIX_DATA32))
            pushDword(env->Registers()->GetRegister(reg), env);
        else
            pushWord(env->Registers()->GetRegister(reg), env);
        
        env->Overrides()->Clear();
    }
    
    // opcode 0x58 to 0x5f
    // pop value from stack into (E)AX, (E)BX, (E)CX, (E)DX, (E)SP, (E)BP, (E)SI, (E)DI
    void X86PopWordRegisterInstruction(X86EmulatorEnvironment* env, X86EmulatorInstructionDecoder* decoder, u8 opcode)
    {
        X86Register reg = (X86Register)((opcode & 0x0f) | 0x8);     //lowermost 4 bit describe target register
        
        X86EMU_DEBUG_MSG("POP " << hex << env->Registers()->GetRegisterName(reg));
        
        if(env->Overrides()->GetOverride(PREFIX_DATA32))
            env->Registers()->SetRegister(reg, popDword(env));
        else
            env->Registers()->SetRegister(reg, popWord(env));
        
        env->Overrides()->Clear();
    }
    
    // opcode 0x6
    // push ES
    void X86PushESInstruction(X86EmulatorEnvironment* env, X86EmulatorInstructionDecoder* decoder, u8 opcode)
    {
        X86EMU_DEBUG_MSG("PUSH ES");
        pushWord(env->Registers()->GetSegmentRegister(ES), env);
        env->Overrides()->Clear();
    }
    
    // opcode 0x7
    // pop ES
    void X86PopESInstruction(X86EmulatorEnvironment* env, X86EmulatorInstructionDecoder* decoder, u8 opcode)
    {
        X86EMU_DEBUG_MSG("POP ES");
        env->Registers()->SetSegmentRegister(ES, popWord(env));
        env->Overrides()->Clear();
    }
    
    // opcode 0xe
    // push CS
    void X86PushCSInstruction(X86EmulatorEnvironment* env, X86EmulatorInstructionDecoder* decoder, u8 opcode)
    {
        X86EMU_DEBUG_MSG("PUSH CS");
        pushWord(env->Registers()->GetSegmentRegister(CS), env);
        env->Overrides()->Clear();
    }
    
    // opcode 0x16
    // push SS
    void X86PushSSInstruction(X86EmulatorEnvironment* env, X86EmulatorInstructionDecoder* decoder, u8 opcode)
    {
        X86EMU_DEBUG_MSG("PUSH SS");
        pushWord(env->Registers()->GetSegmentRegister(SS), env);
        env->Overrides()->Clear();
    }
    
    // opcode 0x17
    // pop SS
    void X86PopSSInstruction(X86EmulatorEnvironment* env, X86EmulatorInstructionDecoder* decoder, u8 opcode)
    {
        X86EMU_DEBUG_MSG("POP SS");
        env->Registers()->SetSegmentRegister(SS, popWord(env));
        env->Overrides()->Clear();
    }
    
    // opcode 0x1e
    // push DS
    void X86PushDSInstruction(X86EmulatorEnvironment* env, X86EmulatorInstructionDecoder* decoder, u8 opcode)
    {
        X86EMU_DEBUG_MSG("PUSH DS");
        pushWord(env->Registers()->GetSegmentRegister(DS), env);
        env->Overrides()->Clear();
    }
    
    // opcode 0x1f
    // pop DS
    void X86PopDSInstruction(X86EmulatorEnvironment* env, X86EmulatorInstructionDecoder* decoder, u8 opcode)
    {
        X86EMU_DEBUG_MSG("POP DS");
        env->Registers()->SetSegmentRegister(DS, popWord(env));
        env->Overrides()->Clear();
    }
    
    // opcode 0x9c
    // push flags
    void X86PushFlagsInstruction(X86EmulatorEnvironment* env, X86EmulatorInstructionDecoder* decoder, u8 opcode)
    {
        X86EMU_DEBUG_MSG("PUSHF");
        if(env->Overrides()->GetOverride(PREFIX_DATA32))
            pushDword(env->Flags()->GetFlagRegister(), env);
        else
            pushWord(env->Flags()->GetFlagRegister(), env);
        
        env->Overrides()->Clear();
    }
    
    // opcode 0x9d
    // pop flags
    void X86PopFlagsInstruction(X86EmulatorEnvironment* env, X86EmulatorInstructionDecoder* decoder, u8 opcode)
    {
        X86EMU_DEBUG_MSG("POPF");
        if(env->Overrides()->GetOverride(PREFIX_DATA32))
            env->Flags()->SetFlagRegister((u16)popDword(env));
        else
            env->Flags()->SetFlagRegister(popWord(env));
        
        env->Overrides()->Clear();
    }
    
    // opcode 0xc8
    // enter
    void X86EnterInstruction(X86EmulatorEnvironment* env, X86EmulatorInstructionDecoder* decoder, u8 opcode)
    {
        u16 local = env->Memory()->FetchWord();
        u8 nesting = env->Memory()->FetchByte();
        
        X86EMU_DEBUG_MSG("ENTER " << hex << (unsigned)local << ", " << (unsigned)nesting);
        
        pushWord((u16)env->Registers()->GetRegister(BP), env);
        u16 newFramePointer = (u16)env->Registers()->GetRegister(SP);
        
        if(nesting > 0)
        {
            u16 curBp = (u16)env->Registers()->GetRegister(BP);
            for(u8 i = 1; i < nesting; i++)
            {
                curBp -= 2;
                pushWord(
                    env->Memory()->ReadWord(env->Registers()->GetSegmentRegister(SS) << 4 + curBp),
                    env
                );
            }
            env->Registers()->SetRegister(BP, curBp);
            pushWord(newFramePointer, env);
        }
        
        env->Registers()->SetRegister(BP, newFramePointer);
        env->Registers()->SetRegister(SP, (u16)(env->Registers()->GetRegister(SP) - local));
        
        env->Overrides()->Clear();
    }
    
    // opcode 0xc9
    // leave
    void X86LeaveInstruction(X86EmulatorEnvironment* env, X86EmulatorInstructionDecoder* decoder, u8 opcode)
    {
        X86EMU_DEBUG_MSG("LEAVE");
        
        env->Registers()->SetRegister(SP, (u16)env->Registers()->GetRegister(BP));
        env->Registers()->SetRegister(BP, popWord(env));
        
        env->Overrides()->Clear();
    }
}

#endif
