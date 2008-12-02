org 0x10000         ;Starting at 1MB
bits 32             ;Hey, we have protected mode!

jmp Stage3

%include "testkernel/stdio.inc"

msg db 0x0A, 0x0A, "This is our test kernel speaking to you!", 0x0A, 0

Stage3:
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov esp, 9000h
    
    call ClrScr
    mov ebx, msg
    call Puts32
    
    cli
    hlt