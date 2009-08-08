#include <kernel/debug.h>
#include <kernel/global.h>
#include <kernel/utils/demangle.h>

using namespace IO;

Elf32SectionHeader* Debug::symbolTable = NULL;
Elf32SectionHeader* Debug::stringTable = NULL;

void Debug::PrintStacktrace(unsigned int n)
{
    // Stack contains:
    //  function argument (n)
    //  Return address in calling function
    //  ebp of calling function (pointed to by current ebp)
    
    unsigned int * ebp = &n - 2;
    kout << "Stack trace:" << endl;
    
    for(unsigned int frame = 0; frame < n; ++frame)
    {
        unsigned int eip = ebp[1];
        if(eip == 0) // No caller on stack
            break;
            
        // Unwind to previous stack frame
        ebp = reinterpret_cast<unsigned int *>(ebp[0]);
        //unsigned int * arguments = &ebp[2];
        
        //get symbol name
        Address symStart;
        char *symName = FindSymbol(eip, &symStart);
        if(symName == NULL)
            kout << "\t" << "<" << hex << eip << ">\t" << "<unknown symbol>" << endl;
        else
        {
            LargeStaticString demangled = LargeStaticString();
            LargeStaticString toDemangle = LargeStaticString((const char*)symName);
            demangle_full(toDemangle, demangled);
            kout << "\t" << "<" << hex << symStart << ">\t" << demangled << endl;
        }
        
        
        //kout << "\t" << hex << eip << endl;
    }
}

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
