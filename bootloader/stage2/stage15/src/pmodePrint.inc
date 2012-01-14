%ifndef _PMODEPRINT_INCLUDED_
%define _PMODEPRINT_INCLUDED_

[BITS 32]

%define VIDMEM          0xB8000
%define COLS            80
%define ROWS            25
%define CHAR_ATTRIB     0x0F

_CurX db 0
_CurY db 0

ClearScreen32:
    pusha
    
    mov     byte [_CurX], 0
    mov     byte [_CurY], 0
    
    mov     eax, VIDMEM
    mov     ecx, COLS*ROWS*2

.next:
    mov     byte [eax], 0
    inc     eax
    loop    .next
    
    popa
    ret

Putch32:
    pusha
    mov     edi, VIDMEM
    
    xor     eax, eax                ;clear eax, we'll need it
    mov     ecx, COLS*2             ;we have 2 bytes per char
    mov     al, byte [_CurY]        ;retrieve current current column
    mul     ecx                     ;multiply current column with bytes per column
    push    eax                     ;and save the result on the stack
    
    mov     al, byte [_CurX]        ;retrieve current row
    mov     cl, 2                   ;we need to multiply it by 2 -> two bytes per char
    mul     cl                      ;multiply - result is now in eax
    pop     ecx                     ;get column result
    add     eax, ecx                ;and add them
    
    xor     ecx, ecx                ;clear ecx
    add     edi, eax                ;add offset to base address
    
    
    cmp     bl, 0x0A                ;new line?
    je      .Row
    
    mov     dl, bl                  ;low byte of dx is character value
    mov     dh, CHAR_ATTRIB         ;high byte is attribute
    mov     word [edi], dx          ;write to videoram position
    
    
    inc     byte [_CurX]            ;increment position
    jmp     .done
    
    
.Row:
    mov     byte [_CurX], 0         ;x position is 0
    inc     byte [_CurY]            ;next row
    
.done:
    popa
    ret


Puts32:
    pusha
    push    ebx                     ;push string address to stack
    pop     edi                     ;and get it back to edi (now it is in ebx and edi)
    
.loop:
    mov     bl, byte [edi]          ;get character at edi
    cmp     bl, 0                   ;EOS?
    je      .done
    
    call    Putch32                 ;Print character
    
    inc edi
    jmp .loop
    
.done:
    popa
    ret

Puthex32:
    pusha
    push    ebx
    mov     ecx, 4
    
.nextByte:
    ;upper nibble
    mov     al, BYTE [esp + ecx - 1]
    and     al, 0xF0
    shr     al, 4
    ;make askii char
    add     al, 0x30
    cmp     al, 0x39
    jbe     .notHex
    add     al, 0x7
.notHex:
    xchg    al, bl
    call    Putch32
    
    ;lower nibble
    mov     al, BYTE [esp + ecx - 1]
    and     al, 0x0F
    ;make askii char
    add     al, 0x30
    cmp     al, 0x39
    jbe     .notHexLow
    add     al, 0x7
.notHexLow:
    xchg    al, bl
    call    Putch32
    
    loop    .nextByte
    
.done:
    pop     ebx
    popa
    ret

%endif
