#include <arch/VBEDebugOutput.h>
//#include <x86emu.h>
#include <arch/types.h>
#include <arch/AddressLayout.h>
#include <kernel/debug.h>
#include <string.h>

using namespace Debug;

VBEDebugOutput::VBEDebugOutput()
{
    /*M.mem_base = BIOS_ADDRESS;
    M.mem_size = BIOS_SIZE;*/
}

void VBEDebugOutput::Test()
{
    /*HAL_DEBUG_MSG("VBE TEST...");
    uint8_t _1234[] = { 0xb8, 0x34, 0x12, 0xc3 };
    
    memset(&M, 0, sizeof(M));
    memcpy((void*)0x0, _1234, sizeof(_1234));*/
    
    /*memcpy((void*)0x12000, _1234, sizeof(_1234));
    M.x86.spc.IP.I16_reg = 0x2000;
    M.x86.seg.CS = 1;
    M.x86.spc.IP.I16_reg = 0x2000;
    M.x86.seg.CS = 1;*/
    
    /*X86EMU_exec();
    
    HAL_DEBUG_MSG("VBE EAX: " << hex << (unsigned)M.x86.gen.A.I32_reg.e_reg);*/
}

VBEDebugOutput::~VBEDebugOutput()
{
}

void VBEDebugOutput::PrintChar(char c)
{   
}

void VBEDebugOutput::Clear()
{    
}
