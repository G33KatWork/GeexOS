#ifndef _X86EMU_X86EMUREGISTERS_H
#define _X86EMU_X86EMUREGISTERS_H

#include <x86emu/x86EmulatorTypes.h>
#include <x86emu/x86SegmentOverrides.h>
#include <string.h>

namespace X86Emu
{
    class X86EmulatorEnvironment;   //circular header inclusions... bargh!
    
    /*
     * Registers are encoded in the opcodes with a 4 bit value:
     * The first 2 bits specify the type of the register:
     * 00 = Lower Byte (AL, BL, CL, DL)
     * 01 = Upper Byte (AH, BH, CH, DH)
     * 10 = Complete Word or DWord (AX, BX, CX, DX)
     * 11 = Index registers (SP, BP, SI, DI)
     *
     * The next two bits specify the register itself:
     * 00 = A or SP
     * 11 = B or DI
     * 01 = C or BP
     * 10 = D or SI
     *
     * Examples:
     * 0000 = AL
     * 1001 = CX
     * 0110 = DH
     * 1100 = SP
     *
     * E*X registers are accessed by using *X register selectors and
     * setting the 32 bit data flag in the emulator environment
     */
    #define REGISTER_TYPE(x)    ((x >> 2) & 0x3)
    #define REGISTER_TYPE_LOW   0x0
    #define REGISTER_TYPE_HIGH  0x1
    #define REGISTER_TYPE_WORD  0x2
    #define REGISTER_TYPE_INDEX 0x3
    
    #define REGISTER(x)         (x & 0x3)
    #define REGISTER_A          0x0
    #define REGISTER_B          0x3
    #define REGISTER_C          0x1
    #define REGISTER_D          0x2
    
    #define NUM_REGS        8
    enum X86Register
    {
        AL = 0x0, AH = 0x4, AX = 0x8,
        BL = 0x3, BH = 0x7, BX = 0xb,
        CL = 0x1, CH = 0x5, CX = 0x9,
        DL = 0x2, DH = 0x6, DX = 0xa,
        SP = 0xc,
        BP = 0xd,
        SI = 0xe,
        DI = 0xf
    };
    
    #define NUM_SEGMENTS    6
    enum X86SegmentRegister
    {
        CS = 0x1,
        DS = 0x3,
        ES = 0x0,
        FS = 0x4,
        GS = 0x5,
        SS = 0x2
    };
    
    class X86Registers
    {
    private:
        u32 registers[NUM_REGS + NUM_SEGMENTS + 1];
        X86EmulatorEnvironment* env;
        
    public:
        X86Registers(X86EmulatorEnvironment* Environment)
        {
            memset(&registers, 0, sizeof(registers));
            env = Environment;
        }
        
        inline u16 GetSegmentRegister(X86SegmentRegister seg) { return registers[NUM_REGS + seg] & 0x0000FFFF; }
        inline void SetSegmentRegister(X86SegmentRegister seg, u16 NewValue) { registers[NUM_REGS + seg] = NewValue; }
        
        inline u16 GetInstructionPointer() { return (u16)(registers[NUM_REGS + NUM_SEGMENTS]); }
        inline void SetInstructionPointer(u16 NewValue) { registers[NUM_REGS + NUM_SEGMENTS] = NewValue; }
        
        u32 GetRegister(X86Register reg);
        void SetRegister(X86Register reg, u32 NewValue);
        
        const char* GetRegisterName(X86Register reg);
    };
}

#endif
