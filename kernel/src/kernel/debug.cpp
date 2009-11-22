#include <kernel/debug.h>

Elf32SectionHeader* Debug::symbolTable = NULL;
Elf32SectionHeader* Debug::stringTable = NULL;

char* Debug::FindSymbol(Address addr, Address *symbolStart)
{   
    if(symbolTable == NULL || stringTable == NULL)
        return NULL;
    
    Elf32Symbol *symbol = (Elf32Symbol *)symbolTable->addr;
    for(unsigned int i = 0; i < symbolTable->size / sizeof(Elf32Symbol); i++)
    {
        if( (addr >= symbol->value) && (addr < (symbol->value + symbol->size)) )
        {
            char *c = (char *)(symbol->name) + stringTable->addr;
            if(symbolStart)
                *symbolStart = symbol->value;
            
            return c;
        }
        
        symbol++;
    }
 
    return NULL;
}
