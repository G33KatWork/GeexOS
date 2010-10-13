#ifndef _X86EMU_X86EMUMOVINSTRUCTIONS_H
#define _X86EMU_X86EMUMOVINSTRUCTIONS_H

#include <x86emu/X86EmulatorEnvironment.h>
#include <x86emu/X86EmulatorInstructionDecoder.h>
#include <x86emu/x86EmulatorDebug.h>

namespace X86Emu
{
    // opcode 0xb0 to 0xb7
    // mov immediate value into AL, BL, CL, DL or AH, BH, CH, DH
    void X86MovByteRegisterImmediateInstruction(X86EmulatorEnvironment* env, X86EmulatorInstructionDecoder* decoder, u8 opcode)
    {
        u8 b = env->Memory()->FetchByte();
        X86Register reg = (X86Register)(opcode & 0x0f);     //lowermost 4 bit describe target register
        X86EMU_DEBUG_MSG("MOV " << hex << env->Registers()->GetRegisterName(reg) << ", " << (unsigned)b);
        env->Registers()->SetRegister(reg, b);
        env->Overrides()->Clear();
    }
    
    // opcode 0xb8 to 0xbf
    // mov immediate value into (E)AX, (E)BX, (E)CX, (E)DX, (E)SP, (E)BP, (E)SI, (E)DI
    void X86MovWordRegisterImmediateInstruction(X86EmulatorEnvironment* env, X86EmulatorInstructionDecoder* decoder, u8 opcode)
    {
        u32 val;
        if(env->Overrides()->GetOverride(PREFIX_DATA32))
            val = env->Memory()->FetchDword();
        else
            val = env->Memory()->FetchWord();
        
        X86Register reg = (X86Register)(opcode & 0x0f);     //lowermost 4 bit describe target register
        X86EMU_DEBUG_MSG("MOV " << hex << env->Registers()->GetRegisterName(reg) << ", " << (unsigned)val);
        
        env->Registers()->SetRegister(reg, val);
        env->Overrides()->Clear();
    }
    
    // opcode 0xa0
    // mov value from memory to AL
    void X86MovALMemoryInstruction(X86EmulatorEnvironment* env, X86EmulatorInstructionDecoder* decoder, u8 opcode)
    {
        u16 offset = env->Memory()->FetchWord();
        
        X86EMU_DEBUG_MSG("MOV AL, [" << hex << (unsigned)offset << "]");
        
        env->Registers()->SetRegister(AL,
            env->Memory()->ReadByte(
                (env->Registers()->GetSegmentRegister(DS) << 4) + offset
            )
        );
        env->Overrides()->Clear();
    }
    
    // opcode 0xa1
    // mov value from memory to (E)AX
    void X86MovAXMemoryInstruction(X86EmulatorEnvironment* env, X86EmulatorInstructionDecoder* decoder, u8 opcode)
    {
        u16 offset = env->Memory()->FetchWord();
        
        X86EMU_DEBUG_MSG("MOV " << env->Registers()->GetRegisterName(AX) << ", [" << hex << (unsigned)offset << "]");
        
        u32 val;
        if(env->Overrides()->GetOverride(PREFIX_DATA32))
            val = env->Memory()->ReadDword((env->Registers()->GetSegmentRegister(DS) << 4) + offset);
        else
            val = env->Memory()->ReadWord((env->Registers()->GetSegmentRegister(DS) << 4) + offset);
        
        env->Registers()->SetRegister(AX, val);
        
        env->Overrides()->Clear();
    }
    
    // opcode 0xa2
    // mov value from AL to memory
    void X86MovMemoryALInstruction(X86EmulatorEnvironment* env, X86EmulatorInstructionDecoder* decoder, u8 opcode)
    {
        u16 offset = env->Memory()->FetchWord();
        
        X86EMU_DEBUG_MSG("MOV [" << hex << (unsigned)offset << "], AL");
        
        env->Memory()->WriteByte(
            (env->Registers()->GetSegmentRegister(DS) << 4) + offset,
            env->Registers()->GetRegister(AL)
        );
        
        env->Overrides()->Clear();
    }
}

#endif
