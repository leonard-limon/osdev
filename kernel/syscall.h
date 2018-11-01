
/*
 * syscall.h - system calls
 * Copyright (c) 2018 - Leonard Limon, LIC
 */

#ifndef __SYSCALL__
#define __SYSCALL__

#include <cpu/types.h>
#include <cpu/int_handlers.h>

#include <mem/gdt.h>

#include <stdlib/mem.h>
#include <stdlib/stdio.h>
#include <stdlib/strings.h>

#define CLI __asm__("cli");
#define STI __asm__("sti");

typedef struct {

   uint32_t gs, fs, es, ds; 							/* Data segment selector */
   uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; 	/* Pushed by pusha. */
   uint32_t __unused1, __unused2;
   uint32_t eip, cs, eflags, useresp, ss; 			   /* Pushed by the processor automatically */

} __attribute__((packed)) syscall_regs_t;

typedef struct {

	uint32_t   cnt;
	void       *args;

} __attribute__((packed)) syscall_args_t;

void do_syscall(syscall_regs_t reg);

#endif // __SYSCALL__ //
