
/*
 * syscall.c - system calls
 * Copyright (c) 2018 - Leonard Limon, LIC
 */

#include "syscall.h"

void do_syscall(syscall_regs_t reg) {

	gdt_entry_t *ds = (gdt_entry_t *)(gdt_desc.offset + (reg.ds & 0xf8));
	void *ds_base = (void *)(ds->base_low + (ds->base_high << 24));

	if (reg.eax == 1) { // putc *
		putc(*(char *)(ds_base + reg.ebx));
	}

	else if (reg.eax == 2) { // print
		CLI;
		print(ds_base + reg.ebx);
		STI;
	}

	else if (reg.eax == 3) { // putc
		putc(reg.ebx);
	}

	else if (reg.eax == 4) { // print_hex
		CLI;
		printh(reg.ebx);
		STI;
	}

	else if (reg.eax == 5) { // getc
		getc();
	}

	else {
		CLI;
		print("SYSCALL #");
		printi(reg.eax);
		print("\n");
		STI;
	}

	return;

}
