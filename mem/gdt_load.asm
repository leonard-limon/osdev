;
; gdt_load.asm - function to load gdt
; Copyright (C) 2018 - Leonard LIMON, LIC
;

[bits 32]
[global gdt_load]
[extern gdt_desc]

gdt_load:
    ; load new gdt
    mov		eax, gdt_desc
    lgdt	[eax]

    ; reload segments
    mov		ax, 0x10	;offset in gdt for data segment
	mov		ds, ax
    mov		es, ax
	mov		fs, ax
	mov		gs, ax

    ; setup new stack
    mov		ss, ax

    ; load code segment (0x08 is offset to code segment)
	jmp		0x08:flush

flush:
	ret
