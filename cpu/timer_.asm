;
; timer_.asm - timer routines
; Copyright (c) 2018 - Leonard Limon, LIC
;

[bits 32]

[global tick]
[global timer_sleep]

; FIXME: fix the sleep routine
timer_sleep:

	push ebp
	mov ebp, esp

	mov ebx, [esp + 4]


	mov esp, ebp
	pop ebp
	ret

tick:
	dd 0, 0, 0, 0
