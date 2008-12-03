MBOOT_PAGE_ALIGN    equ 1<<0    ; Load kernel and modules on a page boundary
MBOOT_MEM_INFO      equ 1<<1    ; Provide your kernel with memory info
MBOOT_AOUT_KLUDGE   equ 1<<16

MBOOT_HEADER_MAGIC  equ 0x1BADB002 ; Multiboot Magic value
MBOOT_HEADER_FLAGS  equ MBOOT_PAGE_ALIGN | MBOOT_MEM_INFO | MBOOT_AOUT_KLUDGE
MBOOT_CHECKSUM      equ -(MBOOT_HEADER_MAGIC + MBOOT_HEADER_FLAGS)


[BITS 32]

[GLOBAL mboot]
[EXTERN code]                   ; Start of the '.text' section.
[EXTERN bss]                    ; Start of the .bss section.
[EXTERN end]                    ; End of the last loadable section.

mboot:
  dd  MBOOT_HEADER_MAGIC
  dd  MBOOT_HEADER_FLAGS
  dd  MBOOT_CHECKSUM
   
  dd  mboot
  dd  code
  dd  bss
  dd  end
  dd  start

[GLOBAL start]
[EXTERN main]

start:
  push    ebx                   ; Load multiboot header location

  cli
  call main
  jmp $