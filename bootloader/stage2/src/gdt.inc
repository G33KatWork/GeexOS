%ifndef _GDT_INCLUDED_
%define _GDT_INCLUDED_

[BITS 16]

loadGDT:
    xor     eax, eax
    mov     ax, ds
    shl     eax, 4
    add     eax, gdt_data
    mov     [gdt_toc + 2], eax      ;set base of gdt
    
    mov     eax, gdt_end
    sub     eax, gdt_data
    mov     [gdt_toc], ax           ;set limit

    cli                             ;disable interrupts
    pusha
    lgdt    [gdt_toc]               ;load gdt
    sti                             ;enable interrupts
    popa
    ret

;---------------------------- GDT ----------------------------

gdt_data: 
	dd 0                            ;null descriptor
	dd 0 

; gdt code:	                        ;code descriptor
	dw 0FFFFh                       ;limit low
	dw 0                            ;base low
	db 0                            ;base middle
	db 10011010b                    ;access
	db 11001111b                    ;granularity
	db 0                            ;base high

; gdt data:	                        ;data descriptor
	dw 0FFFFh                       ;limit low
	dw 0                            ;base low
	db 0                            ;base middle
	db 10010010b                    ;access
	db 11001111b                    ;granularity
	db 0                            ;base high
gdt_end:

gdt_toc: 
    DW 0
    DD 0
	
%define GDT_NULL_DESC 0
%define GDT_CODE_DESC 0x8
%define GDT_DATA_DESC 0x10
	
%endif
