[BITS 32]
[EXTERN pmodeIdt]  ;defined in arch/i386/idt.c

;also defined in bioscall.h
;be sure to change it there too, if necessary
RMODESTACK          EQU     0x7000

[SECTION .data]
realmodeIdt:
    dw      0x3ff   ;limit
    dd      0x0     ;base

[SECTION .text]
switchToReal:
    cli
    
    ;Save protected mode stack
    mov     eax, esp
    mov     [protectedStackPtr], eax
    
    ;get return address for rmode code after transition and put on rmode stack
    mov     eax, [esp]
    mov     [RMODESTACK], eax
    
    ;set up new stack
    mov     eax, RMODESTACK
    mov     esp, eax
    
    ;segment descriptors
    ;TODO define 0x20
    mov     ax, 0x20
    mov     ds, ax
    mov     es, ax
    mov     fs, ax
    mov     gs, ax
    mov     ss, ax

    ;load realmode IVT
    lidt    [realmodeIdt]

    ;jump into pseudo real-mode code selector
    jmp 0x18:.disablePmode

.disablePmode:
[BITS 16]
    ;disable protected mode
    mov     eax, cr0
    and     eax, 0xFFFFFFFE
    mov     cr0, eax
    
    ;jump into realmode
    jmp DWORD 0x0:.rmode

.rmode:
    ;set segment registers to 0
    xor     eax, eax
    mov     ds, ax
    mov     es, ax
    mov     fs, ax
    mov     gs, ax
    mov     ss, ax
    
    sti
    
    ;return to 32 bit address on stack
    db 0x66
    ret

[BITS 16]
switchToProt:
    cli
    
    ;enable protected mode
    mov     eax, cr0
    or      eax, 1
    mov     cr0, eax
    
    ;jump to 32 bit protected mode selector
    jmp DWORD 0x8:.pmodeEnabled

[BITS 32]
.pmodeEnabled:
    ;restore GDT data selectors
    mov     ax, 0x10
    mov     ds, ax
    mov     es, ax
    mov     fs, ax
    mov     gs, ax
    mov     ss, ax
    
    ;save return value to temporary location
    mov     eax, [esp]
    mov     [RMODESTACK], eax
    
    ;restore old pmode stack
    mov     eax, [protectedStackPtr]
    mov     esp, eax
    
    ;store return address to pmode stack
    mov     eax, [RMODESTACK]
    mov     [esp], eax
    
    xor     eax, eax
    
    ;reaload protected mode IDT
    lidt    [pmodeIdt]
    
    ret

[BITS 32]
[global callInt]
callInt:
    push    ebp
    mov     ebp, esp
    
    ;save registers that need to be preserved
    pusha
    
    ;set interrupt number for int instruction
    mov     eax, [ebp+8]
    mov BYTE [.intNo], al
    
    ;get pointer to register struct
    mov     edx, [ebp+12]
    
    ;save pointer on stack to retrieve it afterwards and assign new register values
    push    edx
    
    ;set eax value
    mov     eax, [edx]
    mov     [.eaxVal], eax
    
    ;set es
    mov WORD ax, [edx+4]
    mov WORD [.esVal], ax
    
    ;set ds
    mov WORD ax, [edx+6]
    mov WORD [.dsVal], ax
    
    ;set eflags
    mov WORD ax, [edx+8]
    mov WORD [.eflagsVal], ax
    
    ;set the rest of the registers
    mov DWORD ebx, [edx+12]
    mov DWORD ecx, [edx+16]
    mov DWORD edi, [edx+20]
    mov DWORD esi, [edx+24]
    mov DWORD edx, [edx+28]
    
    call    switchToReal
[BITS 16]
    ;save ds, as it may be modified below
    mov     ax, ds
    push    ax
    
    ;load es
    db      0xb8        ;mov ax, imm16
.esVal:
    dw      0x0
    mov     es, ax
    
    ;load ds
    db      0xb8        ;mov ax, imm16
.dsVal:
    dw      0x0
    mov     ds, ax

    ;load flags
    db      0xb8        ;mov ax, imm16
.eflagsVal:
    dw      0x0
    push    ax
    popf
    
    ;load eax
    db      0x66, 0xb8  ;mov eax, imm32
.eaxVal:
    dd      0x0
    
    ;execute int
    db      0xcd        ;int imm8
.intNo:
    db      0
    
    ;save some of the results in local variables, they will change in switchToProt
    mov     [DWORD cs:.eaxVal], eax
    
    mov     ax, ds
    mov     [DWORD cs:.dsVal], ax
    
    ;restore saved ds
    pop     ax
    mov     ds, ax
    
    ;save eflags
    pushf
    pop     ax
    mov     [DWORD .eflagsVal], ax
    
    ;save es
    mov     ax, es
    mov     [DWORD .esVal], ax
    
    
    call DWORD switchToProt
[BITS 32]
    ;retrieve pointer to register struct
    pop     eax
    
    mov     [eax+12], ebx
    mov     [eax+16], ecx
    mov     [eax+20], edi
    mov     [eax+24], esi
    mov     [eax+28], edx
    
    mov     edx, eax
    
    mov     eax, [.eaxVal]
    mov     [edx], eax
    
    mov     ax, [.esVal]
    mov     [edx+4], ax
    
    mov     ax, [.dsVal]
    mov     [edx+6], ax
    
    mov     ax, [.eflagsVal]
    mov     [edx+8], ax
    
    ;restore saved registers
    popa
    
    mov     esp, ebp
    pop     ebp
    ret

; Variables
[BITS 32]
[SECTION .bss]
protectedStackPtr:      resd  1
