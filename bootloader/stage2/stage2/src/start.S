[BITS 32]
global start
extern _kmain

extern switchToReal
extern switchToProt

STACKSIZE         equ         0x4000                ; 16k Stack

section .text

start:
  cli
  mov esp, bootStack + STACKSIZE    ; Setup stack
  mov ebp, esp

  ;TODO ctors and dtors

  call _kmain                        ; Jump to C-Code

;  call switchToReal
;[BITS 16]
;  mov ah, 0x0E
;  mov al, 'A'
;  mov bh, 0
;  mov bl, 0x14
;  int 0x10
;
;  call DWORD switchToProt

[BITS 32]
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
