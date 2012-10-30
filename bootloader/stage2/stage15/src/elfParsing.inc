[BITS 32]

;parse the ELF file
doELFParsing:
	cmp BYTE [eax + Elf32_Ehdr.e_ident + EI_CLASS], ELFCLASS32
	jne		.wrongClassFail

	cmp	WORD [eax + Elf32_Ehdr.e_machine], EM_386
	jne		.wrongMachineFail

	cmp	WORD [eax + Elf32_Ehdr.e_type], ET_EXEC
	jne		.wrongTypeFail

	;get number of program headers in ecx
	movzx 	ecx, WORD [eax + Elf32_Ehdr.e_phnum]
	cmp		ecx, 0
	jle 	.noProgramHeadersFail

	;get address of first PHDR
	mov 	edx, eax
	add 	edx, [eax + Elf32_Ehdr.e_phoff]

.nextHeader:
	;is this a LOAD program header?
	cmp	DWORD [edx + Elf32_Phdr.p_type], PT_LOAD
	jne		.outNext

	push 	ecx
	mov		esi, DWORD [edx + Elf32_Phdr.p_offset]
	add     esi, eax
	mov		edi, DWORD [edx + Elf32_Phdr.p_vaddr]
	mov 	ecx, DWORD [edx + Elf32_Phdr.p_filesz]
	rep	movsb

	;zero the rest
	mov     esi, DWORD [edx + Elf32_Phdr.p_memsz]
	mov     edi, DWORD [edx + Elf32_Phdr.p_filesz]

	;get a zero in eax
	push    eax
	xor     eax, eax

	;calculate length of remaining memory size
	mov 	ecx, esi
	sub     ecx, edi
	
	;calculate start address
	mov     edi, DWORD [edx + Elf32_Phdr.p_vaddr]
	add     edi, DWORD [edx + Elf32_Phdr.p_filesz]
	
	;zero
	rep     stosb
	pop     eax

	pop		ecx

.outNext:
	;add size of PHDR to get next entry
	movzx   esi, WORD [eax + Elf32_Ehdr.e_phentsize]
	add     edx, esi
	loop	.nextHeader

	jmp		[eax + Elf32_Ehdr.e_entry]

    jmp     .hang

.wrongClassFail:
	mov		ebx, wrongClassFailMsg
	call	Puts32
	jmp		.hang

.wrongMachineFail:
    mov     ebx, wrongMachineFailMsg
    call    Puts32
    jmp     .hang

.wrongTypeFail:
    mov     ebx, wrongTypeFailMsg
    call    Puts32
    jmp     .hang

.noProgramHeadersFail:
    mov     ebx, noProgramHeadersFailMsg
    call    Puts32
    jmp     .hang

.hang:
    jmp     .hang

wrongClassFailMsg:
	db	'Class in ELF File is not ELF32', 10, 0

wrongMachineFailMsg:
	db	'Machine in ELF File is not i386', 10, 0

wrongTypeFailMsg:
	db	'ELF File is not an executable', 10, 0

noProgramHeadersFailMsg:
	db	'ELF File doesnt have any program headers', 10, 0

foo:
	db 'foo', 10, 0
