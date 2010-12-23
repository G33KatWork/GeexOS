#ifndef _ARCHX86_PROCESSOR_H_
#define _ARCHX86_PROCESSOR_H_

#include <types.h>
#include <halinterface/BaseProcessor.h>
#include <arch/internal/LAPIC.h>

namespace Arch
{
    class x86Processor : public BaseProcessor
    {
    public:
        x86Processor(unsigned int processorID, LAPIC* localAPIC) 
        {
            id = processorID;
            lapic = localAPIC;
        }
        virtual ~x86Processor() {}
        
        virtual unsigned int GetID() { return id; }
        virtual void StartUp() {}
        
        LAPIC* GetLAPIC() { return lapic; }

    private:
        unsigned int id;
        LAPIC* lapic;
    };
}

#endif
