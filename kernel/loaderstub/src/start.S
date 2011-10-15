[BITS 32]
global start
global cpuhalt
extern kmain

STACKSIZE         equ         0x4000                ; 16k if you're wondering

MODULEALIGN       equ         1<<0                  ; GRUB multiboot header defines
MEMINFO           equ         1<<1
FLAGS             equ         MODULEALIGN | MEMINFO
MAGIC             equ         0x1BADB002
CHECKSUM          equ         -(MAGIC + FLAGS)

section .text

align 4
MultiBootHeader:                    ; Next is the Grub Multiboot Header
  dd MAGIC
  dd FLAGS
  dd CHECKSUM

start:
  cli
  mov esp, bootStack + STACKSIZE    ; Setup stack
  mov ebp, esp
  push eax                          ; Push magic number from GRUB
  push ebx                          ; Push pointer to GRUB multiboot-information
	
  call kmain                        ; Jump to C-Code

cpuhalt:
  cli
  hlt
  jmp cpuhalt

;---------------------------------------------------------------------------------------------
section .bss
align 32

global bootStack
bootStack:
  resb  STACKSIZE