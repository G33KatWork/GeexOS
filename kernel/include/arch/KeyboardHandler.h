#ifndef KEYBOARDHANDLER_H_
#define KEYBOARDNHANDLER_H_

#include <lib/types.h>
#include <kernel/IInterruptServiceRoutine.h>
#include <arch/ports.h>
#include <kernel/global.h>
#include <kernel/utils/scancodes.h>

using namespace Arch;
using namespace IO;

class KeyboardHandler : public IInterruptServiceRoutine
{
public:
    KeyboardHandler()
    {
        shift_state = false;
    }
    
    void Execute(registers_t* UNUSED(regs))
    {
        uint8_t new_scan_code = inb(0x60);
        switch(new_scan_code)
        {
            case 0x2a: shift_state = true; break;
            case 0xaa: shift_state = false; break;
            default:
                if (new_scan_code & 0x80) {
                  // ignore the break code
               } else {
                  char new_char=(shift_state?shifted:unshifted)[new_scan_code][1];
                  kdbg.PrintChar(new_char);
               } 
           break;
         }
    }
    
private:
    bool shift_state;
};

#endif
