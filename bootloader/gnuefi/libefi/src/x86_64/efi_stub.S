/*
 * Function calling ABI conversion from Linux to EFI for x86_64
 *
 * Copyright (C) 2007 Intel Corp
 *	Bibo Mao <bibo.mao@intel.com>
 *	Huang Ying <ying.huang@intel.com>
 */

/*
 * EFI calling conventions are documented at:
 *   http://msdn.microsoft.com/en-us/library/ms235286%28v=vs.80%29.aspx
 * ELF calling conventions are documented at:
 *   http://www.x86-64.org/documentation/abi.pdf
 *
 * Basically here are the conversion rules:
 * a) our function pointer is in %rdi
 * b) ELF gives us 8-byte aligned %rsp, so we need to pad out to 16-byte
 *    alignment.
 * c) inside each call thunker, we can only adjust the stack by
 *    multiples of 16 bytes. "offset" below refers to however much
 *    we allocate inside a thunker.
 * d) rsi through r8 (elf) aka rcx through r9 (ms) require stack space
 *    on the MS side even though it's not getting used at all.
 * e) arguments are as follows: (elf -> ms)
 *   1) rdi -> rcx (32 saved)
 *   2) rsi -> rdx (32 saved)
 *   3) rdx -> r8 ( 32 saved)
 *   4) rcx -> r9 (32 saved)
 *   5) r8 -> 32(%rsp) (48 saved)
 *   6) r9 -> 40(%rsp) (48 saved)
 *   7) pad+offset+0(%rsp) -> 48(%rsp) (64 saved)
 *   8) pad+offset+8(%rsp) -> 56(%rsp) (64 saved)
 *   9) pad+offset+16(%rsp) -> 64(%rsp) (80 saved)
 *  10) pad+offset+24(%rsp) -> 72(%rsp) (80 saved)
 *  11) pad+offset+32(%rsp) -> 80(%rsp) (96 saved)
 *  12) pad+offset+40(%rsp) -> 88(%rsp) (96 saved)
 * f) because the first argument we recieve in a thunker is actually the
 *    function to be called, arguments are offset as such:
 *   0) rdi -> caller
 *   1) rsi -> rcx (32 saved)
 *   2) rdx -> rdx (32 saved)
 *   3) rcx -> r8 (32 saved)
 *   4) r8 -> r9 (32 saved)
 *   5) r9 -> 32(%rsp) (48 saved)
 *   6) pad+offset+0(%rsp) -> 40(%rsp) (48 saved)
 *   7) pad+offset+8(%rsp) -> 48(%rsp) (64 saved)
 *   8) pad+offset+16(%rsp) -> 56(%rsp) (64 saved)
 *   9) pad+offset+24(%rsp) -> 64(%rsp) (80 saved)
 *  10) pad+offset+32(%rsp) -> 72(%rsp) (80 saved)
 *  11) pad+offset+40(%rsp) -> 80(%rsp) (96 saved)
 *  12) pad+offset+48(%rsp) -> 88(%rsp) (96 saved)
 * e) arguments need to be moved in opposite order to avoid clobbering
 * f) pad_stack leaves the amount of padding it added in %r11 for functions
 *    to use
 * g) efi -> elf calls don't need to pad the stack, because the 16-byte
 *    alignment is also always 8-byte aligned.
 */

#define ENTRY(name)	\
	.globl name;	\
	name:

#define out(val)		\
	push %rax ;		\
	mov val, %rax ;		\
	out %al, $128 ;		\
	pop %rax

#define pad_stack							\
	subq $8, %rsp ; /* must be a multiple of 16 - sizeof(%rip) */	\
	/* stash some handy integers */					\
	mov $0x8, %rax ;						\
	mov $0x10, %r10 ;						\
	/* see if we need padding */					\
	and %rsp, %rax ;						\
	/* store the pad amount in %r11 */				\
	cmovnz %rax, %r11 ;						\
	cmovz %r10, %r11 ;						\
	/* insert the padding */					\
	subq %r11, %rsp ;						\
	/* add the $8 we saved above in %r11 */				\
	addq $8, %r11 ;							\
	/* store the pad amount */					\
	mov %r11, (%rsp) ;						\
	/* compensate for %rip being stored on the stack by call */	\
	addq $8, %r11

#define unpad_stack							\
	/* fetch the pad amount	we saved (%r11 has been clobbered) */	\
	mov (%rsp), %r11 ;						\
	/* remove the padding */					\
	addq %r11, %rsp

ENTRY(efi_call0)
	pad_stack
	subq $32, %rsp
	call *%rdi
	addq $32, %rsp
	unpad_stack
	ret

ENTRY(efi_call1)
	pad_stack
	subq $32, %rsp
	mov  %rsi, %rcx
	call *%rdi
	addq $32, %rsp
	unpad_stack
	ret

ENTRY(efi_call2)
	pad_stack
	subq $32, %rsp
	/* mov %rdx, %rdx */
	mov  %rsi, %rcx
	call *%rdi
	addq $32, %rsp
	unpad_stack
	ret

ENTRY(efi_call3)
	pad_stack
	subq $32, %rsp
	mov  %rcx, %r8
	/* mov %rdx, %rdx */
	mov  %rsi, %rcx
	call *%rdi
	addq $32, %rsp
	unpad_stack
	ret

ENTRY(efi_call4)
	pad_stack
	subq $32, %rsp
	mov %r8, %r9
	mov %rcx, %r8
	/* mov %rdx, %rdx */
	mov %rsi, %rcx
	call *%rdi
	addq $32, %rsp
	unpad_stack
	ret

ENTRY(efi_call5)
	pad_stack
	subq $48, %rsp
	mov %r9, 32(%rsp)
	mov %r8, %r9
	mov %rcx, %r8
	/* mov %rdx, %rdx */
	mov %rsi, %rcx
	call *%rdi
	addq $48, %rsp
	unpad_stack
	ret

ENTRY(efi_call6)
	pad_stack
	subq $48, %rsp
	addq $48, %r11
	addq %rsp, %r11
	mov (%r11), %rax
	mov %rax, 40(%rsp)
	mov %r9, 32(%rsp)
	mov %r8, %r9
	mov %rcx, %r8
	/* mov %rdx, %rdx */
	mov %rsi, %rcx
	call *%rdi
	addq $48, %rsp
	unpad_stack
	ret

ENTRY(efi_call7)
	pad_stack
	subq $64, %rsp
	addq $64, %r11
	addq $8, %r11
	addq %rsp, %r11
	mov (%r11), %rax
	mov %rax, 48(%rsp)
	subq $8, %r11
	mov (%r11), %rax
	mov %rax, 40(%rsp)
	mov %r9, 32(%rsp)
	mov %r8, %r9
	mov %rcx, %r8
	/* mov %rdx, %rdx */
	mov %rsi, %rcx
	call *%rdi
	addq $64, %rsp
	unpad_stack
	ret

ENTRY(efi_call8)
	pad_stack
	subq $64, %rsp
	addq $64, %r11
	addq $16, %r11
	addq %rsp, %r11
	mov (%r11), %rax
	mov %rax, 56(%rsp)
	subq $8, %r11
	mov (%r11), %rax
	mov %rax, 48(%rsp)
	subq $8, %r11
	mov (%r11), %rax
	mov %rax, 40(%rsp)
	mov %r9, 32(%rsp)
	mov %r8, %r9
	mov %rcx, %r8
	/* mov %rdx, %rdx */
	mov %rsi, %rcx
	call *%rdi
	addq $64, %rsp
	unpad_stack
	ret

ENTRY(efi_call9)
	pad_stack
	subq $80, %rsp
	addq $80, %r11
	addq $24, %r11
	addq %rsp, %r11
	mov (%r11), %rax
	mov %rax, 64(%rsp)
	subq $8, %r11
	mov (%r11), %rax
	mov %rax, 56(%rsp)
	subq $8, %r11
	mov (%r11), %rax
	mov %rax, 48(%rsp)
	subq $8, %r11
	mov (%r11), %rax
	mov %rax, 40(%rsp)
	mov %r9, 32(%rsp)
	mov %r8, %r9
	mov %rcx, %r8
	/* mov %rdx, %rdx */
	mov %rsi, %rcx
	call *%rdi
	addq $80, %rsp
	unpad_stack
	ret

ENTRY(efi_call10)
	pad_stack
	subq $80, %rsp
	addq $80, %r11
	addq $32, %r11
	addq %rsp, %r11
	mov (%r11), %rax
	mov %rax, 72(%rsp)
	subq $8, %r11
	mov (%r11), %rax
	mov %rax, 64(%rsp)
	subq $8, %r11
	mov (%r11), %rax
	mov %rax, 56(%rsp)
	subq $8, %r11
	mov (%r11), %rax
	mov %rax, 48(%rsp)
	subq $8, %r11
	mov (%r11), %rax
	mov %rax, 40(%rsp)
	mov %r9, 32(%rsp)
	mov %r8, %r9
	mov %rcx, %r8
	/* mov %rdx, %rdx */
	mov %rsi, %rcx
	call *%rdi
	addq $80, %rsp
	unpad_stack
	ret
