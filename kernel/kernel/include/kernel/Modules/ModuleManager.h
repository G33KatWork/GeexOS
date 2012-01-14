#ifndef _MODULEMANAGER_H_
#define _MODULEMANAGER_H_

#include <arch/BootEnvironment.h>
#include <kernel/Modules/KernelModule.h>

namespace Modules {
    class ModuleManager
    {
    public:
        ModuleManager(KernelBootModuleRepository* repo);
        
        void AddModule(KernelModule* module);
    };
}

#endif
