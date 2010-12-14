#ifndef _ARCHX86_ACPIPARSER_H
#define _ARCHX86_ACPIPARSER_H

#include <types.h>
#include <arch/types.h>

namespace Arch
{   
    class ACPIParser
    {
    public:
        ACPIParser() {}
    
        void Parse();
    };
}

#endif
