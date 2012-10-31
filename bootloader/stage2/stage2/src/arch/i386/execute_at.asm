[BITS 32]
[GLOBAL _arch_execute_at_address_with_stack]
[SECTION .text]

_arch_execute_at_address_with_stack:
	add esp, 4		;get rid of return address
	pop eax			;get entrypoint
	pop esi			;get stackpointer
	pop ebx			;get context

	mov esp, esi 	;set new stack
	push ebx		;push context
	jmp eax			;jump to entrypoint
