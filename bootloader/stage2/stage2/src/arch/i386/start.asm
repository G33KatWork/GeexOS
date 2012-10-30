[BITS 32]
global start
extern kmain

extern switchToReal
extern switchToProt

section .text

start:
  cli
  mov esp, 0x8000                   ; Setup stack
  mov ebp, esp

  ;TODO ctors and dtors

  call kmain                        ; Jump to C-Code

.cpuhalt:
  cli
  hlt
  jmp .cpuhalt
