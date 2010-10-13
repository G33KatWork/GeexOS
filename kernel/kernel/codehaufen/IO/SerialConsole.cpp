#include <kernel/IO/SerialConsole.h>

#include <string.h>
#include <arch/internal/ports.h>    //FIXME: remove this dependency, move this into libarch, since this is elementary for this architecture

using namespace IO;
using namespace Arch;

SerialConsole::SerialConsole(uint16_t port)
    :portNum(port)
{
    outb(port + 1, 0x00);    // Disable all interrupts
    outb(port + 3, 0x80);    // Enable DLAB (set baud rate divisor)
    outb(port + 0, 0x01);    // Set divisor to 1 (lo byte) 115200 baud
    outb(port + 1, 0x00);    //                  (hi byte)
    outb(port + 3, 0x03);    // 8 bits, no parity, one stop bit
    outb(port + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
    outb(port + 4, 0x0B);    // IRQs enabled, RTS/DSR set
    outb(port + 1, 0x0C);    // enable all interrupts.
    
    storedCharacterAvailable = false;
}

SerialConsole::~SerialConsole()
{
}

void SerialConsole::PrintChar(char c)
{
    if(foregroundColor != None || backgroundColor != None)
        startColor();
        
    while(!readyToTransmit());
    outb(portNum, c);
        
    if(foregroundColor != None || backgroundColor != None)
        endColor();
}

void SerialConsole::Clear()
{
}

void SerialConsole::fetchStoredCharacterIfNecessary()
{
    if(!storedCharacterAvailable)
    {
        storedCharacter = getCharFromHW();
        storedCharacterAvailable = true;
    }
}

bool SerialConsole::IsNextChar(char c)
{
    fetchStoredCharacterIfNecessary();
    return c == storedCharacter;
}

char SerialConsole::GetCharFromDevice(InputType type)
{
    fetchStoredCharacterIfNecessary();
    
    if(IsTypeOfChar(type, storedCharacter))
    {
        storedCharacterAvailable = false;
        return storedCharacter;
    }
    else
        return NULL;
}

char SerialConsole::getCharFromHW()
{
    while (!(inb(portNum + 5) & 0x01));
    return inb(portNum);
}

bool SerialConsole::readyToTransmit()
{
    uint8_t status = inb(portNum + 5);
    return (bool) (status & 0x20);
}

void SerialConsole::startColor()
{
#ifdef SERIAL_COLOR
    printColorString("\\e[");
    if (foregroundColor != None)
    {
        switch(foregroundColor)
        {
            case Black:
                printColorString("30");
                break;
            case Red:
                printColorString("31");
                break;
            case Green:
                printColorString("32");
                break;
            case Yellow:
                printColorString("33");
                break;
            case Blue:
                printColorString("34");
                break;
            case Magenta:
                printColorString("35");
                break;
            case Cyan:
                printColorString("36");
                break;
            case White:
                printColorString("37");
                break;
            case LightRed:
                printColorString("1;31");
                break;
            case LightGreen:
                printColorString("1;32");
                break;
            case LightBlue:
                printColorString("1;34");
                break;
            case LightMagenta:
                printColorString("1;35");
                break;
            case LightCyan:
                printColorString("1;36");
                break;
            default:
                printColorString("1");
        }
    }
        
    if (backgroundColor != None)
    {
        switch(backgroundColor)
        {
            case Black:
                printColorString("40");
                break;
            case Red:
                printColorString("41");
                break;
            case Green:
                printColorString("42");
                break;
            case Yellow:
                printColorString("43");
                break;
            case Blue:
                printColorString("44");
                break;
            case Magenta:
                printColorString("45");
                break;
            case Cyan:
                printColorString("46");
                break;
            case White:
                printColorString("47");
                break;
        }
    }
    
    printColorString("m");
#endif
}

void SerialConsole::endColor()
{
#ifdef SERIAL_COLOR
    printColorString("\\e[0m");
#endif
}

void SerialConsole::printColorString(const char* c)
{
    unsigned int i = 0;
    while(c[i])
    {
        while(!readyToTransmit());
        outb(portNum, c[i]);
        i++;
    }
}