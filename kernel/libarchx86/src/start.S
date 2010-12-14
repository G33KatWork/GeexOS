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

STACKSIZE          equ        0x1000 ; 4k initial stack - will be moved later

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
	lgdt [trickgdt]				; load a temp gdt which adds 0x40000000 to all addresses
    
	mov ax, 0x10                ; Segment setup - 0x10 = Kernel data GDT descriptor
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	
	jmp 0x08:higherhalf			; Far jump to higher half kernel
	
higherhalf:						; Here the CPU adds 0x40000000 to all addresses due to the trick gdt
	mov esp, bootStack			; Setup stack
	mov ebp, esp
	push ebp

    call static_ctors_loop
    
    mov [multibootLocation], ebx; Store pointer to GRUB multiboot-information
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

section .setup
trickgdt:
	dw gdt_end - gdt - 1 ; size of the GDT
	dd gdt				 ; linear address of GDT

gdt:
	dd 0, 0												; null gate
	db 0xFF, 0xFF, 0, 0, 0, 10011010b, 11001111b, 0x40	; code selector 0x08: base 0x40000000, limit 0xFFFFFFFF, type 0x9A, granularity 0xCF
	db 0xFF, 0xFF, 0, 0, 0, 10010010b, 11001111b, 0x40	; data selector 0x10: base 0x40000000, limit 0xFFFFFFFF, type 0x92, granularity 0xCF

gdt_end:

;---------------------------------------------------------------------------------------------

section .data
;a comfy place for the multiboot struct location for the x86 HAL
global multibootLocation
multibootLocation:	dw 0

;---------------------------------------------------------------------------------------------

section .bss
;define the initial bootstack which is used until we set up our own
align 0x1000                                            ; align to page boundary
global bootStack
bootStackEnd:
	resb      STACKSIZE
bootStack: