
/*
 * tss.h - task state segment
 * Copyright (c) 2018 - Leonard Limon, LIC
 */

#include "tss.h"

void tss_init(void) {

    // init the TSS to zeros
    memset((uint8_t *)&default_tss, 0, sizeof(default_tss));

    __asm__("  movw %%ss, %0 \n \
       movl %%esp, %1" : "=m" (default_tss.ss0), "=m" (default_tss.esp0) ::);

    // entry 5 = 0x38
	gdt_entries[5].base_low		= 0xffffff & (uint32_t)&default_tss;
    gdt_entries[5].base_high	= 0xf & (((uint32_t)&default_tss) >> 24);
    gdt_entries[5].limit_low	= 0x67 & 0xffff;
    gdt_entries[5].limit_high	= 0xff & (0x67 >> 16);

	gdt_entries[5].type			= SEG_TYPE_32_TSS;
	gdt_entries[5].DPL			= 0;
	gdt_entries[5].present		= YES;
	gdt_entries[5].available	= YES;
	gdt_entries[5].gran			= SEG_GRAN_BYTE;

    // load TSS register
    tss_load();

}
