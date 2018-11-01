;
; bootloader.asm - the bootloader of LCOS
; Copyright (C) 2018 - Leonard LIMON, LIC
;

jmp 	0x7e0:bootldr

%include "a20.inc"

[BITS 16]
[ORG 0x0]

print:					;print null terminated string stored in si
	mov		ah, 0xe		;func 0xe of int 10h
	push	si

	.loop:
		lodsb
		test	al, al
		jz		.done
		mov		ah, 0xe
		int		0x10
		jmp		.loop

	.done:
		pop		si
		ret

printh:					;print hexadecimal passed by stack
	mov		dx, [esp+2]
	mov		cx, 4		;counter

	;print hex header
	mov		ah, 0xe
	mov		al, '0'
	int		0x10
	mov		al, 'x'
	int		0x10

	.loop:
		rol		dx, 4
		mov		ax, dx
		and		ax, 0xf

		cmp		al, 9
		jg		.alpha
		add		al, '0'
		jmp		.disp

	.alpha:
		add		al, 'a' - 0xa

	.disp:
		mov		ah, 0xe
		int		0x10
		dec		cx
		jnz		.loop

	ret

bootldr:
	; init segment and flush cs
	mov		ax, 0x7e0
	mov		ds, ax
	mov		es, ax
	mov		fs, ax
	mov		gs, ax

	pop		dx
	mov		[BOOT_DRV], dl

	; print bootloader message
	mov		si, BOOTLDR_MSG
	call	print

	; get address of kernel
	mov		ax, 0x7c0
	mov		fs, ax
	mov		dl, [BOOT_DRV]
	mov		dh, [fs:0x1cf]	;head
	mov		cl, [fs:0x1d0]	;sect
	mov		ch, [fs:0x1d1]	;cylinder

	xor		ax, ax			;buffer seg and offset
	mov		es, ax
	mov		bx, 0x1000		;address at witch the kernel will be loaded

	mov		ah, 0x2			;func 2 (read) of int 13h
	mov		al, 0x10

	mov		ax, 0x210
	push	ax

	int		0x13

	; check disk read
	jc		diskerr
	pop		cx
	cmp		al, cl			;check #sect read
	jne		diskerr

	; successful read
	mov		dl, [BOOT_DRV]
	push	dx				;push boot drive
	mov		si, KLD_MSG
	call	print

	; check and enable a20
	call	a20_ck
	test	ax, ax
	jne		pmode_en			;a20 enabled
	call	enable_A20
	call	a20_ck
	test	ax, ax
	jne		pmode_en			;a20 enabled

	; could not active a20 line
	mov		si, A20_ERR_MSG
	call	print
	jmp		fatal

pmode_en:
	cli
	lgdt	[GDT_DESC]

	mov		eax, cr0
	or		eax, 0x1
	mov		cr0, eax

	jmp		CODE_SEG:pmode_offset

diskerr:
	mov		al, dl
	push	ax
	mov		si, DISK_ERR_MSG
	call	print
	call	printh
	add		sp, 2

fatal:
	mov		si, FATAL_ERR_MSG
	call	print
	xor		ax, ax
	int		0x16
	int		0x19
	jmp		$

BOOT_DRV:
	db	0x0

BOOTLDR_MSG: db "==== LCOS BOOTLOADER v0.0.1 ====", 10, 13, 0
DISK_ERR_MSG: db "ERROR READING DISK: ", 0
FATAL_ERR_MSG:
	db 10, 13, "FATAL: Could not load LCOS kernel, press any key to reboot...", 0
KLD_MSG: db "Kernel successfully loaded", 10, 13, 0
A20_ERR_MSG: db "Could not activate A20 line", 0

GDT:
    dd 0x0
    dd 0x0

GDT_CODE:
    dw 0xffff    	;limit 0-15
    dw 0x0       	;base 0-15
    db 0x0       	;base 16-23
    db 10011010b 	;flags
    db 11001111b 	;flags + limit 16-19
    db 0x0       	;base 24-31

GDT_DATA:
    dw 0xffff
    dw 0x0
    db 0x0
    db 10010010b
    db 11001111b
    db 0x0

GDT_END:

GDT_DESC:
    dw GDT_END - GDT - 1
    dd GDT + 0x7e00

; some constants for GDT
CODE_SEG equ GDT_CODE - GDT
DATA_SEG equ GDT_DATA - GDT

[BITS 32]

pmode:
	; load new segments
	mov 	ax, DATA_SEG
	mov 	ds, ax
	mov 	es, ax
	mov 	fs, ax
	mov 	gs, ax
	mov 	ss, ax

	; setup new stack
	mov 	ebp, 0x20000
	mov 	esp, ebp

	; push boot drive for kernel
	; TODO: also push available memory (do it before passing to PM)
	; 		can be done passing the address of structure that has everything-
	mov		al, [BOOT_DRV]
	push	eax

	; disable interrupts, kernel will enable them again 
	; when the IDT will be working
	cli

	; jump to kernel
	jmp		CODE_SEG:0x1000
	jmp		$

pmode_offset equ pmode + 0x7e00

times 0x2000 - ($-$$) db 0x0	;padding
