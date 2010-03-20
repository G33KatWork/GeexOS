[BITS 32]
global start
extern main

extern start_ctors
extern end_ctors
extern __cxa_finalize

MODULEALIGN        equ        1<<0
MEMINFO            equ        1<<1
FLAGS              equ        MODULEALIGN | MEMINFO
MAGIC              equ        0x1BADB002
CHECKSUM           equ        -(MAGIC + FLAGS)

STACKSIZE          equ        0x400 ; 1k initial stack - will be moved later

section .multibootHeader       ; Next is the Grub Multiboot Header
align 4
MultiBootHeader:
	dd MAGIC
    dd FLAGS
    dd CHECKSUM


section .text
align 4
start:
    cli
    
	mov esp, bootStack			; Setup stack
	mov ebp, esp
	push ebp

    call static_ctors_loop
    
    push ebx					; Push pointer to GRUB multiboot-information
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

global gdt_flush
extern gp

; load the real gdt
gdt_flush:
	lgdt [gp]
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	jmp 0x08:flush2

flush2:
	ret

;---------------------------------------------------------------------------------------------

section .bss
align 0x1000                                            ; align to page boundary
global bootStack
bootStackEnd:
	resb      STACKSIZE
bootStack: