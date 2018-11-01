/*
 * idt.h - Interrupt Descriptor Table
 * Copyright (C) 2018 - Leonard LIMON, LIC
 */

#ifndef __IDT__
#define __IDT__

#include <cpu/io.h>
#include <cpu/types.h>

#include <mem/gdt.h>

#define NUM_IDT_ENTRIES 256

/* === Interrupt gate and IDT Descriptor types definition === */

typedef struct {

    uint16_t low_offset:	16;
    uint16_t sel:			16;	// GDT selector
	uint16_t res:			8;	// leave it to 0

#define GATE_FLAG_INT	0b110
#define GATE_FLAG_TRAP	0b111

    uint16_t flags:			3;

#define GATE_SIZE_16 0
#define GATE_SIZE_32 1

    uint16_t gate_sz:		1;
    uint16_t zero:			1;	// must be 0
    uint16_t DPL:			2;
    uint16_t present:		1;
    uint16_t high_offset:	16;

} __attribute__((packed)) idt_entry_t;

typedef struct {

    uint16_t limit;
    uint32_t base;

} __attribute__((packed)) idt_desc_t;

/* === Assembly interrupt routines === */

// isr = exception (e.g. divison by zero)
extern void isr00(void);
extern void isr01(void);
extern void isr02(void);
extern void isr03(void);
extern void isr04(void);
extern void isr05(void);
extern void isr06(void);
extern void isr07(void);
extern void isr08(void);
extern void isr09(void);
extern void isr10(void);
extern void isr11(void);
extern void isr12(void);
extern void isr13(void);
extern void isr14(void);
extern void isr15(void);
extern void isr16(void);
extern void isr17(void);
extern void isr18(void);
extern void isr19(void);
extern void isr20(void);
extern void isr21(void);
extern void isr22(void);
extern void isr23(void);
extern void isr24(void);
extern void isr25(void);
extern void isr26(void);
extern void isr27(void);
extern void isr28(void);
extern void isr29(void);
extern void isr30(void);
extern void isr31(void);

// irq -> hardware interupt
extern void irq00(void);
extern void irq01(void);
extern void irq02(void);
extern void irq03(void);
extern void irq04(void);
extern void irq05(void);
extern void irq06(void);
extern void irq07(void);
extern void irq08(void);
extern void irq09(void);
extern void irq10(void);
extern void irq11(void);
extern void irq12(void);
extern void irq13(void);
extern void irq14(void);
extern void irq15(void);

// system calls
extern void _syscall(void);

/* === IDT handling routines === */

extern void idt_load(void);
extern void idt_init(void);

static void idt_entry_set_offset(uint32_t ent_idx, uint32_t *);

/* === IDT definition === */

// #ifndef __IDT__

	idt_desc_t	idt_ptr;
	idt_entry_t	idt_entries[NUM_IDT_ENTRIES];

#else

	extern idt_desc_t	idt_ptr;
	extern idt_entry_t	idt_entries[NUM_IDT_ENTRIES];

#endif // __IDT__ //
