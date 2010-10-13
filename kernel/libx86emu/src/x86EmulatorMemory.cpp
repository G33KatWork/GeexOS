#include <x86emu/x86EmulatorMemory.h>
#include <x86emu/x86EmulatorDebug.h>
#include <x86emu/x86EmulatorEnvironment.h>

using namespace X86Emu;

u8 X86EmulatorMemory::ReadByte(Address a)
{
    if(!checkMemLimits(a, sizeof(u8)))
    {
        X86EMU_DEBUG_MSG("Memory read out of bounds at " << hex << (unsigned)a);
        env->HaltMachine();
        return 0;
    }
    
    return *((u8*)(mem_base + a));
}

u16 X86EmulatorMemory::ReadWord(Address a)
{
    if(!checkMemLimits(a, sizeof(u16)))
    {
        X86EMU_DEBUG_MSG("Memory read out of bounds at " << hex << (unsigned)a);
        env->HaltMachine();
        return 0;
    }
    
    return *((u16*)(mem_base + a));
}

u32 X86EmulatorMemory::ReadDword(Address a)
{ 
    if(!checkMemLimits(a, sizeof(u32)))
    {
        X86EMU_DEBUG_MSG("Memory read out of bounds at " << hex << (unsigned)a);
        env->HaltMachine();
        return 0;
    }
    
    return *((u32*)(mem_base + a));
}

u8 X86EmulatorMemory::FetchByte()
{
    X86EMU_DEBUG_MSG("Fetching byte from memory");
    u32 curIp = env->Registers()->GetInstructionPointer();
    u8 b = ReadByte(curIp);
    env->Registers()->SetInstructionPointer(curIp + 1);
    return b;
}

u16 X86EmulatorMemory::FetchWord()
{
    X86EMU_DEBUG_MSG("Fetching word from memory");
    u32 curIp = env->Registers()->GetInstructionPointer();
    u16 w = ReadWord(curIp);
    env->Registers()->SetInstructionPointer(curIp + 2);
    return w;
}

u32 X86EmulatorMemory::FetchDword()
{
    X86EMU_DEBUG_MSG("Fetching dword from memory");
    u32 curIp = env->Registers()->GetInstructionPointer();
    u32 d = ReadDword(curIp);
    env->Registers()->SetInstructionPointer(curIp + 4);
    return d;
}

void X86EmulatorMemory::WriteByte(Address a, u8 NewValue)
{
    if(!checkMemLimits(a, sizeof(u8)))
    {
        X86EMU_DEBUG_MSG("Memory write out of bounds at " << hex << (unsigned)a);
        env->HaltMachine();
    }
    else
    {
        X86EMU_DEBUG_MSG("Writing byte to memory " << hex << a);
        *((u8*)(mem_base + a)) = NewValue;
    }
}

void X86EmulatorMemory::WriteWord(Address a, u16 NewValue)
{
    if(!checkMemLimits(a, sizeof(u16)))
    {
        X86EMU_DEBUG_MSG("Memory write out of bounds at " << hex << (unsigned)a);
        env->HaltMachine();
    }
    else
    {
        X86EMU_DEBUG_MSG("Writing word to memory " << hex << a);
        *((u16*)(mem_base + a)) = NewValue;
    }
}

void X86EmulatorMemory::WriteDword(Address a, u32 NewValue)
{
    if(!checkMemLimits(a, sizeof(u32)))
    {
        X86EMU_DEBUG_MSG("Memory write out of bounds at " << hex << (unsigned)a);
        env->HaltMachine();
    }
    else
    {
        X86EMU_DEBUG_MSG("Writing dword to memory " << hex << a);
        *((u32*)(mem_base + a)) = NewValue;
    }
}
