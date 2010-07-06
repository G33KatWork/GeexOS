#ifndef _HAL_INTERFACES_CLOCKSOURCE_H
#define _HAL_INTERFACES_CLOCKSOURCE_H

#include <types.h>

namespace Arch
{
    enum ClockType {
        CLKTYPE_PERIODIC,
        CLKTYPE_ONESHOT
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

    typedef struct clock_source ClockSource;
}

#endif
