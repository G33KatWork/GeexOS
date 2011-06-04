OUTPUT_FORMAT("elf32-i386")
ENTRY (start)

PHDRS {
}

SECTIONS {
    . += 0xC0000000;
    
    .text ALIGN (4096) : AT(ADDR(.text))
    {
        code = .; _code = .; __code = .;
        *(.text*)
        
        *(.gnu.linkonce.t*)
    }

    .rodata ALIGN (4096) : AT(ADDR(.rodata))
    {
        *(.rodata*)
        *(.gnu.linkonce.r*)
    }

    .data ALIGN (4096) : AT(ADDR(.data))
    {
        start_ctors = .;
        *(.ctor*)
        end_ctors = .;
        
        data = .; _data = .; __data = .;
        *(.data)

        *(.gnu.linkonce.d*)
    }

    .bss ALIGN (4096) : AT(ADDR(.bss))
    {
        sbss = .;
        bss = .; _bss = .; __bss = .;
        *(COMMON)
        *(.bss)
        ebss = .;

        *(.gnu.linkonce.b*)
    }
    
    end = .; _end = .; __end = .;

    . = 0xC0200000;
    .placement ALIGN(4096) : AT(ADDR(.placement))
    {
        placement = .;
		. += 0x200000;
    }
}