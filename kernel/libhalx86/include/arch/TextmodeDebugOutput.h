#ifndef _ARCHX86_TEXTMODE_OUTPUT_H_
#define _ARCHX86_TEXTMODE_OUTPUT_H_

#include <types.h>
#include <halinterface/BaseOutputDevice.h>

namespace Debug
{
    class TextmodeDebugOutput : public Debug::BaseOutputDevice
    {
    public:
        TextmodeDebugOutput();
        ~TextmodeDebugOutput();
        
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
