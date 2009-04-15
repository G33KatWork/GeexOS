OUTPUT_FORMAT("elf32-i386")
ENTRY (start)

SECTIONS{
    . = 0x00100000;

    .setup :
	{
		*(.setup)
	}

    . += 0xC0000000;
    
    .text : AT(ADDR(.text) - 0xC0000000)
    {
        code = .; _code = .; __code = .;
        *(.text)
        *(.rodata)
    }

    .rodata ALIGN (4096) : AT(ADDR(.rodata) - 0xC0000000)
    {
        *(.rodata)
    }

    .data ALIGN (4096) : AT(ADDR(.data) - 0xC0000000)
    {
        start_ctors = .;
        *(.ctor*)
        end_ctors = .;
        start_dtors = .;
        *(.dtor*)
        end_dtors = .;
        
        data = .; _data = .; __data = .;
        *(.data)
    }

    .bss ALIGN (4096) : AT(ADDR(.bss) - 0xC0000000)
    {
        sbss = .;
        bss = .; _bss = .; __bss = .;
        *(COMMON)
        *(.bss)
        ebss = .;
    }
    
    end = .; _end = .; __end = .;
}