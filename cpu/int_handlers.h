/*
 * int_handlers.h - LCOS interrupts handlers
 * Copyright (C) 2018 - Leonard LIMON, LIC
 */

#ifndef __INT_HANDLERS__
#define __INT_HANDLERS__

#include <cpu/idt.h>
#include <cpu/types.h>

#include <stdlib/stdio.h>

/* === structure that is pushed by processor and interupt common stub === */
typedef struct {

	// Data segment selectors
	uint32_t gs, fs, es, ds;

	// Pushed by pusha
	uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;

	// Interrupt number and error code (if applicable)
	uint32_t int_no, err_code;

	// Pushed by the processor automatically
	uint32_t eip, cs, eflags, useresp, ss;

} __attribute__((packed)) registers_t;

// generic handler typedef
typedef void (*handler_t)(registers_t);

/* === initialization routines === */
extern void int_handlers_init(void);

/* === used by page fault handling === */
extern uint32_t get_faulty_pg();

/* === definition of handlers === */
static void DE(registers_t); // Division by zero
static void DF(registers_t); // Double fault
static void GP(registers_t); // General protection fault
static void PF(registers_t); // Page fault
static void UD(registers_t); // Invalid Opcode

/* === defintion of error messages === */
static char *exception_messages[] = {

	"Division By Zero",
	"Debug",
	"Non Maskable Interrupt",
	"Breakpoint",
	"Into Detected Overflow",
	"Out of Bounds",
	"Invalid Opcode",
	"No Coprocessor",

	"Double Fault",
	"Coprocessor Segment Overrun",
	"Bad TSS",
	"Segment Not Present",
	"Stack Fault",
	"General Protection Fault",
	"Page Fault",
	"Unknown Interrupt",

	"Coprocessor Fault",
	"Alignment Check",
	"Machine Check",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",

	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved"

};

/* === interrupt handlers list === */
//#ifndef __INT_HANDLERS__

	handler_t interrupt_handlers[NUM_IDT_ENTRIES];

#else

	extern handler_t interrupt_handlers[NUM_IDT_ENTRIES];

#endif // __INT_HANDLERS__ //
