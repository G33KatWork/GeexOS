#ifndef _DEBUG_H
#define _DEBUG_H

#include <lib/types.h>
#include <kernel/elf32.h>

namespace Debug
{
    extern Elf32SectionHeader* symbolTable;
    extern Elf32SectionHeader* stringTable;
    
    char* FindSymbol(Address addr, Address *symbolStart = NULL);
}

#endif
