[BITS 32]
global start
extern main

extern start_ctors
extern start_dtors
extern end_ctors
extern end_dtors

MODULEALIGN        equ        1<<0
MEMINFO            equ        1<<1
FLAGS              equ        MODULEALIGN | MEMINFO
MAGIC              equ        0x1BADB002
CHECKSUM           equ        -(MAGIC + FLAGS)

section .text      ; Next is the Grub Multiboot Header

align 4
MultiBootHeader:
	dd MAGIC
    dd FLAGS
    dd CHECKSUM

STACKSIZE equ 0x4000  ; 16k if you're wondering

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
	
higherhalf:						; Here the CPU adds 0x40000000 to all addresses due to the trck gdt
	mov esp, stack				; Setup stack
	mov ebp, esp
	push ebp
	push ebx					; Push pointer to GRUB multiboot-information

	
static_ctors_loop:				; Call constructors
	mov ebx, start_ctors
    jmp .test
.body:
    call [ebx]
    add ebx,4
.test:
    cmp ebx, end_ctors
    jb .body

	
	call main					; Jump to C++-Code
	cli
	

static_dtors_loop:				; Call destructors
    mov ebx, start_dtors
    jmp .test
.body:
    call [ebx]
    add ebx,4
.test:
;    cmp ebx, end_dtors
;    jb .body

cpuhalt:
    hlt
    jmp cpuhalt

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

section .bss
align 32

stack:
	resb      STACKSIZE