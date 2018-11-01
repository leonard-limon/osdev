/*
 * gdt.c - everything related to global descriptor table
 * Copyright (C) 2018 - Leonard LIMON, LIC
 */

#include "gdt.h"

extern void gdt_load(void);

/* === routine that inits and loads gdt === */
void gdt_init(void) {

	// init to default
	memset((uint8_t *)gdt_entries, 0x0, sizeof(gdt_entries));

	for (uint8_t i = 1; i <= 4; ++i) {

		gdt_entries[i].limit_low	= 0xffff;
		gdt_entries[i].limit_high	= 0xf;
		gdt_entries[i].present		= YES;
		gdt_entries[i].available	= YES;
		gdt_entries[i].gran			= SEG_GRAN_4_K;
		gdt_entries[i].op_size		= SEG_OP_SZ_32;

	}

	// KCODE
	gdt_entries[1].type	= SEG_TYPE_CODE | SEG_TYPE_ER | SEG_TYPE_EXP_UP;
	gdt_entries[1].DPL	= 0;

	// KDATA
	gdt_entries[2].type	= SEG_TYPE_DATA | SEG_TYPE_RW | SEG_TYPE_EXP_UP;
	gdt_entries[2].DPL	= 0;

	// UCODE
	gdt_entries[3].type	= SEG_TYPE_CODE | SEG_TYPE_ER | SEG_TYPE_EXP_UP;
	gdt_entries[3].DPL	= 3;

	// UDATA
	gdt_entries[4].type	= SEG_TYPE_DATA | SEG_TYPE_RW | SEG_TYPE_EXP_UP;
	gdt_entries[4].DPL	= 3;

	// init gdt descriptor
	gdt_desc.size	= sizeof(gdt_entries) - 1;
	gdt_desc.offset	= (uint32_t)gdt_entries;

	// load new gdt in asm
	gdt_load();

}
