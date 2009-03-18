
;*******************************************************
;
;	Floppy16.inc
;		Floppy drive interface routines
;
;	OS Development Series
;*******************************************************

%ifndef __FLOPPY16_INC_67343546FDCC56AAB872_INCLUDED__
%define __FLOPPY16_INC_67343546FDCC56AAB872_INCLUDED__

bits	16

bpbOEM	        		db "My OS   "
bpbBytesPerSector:  	DW 512
bpbSectorsPerCluster: 	DB 1
bpbReservedSectors: 	DW 1
bpbNumberOfFATs:     	DB 2
bpbRootEntries: 	    DW 224
bpbTotalSectors:    	DW 2880
bpbMedia: 		        DB 0xf0  ;; 0xF1
bpbSectorsPerFAT: 	    DW 9
bpbSectorsPerTrack: 	DW 18
bpbHeadsPerCylinder: 	DW 2
bpbHiddenSectors: 	    DD 0
bpbTotalSectorsBig:     DD 0
bsDriveNumber: 	        DB 0
bsUnused: 		        DB 0
bsExtBootSignature: 	DB 0x29
bsSerialNumber:	        DD 0xa0a1a2a3
bsVolumeLabel: 	        DB "MOS FLOPPY "
bsFileSystem: 	        DB "FAT12   "

datasector  dw 0x0000
cluster     dw 0x0000

absoluteSector db 0x00
absoluteHead   db 0x00
absoluteTrack  db 0x00

;************************************************;
; Convert CHS to LBA
; LBA = (cluster - 2) * sectors per cluster
;************************************************;
ClusterToLBA:
    sub     ax, 0x0002                          ; zero base cluster number
    xor     cx, cx
    mov     cl, BYTE [bpbSectorsPerCluster]     ; convert byte to word
    mul     cx
    add     ax, WORD [datasector]               ; base data sector
    ret

;************************************************;
; Convert LBA to CHS
; AX=>LBA Address to convert
;
; absolute sector = (logical sector / sectors per track) + 1
; absolute head   = (logical sector / sectors per track) MOD number of heads
; absolute track  = logical sector / (sectors per track * number of heads)
;
;************************************************;
LBAToCHS:
    xor dx, dx
    div     WORD [bpbSectorsPerTrack]           ; calculate
    inc     dl                                  ; adjust for sector 0
    mov     BYTE [absoluteSector], dl
    xor     dx, dx                              ; prepare dx:ax for operation
    div     WORD [bpbHeadsPerCylinder]          ; calculate
    mov     BYTE [absoluteHead], dl
    mov     BYTE [absoluteTrack], al
    ret


;************************************************;
; Reads a series of sectors
; CX=>Number of sectors to read
; AX=>Starting sector
; ES:EBX=>Buffer to read to
;************************************************;

ReadSectors:
.begin:
    mov di, 5                       ;5 retries
.loop:
    push ax
    push bx
    push cx
    call LBAToCHS
    mov ah, 0x02                    ;read sector
    mov al, 0x01                    ;one sector
    mov ch, BYTE [absoluteTrack]    ;track
    mov cl, BYTE [absoluteSector]   ;sector
    mov dh, BYTE [absoluteHead]     ;head
    mov dl, BYTE [bsDriveNumber]    ;drive
    int 0x13                        ;BIOS does the work
    jnc .success                    ;reading was successfull
    xor ax, ax                      ;reset disc
    int 0x13
    dec di                          ;error counter -= 1
    pop cx
    pop bx
    pop ax
    jnz .loop                       ;read again
    
    int 0x18                        ;warm reboot
.success:
    pop cx
    pop bx
    pop ax
    add bx, WORD [bpbBytesPerSector]    ;queue next buffer
    inc ax                              ;queue next sector
    loop .begin                         ;go and do your work...
    ret

%endif		;__FLOPPY16_INC_67343546FDCC56AAB872_INCLUDED__
