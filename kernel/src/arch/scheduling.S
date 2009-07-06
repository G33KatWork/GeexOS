[BITS 32]

[GLOBAL writeStackPointer]
writeStackPointer:
    pop ebx
    pop eax
    mov esp, eax
    jmp ebx

[GLOBAL readStackPointer]
readStackPointer:
    mov eax, esp
    add eax, 4          ; Stack was pushed with return address, so take into account.
    ret

[GLOBAL writeBasePointer]
writeBasePointer:
    mov ebp, [esp+4]
    ret

[GLOBAL readBasePointer]
readBasePointer:
    mov eax, ebp
    ret

[GLOBAL readInstructionPointer]
readInstructionPointer:
    pop eax     ; Get the return address
    jmp eax     ; return - can't use RET because return address popped off
                ; stack.