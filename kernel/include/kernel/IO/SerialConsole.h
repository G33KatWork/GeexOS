#ifndef SERIAL_CONSOLE_H_
#define SERIAL_CONSOLE_H_

#include <lib/types.h>
#include <kernel/IO/CharacterOutputDevice.h>

#define SERIAL_COM1  0x3f8
//#define SERIAL_COLOR

namespace IO
{
    class SerialConsole : public CharacterOutputDevice
    {
    public:
        SerialConsole(uint16_t port);
        ~SerialConsole();
        
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
