#ifndef _X86EMU_X86EMUSEGMENTOVERRIDEPREFIXES_H
#define _X86EMU_X86EMUSEGMENTOVERRIDEPREFIXES_H

#include <x86emu/X86EmulatorEnvironment.h>
#include <x86emu/X86EmulatorInstructionDecoder.h>
#include <x86emu/x86EmulatorDebug.h>

namespace X86Emu
{
    // opcode 0x2e
    void X86SetCSSegmentOverride(X86EmulatorEnvironment* env, X86EmulatorInstructionDecoder* decoder, u8 opcode)
    {
        X86EMU_DEBUG_MSG("Setting CS segment override flag");
        env->Overrides()->SetFlag(SEGOVR_CS, true);
    }
    
    // opcode 0x3e
    void X86SetDSSegmentOverride(X86EmulatorEnvironment* env, X86EmulatorInstructionDecoder* decoder, u8 opcode)
    {
        X86EMU_DEBUG_MSG("Setting DS segment override flag");
        env->Overrides()->SetFlag(SEGOVR_DS, true);
    }
    
    // opcode 0x26
    void X86SetESSegmentOverride(X86EmulatorEnvironment* env, X86EmulatorInstructionDecoder* decoder, u8 opcode)
    {
        X86EMU_DEBUG_MSG("Setting ES segment override flag");
        env->Overrides()->SetFlag(SEGOVR_ES, true);
    }
    
    // opcode 0x64
    void X86SetFSSegmentOverride(X86EmulatorEnvironment* env, X86EmulatorInstructionDecoder* decoder, u8 opcode)
    {
        X86EMU_DEBUG_MSG("Setting FS segment override flag");
        env->Overrides()->SetFlag(SEGOVR_FS, true);
    }
    
    // opcode 0x65
    void X86SetGSSegmentOverride(X86EmulatorEnvironment* env, X86EmulatorInstructionDecoder* decoder, u8 opcode)
    {
        X86EMU_DEBUG_MSG("Setting GS segment override flag");
        env->Overrides()->SetFlag(SEGOVR_GS, true);
    }
    
    // opcode 0x36
    void X86SetSSSegmentOverride(X86EmulatorEnvironment* env, X86EmulatorInstructionDecoder* decoder, u8 opcode)
    {
        X86EMU_DEBUG_MSG("Setting SS segment override flag");
        env->Overrides()->SetFlag(SEGOVR_SS, true);
    }
    
    // opcode 0x66
    void X86Set32BitDataPrefix(X86EmulatorEnvironment* env, X86EmulatorInstructionDecoder* decoder, u8 opcode)
    {
        X86EMU_DEBUG_MSG("Setting 32 bit data flag");
        env->Overrides()->SetFlag(PREFIX_DATA32, true);
    }
    
    // opcode 0x67
    void X86Set32BitAddressPrefix(X86EmulatorEnvironment* env, X86EmulatorInstructionDecoder* decoder, u8 opcode)
    {
        X86EMU_DEBUG_MSG("Setting 32 bit address flag");
        env->Overrides()->SetFlag(PREFIX_ADDR32, true);
    }
    
    // opcode 0xf2
    void X86SetRepnePrefix(X86EmulatorEnvironment* env, X86EmulatorInstructionDecoder* decoder, u8 opcode)
    {
        X86EMU_DEBUG_MSG("Setting REPNE prefix");
        env->Overrides()->SetFlag(PREFIX_REPNE, true);
        env->Overrides()->Clear();
    }
    
    // opcode 0xf3
    void X86SetRepePrefix(X86EmulatorEnvironment* env, X86EmulatorInstructionDecoder* decoder, u8 opcode)
    {
        X86EMU_DEBUG_MSG("Setting REPE prefix");
        env->Overrides()->SetFlag(PREFIX_REPE, true);
        env->Overrides()->Clear();
    }
}

#endif
