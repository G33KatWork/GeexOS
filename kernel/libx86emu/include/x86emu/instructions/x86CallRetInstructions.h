#ifndef _X86EMU_X86EMUCALLRETINSTRUCTIONS_H
#define _X86EMU_X86EMUCALLRETINSTRUCTIONS_H

#include <x86emu/X86EmulatorEnvironment.h>
#include <x86emu/X86EmulatorInstructionDecoder.h>
#include <x86emu/x86EmulatorDebug.h>
#include <x86emu/instructions/x86primitiveInstructions.h>

namespace X86Emu
{
    // opcode 0xc2
    // return to address popped from stack and add 16 bit immediate value to stackpointer
    void X86RetNearImmediateInstruction(X86EmulatorEnvironment* env, X86EmulatorInstructionDecoder* decoder, u8 opcode)
    {
        u16 stackOffset = env->Memory()->FetchWord();
        
        X86EMU_DEBUG_MSG("RET " << hex << stackOffset);
        
        env->Registers()->SetInstructionPointer(popWord(env));
        env->Registers()->SetRegister(SP, (u16)(env->Registers()->GetRegister(SP) + stackOffset));
        
        env->Overrides()->Clear();
    }
    
    // opcode 0xc3
    // return to address popped from stack
    void X86RetNearInstruction(X86EmulatorEnvironment* env, X86EmulatorInstructionDecoder* decoder, u8 opcode)
    {
        X86EMU_DEBUG_MSG("RET");
        
        env->Registers()->SetInstructionPointer(popWord(env));
        
        env->Overrides()->Clear();
    }
    
    // opcode 0xe8
    // push IP to stack and apply offset to it
    void X86CallNearImmediateInstruction(X86EmulatorEnvironment* env, X86EmulatorInstructionDecoder* decoder, u8 opcode)
    {
        s16 offset = (s16)env->Memory()->FetchWord();
        
        X86EMU_DEBUG_MSG("CALL " << hex << offset);
        
        s16 newIp = (s16)(env->Registers()->GetInstructionPointer() + offset);
        
        pushWord(env->Registers()->GetInstructionPointer(), env);
        env->Registers()->SetInstructionPointer((u16)newIp);
        
        env->Overrides()->Clear();
    }
    
    // opcode 0xe9
    // apply offset to IP
    void X86JmpNearImmediateInstruction(X86EmulatorEnvironment* env, X86EmulatorInstructionDecoder* decoder, u8 opcode)
    {
        s16 offset = (s16)env->Memory()->FetchWord();
        
        X86EMU_DEBUG_MSG("JMP " << hex << offset);
        
        s16 newIp = (s16)(env->Registers()->GetInstructionPointer() + offset);
        env->Registers()->SetInstructionPointer((u16)newIp);
        
        env->Overrides()->Clear();
    }
    
    // opcode 0xea
    // set ip and cs from immediate values
    void X86JmpFarImmediateInstruction(X86EmulatorEnvironment* env, X86EmulatorInstructionDecoder* decoder, u8 opcode)
    {
        u16 newIp = env->Memory()->FetchWord();
        u16 newCs = env->Memory()->FetchWord();
        
        X86EMU_DEBUG_MSG("JMP " << hex << newCs << ":" << newIp);
        
        env->Registers()->SetInstructionPointer(newIp);
        env->Registers()->SetSegmentRegister(CS, newCs);
        
        env->Overrides()->Clear();
    }
    
    // opcode 0xeb
    // apply byte offset to IP
    void X86JmpNearImmediateByteInstruction(X86EmulatorEnvironment* env, X86EmulatorInstructionDecoder* decoder, u8 opcode)
    {
        s8 offset = (s8)env->Memory()->FetchByte();
        
        X86EMU_DEBUG_MSG("JMP " << hex << offset);
        
        s16 newIp = (s16)env->Registers()->GetInstructionPointer() + offset;
        env->Registers()->SetInstructionPointer((u16)newIp);
        
        env->Overrides()->Clear();
    }
    
    // opcode 0x9a
    // push CS and IP to stack and set them to new immediate values
    void X86CallFarImmediateInstruction(X86EmulatorEnvironment* env, X86EmulatorInstructionDecoder* decoder, u8 opcode)
    {
        u16 offset = env->Memory()->FetchWord();
        u16 segment = env->Memory()->FetchWord();
        
        X86EMU_DEBUG_MSG("CALL " << hex << segment << ":" << offset);
        
        pushWord(env->Registers()->GetSegmentRegister(CS), env);
        env->Registers()->SetSegmentRegister(CS, segment);
        
        pushWord(env->Registers()->GetInstructionPointer(), env);
        env->Registers()->SetInstructionPointer(offset);
        
        env->Overrides()->Clear();
    }
    
    // opcode 0xca
    // return to address and codesegment popped from stack and add 16 bit immediate value to stackpointer
    void X86RetFarImmediateInstruction(X86EmulatorEnvironment* env, X86EmulatorInstructionDecoder* decoder, u8 opcode)
    {
        u16 stackOffset = env->Memory()->FetchWord();
        
        X86EMU_DEBUG_MSG("RETF " << hex << stackOffset);
        
        env->Registers()->SetInstructionPointer(popWord(env));
        env->Registers()->SetSegmentRegister(CS, popWord(env));
        env->Registers()->SetRegister(SP, (u16)(env->Registers()->GetRegister(SP) + stackOffset));
        
        env->Overrides()->Clear();
    }
    
    // opcode 0xcb
    // return to address and codesegment popped from stack
    void X86RetFarInstruction(X86EmulatorEnvironment* env, X86EmulatorInstructionDecoder* decoder, u8 opcode)
    {
        X86EMU_DEBUG_MSG("RETF");
        
        env->Registers()->SetInstructionPointer(popWord(env));
        env->Registers()->SetSegmentRegister(CS, popWord(env));
        
        env->Overrides()->Clear();
    }
}

#endif
