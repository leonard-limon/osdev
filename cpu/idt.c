/*
 * idt.c - Interrupt Descriptor Table
 * Copyright (C) 2018 - Leonard LIMON, LIC
 */

#include "idt.h"

extern void idt_init(void) {

	// remap irq table in PIC
	outb(0x20, 0x11);
  	outb(0xA0, 0x11);
  	outb(0x21, 0x20);
  	outb(0xA1, 0x28);
  	outb(0x21, 0x04);
  	outb(0xA1, 0x02);
  	outb(0x21, 0x01);
  	outb(0xA1, 0x01);
  	outb(0x21, 0x00);
  	outb(0xA1, 0x00);

	for (uint32_t i = 0; i < 0x30; ++i) {

		idt_entries[i].sel		= KCODE_SEG;
		idt_entries[i].res		= 0;
		idt_entries[i].flags	= GATE_FLAG_INT;
		idt_entries[i].gate_sz	= GATE_SIZE_32;
		idt_entries[i].zero		= 0;
		idt_entries[i].DPL		= 0;
		idt_entries[i].present	= YES;

	}

	// isr
	idt_entry_set_offset(0x00, (uint32_t *)isr00);
	idt_entry_set_offset(0x01, (uint32_t *)isr01);
	idt_entry_set_offset(0x02, (uint32_t *)isr02);
	idt_entry_set_offset(0x03, (uint32_t *)isr03);
	idt_entry_set_offset(0x04, (uint32_t *)isr04);
	idt_entry_set_offset(0x05, (uint32_t *)isr05);
	idt_entry_set_offset(0x06, (uint32_t *)isr06);
	idt_entry_set_offset(0x07, (uint32_t *)isr07);
	idt_entry_set_offset(0x08, (uint32_t *)isr08);
	idt_entry_set_offset(0x09, (uint32_t *)isr09);
	idt_entry_set_offset(0x0a, (uint32_t *)isr10);
	idt_entry_set_offset(0x0b, (uint32_t *)isr11);
	idt_entry_set_offset(0x0c, (uint32_t *)isr12);
	idt_entry_set_offset(0x0d, (uint32_t *)isr13);
	idt_entry_set_offset(0x0e, (uint32_t *)isr14);
	idt_entry_set_offset(0x0f, (uint32_t *)isr15);
	idt_entry_set_offset(0x10, (uint32_t *)isr16);
	idt_entry_set_offset(0x11, (uint32_t *)isr17);
	idt_entry_set_offset(0x12, (uint32_t *)isr18);
	idt_entry_set_offset(0x13, (uint32_t *)isr19);
	idt_entry_set_offset(0x14, (uint32_t *)isr20);
	idt_entry_set_offset(0x15, (uint32_t *)isr21);
	idt_entry_set_offset(0x16, (uint32_t *)isr22);
	idt_entry_set_offset(0x17, (uint32_t *)isr23);
	idt_entry_set_offset(0x18, (uint32_t *)isr24);
	idt_entry_set_offset(0x19, (uint32_t *)isr25);
	idt_entry_set_offset(0x1a, (uint32_t *)isr26);
	idt_entry_set_offset(0x1b, (uint32_t *)isr27);
	idt_entry_set_offset(0x1c, (uint32_t *)isr28);
	idt_entry_set_offset(0x1d, (uint32_t *)isr29);
	idt_entry_set_offset(0x1e, (uint32_t *)isr30);
	idt_entry_set_offset(0x1f, (uint32_t *)isr31);

	// irq
	idt_entry_set_offset(0x20, (uint32_t *)irq00);
	idt_entry_set_offset(0x21, (uint32_t *)irq01);
	idt_entry_set_offset(0x22, (uint32_t *)irq02);
	idt_entry_set_offset(0x23, (uint32_t *)irq03);
	idt_entry_set_offset(0x24, (uint32_t *)irq04);
	idt_entry_set_offset(0x25, (uint32_t *)irq05);
	idt_entry_set_offset(0x26, (uint32_t *)irq06);
	idt_entry_set_offset(0x27, (uint32_t *)irq07);
	idt_entry_set_offset(0x28, (uint32_t *)irq08);
	idt_entry_set_offset(0x29, (uint32_t *)irq09);
	idt_entry_set_offset(0x2a, (uint32_t *)irq10);
	idt_entry_set_offset(0x2b, (uint32_t *)irq11);
	idt_entry_set_offset(0x2c, (uint32_t *)irq12);
	idt_entry_set_offset(0x2d, (uint32_t *)irq13);
	idt_entry_set_offset(0x2e, (uint32_t *)irq14);
	idt_entry_set_offset(0x2f, (uint32_t *)irq15);

	// syscall
	idt_entry_set_offset(0x80, (uint32_t *)_syscall);

	idt_entries[0x80].sel		= KCODE_SEG;
	idt_entries[0x80].res		= 0;
	idt_entries[0x80].flags		= GATE_FLAG_TRAP;
	idt_entries[0x80].gate_sz	= GATE_SIZE_32;
	idt_entries[0x80].zero		= 0;
	idt_entries[0x80].DPL		= 3;
	idt_entries[0x80].present	= YES;

	// init IDT Descriptor
	idt_ptr.base	= (uint32_t)&idt_entries;
	idt_ptr.limit	= sizeof(idt_entries) - 1;

	// actually load the idt
    idt_load();

}

static void idt_entry_set_offset(uint32_t ent_idx, uint32_t *offset) {

	idt_entries[ent_idx].low_offset		= (uint32_t)offset & 0xffff;
	idt_entries[ent_idx].high_offset	= ((uint32_t)offset >> 16) & 0xffff;

}
