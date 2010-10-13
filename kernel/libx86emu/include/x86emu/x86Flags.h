#ifndef _X86EMU_X86EMUFLAGS_H
#define _X86EMU_X86EMUFLAGS_H

#include <x86emu/x86EmulatorTypes.h>
#include <string.h>

namespace X86Emu
{
    #define NUM_FLAGS       9
    enum X86Flag
    {
        FLAG_CARRY      = 0x0,
        FLAG_PARITY     = 0x1,
        FLAG_AUX        = 0x2,
        FLAG_ZERO       = 0x3,
        FLAG_SIGN       = 0x4,
        FLAG_TRAP       = 0x5,
        FLAG_INTERRUPT  = 0x6,
        FLAG_DIRECTION  = 0x7,
        FLAG_OVERFLOW   = 0x8
    };
    
    class X86Flags
    {
    private:
        bool flagValues[NUM_FLAGS];
        
    public:
        X86Flags() { ClearFlags(); }
        
        inline void ClearFlags()
        {
            memset(&flagValues, false, sizeof(flagValues));
        }
        
        inline bool GetFlag(X86Flag Flag)
        {
            return flagValues[Flag];
        }
        
        inline void SetFlag(X86Flag Flag, bool Value)
        {
            flagValues[Flag] = Value;
        }
        
        u16 GetFlagRegister()
        {
            u16 f = 0;
            flagValues[FLAG_CARRY] ? f |= 0x0001 : f;
            flagValues[FLAG_PARITY] ? f |= 0x0004 : f;
            flagValues[FLAG_AUX] ? f |= 0x0010 : f;
            flagValues[FLAG_ZERO] ? f |= 0x0040 : f;
            flagValues[FLAG_SIGN] ? f |= 0x0080 : f;
            flagValues[FLAG_TRAP] ? f |= 0x0100 : f;
            flagValues[FLAG_INTERRUPT] ? f |= 0x0200 : f;
            flagValues[FLAG_DIRECTION] ? f |= 0x0400 : f;
            flagValues[FLAG_OVERFLOW] ? f |= 0x0800 : f;
            
            f |= 0x0002;    //Always on
            return f;
        }
        
        void SetFlagRegister(u16 f)
        {
            flagValues[FLAG_CARRY] = f & 0x0001;
            flagValues[FLAG_PARITY] = f & 0x0004;
            flagValues[FLAG_AUX] = f & 0x0010;
            flagValues[FLAG_ZERO] = f & 0x0040;
            flagValues[FLAG_SIGN] = f & 0x0080;
            flagValues[FLAG_TRAP] = f & 0x0100;
            flagValues[FLAG_INTERRUPT] = f & 0x0200;
            flagValues[FLAG_DIRECTION] = f & 0x0400;
            flagValues[FLAG_OVERFLOW] = f & 0x0800;
        }
    };
}

#endif
