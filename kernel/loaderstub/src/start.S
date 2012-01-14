[BITS 32]
global start
global _cpuhalt
extern _kmain

extern text
extern __bss_start__
extern __bss_end__

STACKSIZE         equ         0x4000                ; 16k if you're wondering

MODULEALIGN       equ         1<<0                  ; GRUB multiboot header defines
MEMINFO           equ         1<<1
EXTENDEDINFO      equ         1<<16
FLAGS             equ         MODULEALIGN | MEMINFO | EXTENDEDINFO
MAGIC             equ         0x1BADB002
CHECKSUM          equ         -(MAGIC + FLAGS)

section .text

align 4
MultiBootHeader:                    ; Next is the Grub Multiboot Header
  dd MAGIC
  dd FLAGS
  dd CHECKSUM
  dd MultiBootHeader                ;header_addr
  dd MultiBootHeader                          ;load_addr
  dd __bss_start__                            ;load_end_addr
  dd __bss_end__                            ;bss_end_addr
  dd start                          ;entry_addr
  dd 0              ; mode_type
  dd 0               ; width
  dd 0               ; height
  dd 0               ; depth

start:
  cli
  mov esp, bootStack + STACKSIZE    ; Setup stack
  mov ebp, esp
  push eax                          ; Push magic number from GRUB
  push ebx                          ; Push pointer to GRUB multiboot-information
	
  call _kmain                        ; Jump to C-Code

_cpuhalt:
  cli
  hlt
  jmp _cpuhalt

;---------------------------------------------------------------------------------------------
section .bss
align 32

global bootStack
bootStack:
  resb  STACKSIZE