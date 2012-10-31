[BITS 16]
[SECTION .text]
[ORG 0x7C00]
;[MAP ALL stage1.map]

ROOTDIRSEGMENT  EQU     0x2000
LOWFATSEGMENT   EQU     0x3000
HIGHFATSEGMENT  EQU     0x4000
STAGE2SEGMENT   EQU     0x800

; FAT 12/16 Layout:
; +----------------+------------------+------+------+----------------+-------------+
; | Hidden Sectors | Reserved Sectors | FAT1 | FAT2 | Root Directory | Data region |
; +----------------+------------------+------+------+----------------+-------------+
;                  ^
;                  |
;       Partition Start for HDDs

start:
    jmp entry
    nop

;fill in 0s between entrypoint and boot parameter block
times 03h-$+start       DB 0

;dummy boot parameter block
;needs to be replaced with real block when stage is installed to HDD
bpbOemId:               DB "        "
bpbBytesPerSector:  	DW 512
bpbSectorsPerCluster: 	DB 2
bpbReservedSectors: 	DW 1
bpbNumberOfFATs: 	    DB 2
bpbRootdirEntries: 	    DW 512
bpbTotalSectors: 	    DW 20417
bpbMedia: 	            DB 0xF8
bpbSectorsPerFAT: 	    DW 40
bpbSectorsPerTrack: 	DW 32
bpbHeadsPerCylinder: 	DW 16
bpbHiddenSectors: 	    DD 63
bpbTotalSectorsBig:     DD 0
bsDriveNumber: 	        DB 0x80
bsReserved:	            DB 0
bsExtBootSignature: 	DB 0x29
bsSerialNumber:	        DD 0x4BFF1F13
bsVolumeLabel: 	        DB "NO NAME    "
bsFileSystem: 	        DB "FAT16   "

entry:
    ;set up segment registers
    xor	ax, ax
    mov	ds, ax
    
    ;set up a stack
    mov	ss, ax                                  ;if we set ss and sp in consecutive instructions, we don't need cli/sti'
    mov sp, 0x7C00
    
    mov BYTE [bsDriveNumber], dl
    
    ;set up fs and gs for high fat and low fat
    push LOWFATSEGMENT
    pop fs
    push HIGHFATSEGMENT
    pop gs
    
    ;calculate root directory size
    mov eax, 32                      ;root dir entry = 32 Bytes
    mul WORD [bpbRootdirEntries]
    xor edx, edx
    div WORD [bpbBytesPerSector]
    mov DWORD [rootDirLenSectors], eax
        
    ;calculate root directory start sector
    ;root directory comes after the FATs, reserved sectors and hidden sectors
    mov al, BYTE [bpbNumberOfFATs]
    cbw                                             ;zero ah
    mov bx, WORD [bpbSectorsPerFAT]
    mul bx
    xor ebx, ebx
    mov bx, WORD [bpbReservedSectors]
    add eax, ebx
    add eax, DWORD [bpbHiddenSectors]
    mov DWORD [rootDirStartSector], eax
    
    ;calculate data region start sector
    mov ebx, DWORD [rootDirStartSector]
    add ebx, DWORD [rootDirLenSectors]
    mov DWORD [dataRegionStartSector], ebx

;.detectFat:
    ;calculate total data cluster amount for fat12/16 detection
    ;sub ebx, DWORD [bpbHiddenSectors]
    
    ;mov ax, WORD [bpbTotalSectors]                  ;if total sectors are too big to fit here, the DWORD value is used
    ;add eax, DWORD [bpbTotalSectorsBig]
    ;sub eax, ebx
    
    ;add eax, 2                                      ;FIXME: Use sectors per cluster here?
    
    ;xor edx, edx
    ;movzx bx, BYTE [bpbSectorsPerCluster]
    ;div ebx                                         ;eax=edx:eax/ebx  ebx=remainder
    
    ;cmp eax, 4087
    ;jae .fat16detected
;.fat12detected:
;    mov BYTE [isFat12], 1
;    jmp .readRootdir
;.fat16detected:
;    mov BYTE [isFat12], 0

;FIXME: Does this always work?
.detectFat:
    cmp BYTE [bsFileSystem+4], '6'
    je .fat16detected
.fat12detected:
    mov BYTE [isFat12], 1
    jmp .readRootdir
.fat16detected:
    mov BYTE [isFat12], 0

.readRootdir:
    ;Read the root dir sectors to 0x10000
    mov eax, DWORD [rootDirStartSector]
    mov ecx, DWORD [rootDirLenSectors]
    push ROOTDIRSEGMENT
    pop es
    xor di, di
    call readSectors                            ;read ecx sectors starting from eax into es:di
    jc .errorRead
    
    ;search file
    mov cx, WORD [bpbRootdirEntries]            ;we have bpbRootdirEntries entries
.searchLoop:
    push cx
    
    mov cx, 11                                  ;compare 11 characters
    mov si, Stage2Name
    push di
    rep cmpsb
    
    pop di
    je .foundFile
    
    pop cx
    add di, 32                                  ;next entry
    loop .searchLoop
    jmp .errorNotFound

.foundFile:
    mov dx, [es:di + 0x1A] ;get start cluster
    mov WORD [fileStartCluster], dx
    
    ;now we read the FAT completely into memory
    ;FAT12 can have 4086 clusters with 12 bit each. max FAT size: 6129 Byte
    ;FAT16 can have 65526 clusters with 16 bit each. max FAT size: 131052 Byte = ca. 128KB
    ;FAT32 is much more complicated, but we don't handle it here anyways
    
    mov eax, DWORD [bpbHiddenSectors]
    mov bx, WORD [bpbReservedSectors]
    add eax, ebx                                ;eax contains fat start
    
    mov cx, WORD [bpbSectorsPerFAT]
    
    cmp cx, 0x80                                ;do we need to read the 2nd half of the fat?
    jbe .readFirstFATHalf
    
    push eax
    
    ;read second FAT half
    sub cx, 0x80                                ;sectorcount
    push HIGHFATSEGMENT
    pop es                                      ;destination segment
    xor di, di                                  ;destination offset
    add eax, 0x80                               ;read start
    
    call readSectors                            ;read ecx sectors starting from eax into es:di
    jc .errorRead
    
    mov cx, 0x80                                ;if we read a 2nd half, the first one will have 0x80 sectors
    pop eax
    
.readFirstFATHalf:
    ;read first FAT half
    push LOWFATSEGMENT
    pop es                                      ;destination segment
    xor di, di                                  ;destination offset
    
    call readSectors                            ;read ecx sectors starting from eax into es:di
    jc .errorRead
    
    
.loadfile:
    ;now we are going to load the file itself
    push STAGE2SEGMENT
    pop es                                      ;destination segment
    xor di, di                                  ;destination offset
    mov ax, WORD [fileStartCluster]             ;get first cluster number in eax

.loadNextCluster:
    push eax                                    ;save eax - will be argument to getNextCluster
    movzx cx, BYTE [bpbSectorsPerCluster]
    sub eax, 2                                  ;zero-base cluster
    mul ecx
    add eax, DWORD [dataRegionStartSector]      ;eax now contains start sector of current cluster
    
    call readSectors                            ;read ecx sectors starting from eax into es:di
    jc .errorRead
    
    movzx cx, BYTE [bpbSectorsPerCluster]       ;add clustersize in bytes to the destination offset
    mov ax, WORD [bpbBytesPerSector]
    mul cx
    add di, ax
    
    jnc .noNextSegment                          ;if carry is set, we need to write to the next segment
.nextSeg:
    push es
    pop ax
    add ax, 0x1000
    mov es, ax
.noNextSegment:
    
    pop eax                                     ;get the next cluster
    call getNextCluster
    
    cmp ax, WORD 0xffff                         ;last cluster?
    je .fileLoaded
    jmp .loadNextCluster

.fileLoaded:
    jmp word STAGE2SEGMENT:0x0                  ;jump to stage2

.errorNotFound:
    call bios_print
    db 'GXLDR missing', 0
    jmp .hlt

.errorRead:
    call bios_print
    db 'IO Error', 0

.hlt:
    cli
    hlt

;------- FAT Handling --------
;takes current cluster ax and returns next in ax
getNextCluster:
    mov cl, BYTE[isFat12]
    cmp cl, 0
    je .fat16

.fat12:
    mov si, ax
    mov dx, ax
    shr dx, 1
    add si, dx
    
    test ax, 1
    mov ax, [fs:si]                             ;get cluster value
    jnz .odd_cluster

.even_cluster:
    and ax, 0xFFF
    jmp .checkEof12
.odd_cluster:
    shr ax, 4
    
.checkEof12:
    cmp ax, 0xff8                              ;next cluster greater equal 0xfff8? -> EOF
    jb .out
    mov ax, 0xffff
    
    jmp .out

.fat16:
    add ax, ax                                  ;ax = 2*ax, if carry is set, use high FAT
    mov si, ax
    jc .highfat16
    
    mov ax, [fs:si]                             ;get cluster value
    
    jmp .checkEof16

.highfat16:
    mov ax, [gs:si]

.checkEof16:
    cmp ax, 0xfff8                              ;next cluster greater equal 0xfff8? -> EOF
    jb .out
    mov ax, 0xffff

.out:
    ret

;---------  Disk IO ----------
; Reads ecx disk sectors into es:di starting from sector eax
readSectors:
    ; build lba packet on stack
    push DWORD 0                                ;upper 32 bits of sector start
    push DWORD eax                              ;lower sector dword
    push es                                     ;destination segment
    push di                                     ;destination offset
    push cx                                     ;sectorcount
    push 0x0016                                 ;length of packet and zero byte
    
    ;issue read command
    mov si, sp                                  ;get pointer to structure on stack
    mov dl, BYTE [bsDriveNumber]
    mov ax, 0x4200
    int 0x13
    
    add sp, 16                                  ;clear struct on stack
    
    ret

;--------- Printing ----------
;print string after call to this function
;dirty, but nice
bios_print:
    mov bx, 07  ;color = gray on black
    pop si
    lodsb
    push si
    or al, al
    jz .done
    mov ah, 0x0E
    int 0x10
    jmp bios_print
.done:
    ret

;name of the second stage filename
Stage2Name:
    db 'GXLDR      '

;fill the loader with 0s
times 512 - 2 - ($-$$)  DB 0

;boot signature
DB 0x55
DB 0xAA

;--------- Variables ----------
[SECTION .bss]
rootDirLenSectors:      RESD    1
rootDirStartSector:     RESD    1
dataRegionStartSector:  RESD    1
isFat12:                RESB    1
fileStartCluster:       RESW    1

;--------- Misc stuff -----------
;Converting and dumping a variable for debugging on real hardware
;    mov DWORD [foovar], eax
;
;    mov cx, 0x04
;    mov si, foovar
;.again:
;
;    mov al, BYTE [ds:si]
;    and al, 0xF0
;    shr al, 4
;    add al, 0x30
;    mov ah, 0x0E
;    int 0x10
;
;    mov al, BYTE [ds:si]
;    and al, 0xF
;    add al, 0x30
;    mov ah, 0x0E
;    int 0x10
;
;    add si, 1
;
;    loop .again
