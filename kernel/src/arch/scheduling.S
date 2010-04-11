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

[GLOBAL switchToThread]
switchToThread:
    mov ebx, dword[esp + 4]     ; Get thread info
    
    mov ecx, dword[ebx + 16]    ; restore registers
    mov edx, dword[ebx + 20]
    mov esp, dword[ebx + 28]
    mov ebp, dword[ebx + 32]
    mov esi, dword[ebx + 36]
    mov edi, dword[ebx + 40]
    
    mov eax, dword[ebx + 48]    ; restore segments
    mov es,  eax
    mov eax, dword[ebx + 44]
    mov ds,  eax
    mov eax, dword[ebx + 52]
    mov fs,  eax
    mov eax, dword[ebx + 56]
    mov gs,  eax
    ;mov eax, dword[ebx + 60]
    ;mov ss,  eax               ;FIXME: HÄ? INT13?
    
    mov eax, dword[ebx + 12]    ; finally restore eax
    
    push dword[ebx + 8]         ; push eflags
    push dword[ebx + 4]         ; push cs
    push dword[ebx + 0]         ; push eip
    
    push dword[ebx + 24]        ; restore ebx
    pop ebx
    
    iretd                       ; return to thread
    
    