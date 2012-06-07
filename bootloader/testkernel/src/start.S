[BITS 32]
global start
extern _kmain

section .text

start:
  cli
  call _kmain

.cpuhalt:
  cli
  hlt
  jmp .cpuhalt
