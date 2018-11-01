/*
 * gdt.h - everything related to global descriptor table
 * Copyright (C) 2018 - Leonard LIMON, LIC
 */

#ifndef __GDT__
#define __GDT__

#include <cpu/types.h>

#include <stdlib/mem.h>

/* === Segments definition === */

#define GDT_SIZE 	6

#define KCODE_SEG   0x08
#define KDATA_SEG   0x10
#define KSTACK_SEG  0x10

#define UCODE_SEG   0x1b
#define UDATA_SEG   0x23
#define USTACK_SEG  0x23

/* === GDT Descriptors and Descriptor === */

typedef struct {

	uint32_t limit_low: 	16;
	uint32_t base_low: 		24;

    // access byte
	uint32_t type:			5;	// use macros

#define SEG_TYPE_ACC	0b00001	// accessed

#define SEG_TYPE_DATA	0b10000	// data
#define SEG_TYPE_CODE	0b11000	// code

#define SEG_TYPE_RD		0b10000	// read only
#define SEG_TYPE_RW		0b10010	// read write
#define SEG_TYPE_EX		0b10000	// execute only
#define SEG_TYPE_ER		0b10010	// execute read

#define SEG_TYPE_EXP_UP	0b10000	// expand up
#define SEG_TYPE_EXP_DN	0b10100	// expand down
#define SEG_TYPE_CONF	0b10100	// conforming

#define SEG_TYPE_16_TSS	0b00001	// 16-bit TSS
#define SEG_TYPE_32_TSS	0b01001	// 32-bit available TSS

	uint32_t DPL: 			2; 	// privilege level (ring)
	uint32_t present: 		1; 	// present in memory

	// granularity byte
	uint32_t limit_high: 	4;
	uint32_t available: 	1;
	uint32_t _64_bit_seg: 	1; 	// indicate 64 bit op size

#define SEG_OP_SZ_16	0
#define	SEG_OP_SZ_32	1

	uint32_t op_size: 		1; 	// operation size

#define SEG_GRAN_BYTE	0
#define SEG_GRAN_4_K	1

	uint32_t gran: 			1; 	// granularity

	uint32_t base_high: 	8;

} __attribute__((packed)) gdt_entry_t;

typedef struct {

	uint16_t size;
	uint32_t offset;

} __attribute__((packed)) gdt_desc_t;

/* === Init function === */

extern void gdt_init(void);

/* === definition of GDT and its descriptor === */

// #ifndef __GDT__

	gdt_desc_t	gdt_desc;
	gdt_entry_t	gdt_entries[GDT_SIZE];

#else // __GDT__ //

	extern gdt_desc_t	gdt_desc;
	extern gdt_entry_t	gdt_entries[GDT_SIZE];

#endif // __GDT__ //
