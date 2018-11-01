
/*
 * paging.h - everything related to paging
 * Copyright (c) 2018 - Leonard Limon, LIC
 */

#ifndef __PAGING__
#define __PAGING__

#include <cpu/types.h>

#include <mem/alloc_bitmap.h>

#define PAGE_SIZE			0x1000
#define RAM_MAXPAGE			0x100000
#define USER_SPACE			0x100000
#define NUM_PAGE_ENTRIES	PAGE_SIZE / 4

void paging_init(uint32_t **kernel_pd);

#endif // __PAGING__ //
