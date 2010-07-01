[GLOBAL gdt_flush]
extern gp

; load the real gdt
gdt_flush:
	lgdt [gp]
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	jmp 0x08:flush2

flush2:
	ret

[GLOBAL tss_flush]
tss_flush:
    mov ax, 0x2B        ;5th selector in GDT = 0x26 - lowest 2 bits set to define rpl to ring 3
    ltr ax
    ret
