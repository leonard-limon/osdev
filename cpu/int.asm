;
; interrupt.asm - low level interrupt routines
; Copyright (c) 2018 - Leonard Limon, LIC
;

[bits 32]

[GLOBAL idt_load]
[GLOBAL get_faulty_pg]
[GLOBAL _syscall]

[extern do_syscall]
[extern isr_handler]
[extern irq_handler]

[extern idt_ptr]

get_faulty_pg:
	mov eax, cr2
	ret

idt_load:
	mov eax, idt_ptr
	lidt [eax]

	sti						; enable interrupts again
	ret


; Common ISR code (for exceptions)
isr_common_stub:
    ; 1. Save CPU state
	pushad
	push ds
	push es
	push fs
	push gs

	mov ax, 0x10  	;kernel data segment descriptor
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

    ; 2. Call C handler
	call isr_handler

    ; 3. Restore state

	pop gs
	pop fs
	pop es
	pop ds
	popad

	add esp, 8 	;Clean up the pushed error code and pushed ISR number

	sti			;enable interruption again
	iretd		;pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP

irq_common_stub: ; for hardware interrupts
	pushad
	push	ds
	push	es
	push	fs
	push	gs

	mov		ax, 0x10
	mov		ds, ax
	mov		es, ax
	mov		fs, ax
	mov		gs, ax

	; used by scheduler
	mov		[int_esp], esp

	call	irq_handler

	pop		gs
	pop		fs
	pop		es
	pop		ds
	popad

	add		esp, 8
	sti
	iretd

[global int_esp]
int_esp:
db 0, 0, 0, 0

%macro ISR_NOERRCODE 1
	[GLOBAL isr%1]
  	isr%1:
    	cli					;# disable interrupts
    	push byte 0			;# push dummy error code
    	push byte %1		;# push interrupt number
    	jmp isr_common_stub ;# go to common handler
%endmacro

%macro ISR_ERRCODE 1
  	[GLOBAL isr%1]
  	isr%1:
    	cli
    	push byte %1
    	jmp isr_common_stub
%endmacro

%macro IRQ 2
	[GLOBAL irq%1]
	irq%1:
		cli
		push byte 0
		push byte %2
		jmp irq_common_stub
%endmacro

ISR_NOERRCODE	00
ISR_NOERRCODE	01
ISR_NOERRCODE	02
ISR_NOERRCODE	03
ISR_NOERRCODE	04
ISR_NOERRCODE	05
ISR_NOERRCODE	06
ISR_NOERRCODE	07
ISR_ERRCODE		08
ISR_NOERRCODE	09
ISR_ERRCODE		10
ISR_ERRCODE		11
ISR_ERRCODE		12
ISR_ERRCODE		13
ISR_ERRCODE		14
ISR_NOERRCODE	15
ISR_NOERRCODE	16
ISR_NOERRCODE	17
ISR_NOERRCODE	18
ISR_NOERRCODE	19
ISR_NOERRCODE	20
ISR_NOERRCODE	21
ISR_NOERRCODE	22
ISR_NOERRCODE	23
ISR_NOERRCODE	24
ISR_NOERRCODE	25
ISR_NOERRCODE	26
ISR_NOERRCODE	27
ISR_NOERRCODE	28
ISR_NOERRCODE	29
ISR_NOERRCODE	30
ISR_NOERRCODE	31

IRQ 00, 32
IRQ 01, 33
IRQ 02, 34
IRQ 03, 35
IRQ 04, 36
IRQ 05, 37
IRQ 06, 38
IRQ 07, 39
IRQ 08, 40
IRQ 09, 41
IRQ 10, 42
IRQ 11, 43
IRQ 12, 44
IRQ 13, 45
IRQ 14, 46
IRQ 15, 47

_syscall:

	push 0x80 	;push dummy error code
	push 0x80 	;idem

	pushad

	push ds
	push es
	push fs
	push gs

	mov bx, 0x10
	mov ds, bx
	mov es, bx
	mov fs, bx
	mov gs, bx

	call do_syscall

	pop gs
	pop fs
	pop es
	pop ds

	popad

	add esp, 0x8

	iretd
