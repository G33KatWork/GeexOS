#ifndef _ARCHX86_VBE_OUTPUT_H_
#define _ARCHX86_VBE_OUTPUT_H_

#include <types.h>

#include <halinterface/BaseDebugOutputDevice.h>

namespace Debug
{
    class VBEDebugOutput : public Debug::BaseDebugOutputDevice
    {
    public:
        VBEDebugOutput();
        ~VBEDebugOutput();
        
        void PrintChar(char c);
        void Clear();
        
        void Test();
        
    private:
    };
}

#endif
