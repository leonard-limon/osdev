/*
 * int_handlers.c - LCOS interrupts handlers
 * Copyright (C) 2018 - Leonard LIMON, LIC
 */

#include "int_handlers.h"

/* === processor interupt === */
extern void isr_handler(registers_t r) {

	// print exception informations
	/* TODO: At the moment, printing informations is done for debuging purposes
	 * but later this step could be skipped and the handler could directly pass
	 * control to the appropriate handler (this impliese that all handlers are
	 * defined)
	 */
	set_cattr(YELLOW_FORGND);
    print("\nreceived interrupt ");
	printh(r.int_no);
    print(": ");
    print(exception_messages[r.int_no]);
    print("\n");

    // if interrupt handler is set, run it
    if (interrupt_handlers[r.int_no]) {
        interrupt_handlers[r.int_no](r);
	}

	// halt the system
	/* TODO: later on define "faults" and "abort" so that some interruption
	 * do not halt the system (division by zero for example) and so that the
	 * kernel can handle these interruptions
	 */
	while (1);

}

/* === hardware interupt === */
extern void irq_handler(registers_t r) {

	// if interrupt was raised by slave PIC send EOI to slave
	if (r.int_no >= 40) {
        outb(0xa0, 0x20);
	}

	// send EOI to master
	outb(0x20, 0x20);

	// if interrupt handler exists, run it
	if (interrupt_handlers[r.int_no]) {
        interrupt_handlers[r.int_no](r);
	}

}

/* === maps functions that handle the interrupt === */
extern void int_handlers_init(void) {

    interrupt_handlers[0x00] = DE;
	interrupt_handlers[0x06] = UD;
	interrupt_handlers[0x08] = DF;
    interrupt_handlers[0x0D] = GP;
	interrupt_handlers[0x0E] = PF;

}

/* === interupt handlers === */
static void DE(registers_t r) {

    set_cattr(BLUE_BACKGND | WHITE_FORGND);
	print("\nDIVISION BY ZERO, SYSTEM HALTED\n");
	while(1);

}

static void DF(registers_t r) {

	set_cattr(BLUE_BACKGND | WHITE_FORGND);
	print("\nDOUBLE FAULT, SYSTEM HALTED\n");
	__asm__("hlt");

}

static void GP(registers_t r) {

    set_cattr(BLUE_BACKGND | WHITE_FORGND);
    print("/!\\ GENERAL PROTECTION FAULT, SYSTEM HALTED /!\\ err code = ");
	printh(r.err_code);
	print("     eip = ");
	printh(r.eip);
	print("     cs = ");
	printh(r.cs);
	print("		sp = ");
	printh(r.esp);

    __asm__("hlt");
    while(1);

}

static void PF(registers_t r) {

	uint32_t faulty_pg = get_faulty_pg();
	print("\n\nPage fault, system halted\nFaulty page: ");
	printh(faulty_pg);
	print(" err code = ");
	printh(r.err_code);

	__asm__("cli\nhlt");
	while (1);
}

static void UD(registers_t r) {

	set_cattr(BLUE_BACKGND | WHITE_FORGND);
	print("/!\\ INVALID OPCODE, SYSTEME HALTED /!\\ at addr = ");
	printh(r.eip);

	__asm__("cli\nhlt");
	while(1);

}
