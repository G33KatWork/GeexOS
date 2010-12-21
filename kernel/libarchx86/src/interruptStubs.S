[BITS 32]

%macro ISR_NOERRCODE 1
	[GLOBAL exception%1]
	exception%1:
		cli
		push byte 0
		push byte %1
		jmp exception_common_stub
%endmacro

%macro ISR_ERRCODE 1
	[GLOBAL exception%1]
	exception%1:
		cli
		push byte %1
		jmp exception_common_stub
%endmacro

%macro IRQ 1
	[GLOBAL irq%1]
	irq%1:
		cli
		push byte 0
		push byte %1
		jmp irq_common_stub
%endmacro

;Define standard ISRs (Exceptions, Errors etc.)
ISR_NOERRCODE 0
ISR_NOERRCODE 1
ISR_NOERRCODE 2
ISR_NOERRCODE 3
ISR_NOERRCODE 4
ISR_NOERRCODE 5
ISR_NOERRCODE 6
ISR_NOERRCODE 7
ISR_ERRCODE   8
ISR_NOERRCODE 9
ISR_ERRCODE   10
ISR_ERRCODE   11
ISR_ERRCODE   12
ISR_ERRCODE   13
ISR_ERRCODE   14
ISR_NOERRCODE 15
ISR_NOERRCODE 16
ISR_NOERRCODE 17
ISR_NOERRCODE 18
ISR_NOERRCODE 19
ISR_NOERRCODE 20
ISR_NOERRCODE 21
ISR_NOERRCODE 22
ISR_NOERRCODE 23
ISR_NOERRCODE 24
ISR_NOERRCODE 25
ISR_NOERRCODE 26
ISR_NOERRCODE 27
ISR_NOERRCODE 28
ISR_NOERRCODE 29
ISR_NOERRCODE 30
ISR_NOERRCODE 31

IRQ 32
IRQ 33
IRQ 34
IRQ 35
IRQ 36
IRQ 37
IRQ 38
IRQ 39
IRQ 40
IRQ 41
IRQ 42
IRQ 43
IRQ 44
IRQ 45
IRQ 46
IRQ 47
IRQ 48
IRQ 49
IRQ 50
IRQ 51
IRQ 52
IRQ 53
IRQ 54
IRQ 55
IRQ 56
IRQ 57
IRQ 58
IRQ 59
IRQ 60

;Our C-Handler for fault interrupts
[EXTERN fault_handler]
exception_common_stub:
	pusha					; push all registers (see regs.h for corresponding struct)

	mov ax, ds		; save old data segment decriptor
	push eax
	
	push es
	push fs
	push gs

	mov ax, 0x10	; set kernel data segment descriptor
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	call fault_handler

    pop gs
    pop fs
    pop es

	pop eax			; reload original data segment descriptor
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	popa
	add esp, 8     ; clean up the pushed error code and pushed ISR number
	sti
	iret

;Our C-Handler for common interrupts
[EXTERN interrupt_handler]
irq_common_stub:
	pusha					; push all registers (see regs.h for corresponding struct)

	mov ax, ds		; save old data segment decriptor
	push eax
	
	push es
	push fs
	push gs

	mov ax, 0x10	; set kernel data segment descriptor
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	call interrupt_handler
	
	pop gs
    pop fs
    pop es

	pop eax			; reload original data segment descriptor
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	popa
	add esp, 8     ; clean up the pushed error code and pushed ISR number
	sti
	iret
