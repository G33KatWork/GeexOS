#ifndef _ARCH_CLOCK_SOURCE_H
#define _ARCH_CLOCK_SOURCE_H

#include <types.h>

namespace Arch
{
    enum ClockType {
        PERIODIC,
        ONESHOT
    };
    
    struct clock_source
    {
        const char *name;
        unsigned long tickLength;
        ClockType type;
        void (*prepare)(uint64_t ns);
        void (*enable)(void);
        void (*disable)(void);
    };

    typedef struct clock_source ClockSource_t;
}

#endif
