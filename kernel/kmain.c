/*
 * kmain.c - LCOS main kernel function
 * Copyright (C) 2018 - Leonard LIMON, LIC
 */

#include <mem/gdt.h>
#include <mem/paging.h>

#include <cpu/idt.h>
#include <cpu/tss.h>
#include <cpu/timer.h>
#include <cpu/int_handlers.h>

#include <kernel/scheduler.h>

#include <stdlib/stdio.h>

#include <drivers/keyboard.h>

extern void task1(void);

extern void kmain() {

	gdt_init();
	screen_clear();
	print("[ OK ] GDT loaded at ");
	printh((uint32_t)gdt_entries);

	idt_init();
	print("\n[ OK ] IDT loaded at ");
	printh((uint32_t)idt_entries);

	int_handlers_init();
	print("\n[ OK ] Interrupt handlers set\n");

	__asm__("int $0");

	timer_init();
	print("[ OK ] Timer initialized\n");

	kbd_init();
	print("[ OK ] Keyboard initialized\n");

	tss_init();
	print("[ OK ] TSS initialized\n");

	uint32_t *kpd;
	paging_init(&kpd);
	print("[ OK ] Paging initialized\n");

	scheduler_init(kpd);
	print("[ OK ] Scheduler initialized\n");

	load_task(task1, 20);

}
