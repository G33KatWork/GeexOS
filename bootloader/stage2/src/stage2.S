[BITS 16]
[ORG 0x0]
[MAP ALL stage2.map]

start:
    
entry:
    cli
    ;set up segment registers
    push cs
    pop ax
    mov	ds, ax
    mov	es, ax
    mov	es, ax

    ;set up a stack
    push 0x5000
    pop ss
    mov sp, 0x0
    
    sti

    call bios_print
    db 'GeexLDR Stage2', 13, 10, 0

    cli
    hlt

foo: resb 4096

;--------- Printing ----------
;print string after call to this function
;dirty, but nice
bios_print:
    pop si
    lodsb
    push si
    or al, al
    jz .done
    mov ah, 0x0E
    int 0x10
    jmp bios_print
.done:
    ret
