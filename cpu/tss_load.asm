
;
; tss_load.asm - functions to load TSS
; Copyright (c) 2018 - Leonard Limon, LIC
;

[bits 32]

[global tss_load]

tss_load:
	cli
    mov		eax, 0x28
    ltr		ax
	sti
    ret
