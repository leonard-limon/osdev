
/*
 * paging.h - everything related to paging
 * Copyright (c) 2018 - Leonard Limon, LIC
 */

#include "paging.h"

extern void paging_load(uint32_t pg_dir);

void paging_init(uint32_t **kernel_pd) {

	mem_bitmap_init();

	/* TODO: setup kernel paging so that it covers all
	 * memory in a flat way
	 */

	uint32_t *pd = (uint32_t *)alloc_first_free_frame(0x00);
	uint32_t *pt = (uint32_t *)alloc_first_free_frame(0x00);

	if (kernel_pd) {
		*kernel_pd = pd;
	}

	for (uint32_t i = 0; i < NUM_PAGE_ENTRIES; ++i) {

		pd[i] =  0;
		pt[i] = (i * PAGE_SIZE) | 0x3;

	}

	pd[0] = (uint32_t)pt | 0b011; // 011 = kernel, r/w, present

	// actually enable paging
    paging_load((uint32_t)pd);

}
