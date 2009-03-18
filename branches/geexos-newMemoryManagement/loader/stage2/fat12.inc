
;*******************************************************
;
;	Fat12.inc
;		FAT12 filesystem for 3-1/2 floppies
;
;	OS Development Series
;*******************************************************

%ifndef __FAT12_INC_67343546FDCC56AAB872_INCLUDED__
%define __FAT12_INC_67343546FDCC56AAB872_INCLUDED__

bits	16

%include "stage2/floppy16.inc"

%define ROOT_OFFSET     0x2e00
%define FAT_SEG         0x2c0
%define ROOT_SEG        0x2e0

;*******************************************
; LoadRoot ()
;	- Load Root Directory Table to ROOT_SEG
;*******************************************
LoadRoot:
    pusha
    push es
    
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
    
    push word ROOT_SEG              ;read to ROOT_SEG
    pop es                          ;set new extra segment
    mov bx, 0
    call ReadSectors
    
    pop es                          ;restore old extra segment
    popa
    ret

;*******************************************
; LoadFAT ()
;	- Loads FAT table to 0x7c00
;
;	Parm/ ES:DI => Root Directory Table
;*******************************************
LoadFAT:
    pusha
    push es
    
    xor ax, ax                      ;compute fat size -> cx
    mov al, BYTE [bpbNumberOfFATs]
    mul WORD [bpbSectorsPerFAT]
    mov cx, ax
    
    mov ax, WORD [bpbReservedSectors]   ;store beginning of FAT in ax
    
    push word FAT_SEG               ;read FAT to FAT_SEG
    pop es
    
    xor bx,bx
    call ReadSectors
    
    pop es
    popa
    ret
    
	
;*******************************************
; FindFile ()
;	- Search for filename in root table
;
; parm/ DS:SI => File name
; ret/ AX => File index number in directory table. -1 if error
;*******************************************
FindFile:
    push cx
    push dx
    push bx
    mov bx, si                      ;copy filename

    mov cx, WORD [bpbRootEntries]   ;load counter for all root entries
    mov di, ROOT_OFFSET             ;move index to first root entry
    cld
    
.loop:
    push cx
    mov cx, 0x000B                  ;11 chars in name
    mov si, bx
    push di
    rep cmpsb                       ;find matching bytes between es:di and si
    pop di
    je .found                       ;found!
    pop cx
    add di, 0x0020                  ;each entry is 32byte long -> next
    loop .loop
    
.notFound:                          ;not found :'(
    pop bx
    pop dx
    pop cx
    mov ax, -1                      ;error -1
    ret

.found:
    pop ax
    pop bx
    pop dx
    pop cx
    ret


;*******************************************
; LoadFile ()
;	- Load file
; parm/ ES:SI => File to load
; parm/ EBX:BP => Buffer to load file to
; ret/ AX => -1 on error, 0 on success
; ret/ CX => number of sectors read
;*******************************************

LoadFile:

	xor	ecx, ecx		; size of file in sectors
	push	ecx

.FIND_FILE:

	push	bx			; BX=>BP points to buffer to write to; store it for later
	push	bp
	call	FindFile		; find our file. ES:SI contains our filename
	cmp	ax, -1
	jne	.LOAD_IMAGE_PRE
	pop	bp
	pop	bx
	pop	ecx
	mov	ax, -1
	ret

.LOAD_IMAGE_PRE:

	sub	edi, ROOT_OFFSET
	sub	eax, ROOT_OFFSET

	; get starting cluster

	push	word ROOT_SEG		;root segment loc
	pop	es
	mov	dx, WORD [es:di + 0x001A]; DI points to file entry in root directory table. Refrence the table...
	mov	WORD [cluster], dx	; file's first cluster
	pop	bx			; get location to write to so we dont screw up the stack
	pop	es
	push    bx			; store location for later again
	push	es
	call	LoadFAT

.LOAD_IMAGE:

	; load the cluster

	mov	ax, WORD [cluster]	; cluster to read
	pop	es			; bx:bp=es:bx
	pop	bx
	call	ClusterToLBA
	xor	cx, cx
	mov     cl, BYTE [bpbSectorsPerCluster]
	call	ReadSectors
	pop	ecx
	inc	ecx			; add one more sector to counter
	push	ecx
	push	bx
	push	es
	mov	ax, FAT_SEG		;start reading from fat
	mov	es, ax
	xor	bx, bx

	; get next cluster

	mov     ax, WORD [cluster]	; identify current cluster
	mov     cx, ax			; copy current cluster
	mov     dx, ax
	shr     dx, 0x0001		; divide by two
	add     cx, dx			; sum for (3/2)

	mov	bx, 0			;location of fat in memory
	add	bx, cx
	mov	dx, WORD [es:bx]
	test	ax, 0x0001		; test for odd or even cluster
	jnz	.ODD_CLUSTER

.EVEN_CLUSTER:

	and	dx, 0000111111111111b	; take low 12 bits
	jmp	.DONE

.ODD_CLUSTER:

	shr	dx, 0x0004		; take high 12 bits

.DONE:

	mov	WORD [cluster], dx
	cmp	dx, 0x0ff0		; test for end of file marker
	jb	.LOAD_IMAGE

.SUCCESS:
	pop	es
	pop	bx
	pop	ecx
	xor	ax, ax
	ret

%endif		;__FAT12_INC_67343546FDCC56AAB872_INCLUDED__

