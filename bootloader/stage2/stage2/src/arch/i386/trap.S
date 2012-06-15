[BITS 32]
[SECTION .text]

%macro TRAP_NOERRCODE 1
	[GLOBAL _ex%1]
	_ex%1:
		push byte 0
		push byte %1
		jmp trap_handler
%endmacro

%macro TRAP_ERRCODE 1
	[GLOBAL _ex%1]
	_ex%1:
		push byte %1
		jmp trap_handler
%endmacro

;Define standard ISRs (Exceptions, Errors etc.)
TRAP_NOERRCODE 0
TRAP_NOERRCODE 1
TRAP_NOERRCODE 2
TRAP_NOERRCODE 3
TRAP_NOERRCODE 4
TRAP_NOERRCODE 5
TRAP_NOERRCODE 6
TRAP_NOERRCODE 7
TRAP_ERRCODE   8
TRAP_NOERRCODE 9
TRAP_ERRCODE   10
TRAP_ERRCODE   11
TRAP_ERRCODE   12
TRAP_ERRCODE   13
TRAP_ERRCODE   14
;no exception 15
TRAP_NOERRCODE 16
TRAP_NOERRCODE 17
TRAP_NOERRCODE 18
TRAP_NOERRCODE 19

;Our C-Handler for fault interrupts
[EXTERN _printException]
trap_handler:
	cli
	pusha           ; push all registers (see regs.h for corresponding struct)

	mov ax, ds      ; save old data segment decriptor
	push eax
	mov ax, es
	push eax
	mov ax, fs
	push eax
	mov ax, gs
	push eax

	mov ax, 0x10    ; set kernel data segment descriptor
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	call _printException

	pop eax
	mov gs, ax
	pop eax
	mov fs, ax
	pop eax
	mov es, ax
	pop eax
	mov ds, ax

	popa

	add esp, 8		;remove interrupt number and errorcode from stack
	sti
	iret
