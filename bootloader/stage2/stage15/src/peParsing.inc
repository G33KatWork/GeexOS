[BITS 32]

;parse the pe file
doPEParsing:
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
    mov     esi, TEMPORARY_FILE_LOCATION                  ;get pe location
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

.jumpEntrypoint:
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

.peNoDataTypeFlagsFail:
    mov     ebx, peNoDataTypeFlagsFailMsg
    call    Puts32
    mov     ebx, eax + peSectionHeader.name
    call    Puts32

.hang:
    jmp     .hang

peRelocSupportFailMsg:
    db  'Relocations are not supported', 10, 0
peNoDataTypeFlagsFailMsg:
    db  'No proper flags for data type set for section ', 0
