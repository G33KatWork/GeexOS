OUTPUT_FORMAT("elf32-i386")
ENTRY (start)

SECTIONS{
    . = 0x00100000;

    .multibootHeader :
    {
        *(.multibootHeader)
    }
    
    .text : AT(ADDR(.text))
    {
        code = .; _code = .; __code = .;
        *(.text)
        *(.rodata)
    }

    .rodata ALIGN (4096) : AT(ADDR(.rodata))
    {
        *(.rodata)
    }

    .data ALIGN (4096) : AT(ADDR(.data))
    {
        start_ctors = .;
        *(.ctor*)
        end_ctors = .;
        
        data = .; _data = .; __data = .;
        *(.data)
    }

    .bss ALIGN (4096) : AT(ADDR(.bss))
    {
        sbss = .;
        bss = .; _bss = .; __bss = .;
        *(COMMON)
        *(.bss)
        ebss = .;
    }
    
    end = .; _end = .; __end = .;
}