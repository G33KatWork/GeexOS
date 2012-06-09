[BITS 32]
global _start
extern _kmain

section .text

_start:
  cli
  call _kmain

.cpuhalt:
  cli
  hlt
  jmp .cpuhalt
