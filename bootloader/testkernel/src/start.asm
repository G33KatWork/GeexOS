[BITS 32]
global start
extern kmain

section .text

start:
  ;loader already pushed pointer to loader block onto stack
  cli
  call kmain

.cpuhalt:
  cli
  hlt
  jmp .cpuhalt
