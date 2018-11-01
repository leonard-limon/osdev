
/*
 * alloc_bitmap.c - handles bitmap that alloc memory frames
 * Copyright (c) 2018 - Leonard Limon, LIC
 */

#include "alloc_bitmap.h"

static uint8_t *bitmap = (uint8_t *)BITMAP_BASE;

/* === bitmap handling === */
bool frame_state(uint32_t frame) {
	frame /= PAGE_SIZE;
	return bitmap[frame / 8] & (0x01 << (frame % 8));
}

void alloc_frame(uint32_t frame) {
	frame /= PAGE_SIZE;
	bitmap[frame / 8] |= (0x01 << (frame % 8));
}

void free_frame(uint32_t frame) {
	frame /= PAGE_SIZE;
	bitmap[frame / 8] &= ~(0x01 << (frame % 8));
}

uint32_t first_free_frame(uint32_t from_frame) {

	from_frame /= PAGE_SIZE * 8;
	while (bitmap[from_frame] == 0xff) ++from_frame;

	uint8_t mask = 0x01;
	uint8_t j = 0;

	while (bitmap[from_frame] & mask) {
		mask <<= 1;
		++j;
	}

	return (from_frame * 8 + j) * PAGE_SIZE;

}

uint32_t alloc_first_free_frame(uint32_t from_frame) {

	uint32_t frame = first_free_frame(from_frame);
	alloc_frame(frame);
	return frame;

}

/* === bitmap Initialization === */
void mem_bitmap_init(void) {

	uint32_t pg = 0;

	// init bitmap to zeros
	for (; pg < RAM_MAXPAGE / 8; pg += PAGE_SIZE) {
		bitmap[pg / 8] = 0;
	}

	// kernel reserved pages (kcode, kstack, memory bitmap)
	/* TODO: alloc pages by groups of 8 (faster) */
	for (pg = 0x00; pg < BITMAP_LIM; pg += PAGE_SIZE) {
		alloc_frame(pg);
	}

	// hardware reserved pages
	/* TODO: alloc pages by groups of 8 (faster) */
	for (pg = 0xA0000; pg < 0x100000; pg += PAGE_SIZE) {
		alloc_frame(pg);
	}

}
