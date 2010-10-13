#ifndef _X86EMU_X86EMUMEMORY_H
#define _X86EMU_X86EMUMEMORY_H

#include <x86emu/x86EmulatorTypes.h>
#include <x86emu/x86EmulatorDebug.h>

namespace X86Emu
{
    class X86EmulatorEnvironment;   //circular header inclusions... bargh!
    
    class X86EmulatorMemory
    {
    private:
        X86EmulatorEnvironment* env;
        Address mem_base;
        size_t mem_size;
    
        bool checkMemLimits(Address a, size_t readSize) { return (a + mem_base + readSize) <= (mem_base + mem_size); }
        
    public:
        X86EmulatorMemory(Address MemoryBase, size_t MemorySize, X86EmulatorEnvironment* Environment)
        {
            X86EMU_DEBUG_MSG("Initializing emulator memory at " << hex << MemoryBase << " with size " << MemorySize);
            mem_base = MemoryBase;
            mem_size = MemorySize;
            env = Environment;
        }
        
        inline Address MemoryBase() { return mem_base; }
        inline size_t MemorySize() { return mem_size; }
        
        u8 ReadByte(Address a);
        u16 ReadWord(Address a);
        u32 ReadDword(Address a);
        
        u8 FetchByte();
        u16 FetchWord();
        u32 FetchDword();
        
        void WriteByte(Address a, u8 NewValue);
        void WriteWord(Address a, u16 NewValue);
        void WriteDword(Address a, u32 NewValue);
    };
}

#endif
