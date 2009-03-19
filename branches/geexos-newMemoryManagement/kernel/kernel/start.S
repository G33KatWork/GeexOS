[BITS 32]
[GLOBAL start]
[EXTERN entry]                  ; C entry point to kernel


MBOOT_PAGE_ALIGN    equ 1<<0    ; Load kernel and modules on a page boundary
MBOOT_MEM_INFO      equ 1<<1    ; Provide your kernel with memory info

MBOOT_HEADER_MAGIC  equ 0x1BADB002 ; Multiboot Magic value
MBOOT_HEADER_FLAGS  equ MBOOT_PAGE_ALIGN | MBOOT_MEM_INFO
MBOOT_CHECKSUM      equ -(MBOOT_HEADER_MAGIC + MBOOT_HEADER_FLAGS)

ALIGN 4
multiboot_header:
  dd  MBOOT_HEADER_MAGIC
  dd  MBOOT_HEADER_FLAGS
  dd  MBOOT_CHECKSUM

;A self-help group:
;Leader: "Here we are coming in from our nifty bootloader. Say thanks to our bootloader"
;All:	 "Thanks bootloader!"
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
	mov esp, sys_stack			; Setup stack
	mov ebp, esp
	push ebp
	push ebx					; Push pointer to GRUB multiboot-information
	
	call entry					; Jump to C-Code
	jmp $

;---------------------------------------------------------------------------------------------

[global gdt_flush]
[extern gp]

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

[section .setup]
trickgdt:
	dw gdt_end - gdt - 1 ; size of the GDT
	dd gdt				 ; linear address of GDT

gdt:
	dd 0, 0												; null gate
	db 0xFF, 0xFF, 0, 0, 0, 10011010b, 11001111b, 0x40	; code selector 0x08: base 0x40000000, limit 0xFFFFFFFF, type 0x9A, granularity 0xCF
	db 0xFF, 0xFF, 0, 0, 0, 10010010b, 11001111b, 0x40	; data selector 0x10: base 0x40000000, limit 0xFFFFFFFF, type 0x92, granularity 0xCF

gdt_end:

;---------------------------------------------------------------------------------------------

[section .bss]
resb 0x1000			;reserve 0x1000 Bytes for stack
sys_stack:
