#ifndef MONITOR_H_
#define MONITOR_H_

#include <lib/types.h>

#include <kernel/IO/CharacterOutputDevice.h>

namespace IO
{
    class Monitor : public CharacterOutputDevice
    {
    public:
        Monitor();
        ~Monitor();
        
        void PrintChar(char c);
        void Clear();
        
    private:
        unsigned short *vidmem;
        unsigned int cursorX;
		unsigned int cursorY;
        
        void scroll();
        void moveCursor();
    };
}
#endif
