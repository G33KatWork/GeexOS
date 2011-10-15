#ifndef _ARCHX86_SERIAL_OUTPUT_H_
#define _ARCHX86_SERIAL_OUTPUT_H_

#include <types.h>
#include <halinterface/BaseOutputDevice.h>

#define SERIAL_COM1  0x3f8
#define SERIAL_COM2  0x2f8
//#define SERIAL_COLOR

namespace Debug
{
    class SerialDebugOutput : public Debug::BaseOutputDevice
    {
    public:
        SerialDebugOutput();
        SerialDebugOutput(uint16_t port);
        ~SerialDebugOutput();
        
        //CharacterOutputDevice Methods
        void PrintChar(char c);
        void Clear();
        
    private:
        uint16_t portNum;
        
        bool readyToTransmit();
        void startColor();
        void endColor();
        
        //We cannot just use "PrintString" from CharacterOutputDevice
        //for outputting color-start and end strings. We would end up in
        //and endless loop, then we are smashing the stack, are triple
        //faulting and we burn in hell
        void printColorString(const char* c);
    };
}
#endif
