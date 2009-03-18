[BITS 16]                   ;16 Bit code
[ORG 0x0]                   ;register init see main

start:
    jmp main
    
;-------- OEM Parameter Block --------

TIMES 0Bh-$+start DB 0

bpbBytesPerSector:      DW 512
bpbSectorsPerCluster: 	DB 1
bpbReservedSectors: 	DW 1
bpbNumberOfFATs: 	    DB 2
bpbRootEntries: 	    DW 224
bpbTotalSectors: 	    DW 2880
bpbMedia: 	            DB 0xF0
bpbSectorsPerFAT: 	    DW 9
bpbSectorsPerTrack: 	DW 18
bpbHeadsPerCylinder: 	DW 2
bpbHiddenSectors: 	    DD 0
bpbTotalSectorsBig:     DD 0
bsDriveNumber: 	        DB 0
bsUnused: 	            DB 0
bsExtBootSignature: 	DB 0x29
bsSerialNumber:	        DD 0xa0a1a2a3
bsVolumeLabel: 	        DB "MOS FLOPPY "
bsFileSystem: 	        DB "FAT12   "


;-------- Loader --------

main:
    cli
    mov ax, 0x07C0          ;Setup segments
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    
    mov ax, 0x0000          ;Setup stack
    mov ss, ax
    mov sp, 0xFFFF
    sti
    
    mov si, HelloMsg        ;Say hello
    call PutStr
    
    ;jmp LoadRoot
    
;----------------

LoadRoot:                           ;load root directory and store in cx
    xor cx,cx                       ;compute size of root directory -> cx (should be 14 sectors)
    xor dx,dx
    mov ax,0x0020                   ;32 byte directory entry
    mul WORD [bpbRootEntries]
    div WORD [bpbBytesPerSector]
    xchg ax, cx
    
    mov al, BYTE [bpbNumberOfFATs]  ;compute location of root directory -> ax (should be 19)
    mul WORD [bpbSectorsPerFAT]
    add ax, WORD [bpbReservedSectors]
    mov WORD [datasector], ax       ;??????
    add WORD [datasector], cx       ;??????
    
    mov bx, 0x0200                  ;copy root dir into 7C00:0200
    call ReadFloppy
    
FindFile:
    mov cx, WORD [bpbRootEntries]   ;load counter for all root entries
    mov di, 0x0200                  ;move index to first root entry
.loop:
    push cx
    mov cx, 0x000B                  ;11 chars
    mov si, ImageName
    push di
    rep cmpsb                       ;find matching bytes between es:di and si
    pop di
    je LoadFat                      ;found!
    pop cx
    add di, 0x0020                  ;each entry is 32byte long -> next
    loop .loop
    jmp DisplayFailure              ;not found :'(
    
    
LoadFat:
    mov si, CRLF
    call PutStr
    mov dx, WORD [di + 0x001A]      ;file's first cluster (byte 26)
    mov WORD[cluster], dx
    
    xor ax, ax                      ;compute fat size -> cx
    mov al, BYTE [bpbNumberOfFATs]
    mul WORD [bpbSectorsPerFAT]
    mov cx, ax
    
    mov ax, WORD [bpbReservedSectors]   ;store beginning of FAT in ax
    
    mov bx, 0x0200                  ;read FAT t0 0x07C0:0200
    call ReadFloppy
    

ReadFile:
    mov si, CRLF                    ;read File to 0050:0000
    call PutStr
    mov ax, 0x0050
    mov es, ax
    mov bx, 0x0000
    push bx

LoadImage:
    mov ax, WORD [cluster]          ;cluster to read
    pop bx                          ;buffer
    call ClusterToLBA
    xor cx, cx
    mov cl, BYTE [bpbSectorsPerCluster]
    call ReadFloppy
    push bx
    
    mov ax, WORD [cluster]
    mov cx, ax
    mov dx, ax
    shr dx, 1                       ;divide by 2
    add cx, dx                      ;sum for 3/2
    mov bx, 0x0200                  ;FAT location in memory
    add bx, cx                      ;cluster number
    mov dx, WORD [bx]               ;read two bytes
    test ax, 1
    jnz .oddCluster

.evenCluster:
    and dx, 0000111111111111b       ;take lower 12 bits
    jmp .done
    
.oddCluster:
    shr dx, 4                       ;take higher 12 bits
    
.done:
    mov WORD [cluster], dx          ;set next cluster
    cmp dx, 0x0FF0                  ;EOF?
    jb LoadImage
    
    mov si, CRLF                    ;almost done!
    call PutStr
    push WORD 0x0050                ;load address for stage2 (0x50:0 = 0x500)
    push WORD 0x0000
    retf                            ;go and have some fun...
    
;----------------

;AX -> Cluster to Convert
;
;AX = (cluster - 2) * sectors per cluster
ClusterToLBA:
    sub     ax, 0x0002                          ; zero base cluster number
    xor     cx, cx
    mov     cl, BYTE [bpbSectorsPerCluster]     ; convert byte to word
    mul     cx
    add     ax, WORD [datasector]               ; base data sector
    ret


;AX -> LBA Address to convert
;
;absolute sector = (logical sector / sectors per track) + 1
;absolute head   = (logical sector / sectors per track) MOD number of heads
;absolute track  = logical sector / (sectors per track * number of heads)
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
    
;----------------

ReadFloppy:
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
    
    call DisplayFailure
.success:
    mov si, ProgressMsg
    call PutStr
    pop cx
    pop bx
    pop ax
    add bx, WORD [bpbBytesPerSector]    ;queue next buffer
    inc ax                              ;queue next sector
    loop .begin                         ;go and do your work...
    ret

;----------------

DisplayFailure:
    mov si, FailMsg
    call PutStr
    mov ah, 0x00
    int 0x16                        ;await keypress
    int 0x19                        ;warm boot

;----------------

PutStr:
    lodsb                           ;Loads [SI] in al
    or al,al
    jz .return
    mov ah, 0x0E                    ;Nope-Print char
    int 0x10                        ;call BIOS function
    jmp PutStr
.return:
    ret

;-------- Data --------

absoluteSector      db 0x00
absoluteHead        db 0x00
absoluteTrack       db 0x00

datasector          dw 0x0000
cluster             dw 0x0000
ImageName           db "STAGE2  BIN"

HelloMsg            db 'GeexOS Loader',13,10,'Loading Stage2...',13,10,0
FailMsg             db 'STAGE2.BIN not found. Press any key...',13,10,0
CRLF                db 13,10,0
ProgressMsg         db '.',0

times 510-($-$$)    db 0            ;Fill rest with zeros
dw 0xAA55                           ;Expected Bootloader signature