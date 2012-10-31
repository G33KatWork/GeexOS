;--------- PE File format ---------
%include "src/pe.inc"

;--------- ELF File format ---------
%include "src/elf.inc"

LOADADDRESS     EQU     0x8000

[BITS 16]
[ORG 0x0]
;[MAP ALL stage15.map]

start:
    ;we are loaded at 0x800:0 and upwards by first stage
    
    ;set up segment registers
    push    cs
    pop     ax
    mov     ds, ax
    mov     es, ax
    mov     fs, ax
    mov     gs, ax

    ;set up a stack at 0:8000 (=0x8000) directly under our code
    push    0
    pop     ss
    mov     sp, LOADADDRESS

    call    bios_print
    db 'GeexLDR Stage 1.5', 13, 10, 0

    call    enableA20
    cmp     ax, 0
    je      .a20error

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
[SEGMENT start=LOADADDRESS+512 vstart=LOADADDRESS+512]

TEMPORARY_FILE_LOCATION   EQU     0x200000

pmodeEntry:
    mov     ax, GDT_DATA_DESC
    mov     ds, ax
    mov     ss, ax
    mov     es, ax
    mov     esp, LOADADDRESS
    
    call    ClearScreen32
    
    mov     ebx, pmodeMsg
    call    Puts32
    
    ;move the pe file to 1MB temporarily
    mov     edi, TEMPORARY_FILE_LOCATION
    mov     esi, stage2PEFile
    mov     ecx, stage2PEEnd - stage2PEFile
    rep     movsb
    
    ;check for PE header magic value
.checkPEHeader:
    mov     eax, TEMPORARY_FILE_LOCATION
    add     eax, DWORD [eax+0x3C]                       ;get pe header start
    cmp     DWORD [eax], 0x00004550
    jne     .checkELFHeader                             ;no PE header? try elf
    jmp     doPEParsing                                 ;otherwise parse PE

.checkELFHeader:
    mov     eax, TEMPORARY_FILE_LOCATION
    cmp     DWORD [eax], 0x464C457F
    jne     .signatureFail                              ;no ELF magic value, bail out
    jmp     doELFParsing

.signatureFail:
    mov     ebx, signatureFailMsg
    call    Puts32
    jmp     .hang

.hang:
    jmp     .hang

pmodeMsg:
    db  'GeexLDR Stage 1.5 PMODE', 10, 0
signatureFailMsg:
    db  'Magic value for Stage2 is neither PE nor ELF', 10, 0

%include "src/peParsing.inc"
%include "src/elfParsing.inc"

;------ 32 Bit Printing ------
%include "src/pmodePrint.inc"

;PE file for stage2 comes here through incbin
align 512
stage2PEFile:
    incbin "../stage2/obj/stage2.elf"
stage2PEEnd: