[BITS 32]
global start
extern main

extern start_ctors
extern end_ctors
extern __cxa_finalize

STACKSIZE          equ        0x1000 ; 4k initial stack - will be moved later

section .text
align 4
start:
    mov ebx, [esp+4]            ; Get pointer to kernel information (pushed by loader stub before call) into ebx
    
	mov esp, bootStack			; Setup stack
	mov ebp, esp
	push ebp

    call static_ctors_loop
    
    mov [kernelInformation], ebx                    ; Push pointer to multiboot information as first argument
	call main					; Jump to C++-Code
	cli
	
	push 0
	call __cxa_finalize			; Call destructors with arg 0 (destroy all)

cpuhalt:
    hlt
    jmp cpuhalt
	
	
static_ctors_loop:				; Call constructors
    push ebx
    
	mov ebx, start_ctors
    jmp .test
.body:
    call [ebx]
    add ebx,4
.test:
    cmp ebx, end_ctors
    jb .body
    
    pop ebx
    ret

;---------------------------------------------------------------------------------------------

section .data
;a comfy place for the multiboot struct location for the x86 HAL
global kernelInformation
kernelInformation:	dw 0

;---------------------------------------------------------------------------------------------

section .bss
;define the initial bootstack which is used until we set up our own
align 4096                                            ; align to page boundary
global bootStack
bootStackEnd:
	resb      STACKSIZE
bootStack: