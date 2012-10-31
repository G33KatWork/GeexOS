[BITS 32]
global _start
extern _kmain

section .text

_start:
  ;loader already pushed pointer to loader block onto stack
  cli
  call _kmain

.cpuhalt:
  cli
  hlt
  jmp .cpuhalt
