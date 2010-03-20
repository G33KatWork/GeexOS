#ifndef _DEBUGGING_SYMBOLS_H
#define _DEBUGGING_SYMBOLS_H

#include <lib/types.h>
#include <kernel/elf32.h>

namespace Debug
{
    extern Elf32SectionHeader* symbolTable;
    extern Elf32SectionHeader* stringTable;
    
    char* FindSymbol(Address addr, Address *symbolStart = NULL);
}

#endif
