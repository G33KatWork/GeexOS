#include <x86emu/x86Emulator.h>
#include <x86emu/x86EmulatorDebug.h>

using namespace X86Emu;

void X86Emulator::Run()
{
    while(!env->MachineHalted())
    {
        decoder.ExecuteNext(env);
    }
    
    X86EMU_DEBUG_MSG("Emulator halted...");
}
