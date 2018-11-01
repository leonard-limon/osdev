;
; kern_en.asm - LCOS kernel entry
; Copyright (C) 2018 - Leonard LIMON, LIC
;

[BITS 32]
[global BOOT_DRV]
[extern kmain]

[global _start]
_start:

; fetch boot drive (and other info later)
pop     eax
mov     [BOOT_DRV], al

; re init stack
mov     esp, 0x20000

; goto to c code
call    kmain
jmp     $

BOOT_DRV:
    db 0x0

[global task1]
[extern printh]
task1:
	mov		eax, 0x04
	mov		ebx, 0x12345678
	int		0x80
	jmp		$
