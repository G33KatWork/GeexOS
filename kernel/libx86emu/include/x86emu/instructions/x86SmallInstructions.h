#ifndef _X86EMU_X86EMUNOPINSTRUCTION_H
#define _X86EMU_X86EMUNOPINSTRUCTION_H

#include <x86emu/X86EmulatorEnvironment.h>
#include <x86emu/X86EmulatorInstructionDecoder.h>
#include <x86emu/x86EmulatorDebug.h>

namespace X86Emu
{
    // opcode 0x90
    void X86NopInstruction(X86EmulatorEnvironment* env, X86EmulatorInstructionDecoder* decoder, u8 opcode)
    {
        X86EMU_DEBUG_MSG("NOP");
        env->Overrides()->Clear();
    }
    
    // opcode 0x9b
    void X86WaitInstruction(X86EmulatorEnvironment* env, X86EmulatorInstructionDecoder* decoder, u8 opcode)
    {
        X86EMU_DEBUG_MSG("WAIT");
        env->Overrides()->Clear();
    }
    
    void X86HltInstruction(X86EmulatorEnvironment* env, X86EmulatorInstructionDecoder* decoder, u8 opcode)
    {
        X86EMU_DEBUG_MSG("HLT");
        env->Overrides()->Clear();
        env->HaltMachine();
    }
    
    void X86CmcInstruction(X86EmulatorEnvironment* env, X86EmulatorInstructionDecoder* decoder, u8 opcode)
    {
        X86EMU_DEBUG_MSG("CMC");
        env->Flags()->SetFlag(FLAG_CARRY, !env->Flags()->GetFlag(FLAG_CARRY));
        env->Overrides()->Clear();
    }
    
    // opcode 0xf8
    void X86ClcInstruction(X86EmulatorEnvironment* env, X86EmulatorInstructionDecoder* decoder, u8 opcode)
    {
        X86EMU_DEBUG_MSG("CLC");
        env->Flags()->SetFlag(FLAG_CARRY, false);
        env->Overrides()->Clear();
    }
    
    // opcode 0xf9
    void X86StcInstruction(X86EmulatorEnvironment* env, X86EmulatorInstructionDecoder* decoder, u8 opcode)
    {
        X86EMU_DEBUG_MSG("STC");
        env->Flags()->SetFlag(FLAG_CARRY, true);
        env->Overrides()->Clear();
    }
    
    // opcode 0xfa
    void X86CliInstruction(X86EmulatorEnvironment* env, X86EmulatorInstructionDecoder* decoder, u8 opcode)
    {
        X86EMU_DEBUG_MSG("CLI");
        env->Flags()->SetFlag(FLAG_INTERRUPT, false);
        env->Overrides()->Clear();
    }
    
    // opcode 0xfb
    void X86StiInstruction(X86EmulatorEnvironment* env, X86EmulatorInstructionDecoder* decoder, u8 opcode)
    {
        X86EMU_DEBUG_MSG("STI");
        env->Flags()->SetFlag(FLAG_INTERRUPT, true);
        env->Overrides()->Clear();
    }
    
    // opcode 0xfc
    void X86CldInstruction(X86EmulatorEnvironment* env, X86EmulatorInstructionDecoder* decoder, u8 opcode)
    {
        X86EMU_DEBUG_MSG("CLD");
        env->Flags()->SetFlag(FLAG_DIRECTION, false);
        env->Overrides()->Clear();
    }
    
    // opcode 0xfd
    void X86StdInstruction(X86EmulatorEnvironment* env, X86EmulatorInstructionDecoder* decoder, u8 opcode)
    {
        X86EMU_DEBUG_MSG("STD");
        env->Flags()->SetFlag(FLAG_DIRECTION, true);
        env->Overrides()->Clear();
    }
    
    // opcode 0xf0
    void X86LockInstruction(X86EmulatorEnvironment* env, X86EmulatorInstructionDecoder* decoder, u8 opcode)
    {
        X86EMU_DEBUG_MSG("LOCK");
        env->Overrides()->Clear();
    }
    
    // opcode 0x9e
    // set flags to AH
    void X86SetFlagsFromAHInstruction(X86EmulatorEnvironment* env, X86EmulatorInstructionDecoder* decoder, u8 opcode)
    {
        X86EMU_DEBUG_MSG("SAHF");
        env->Flags()->SetFlagRegister(env->Registers()->GetRegister(AH));
        env->Overrides()->Clear();
    }
    
    // opcode 0x9f
    // set AH to flags
    void X86SetAHFromFlagsInstruction(X86EmulatorEnvironment* env, X86EmulatorInstructionDecoder* decoder, u8 opcode)
    {
        X86EMU_DEBUG_MSG("LAHF");
        env->Registers()->SetRegister(AH, (u8)env->Flags()->GetFlagRegister());
        env->Overrides()->Clear();
    }
}

#endif
