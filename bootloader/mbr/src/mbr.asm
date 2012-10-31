[BITS 16]
[ORG 0x7C00]               ;we are at 0x7C00, but we copy ourselves early to this location (0x1FE0:0x7C00)

start:
    cli
    
    ;set up segments for copying
    xor ax, ax
    mov ds, ax
    
    ;copy ourself to segment 0x1FE0
    mov ax, 0x1FE0
    mov es, ax
    
    ;relocate
    mov si, 0x7C00  ;from ds:si (0x0:0x7C00)
    mov di, 0x7C00  ;to es:di (0x1FE0:0x7C00)
    mov cx, 0x100   ;256 words
    rep movsw
    
    ;jump to relocated code
    jmp word 0x1FE0:0x7C00 + relocated - start

relocated:
    mov ds, ax  ;load 0x1FE0 segment in ds and ss
    mov ss, ax
    
    xor ax, ax  ;es is now segment 0x0, we copy the chainloaded bootloader from the partition to this segment
    mov es, ax
    
    ;set up a stack
    mov sp, 0xFFE
    sti
    
    ;get partitionTable address
    mov bp, partitionTable
    
    ;get first byte from partition table
    lea di, [bp]
testNext:
    test byte [di], 0x80            ;check for active flag
    jne activePartitionFound        ;nope
    add di, 0x10                    ;increment pointer to next partition in table
    cmp di, partitionTableEnd       ;did we reach the end of the partition table?
    jb testNext

    ;we didn't find anything, display error message
    jmp errorNoActivePartition

activePartitionFound:
    ;we found an active partition, di contains the pointer to the partition entry
    ;call bios_print
    ;db  'Found active partition', 13, 10, 0
    
    call readBootSector
    jc errorRead
    
    cmp word [es:(0x7C00 + 0x01FE)], 0xaa55  ;check for boot signature
    jne errorInvalidBootSignature
    
    jmp word 0x0:0x7C00
    
;read 512 bytes from drive described by partition table entry
;di contains the memory address of the partition table entry
;dl needs to contain the BIOS drive number
;sets carry bit on failure
readBootSector:
    ;check for LBA support
    ;http://en.wikipedia.org/wiki/INT_13H#INT_13h_AH.3D41h:_Check_Extensions_Present
    mov bx, 0x55aa
    mov ah, 0x41
    int 0x13
    
    jc noLBA           ;test for command success
    cmp bx, 0xAA55      ;test for magic return value
    jne noLBA
    
    and cl, 1           ;test for drive access with lba packet
    jz noLBA

    jmp LBA


_bios_lba_packet:
    db 16            ;size of the packet (16 Bytes)
    db 0             ;always 0
    dw 1             ;transfer 1 sector
    dw 0x7C00        ;destination offset (0x7C00)
    dw 0x0000        ;destination segment (0)
_bios_lba_low:
    dw 0
_bios_lba_high:
    dw 0
    dw 0,0

LBA:
    ;read lba start address from partition table into packet structure
    mov ax, [di + 8]
    mov word [ds:_bios_lba_low], ax
    mov ax, [di + 8 + 2]
    mov word [ds:_bios_lba_high], ax
    
    ;issue rad command
    mov ax, 0x4200
    mov si, _bios_lba_packet
    
    ;read!
    int 0x13
    ret

noLBA:
    mov ax, 0x0201  ;AH = 0x02 - read sectors, AL = 0x01 - sector count
    mov bx, 0x7c00  ;ES:BX = destination buffer
    mov cx, [di+2]  ;CX = Track & Sector
    mov dh, [di+1]  ;DH = Head - DL = BIOS drive number
    
    int 0x13
    ret

;------- Error Messages --------

errorInvalidBootSignature:
    call bios_print
    db 'Active partition misses boot sig', 13, 10, 0
    jmp hang

errorNoActivePartition:
    call bios_print
    db  'No active partition found', 13, 10, 0
    jmp hang

errorRead:
    call bios_print
    db 'Read failed', 13, 10, 0
    jmp hang

hang:
    jmp hang

;--------- Printing ----------

;print string after call to this function
;dirty, but nice
bios_print:
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

;fill up the file with zeros until we get 512 bytes
;4*16 byte partition table
;2 byte signature (0x55AA)
times 512 - 4*16 - 2 - ($-$$) db 0

;the partition table needs to be preserved when this code is copied to the mbr
partitionTable:
    times 16*4 db 0
partitionTableEnd:

;boot signature
    db 0x55
    db 0xAA
