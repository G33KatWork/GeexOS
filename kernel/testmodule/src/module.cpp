#include <halinterface/HAL.h>

using namespace Arch;
using namespace Debug;

extern "C" void ModuleEntryPoint();

void ModuleEntryPoint()
{
	*CurrentHAL->GetCurrentDebugOutputDevice() << "Modul Entrypoint!" << endl;
}

void ModuleStartup()
{
    *CurrentHAL->GetCurrentDebugOutputDevice() << "Modul sagt Hallo!" << endl;
}

void ModuleShutdown()
{
    *CurrentHAL->GetCurrentDebugOutputDevice() << "Modul sagt Tschuess!" << endl;
}
