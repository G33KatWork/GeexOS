[BITS 32]
global start
extern _kmain

STACKSIZE         equ         0x4000                ; 16k Stack

section .text

start:
  cli
  mov esp, bootStack + STACKSIZE    ; Setup stack
  mov ebp, esp

  ;TODO ctors and dtors

  call _kmain                        ; Jump to C-Code

.cpuhalt:
  cli
  hlt
  jmp .cpuhalt

;---------------------------------------------------------------------------------------------
section .bss
align 32

global bootStack
bootStack:
  resb  STACKSIZE
