OUTPUT_FORMAT("elf32-i386")
ENTRY (start)

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
    
    .dynamic : { *(.dynamic) }
    .dynsym  ALIGN (4096)  : AT(ADDR(.dynsym)) { *(.dynsym) }
    .dynstr  ALIGN (4096)  : AT(ADDR(.dynstr)) { *(.dynstr) }

    end = .; _end = .; __end = .;

    . = 0xC0200000;
    .placement ALIGN(4096) : AT(ADDR(.placement))
    {
        placement = .;
        . += 0x200000;
    }
    
    /DISCARD/ :
    {
        /* We have no exceptions, throw this stuff away */
        *(.eh_frame)
        *(.eh_frame_hdr)
        *(.gcc_except_table)
        
        *(.comment)
        *(.note*)
        
        /* Throw away the relocation sections, the kernel is not relocatable for now */
        /**(.rel*)*/
    }
}