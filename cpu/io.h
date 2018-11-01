/*
 * io.asm - cpu ports input output routines defintion
 * Copyright (C) 2018 - Leonard LIMON, LIC
 */

#ifndef __IO__
#define __IO__

#include <cpu/types.h>

extern uint8_t inb(uint16_t pt);
extern uint16_t inw(uint16_t pt);

extern void outb(uint16_t pt, uint8_t data);
extern void outw(uint16_t pt, uint16_t data);

#endif // __IO__ //
