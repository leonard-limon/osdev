
/*
 * scheduler.h - everything related to task loading and switching
 * Copyright (c) 2018 - Leonard Limon, LIC
 */

#ifndef __SCHEDULER__
#define __SCHEDULER__

#include <cpu/tss.h>
#include <cpu/types.h>

#include <mem/paging.h>
#include <mem/alloc_bitmap.h>

#include <stdlib/mem.h>
#include <stdlib/stdio.h>

#define MAX_PROCESS		32
#define KERNEL_TASK_PID	0
#define USER_SPACE		0x100000

typedef struct {

	uint32_t gs, fs, es, ds;
	uint32_t edi, esi, ebp, esp__;
	uint32_t ebx, edx, ecx, eax;
	uint32_t _errc2, _errc1;
	uint32_t eip, cs, eflags;

} __attribute__((packed)) process_regs_t;

typedef struct {

	uint32_t pid;
	uint32_t cr3;
	process_regs_t regs;

	struct {

		uint32_t ss;
		uint32_t *ebp, *esp;

	} k_stack __attribute__((packed));

	struct {

		uint32_t ss;
		uint32_t *esp;

	} u_stack __attribute__((packed));

} __attribute__((packed)) process_t;

extern void scheduler(void);
extern void scheduler_init(uint32_t *ker_pg_dir);

extern void updt_cr3(uint32_t);
extern void switch_task(uint32_t stack_ptr);

extern uint32_t int_esp;

/* === task management === */
void load_task(const uint32_t * const, const uint32_t);

#endif // __SCHEDULER__ //
