
/*
 * alloc_bitmap.h - handles bitmap that alloc memory frames
 * Copyright (c) 2018 - Leonard Limon, LIC
 */

#ifndef __ALLOC_BITMAP__
#define __ALLOC_BITMAP__

#include <cpu/types.h>

#include <mem/paging.h>

#define BITMAP_BASE 0x40000
#define BITMAP_LIM	0x60000

/* === bitmap initializer === */
void mem_bitmap_init(void);

/* === bitmap handling === */
bool frame_state(uint32_t);
void alloc_frame(uint32_t);
void free_frame(uint32_t);

uint32_t first_free_frame(uint32_t);
uint32_t alloc_first_free_frame(uint32_t);

#endif // __ALLOC_BITMAP__ //
