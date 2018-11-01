
/*
 * tss.h - task state segment
 * Copyright (c) 2018 - Leonard Limon, LIC
 */

#ifndef __TSS__
#define __TSS__

#include <cpu/types.h>

#include <mem/gdt.h>

typedef struct {

    uint16_t previous_task, __previous_task_unused;
    uint32_t esp0;
    uint16_t ss0, __ss0_unused;
    uint32_t esp1;
    uint16_t ss1, __ss1_unused;
    uint32_t esp2;
    uint16_t ss2, __ss2_unused;
    uint32_t cr3;
    uint32_t eip, eflags, eax, ecx, edx, ebx, esp, ebp, esi, edi;
    uint16_t es, __es_unused;
    uint16_t cs, __cs_unused;
    uint16_t ss, __ss_unused;
    uint16_t ds, __ds_unused;
    uint16_t fs, __fs_unused;
    uint16_t gs, __gs_unused;
    uint16_t ldt_selector, __ldt_sel_unused;
    uint16_t debug_flag, io_map;

} __attribute__ ((packed)) tss_t;

extern void tss_init(void);
extern void tss_load(void);

/* === define tss === */
//#ifndef __TSS__

	tss_t default_tss;

#else

	extern tss_t default_tss;

#endif // __TSS__ //
