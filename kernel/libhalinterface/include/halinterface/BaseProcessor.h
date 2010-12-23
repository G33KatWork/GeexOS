#ifndef _HAL_INTERFACES_BASEPROCESSOR_H
#define _HAL_INTERFACES_BASEPROCESSOR_H

#include <types.h>

namespace Arch
{
    class BaseProcessor
    {
    protected:
        BaseProcessor() {}
        virtual ~BaseProcessor() {}
    
    public:
        /// Returns a unique ID of the processor in the System
        virtual unsigned int GetID() = 0;
        
        /// Starts the processor
        virtual void StartUp() = 0;
    };
}

#endif
