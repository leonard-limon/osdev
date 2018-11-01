
;
; scheduler.asm - low level scheduler functions
; Copyright (c) 2018 - Leonard Limon, LIC
;

[BITS 32]

[GLOBAL updt_cr3]
[GLOBAL switch_task]

updt_cr3:
	mov eax, [esp + 4]
	mov cr3, eax
	ret

switch_task:
	mov eax, [esp + 4]
	mov esp, eax

	pop gs
	pop fs
	pop es
	pop ds

	popad

	add esp, 8

    iretd
