
/*
 * scheduler.c - everything related to task loading and switching
 * Copyright (c) 2018 - Leonard Limon, LIC
 */

#include "scheduler.h"

static uint32_t num_process;
static process_t *curr;
static process_t proc_list[MAX_PROCESS];

void scheduler_init(uint32_t *ker_pg_dir) { // init kernel_task process

	curr = proc_list;

	curr->pid = 0;
	curr->cr3 = (uint32_t)ker_pg_dir;

	num_process = 1;

}

void scheduler(void) {

	if (num_process < 2) { // only kernel task
		return;
	}

	uint32_t *stack_ptr = int_esp;

	const uint32_t NEXT_PROC_PID = (curr->pid + 1) % num_process;

	// save curr process registers
	curr->regs = *(process_regs_t *)stack_ptr;
	curr->k_stack.esp = stack_ptr;

	// if process is a user process save its stack informations
	if (curr->regs.cs == UCODE_SEG) {

		curr->u_stack.esp	= (uint32_t *)stack_ptr[17];
		curr->u_stack.ss	= stack_ptr[18];

	}

	// switch to next process
	curr = &proc_list[NEXT_PROC_PID];

	// re-enable interrupt after switching back
	curr->regs.eflags = (curr->regs.eflags | 0x200) & 0xffffbfff;

	stack_ptr = curr->k_stack.esp;

	// push process-to-be-executed registers
	*(process_regs_t *)stack_ptr = curr->regs;

	// if new process is in user mode, restore stack informations
	if (curr->regs.cs == UCODE_SEG) {

		stack_ptr[17] = (uint32_t)curr->u_stack.esp;
		stack_ptr[18] = curr->u_stack.ss;

	}

	// if new proccess is in user mode save its kstack pointer
	if (NEXT_PROC_PID != KERNEL_TASK_PID) {
		default_tss.esp0 = (uint32_t)curr->k_stack.ebp;
	}

	// update the page directory and switch
	updt_cr3(curr->cr3);
	switch_task((uint32_t)stack_ptr);

}

void load_task(const uint32_t * const task, const uint32_t size) {

	if (size > PAGE_SIZE) { // not handled yet
		print("Task to be big to be loaded!\n"); return;
	}

	uint32_t * const CODE_PHY_ADDR
		= (uint32_t *)alloc_first_free_frame(USER_SPACE);
	uint32_t * const DATA_PHY_ADDR
		= (uint32_t *)alloc_first_free_frame(USER_SPACE);
	uint32_t * const STACK_PHY_ADDR
		= (uint32_t *)alloc_first_free_frame(USER_SPACE);
	uint32_t * const K_STACK_PHY_ADDR
		= (uint32_t *)alloc_first_free_frame(0x00);

	if (K_STACK_PHY_ADDR >= (uint32_t *)USER_SPACE) { // no more kernel room
		print("Not enough memory to create kernel stack!\n"); return;
	}

	uint32_t * const PD = (uint32_t *)alloc_first_free_frame(0x00);
	uint32_t * const PT = (uint32_t *)alloc_first_free_frame(0x00);

	memcp((uint8_t *)task, (uint8_t *)CODE_PHY_ADDR, size);

	for (int i = 0; i < NUM_PAGE_ENTRIES; ++i) {
		PD[i] = PT[i] = 0;
	}

	PT[0] = (uint32_t)CODE_PHY_ADDR | 0b101;
	PT[2] = (uint32_t)DATA_PHY_ADDR | 0b111;
	PT[1] = (uint32_t)STACK_PHY_ADDR | 0b111;

	PD[0] = ((uint32_t *)(proc_list[KERNEL_TASK_PID].cr3))[0]; // kernel space
	PD[1] = (uint32_t) PT | 0b111; // user space

	__asm__("cli"); // not needed if only the kernel can create processes

 	process_t *new_proc = &proc_list[num_process];

	new_proc->pid			= num_process;
	new_proc->regs.cs 		= UCODE_SEG | 3;
	new_proc->regs.eip		= 0x00400000;
	new_proc->regs.ds		= UDATA_SEG | 3;
	new_proc->regs.es		= UDATA_SEG | 3;
	new_proc->regs.fs		= UDATA_SEG | 3;
	new_proc->regs.gs		= UDATA_SEG | 3;
	new_proc->cr3			= (uint32_t)PD;

	new_proc->u_stack.ss	= USTACK_SEG;
	new_proc->u_stack.esp	= (uint32_t *)0x00402000;

	new_proc->k_stack.ss	= KSTACK_SEG;
	new_proc->k_stack.ebp	= (uint32_t *)((uint32_t) K_STACK_PHY_ADDR + PAGE_SIZE);
	new_proc->k_stack.esp	= (uint32_t *)((uint32_t) K_STACK_PHY_ADDR + PAGE_SIZE);
	new_proc->k_stack.esp	-= 19;	// to compensate from items pushed by interrupt

	num_process++;

	__asm__("sti"); // not needed if only the kernel can create proccess

	print("[ OK ] Task Loaded at ");
	printh((uint32_t)CODE_PHY_ADDR);
	print("\n");

}
