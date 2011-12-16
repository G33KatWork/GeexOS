%ifndef _A20_INCLUDED_
%define _A20_INCLUDED_

enableA20:
    call    isA20Enabled
    cmp     ax, 1
    je      .enabled

    call    a20BiosEnable

    call    isA20Enabled
    cmp     ax, 1
    je      .enabled

    call    a20KeyboardEnable

    call    isA20Enabled
    cmp     ax, 1
    je      .enabled

    call    a20KeyboardFastEnable

    call    isA20Enabled
    cmp     ax, 1
    je      .enabled

    jmp     .err

.enabled:
    mov     ax, 1
    ret

.err:
    xor     ax, ax
    ret

;Enable A20 with BIOS function
a20BiosEnable:
    mov     ax, 0x2401
    int     0x15
    ret

;Use keyboard enable
a20KeyboardEnable:
    cli
 
    call    a20wait
    mov     al,0xAD
    out     0x64,al
 
    call    a20wait
    mov     al,0xD0
    out     0x64,al
 
    call    a20wait2
    in      al,0x60
    push    eax
 
    call    a20wait
    mov     al,0xD1
    out     0x64,al
 
    call    a20wait
    pop     eax
    or      al,2
    out     0x60,al
 
    call    a20wait
    mov     al,0xAE
    out     0x64,al
 
    call    a20wait
    sti
    ret
 
a20wait:
    in      al,0x64
    test    al,2
    jnz     a20wait
    ret

a20wait2:
    in      al,0x64
    test    al,1
    jz      a20wait2
    ret

;Use fast keyboard enable
a20KeyboardFastEnable:
    in      al, 0x92
    or      al, 2
    out     0x92, al
    ret

;Checks wether the A20 gate is enabled
;returns 1 if enabled, 0 otherwise
isA20Enabled:
    pushf
    push ds
    push es
    push di
    push si

    cli

    xor ax, ax ; ax = 0
    mov es, ax

    not ax ; ax = 0xFFFF
    mov ds, ax

    mov di, 0x0500
    mov si, 0x0510

    mov al, byte [es:di]
    push ax

    mov al, byte [ds:si]
    push ax

    mov byte [es:di], 0x00
    mov byte [ds:si], 0xFF

    cmp byte [es:di], 0xFF

    pop ax
    mov byte [ds:si], al

    pop ax
    mov byte [es:di], al

    mov ax, 0
    je .out

    mov ax, 1

.out:
    pop si
    pop di
    pop es
    pop ds
    popf

    ret

%endif
