/*
 * memory.h - memory related operations
 * Copyright (C) 2018 - Leonard LIMON, LIC
 */


#ifndef __MEM__
#define __MEM__

#include <cpu/types.h>

void memcp(const uint8_t *const src, uint8_t *const dest, uint32_t size);
void memset(uint8_t *dest, uint8_t val, uint8_t size);

#endif // __MEM__ //
