[SECTION .text]
[BITS 32]
global _start
extern _kmain
extern __CTOR_LIST__

STACKSIZE          equ        0x10000 ; 64k stack for the main kernel thread

section .text
align 4
_start:
    pop ebx								; Get pointer to kernel information (pushed by loader) into ebx
    mov [_kernelInformation], ebx       ; Store pointer to boot environment information for the HAL

    call callCtors                      ; Call ctors
	call _kmain							; Jump to C++-Code
	cli
	
	;TODO: call dtors! they are registered with atexit

.cpuhalt:
    hlt
    jmp .cpuhalt

callCtors:
    push ebx
    push eax

    mov ebx, __CTOR_LIST__
    add ebx, 4              ;skip first 0xFFFFFFFF

    mov eax, [ebx]          ;get first entry

.body:
    call eax                ;call ctor

    add ebx, 4              ;advance pointer to next entry
    mov eax, [ebx]          ;get next entry

.test:
    test eax, eax           ;end of list?
    jnz .body

    pop eax
    pop ebx
    ret


;---------------------------------------------------------------------------------------------

section .data
;a comfy place for the multiboot struct location for the x86 HAL
global _kernelInformation
_kernelInformation:	dd 0

;---------------------------------------------------------------------------------------------

section .bss
;define the bootstack which is used by the main kernel thread for all initialization tasks
align 4096                                            ; align to page boundary
global bootStack
bootStackEnd:
	resb      STACKSIZE
bootStack: