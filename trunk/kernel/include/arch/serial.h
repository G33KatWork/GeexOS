#ifndef _SERIAL_H
#define _SERIAL_H

#include <lib/types.h>

namespace Arch
{
    class Serial {
        //TODO: Implement this...
    private:
        
    public:
        enum BaudRates {
            /*Baudrate  =   Divider*/
            _115200     =   1,
            _57600      =   2,
            _38400      =   4,
            _19200      =   8,
            _9600       =   16,
            _4800       =   32,
            _2400       =   64,
            _1200       =   128,
            _300        =   384,
            _50         =   2304
        }
    };
}

#endif
