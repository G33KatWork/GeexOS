[SECTION .text]
[BITS 32]

%macro ISR_NOERRCODE 1
	[GLOBAL _exception%1]
	_exception%1:
		cli
		push dword 0
		push dword %1
		jmp exception_common_stub
%endmacro

%macro ISR_ERRCODE 1
	[GLOBAL _exception%1]
	_exception%1:
		cli
		push dword %1
		jmp exception_common_stub
%endmacro

%macro IRQ 1
	[GLOBAL _irq%1]
	_irq%1:
		cli
		push dword 0
		push dword %1
		jmp irq_common_stub
%endmacro

;Define standard Exceptions
ISR_NOERRCODE 0
ISR_NOERRCODE 1
ISR_NOERRCODE 2
ISR_NOERRCODE 3
ISR_NOERRCODE 4
ISR_NOERRCODE 5
ISR_NOERRCODE 6
ISR_NOERRCODE 7
ISR_ERRCODE   8
ISR_NOERRCODE 9
ISR_ERRCODE   10
ISR_ERRCODE   11
ISR_ERRCODE   12
ISR_ERRCODE   13
ISR_ERRCODE   14
ISR_NOERRCODE 15
ISR_NOERRCODE 16
ISR_NOERRCODE 17
ISR_NOERRCODE 18
ISR_NOERRCODE 19
ISR_NOERRCODE 20
ISR_NOERRCODE 21
ISR_NOERRCODE 22
ISR_NOERRCODE 23
ISR_NOERRCODE 24
ISR_NOERRCODE 25
ISR_NOERRCODE 26
ISR_NOERRCODE 27
ISR_NOERRCODE 28
ISR_NOERRCODE 29
ISR_NOERRCODE 30
ISR_NOERRCODE 31

;legacy interrupts
IRQ 32
IRQ 33
IRQ 34
IRQ 35
IRQ 36
IRQ 37
IRQ 38
IRQ 39
IRQ 40
IRQ 41
IRQ 42
IRQ 43
IRQ 44
IRQ 45
IRQ 46
IRQ 47

;additional ioapic interrupts
IRQ 48
IRQ 49
IRQ 50
IRQ 51
IRQ 52
IRQ 53
IRQ 54
IRQ 55

;msi interrupts
IRQ 56
IRQ 57
IRQ 58
IRQ 59
IRQ 60
IRQ 61
IRQ 62
IRQ 63
IRQ 64
IRQ 65
IRQ 66
IRQ 67
IRQ 68
IRQ 69
IRQ 70
IRQ 71
IRQ 72
IRQ 73
IRQ 74
IRQ 75
IRQ 76
IRQ 77
IRQ 78
IRQ 79
IRQ 80
IRQ 81
IRQ 82
IRQ 83
IRQ 84
IRQ 85
IRQ 86
IRQ 87
IRQ 88
IRQ 89
IRQ 90
IRQ 91
IRQ 92
IRQ 93
IRQ 94
IRQ 95
IRQ 96
IRQ 97
IRQ 98
IRQ 99
IRQ 100
IRQ 101
IRQ 102
IRQ 103
IRQ 104
IRQ 105
IRQ 106
IRQ 107
IRQ 108
IRQ 109
IRQ 110
IRQ 111
IRQ 112
IRQ 113
IRQ 114
IRQ 115
IRQ 116
IRQ 117
IRQ 118
IRQ 119
IRQ 120
IRQ 121
IRQ 122
IRQ 123
IRQ 124
IRQ 125
IRQ 126
IRQ 127
IRQ 128
IRQ 129
IRQ 130
IRQ 131
IRQ 132
IRQ 133
IRQ 134
IRQ 135
IRQ 136
IRQ 137
IRQ 138
IRQ 139
IRQ 140
IRQ 141
IRQ 142
IRQ 143
IRQ 144
IRQ 145
IRQ 146
IRQ 147
IRQ 148
IRQ 149
IRQ 150
IRQ 151
IRQ 152
IRQ 153
IRQ 154
IRQ 155
IRQ 156
IRQ 157
IRQ 158
IRQ 159
IRQ 160
IRQ 161
IRQ 162
IRQ 163
IRQ 164
IRQ 165
IRQ 166
IRQ 167
IRQ 168
IRQ 169
IRQ 170
IRQ 171
IRQ 172
IRQ 173
IRQ 174
IRQ 175
IRQ 176
IRQ 177
IRQ 178
IRQ 179
IRQ 180
IRQ 181
IRQ 182
IRQ 183
IRQ 184
IRQ 185
IRQ 186
IRQ 187
IRQ 188
IRQ 189
IRQ 190
IRQ 191
IRQ 192
IRQ 193
IRQ 194
IRQ 195
IRQ 196
IRQ 197
IRQ 198
IRQ 199
IRQ 200
IRQ 201
IRQ 202
IRQ 203
IRQ 204
IRQ 205
IRQ 206
IRQ 207
IRQ 208
IRQ 209
IRQ 210
IRQ 211
IRQ 212
IRQ 213
IRQ 214
IRQ 215
IRQ 216
IRQ 217
IRQ 218
IRQ 219
IRQ 220
IRQ 221
IRQ 222
IRQ 223
IRQ 224
IRQ 225
IRQ 226
IRQ 227
IRQ 228
IRQ 229
IRQ 230
IRQ 231
IRQ 232
IRQ 233
IRQ 234
IRQ 235
IRQ 236
IRQ 237
IRQ 238
IRQ 239
IRQ 240
IRQ 241
IRQ 242
IRQ 243
IRQ 244
IRQ 245
IRQ 246
IRQ 247
IRQ 248
IRQ 249
IRQ 250

;local apic interrupts
IRQ 251     ;lapic timer
IRQ 252     ;??
IRQ 253     ;ici interrupt
IRQ 254     ;smp error
IRQ 255     ;spurious interrupt handler

;Our C-Handler for fault interrupts
[EXTERN _fault_handler]
exception_common_stub:
	pusha					; push all registers (see regs.h for corresponding struct)

	mov ax, ds		; save old data segment decriptor
	push eax
	
	push es
	push fs
	push gs

	mov ax, 0x10	; set kernel data segment descriptor
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	call _fault_handler

    pop gs
    pop fs
    pop es

	pop eax			; reload original data segment descriptor
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	popa
	add esp, 8     ; clean up the pushed error code and pushed ISR number
	sti
	iret

;Our C-Handler for common interrupts
[EXTERN _interrupt_handler]
irq_common_stub:
	pusha					; push all registers (see regs.h for corresponding struct)

	mov ax, ds		; save old data segment decriptor
	push eax
	
	push es
	push fs
	push gs

	mov ax, 0x10	; set kernel data segment descriptor
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	call _interrupt_handler
	
	pop gs
    pop fs
    pop es

	pop eax			; reload original data segment descriptor
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	popa
	add esp, 8     ; clean up the pushed error code and pushed ISR number
	sti
	iret
