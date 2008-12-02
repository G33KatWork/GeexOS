[BITS 16]                   ;16 Bit code
[ORG 0x500]
jmp main

%include "stage2/common.inc"
%include "stage2/stdio.inc"
%include "stage2/gdt.inc"
%include "stage2/a20.inc"
%include "stage2/fat12.inc"
%include "stage2/floppy16.inc"

;-------- Data --------

ProtectedModeMsg    db  "Kernel image loaded, swichting to protected mode...",13,10,0
LoadingMsg          db  "Searching for Kernel...",13,10,0
FailureMsg          db  "FATAL: Missing kernel image. Press any key to reboot",13,10,0

;----------------------

main:
    cli
    xor ax, ax          ;segments at 0x0
    mov es, ax
    mov ds, ax
    mov ax, 0x9000      ;stack at 0x9000
    mov ss, ax
    mov sp, 0xFFFF
    sti
    
    call InstallGDT
    
    call EnableA20_KKbrd_Out    ;enable A20 line
    
    mov si, LoadingMsg
    call Puts16
    
    call LoadRoot       ;Load root file table
    
    mov ebx, 0          ;BX:BP -> buffer to load to
    mov bp, IMAGE_RMODE_BASE
    
    mov si, ImageName
    call LoadFile
    mov dword [ImageSize], ecx
    cmp ax, 0            ;success?
    je EnterStage3
    mov si, FailureMsg  ;Error Message
    call Puts16
    mov ah, 0
    int 0x16
    int 0x19
    cli
    hlt
    
EnterStage3:
    mov si, ProtectedModeMsg
    call Puts16
    
    cli
    mov eax, cr0
    or eax, 1
    mov cr0, eax
    
    jmp CODE_DESC:Stage3    ;far jump to Stage3

;-------------------------------- 32 Bit Code --------------------------------
[BITS 32]

BadImage db "*** FATAL: Invalid or corrupt kernel image. Halting system.", 0

Stage3:
    mov ax, 0x10        ;set data segments to data selector (0x10)
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov esp, 0x90000     ;stack begins now 90000h
    
CopyImage:    
    mov eax, dword [ImageSize]
    movzx ebx, word [bpbBytesPerSector]
    mul ebx
    mov ebx, 4
    div ebx
    cld
    mov esi, IMAGE_RMODE_BASE
    mov edi, IMAGE_PMODE_BASE
    mov ecx, eax
    rep movsd

ExecuteImage:
    jmp CODE_DESC:IMAGE_PMODE_BASE
    
    cli
    hlt
    