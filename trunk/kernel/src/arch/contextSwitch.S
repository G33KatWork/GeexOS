[BITS 32]

CTX_OFF_GS      equ     0   ;GS
CTX_OFF_FS      equ     2   ;FS
CTX_OFF_DS      equ     4   ;DS
CTX_OFF_CS      equ     6   ;CS
CTX_OFF_SS      equ     8   ;SS
CTX_OFF_ES      equ     10  ;ES

CTX_OFF_EIP     equ     12  ;EIP offset
CTX_OFF_ESP     equ     16	;ESP offset
CTX_OFF_EBP		equ     20	;EBP offset
CTX_OFF_EBX		equ     24	;EBX offset
CTX_OFF_EDI		equ     28	;EDI offset
CTX_OFF_ESI		equ     32	;ESI offset

CTX_OFF_EDX		equ     36	;EDX offset
CTX_OFF_ECX		equ     40	;ECX offset
CTX_OFF_EAX		equ     44	;EAX offset

CTX_OFF_EFLAGS	equ     48	;EFLAGS offset


;int context_save(context *ctx)
[GLOBAL context_save]
context_save: 
        mov     eax, [esp+4]

        ;Save old instruction/stack pointers
        mov     ecx, [esp]
        mov     [CTX_OFF_EIP+eax],ecx
        mov     [CTX_OFF_ESP+eax],esp
        
        ;Save segment registers
        mov     [CTX_OFF_GS+eax],gs
        mov     [CTX_OFF_FS+eax],fs
        mov     [CTX_OFF_DS+eax],ds
        mov     [CTX_OFF_CS+eax],cs
        mov     [CTX_OFF_SS+eax],ss
        mov     [CTX_OFF_ES+eax],es

        ;Save old callee-save registers
        mov     [CTX_OFF_EBP+eax],ebp
        mov     [CTX_OFF_EBX+eax],ebx
        mov     [CTX_OFF_EDI+eax],edi
        mov     [CTX_OFF_ESI+eax],esi
        mov     [CTX_OFF_EDX+eax],edx
        mov     [CTX_OFF_ECX+eax],ecx
        mov     [CTX_OFF_EAX+eax],eax
        
        ret

;int context_restore(context *ctx)
[global context_restore]
context_restore: 
        mov     eax, [esp+4]

        ;Restore new callee-save registers
        mov     esi, [CTX_OFF_ESI+eax]
        mov     edi, [CTX_OFF_EDI+eax]
        mov     ebx, [CTX_OFF_EBX+eax]
        mov     ebp, [CTX_OFF_EBP+eax]

        ;Restore new instruction/stack pointers
        mov     esp, [CTX_OFF_ESP+eax]
        mov     ecx, [CTX_OFF_EIP+eax]
        mov     [esp],ecx

        ;xor     eax,eax    ;WTF???
        ;inc     eax
        ret
