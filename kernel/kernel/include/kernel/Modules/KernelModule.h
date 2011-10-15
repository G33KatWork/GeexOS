#ifndef _KERNEL_MODULE_H_
#define _KERNEL_MODULE_H_

#include <types.h>
#include <kernel/Objects/GXObject.h>

namespace Modules
{
    class KernelModule : public GXObject
    {
        GXDeclareDefaultMetadata(KernelModule)
        
    public:
        void Plug() { MAIN_DEBUG_MSG("Mod, mod, mod!!!1!"); }    
    };
    
    GXImplementMetaClassAndDefaultStructors(KernelModule, GXObject)
}

#endif
