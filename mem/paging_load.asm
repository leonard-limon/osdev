
;
; paging_load.asm - function to load paging cr3
; Copyright (c) 2018 - Leonard Limon, LIC
;

[bits 32]

[global paging_load]
[extern print_ok]

paging_load:
	push	eax

	; eax has the address of the page directory
	mov		eax, [esp + 8] 	; get pointer passed by stack
	mov		cr3, eax

	;mov ebx, cr4        ; read current cr4
	;or  ebx, 0x00000010 ; set PSE (pages of 4MB)
	;mov cr4, ebx        ; update cr4

	mov		ebx, cr0        ; read current cr0
	or		ebx, 0x80000000 ; set PG
	mov		cr0, ebx        ; update cr0

	pop		eax

	ret
