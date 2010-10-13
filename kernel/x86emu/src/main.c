#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>

#include "x86emu.h"

char _1234[] = { 0xb8, 0x34, 0x12 };

char* memory;

void printk(const char *fmt, ...)
{
    va_list argptr;
    va_start(argptr, fmt);
    vfprintf(stdout, fmt, argptr);
    fflush(stdout);
    va_end(argptr);
}

//M defined in x86emu/regs.h
int main(int argc, char* argv[])
{
    memset(&M, 0, sizeof(M));
    memory = malloc(0x1000);
    M.mem_base = (unsigned long)memory;
    M.mem_size = 0x1000;
    
    memcpy(memory, _1234, sizeof(_1234));
    
    X86EMU_exec();
    
    printf("eax: %x", M.x86.gen.A.I32_reg.e_reg);
    
    return 0;
}