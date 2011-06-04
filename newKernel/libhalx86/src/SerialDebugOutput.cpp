#include <arch/SerialDebugOutput.h>

#include <string.h>
#include <arch/internal/PortIO.h>

using namespace Arch;
using namespace Debug;

SerialDebugOutput::SerialDebugOutput()
{
    portNum = SERIAL_COM1;
    
    outb(portNum + 1, 0x00);    // Disable all interrupts
    outb(portNum + 3, 0x80);    // Enable DLAB (set baud rate divisor)
    outb(portNum + 0, 0x01);    // Set divisor to 1 (lo byte) 115200 baud
    outb(portNum + 1, 0x00);    //                  (hi byte)
    outb(portNum + 3, 0x03);    // 8 bits, no parity, one stop bit
    outb(portNum + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
    outb(portNum + 4, 0x0B);    // IRQs enabled, RTS/DSR set
    outb(portNum + 1, 0x0C);    // enable all interrupts.
}

SerialDebugOutput::SerialDebugOutput(uint16_t port)
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
}

SerialDebugOutput::~SerialDebugOutput()
{
}

void SerialDebugOutput::PrintChar(char c)
{
    if(foregroundColor != None || backgroundColor != None)
        startColor();
        
    while(!readyToTransmit());
    outb(portNum, c);
        
    if(foregroundColor != None || backgroundColor != None)
        endColor();
}

void SerialDebugOutput::Clear()
{
}

bool SerialDebugOutput::readyToTransmit()
{
    uint8_t status = inb(portNum + 5);
    return (bool) (status & 0x20);
}

void SerialDebugOutput::startColor()
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

void SerialDebugOutput::endColor()
{
#ifdef SERIAL_COLOR
    printColorString("\\e[0m");
#endif
}

void SerialDebugOutput::printColorString(const char* c)
{
    unsigned int i = 0;
    while(c[i])
    {
        while(!readyToTransmit());
        outb(portNum, c[i]);
        i++;
    }
}
