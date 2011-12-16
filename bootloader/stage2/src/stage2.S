[BITS 16]
[ORG 0x0]
[MAP ALL stage2.map]

start:
    ;we are loaded at 0x4000:0 and upwards by first stage
    
    ;set up segment registers
    push    cs
    pop     ax
    mov     ds, ax
    mov     es, ax
    mov     fs, ax
    mov     gs, ax

    ;set up a stack at 0x3000:0 (=0x30000) directly under our code
    push    0x3000
    pop     ss
    mov     sp, 0x0

    call    bios_print
    db 'GeexLDR Stage2', 13, 10, 0

    call    enableA20
    cmp     ax, 0
    je      .a20error

.foo:
    call    loadGDT
    cli
    mov     eax, cr0
    or      eax, 1
    mov     cr0, eax
    
    jmp DWORD   0x8:pmodeEntry
    
.a20error:
    call    bios_print
    db 'Enabling A20 gate failed', 13, 10, 0

.out:
    cli
    hlt

;--------- A20 Gate ----------
%include "src/a20.inc"

;------------ GDT ------------
%include "src/gdt.inc"

;--------- Printing ----------
;print string after call to this function
;dirty, but nice
bios_print:
    pop     si
    lodsb
    push    si
    or      al, al
    jz      .done
    mov     ah, 0x0E
    int     0x10
    jmp     bios_print
.done:
    ret

;HACKHACK
;This is a hack, because I couldn't get NASM to calculate the correct offset to pmode labels
times 512 - ($-$$)  DB 0

;---------- 32 Bit -----------
[BITS 32]
[SEGMENT start=0x40000+512 vstart=0x40000+512 follows=.text]
pmodeEntry:
    mov     ax, GDT_DATA_DESC
    mov     ds, ax
    mov     ss, ax
    mov     es, ax
    mov     esp, 0x30000
    
    call    ClearScreen32
    
    mov     ebx, pmodeMsg
    call    Puts32

.hang:
    jmp     .hang

pmodeMsg:   db  'Welcome to protected mode', 10, 0

;------ 32 Bit Printing ------
%include "src/pmodePrint.inc"
