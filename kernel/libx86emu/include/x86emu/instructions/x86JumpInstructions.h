#ifndef _X86EMU_X86EMUJUMPINSTRUCTIONS_H
#define _X86EMU_X86EMUJUMPINSTRUCTIONS_H

#include <x86emu/X86EmulatorEnvironment.h>
#include <x86emu/X86EmulatorInstructionDecoder.h>
#include <x86emu/x86EmulatorDebug.h>

namespace X86Emu
{
    // opcode 0x70
    // jump to byte offset if overflow flag is set
    void X86JumpNearOverflowInstruction(X86EmulatorEnvironment* env, X86EmulatorInstructionDecoder* decoder, u8 opcode)
    {
        s8 offset = (s8)env->Memory()->FetchByte();
        X86EMU_DEBUG_MSG("JO " << hex << offset);
        
        u16 newIP = env->Registers()->GetInstructionPointer() + (s16)offset;
        
        if(env->Flags()->GetFlag(FLAG_OVERFLOW))
        {
            X86EMU_DEBUG_MSG("Jumping to " << hex << newIP);
            env->Registers()->SetInstructionPointer(newIP);
        }
        
        env->Overrides()->Clear();
    }
    
    // opcode 0x71
    // jump to byte offset if overflow flag is not set
    void X86JumpNearNoOverflowInstruction(X86EmulatorEnvironment* env, X86EmulatorInstructionDecoder* decoder, u8 opcode)
    {
        s8 offset = (s8)env->Memory()->FetchByte();
        X86EMU_DEBUG_MSG("JNO " << hex << offset);
        
        u16 newIP = env->Registers()->GetInstructionPointer() + (s16)offset;
        
        if(!env->Flags()->GetFlag(FLAG_OVERFLOW))
        {
            X86EMU_DEBUG_MSG("Jumping to " << hex << newIP);
            env->Registers()->SetInstructionPointer(newIP);
        }
        
        env->Overrides()->Clear();
    }
    
    // opcode 0x72
    // jump to byte offset if carry flag is set
    void X86JumpNearCarryInstruction(X86EmulatorEnvironment* env, X86EmulatorInstructionDecoder* decoder, u8 opcode)
    {
        s8 offset = (s8)env->Memory()->FetchByte();
        X86EMU_DEBUG_MSG("JC " << hex << offset);
        
        u16 newIP = env->Registers()->GetInstructionPointer() + (s16)offset;
        
        if(env->Flags()->GetFlag(FLAG_CARRY))
        {
            X86EMU_DEBUG_MSG("Jumping to " << hex << newIP);
            env->Registers()->SetInstructionPointer(newIP);
        }
        
        env->Overrides()->Clear();
    }
    
    // opcode 0x73
    // jump to byte offset if carry flag is not set
    void X86JumpNearNoCarryInstruction(X86EmulatorEnvironment* env, X86EmulatorInstructionDecoder* decoder, u8 opcode)
    {
        s8 offset = (s8)env->Memory()->FetchByte();
        X86EMU_DEBUG_MSG("JNC " << hex << offset);
        
        u16 newIP = env->Registers()->GetInstructionPointer() + (s16)offset;
        
        if(!env->Flags()->GetFlag(FLAG_CARRY))
        {
            X86EMU_DEBUG_MSG("Jumping to " << hex << newIP);
            env->Registers()->SetInstructionPointer(newIP);
        }
        
        env->Overrides()->Clear();
    }
    
    // opcode 0x74
    // jump to byte offset if zero flag is set
    void X86JumpNearZeroInstruction(X86EmulatorEnvironment* env, X86EmulatorInstructionDecoder* decoder, u8 opcode)
    {
        s8 offset = (s8)env->Memory()->FetchByte();
        X86EMU_DEBUG_MSG("JZ " << hex << offset);
        
        u16 newIP = env->Registers()->GetInstructionPointer() + (s16)offset;
        
        if(env->Flags()->GetFlag(FLAG_ZERO))
        {
            X86EMU_DEBUG_MSG("Jumping to " << hex << newIP);
            env->Registers()->SetInstructionPointer(newIP);
        }
        
        env->Overrides()->Clear();
    }
    
    // opcode 0x75
    // jump to byte offset if zero flag is not set
    void X86JumpNearNoZeroInstruction(X86EmulatorEnvironment* env, X86EmulatorInstructionDecoder* decoder, u8 opcode)
    {
        s8 offset = (s8)env->Memory()->FetchByte();
        X86EMU_DEBUG_MSG("JNZ " << hex << offset);
        
        u16 newIP = env->Registers()->GetInstructionPointer() + (s16)offset;
        
        if(!env->Flags()->GetFlag(FLAG_ZERO))
        {
            X86EMU_DEBUG_MSG("Jumping to " << hex << newIP);
            env->Registers()->SetInstructionPointer(newIP);
        }
        
        env->Overrides()->Clear();
    }
    
    // opcode 0x76
    // jump to byte offset if carry or zero flag is set
    void X86JumpNearCarryOrZeroInstruction(X86EmulatorEnvironment* env, X86EmulatorInstructionDecoder* decoder, u8 opcode)
    {
        s8 offset = (s8)env->Memory()->FetchByte();
        X86EMU_DEBUG_MSG("JBE " << hex << offset);
        
        u16 newIP = env->Registers()->GetInstructionPointer() + (s16)offset;
        
        if(env->Flags()->GetFlag(FLAG_CARRY) || env->Flags()->GetFlag(FLAG_ZERO))
        {
            X86EMU_DEBUG_MSG("Jumping to " << hex << newIP);
            env->Registers()->SetInstructionPointer(newIP);
        }
        
        env->Overrides()->Clear();
    }
    
    // opcode 0x77
    // jump to byte offset if carry and zero flag are not set
    void X86JumpNearNotCarryOrZeroInstruction(X86EmulatorEnvironment* env, X86EmulatorInstructionDecoder* decoder, u8 opcode)
    {
        s8 offset = (s8)env->Memory()->FetchByte();
        X86EMU_DEBUG_MSG("JNBE " << hex << offset);
        
        u16 newIP = env->Registers()->GetInstructionPointer() + (s16)offset;
        
        if(!env->Flags()->GetFlag(FLAG_CARRY) && !env->Flags()->GetFlag(FLAG_ZERO))
        {
            X86EMU_DEBUG_MSG("Jumping to " << hex << newIP);
            env->Registers()->SetInstructionPointer(newIP);
        }
        
        env->Overrides()->Clear();
    }
    
    // opcode 0x78
    // jump to byte offset if sign flag is set
    void X86JumpNearSignInstruction(X86EmulatorEnvironment* env, X86EmulatorInstructionDecoder* decoder, u8 opcode)
    {
        s8 offset = (s8)env->Memory()->FetchByte();
        X86EMU_DEBUG_MSG("JS " << hex << offset);
        
        u16 newIP = env->Registers()->GetInstructionPointer() + (s16)offset;
        
        if(env->Flags()->GetFlag(FLAG_SIGN))
        {
            X86EMU_DEBUG_MSG("Jumping to " << hex << newIP);
            env->Registers()->SetInstructionPointer(newIP);
        }
        
        env->Overrides()->Clear();
    }
    
    // opcode 0x79
    // jump to byte offset if sign flag is not set
    void X86JumpNearNoSignInstruction(X86EmulatorEnvironment* env, X86EmulatorInstructionDecoder* decoder, u8 opcode)
    {
        s8 offset = (s8)env->Memory()->FetchByte();
        X86EMU_DEBUG_MSG("JNS " << hex << offset);
        
        u16 newIP = env->Registers()->GetInstructionPointer() + (s16)offset;
        
        if(!env->Flags()->GetFlag(FLAG_SIGN))
        {
            X86EMU_DEBUG_MSG("Jumping to " << hex << newIP);
            env->Registers()->SetInstructionPointer(newIP);
        }
        
        env->Overrides()->Clear();
    }
    
    // opcode 0x7a
    // jump to byte offset if parity flag is set
    void X86JumpNearParityInstruction(X86EmulatorEnvironment* env, X86EmulatorInstructionDecoder* decoder, u8 opcode)
    {
        s8 offset = (s8)env->Memory()->FetchByte();
        X86EMU_DEBUG_MSG("JP " << hex << offset);
        
        u16 newIP = env->Registers()->GetInstructionPointer() + (s16)offset;
        
        if(env->Flags()->GetFlag(FLAG_PARITY))
        {
            X86EMU_DEBUG_MSG("Jumping to " << hex << newIP);
            env->Registers()->SetInstructionPointer(newIP);
        }
        
        env->Overrides()->Clear();
    }
    
    // opcode 0x7b
    // jump to byte offset if parity flag is not set
    void X86JumpNearNoParityInstruction(X86EmulatorEnvironment* env, X86EmulatorInstructionDecoder* decoder, u8 opcode)
    {
        s8 offset = (s8)env->Memory()->FetchByte();
        X86EMU_DEBUG_MSG("JNP " << hex << offset);
        
        u16 newIP = env->Registers()->GetInstructionPointer() + (s16)offset;
        
        if(!env->Flags()->GetFlag(FLAG_PARITY))
        {
            X86EMU_DEBUG_MSG("Jumping to " << hex << newIP);
            env->Registers()->SetInstructionPointer(newIP);
        }
        
        env->Overrides()->Clear();
    }
    
    // opcode 0x7c
    // jump to byte offset if sign flag not equal to overflow flag
    void X86JumpNearLessInstruction(X86EmulatorEnvironment* env, X86EmulatorInstructionDecoder* decoder, u8 opcode)
    {
        s8 offset = (s8)env->Memory()->FetchByte();
        X86EMU_DEBUG_MSG("JL " << hex << offset);
        
        u16 newIP = env->Registers()->GetInstructionPointer() + (s16)offset;
        
        if(env->Flags()->GetFlag(FLAG_SIGN) != env->Flags()->GetFlag(FLAG_OVERFLOW))
        {
            X86EMU_DEBUG_MSG("Jumping to " << hex << newIP);
            env->Registers()->SetInstructionPointer(newIP);
        }
        
        env->Overrides()->Clear();
    }
    
    // opcode 0x7d
    // jump to byte offset if sign flag equal to overflow flag
    void X86JumpNearNotLessInstruction(X86EmulatorEnvironment* env, X86EmulatorInstructionDecoder* decoder, u8 opcode)
    {
        s8 offset = (s8)env->Memory()->FetchByte();
        X86EMU_DEBUG_MSG("JNL " << hex << offset);
        
        u16 newIP = env->Registers()->GetInstructionPointer() + (s16)offset;
        
        if(env->Flags()->GetFlag(FLAG_SIGN) == env->Flags()->GetFlag(FLAG_OVERFLOW))
        {
            X86EMU_DEBUG_MSG("Jumping to " << hex << newIP);
            env->Registers()->SetInstructionPointer(newIP);
        }
        
        env->Overrides()->Clear();
    }
    
    // opcode 0x7e
    // jump to byte offset if sign flag not equal to overflow flag or zero flag is set
    void X86JumpNearLessOrEqualInstruction(X86EmulatorEnvironment* env, X86EmulatorInstructionDecoder* decoder, u8 opcode)
    {
        s8 offset = (s8)env->Memory()->FetchByte();
        X86EMU_DEBUG_MSG("JLE " << hex << offset);
        
        u16 newIP = env->Registers()->GetInstructionPointer() + (s16)offset;
        
        if((env->Flags()->GetFlag(FLAG_SIGN) != env->Flags()->GetFlag(FLAG_OVERFLOW)) || env->Flags()->GetFlag(FLAG_ZERO))
        {
            X86EMU_DEBUG_MSG("Jumping to " << hex << newIP);
            env->Registers()->SetInstructionPointer(newIP);
        }
        
        env->Overrides()->Clear();
    }
    
    // opcode 0x7f
    // jump to byte offset if sign flag equal to overflow flag and zero flag is clear
    void X86JumpNearNotLessOrEqualInstruction(X86EmulatorEnvironment* env, X86EmulatorInstructionDecoder* decoder, u8 opcode)
    {
        s8 offset = (s8)env->Memory()->FetchByte();
        X86EMU_DEBUG_MSG("JNLE " << hex << offset);
        
        u16 newIP = env->Registers()->GetInstructionPointer() + (s16)offset;
        
        if((env->Flags()->GetFlag(FLAG_SIGN) == env->Flags()->GetFlag(FLAG_OVERFLOW)) && !env->Flags()->GetFlag(FLAG_ZERO))
        {
            X86EMU_DEBUG_MSG("Jumping to " << hex << newIP);
            env->Registers()->SetInstructionPointer(newIP);
        }
        
        env->Overrides()->Clear();
    }
}

#endif
