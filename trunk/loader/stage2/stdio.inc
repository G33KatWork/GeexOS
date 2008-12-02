%ifndef __STDIO_INC_67343546FDCC56AAB872_INCLUDED__
%define __STDIO_INC_67343546FDCC56AAB872_INCLUDED__

;************************************************;
;	Puts16 ()
;		-Prints a null terminated string
;	DS=>SI: 0 terminated string
;************************************************;

bits	16

Puts16:
		pusha				; save registers
.Loop1:
		lodsb				; load next byte from string from SI to AL
		or	al, al			; Does AL=0?
		jz	Puts16Done		; Yep, null terminator found-bail out
		mov	ah, 0eh			; Nope-Print the character
		int	10h			; invoke BIOS
		jmp	.Loop1			; Repeat until null terminator found
Puts16Done:
		popa				; restore registers
		ret				; we are done, so return

;------------------------------------------------------------------------------
bits	32

%define         VIDMEM  0xB8000
%define         COLS    80
%define         ROWS    25
%define         C_ATTR  14

_CurX           db  0
_CurY           db  0

;************************************************;
;	Putch32 ()
;		-Prints a character
;	BL: The character
;************************************************;
Putch32:
    pusha
    mov edi, VIDMEM
    
    xor eax, eax        ;calculate Y-position
    mov ecx, COLS*2
    mov al, byte[_CurY]
    mul ecx
    push eax            ;save _CurY * COLS
    
    mov al, byte [_CurX]
    mov cl, 2
    mul cl              ;multiply by 2 because each char = 2Bytes
    pop ecx
    add eax, ecx        ;add X-offset to row (x + y * width)
    
    xor ecx, ecx
    add edi, eax        ;add calculated offset to video ram beginning
    
    
    
    ;printing itself:
    
    cmp bl, 0x0A        ;if we are \n
    je .NewRow
    
    mov dl, bl
    mov dh, C_ATTR
    mov word [edi], dx  ;write char + attribute to current video ram location
    
    inc byte [_CurX]    ;Update position
    cmp byte [_CurX], COLS   ;EOL
    je .NewRow
    jmp .done
    
.NewRow:
    mov byte [_CurX], 0
    inc byte [_CurY]
    
.done:
    popa
    ret
    
;************************************************;
;	Puts32 ()
;		-Prints a null terminated string
;	EBX: The beginning of the string
;************************************************;
Puts32:
    pusha
    push ebx
    pop edi
    
.loop:
    mov bl, byte [edi]
    cmp bl, 0           ;EOS?
    je .done
    
    call Putch32
    inc edi             ;next char
    jmp .loop
    
.done:
    mov bh, byte [_CurY]    ;update cursor
    mov bl, byte [_CurX]
    call MovCur
    
    popa
    ret
    
;************************************************;
;	MovCur ()
;		-Moves hardware CRT Cursor the defined position
;	BH: Y-Position
;   BL: X-Position
;************************************************;
MovCur:
    pusha
    
    xor eax, eax        ;Calculate linear position
    mov ecx, COLS
    mov al, bh          ;y*     <-|
    mul ecx             ;COLS     |-- y*width + x
    add al, bl          ;+x     <-|
    mov ebx, eax
    
    ;low byte
    mov al, 0x0f        ;0x0f = low byte cursor location
    mov dx, 0x03D4      ;0x03D4 = CRT Index register
    out dx, al
    
    mov al, bl
    mov dx, 0x03D5      ;0x03D5 = CRT Data register
    out dx, al
    
    xor eax, eax
    
    ;high byte
    mov al, 0x0e        ;0x0e = high byte cursor location
    mov dx, 0x03D4      ;0x03D4 = CRT Index register
    out dx, al
    
    mov al, bh
    mov dx, 0x03D5      ;0x03D5 = CRT Data register
    out dx, al
    
    popa
    ret
    
;************************************************;
;	ClrScr ()
;		-Clear screen
;************************************************;
ClrScr:
    pusha
    cld                 ;prepare auto increment for DI
    mov edi, VIDMEM
    mov cx, 2000        ;2000 chars
    mov ah, C_ATTR
    mov al, ' '
    rep stosw           ;Fill (E)CX words at ES:[(E)DI] with AX
    
    mov byte [_CurX], 0
    mov byte [_CurY], 0

    popa
    ret

%endif ;__STDIO_INC_67343546FDCC56AAB872_INCLUDED__