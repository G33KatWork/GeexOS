#include <halinterface/HAL.h>

using namespace Arch;
using namespace Debug;

void ModuleStartup()
{
    *CurrentHAL->GetCurrentDebugOutputDevice() << "Modul sagt Hallo!" << endl;
}

void ModuleShutdown()
{
    *CurrentHAL->GetCurrentDebugOutputDevice() << "Modul sagt Tschuess!" << endl;
}
