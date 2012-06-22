[SECTION .text]
global _gdt_flush
extern _gp

; load the real gdt
_gdt_flush:
	lgdt [_gp]
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	jmp 0x08:flush2

flush2:
	ret

global _tss_flush
_tss_flush:
    mov ax, 0x2B        ;5th selector in GDT = 0x26 - lowest 2 bits set to define rpl to ring 3
    ltr ax
    ret
