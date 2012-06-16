[BITS 32]
[GLOBAL _arch_execute_at_address_with_stack]
[SECTION .text]

_arch_execute_at_address_with_stack:
	add esp, 4
	pop eax			;entrypoint
	pop esp			;stackpointer
	mov ebp, esp
	jmp eax
