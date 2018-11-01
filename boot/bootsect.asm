;
; bootsect.asm - the bootsector of an LCOS formated disk
; Copyright (C) 2018 - Leonard LIMON, LIC
;

[BITS 16]
[ORG 0x0]

jmp	0x7c0:main

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

main:
	cli

	; setup segments
	mov		ax, 0x7c0
	mov		ds,	ax
	mov		es, ax
	mov		fs, ax
	mov		gs, ax

	; setup stack
	xor		ax, ax
	mov		sp, 0xffff
	mov		ss, ax

	; save boot drive
	mov		[BOOT_DRV], dl

	sti

	; check if part 1 is active
	cmp		byte [PART_1], 0x80
	jne		no_bootldr

	; load second stage bootloader
	mov		dh, [PART_1+1]	;head
	mov		cl, [PART_1+2]	;sect
	mov		ch, [PART_1+3]	;cylinder

	mov		ax, 0x7e0		;buffer seg and offset
	mov		es, ax
	xor		bx, bx

	mov		ah, 0x2			;func 2 (read) of int 13h
	mov		al, [PART_1+6]
	sub		al, cl			;sect count
	push	ax

	int		0x13

	;check disk read
	jc		diskerr
	pop		cx
	cmp		al, cl			;check #sect read
	jne		diskerr

	;successful read
	mov		dl, [BOOT_DRV]
	push	dx				;push boot drive
	jmp		0x7e0:0x0
	jmp		$

no_bootldr:
	mov		si, NO_BOOTLDR_MSG
	call	print
	jmp		fatal

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

BOOT_DRV:		db 0x0
DISK_ERR_MSG:	db "DISK READ ERROR: ", 0
FATAL_ERR_MSG:	
	db 10, 13, "FATAL: Could not load LCOS bootloader, press a key to reboot...", 0
NO_BOOTLDR_MSG:	db "*** Could not find LCOS bootloader", 10, 13, 0

times 0x1b8 - ($-$$) nop

SIG: 	dd 0x00
NULL: 	dw 0x00
PART_TBL:
PART_1:
	db 	0x80			;status
	db	0x00			;head
	db	0x11			;sect
	db	0x00			;cylinder
	db	0x83			;part type
	db	0x00			;end head
	db	0x21			;end sect
	db	0x00			;end cylinder
	dd	0x10			;first LBA sect
	dd 	0x10			;sect count

PART_2:
	db 	0x00			;status
	db	0x00			;head
	db	0x21			;sect
	db	0x00			;cylinder
	db	0x83			;part type
	db	0xff			;end head
	db	0xff			;end sect
	db	0xff			;end cylinder
	dd	0x20			;first LBA sect
	dd 	0xffffffff		;sect count

PART_3:
	times 0x10 db 0x00

PART_4:
	times 0x10 db 0x00

BOOT_SIG: db 0x55, 0xaa

times 0x2000 - ($-$$) db 0x0	;padding
