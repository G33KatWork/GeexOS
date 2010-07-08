#ifndef _ARCHX86_PIT_H
#define _ARCHX86_PIT_H

#include <types.h>
#include <arch/types.h>

namespace Arch
{
    class PIT
    {
    public:
        PIT(uint32_t frequency);
        
        static void Disable();
        static void Enable();
    };
}

#endif
