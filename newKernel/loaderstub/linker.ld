OUTPUT_FORMAT(elf32-i386)
OUTPUT_ARCH(i386)
ENTRY (start)

SECTIONS{
    . = 0x00100000;

    .text :
    {
        *(.text*)
    }

    .rodata ALIGN (0x1000) :
    {
        *(.rodata*)
    }

    .data ALIGN (0x1000) :
    {
        *(.data*)
    }

    .bss :
    {
        sbss = .;
        *(COMMON)
        *(.bss*)
        ebss = .;
    }
}
