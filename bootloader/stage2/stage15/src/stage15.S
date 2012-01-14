;--------- PE Header ---------
%include "src/pe.inc"

[BITS 16]
[ORG 0x0]
[MAP ALL stage15.map]

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
    db 'GeexLDR Stage 1.5', 13, 10, 0

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
[SEGMENT start=0x40000+512 vstart=0x40000+512]

TEMPORARY_PE_LOCATION   EQU     0x10000

pmodeEntry:
    mov     ax, GDT_DATA_DESC
    mov     ds, ax
    mov     ss, ax
    mov     es, ax
    mov     esp, 0x30000
    
    call    ClearScreen32
    
    mov     ebx, pmodeMsg
    call    Puts32
    
    ;move the pe file to 1MB temporarily
    mov     edi, TEMPORARY_PE_LOCATION
    mov     esi, stage2PEFile
    mov     ecx, stage2PEEnd - stage2PEFile
    rep     movsb
    
    ;parse the pe file
.parse:
    mov     eax, TEMPORARY_PE_LOCATION
    add     eax, DWORD [eax+0x3C]                       ;get pe header start
    cmp     DWORD [eax], 0x00004550
    jne     .peMagicFail
    
    add     eax, 4                                      ;skip signature
    
    movzx   ecx, WORD [eax+peHeader.numSections]        ;get number of sections
    
    movzx   edx, WORD [eax+peHeader.optHeaderSize]      ;get optional header size
    add     eax, peHeader_size                          ;add pe header size, eax contains pointer to peOptHeader
    push    eax                                         ;save pointer to optional header, we need it later to find the entrypoint
    
    mov     ebp, DWORD [eax+peOptHeader.imageBase]      ;get image base from Opt Header

    add     eax, edx
    
.nextSection:
    ;check for relocations, they are not supported
    cmp     DWORD [eax + peSectionHeader.numRelocs], 0
    jg      .noRelocSupportFail
    
    ;get section characteristics
    mov     ebx, DWORD [eax + peSectionHeader.characteristics]
    
    ;is section discardable?
    bt      ebx, SEC_DISCARDABLE
    jc      .gotonext
    
    ;has section initialized data?
    bt      ebx, SEC_DATA_INITIALIZED
    jc     .initializedData
    
    ;has section uninitialized data?
    bt      ebx, SEC_DATA_UNINITIALIZED
    jc      .uninitializedData
    
    ;has section code?
    bt      ebx, SEC_CODE
    jc      .codeSection

    ;no data type for sections set - WTF?
    jmp     .peNoDataTypeFlagsFail
    
.codeSection:
.initializedData:
    ;copy the section to destination
    mov     esi, TEMPORARY_PE_LOCATION                  ;get pe location
    add     esi, DWORD [eax + peSectionHeader.pointerToRawData] ;get pointer to section data
    mov     edi, ebp
    add     edi, DWORD [eax + peSectionHeader.virtualAddress]   ;get destination pointer
    push    ecx
    mov     ecx, DWORD [eax + peSectionHeader.sizeOfRawData]    ;get length for copy operation
    rep     movsb                                       ;copy data
    pop     ecx
    jmp     .gotonext

.uninitializedData:
    mov     edi, ebp
    add     edi, DWORD [eax + peSectionHeader.virtualAddress]   ;get destination pointer
    push    ecx
    mov     ecx, DWORD [eax + peSectionHeader.physAddrVirtSize]
    push    eax
    mov     eax, 0
    rep     stosb
    pop     eax
    pop     ecx
    
.gotonext:
    ;go to next section
    add     eax, peSectionHeader_size
    loop    .nextSection
    
    ;jump to entrypoint
    pop     eax                                         ;restore pointer to pe optional header
    mov     ebx, DWORD [eax + peOptHeader.imageBase]
    add     ebx, DWORD [eax + peOptHeader.entrypoint]
    jmp     ebx
    
    ;never reached
    jmp     .hang

.noRelocSupportFail:
    mov     ebx, peRelocSupportFailMsg
    call    Puts32
    jmp     .hang

.peMagicFail:
    mov     ebx, peSignatureFailMsg
    call    Puts32
    jmp     .hang

.peNoDataTypeFlagsFail:
    mov     ebx, peNoDataTypeFlagsFailMsg
    call    Puts32
    mov     ebx, eax + peSectionHeader.name
    call    Puts32

.hang:
    jmp     .hang

pmodeMsg:
    db  'GeexLDR Stage 1.5', 10, 0
peSignatureFailMsg:
    db  'Magic value for Stage2 is not PE', 10, 0
peRelocSupportFailMsg:
    db  'Relocations are not supported', 10, 0
peNoDataTypeFlagsFailMsg:
    db  'No proper flags for data type set for section ', 0

;------ 32 Bit Printing ------
%include "src/pmodePrint.inc"

;PE file for stage2 comes here through incbin
align 512
stage2PEFile:
    incbin "../stage2/stage2.exe"
stage2PEEnd: