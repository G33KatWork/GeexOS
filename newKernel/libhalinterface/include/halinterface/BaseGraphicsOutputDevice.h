#ifndef _HAL_INTERFACES_BASE_GRAPHICS_OUTPUT_DEVICE_H
#define _HAL_INTERFACES_BASE_GRAPHICS_OUTPUT_DEVICE_H

#include <types.h>
#include <halinterface/BaseOutputDevice.h>

namespace Debug
{   
    //Baseclass for simple character output devices
    class BaseGraphicsOutputDevice : public BaseOutputDevice
    {
    public:
        BaseGraphicsOutputDevice() {}
        ~BaseGraphicsOutputDevice() {}

        virtual void Initialize() = 0;

        //BaseOutputDevice Methods
        void PrintChar(char c __attribute__((unused))) {}
        void Clear() {}
    };
}

#endif
