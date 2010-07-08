OUTPUT_FORMAT("elf32-i386")
ENTRY (start)

SECTIONS{
    . = 0x00100000;

    .setup :
	 {
	   *(.multibootHeader)
		*(.setup)
	}

    . += 0xC0000000;
    
    .text ALIGN (4096) : AT(ADDR(.text) - 0xC0000000)
    {
        code = .; _code = .; __code = .;
        *(.text*)
				*(.gnu.linkonce.t*)
    }

    .rodata ALIGN (4096) : AT(ADDR(.rodata) - 0xC0000000)
    {
        *(.rodata*)
				*(.gnu.linkonce.r*)
    }

    .data ALIGN (4096) : AT(ADDR(.data) - 0xC0000000)
    {
        start_ctors = .;
        *(.ctor*)
        end_ctors = .;
        
        data = .; _data = .; __data = .;
        *(.data)

				*(.gnu.linkonce.d*)
    }

    .bss ALIGN (4096) : AT(ADDR(.bss) - 0xC0000000)
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
    .placement ALIGN(4096) : AT(ADDR(.placement) - 0xC0000000)
    {
        placement = .;
        *(.placement)
    }
}