/*
 * memory.c - memory related operations
 * Copyright (C) 2018 - Leonard LIMON, LIC
 */


#include "mem.h"

void memcp(const uint8_t *const src, uint8_t *const dest, uint32_t size) {

    for (uint32_t i = 0; i < size; i++) {
		dest[i] = src[i];
    }

}

void memset(uint8_t *dest, uint8_t val, uint8_t size) {

	while(size--) {
		dest[size] = val;
	}

}
