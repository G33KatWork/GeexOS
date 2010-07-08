#ifndef SERIAL_CONSOLE_H_
#define SERIAL_CONSOLE_H_

#include <types.h>
#include <kernel/IO/CharacterOutputDevice.h>
#include <kernel/IO/CharacterInputDevice.h>

#define SERIAL_COM1  0x3f8
#define SERIAL_COM2  0x2f8
//#define SERIAL_COLOR

namespace IO
{
    class SerialConsole : public CharacterOutputDevice, public CharacterInputDevice
    {
    public:
        SerialConsole(uint16_t port);
        ~SerialConsole();
        
        //CharacterOutputDevice Methods
        void PrintChar(char c);
        void Clear();
        
    protected:
        //CharacterInputDevice Methods
        bool IsNextChar(char c);
        char GetCharFromDevice(InputType type);
        
    private:
        uint16_t portNum;
        
        bool storedCharacterAvailable;
        char storedCharacter;
        
        bool readyToTransmit();
        void startColor();
        void endColor();
        
        //We cannot just use "PrintString" from CharacterOutputDevice
        //for outputting color-start and end strings. We would end up in
        //and endless loop, then we are smashing the stack, are triple
        //faulting and we burn in hell
        void printColorString(const char* c);
        
        void fetchStoredCharacterIfNecessary();
        char getCharFromHW();
    };
}
#endif
