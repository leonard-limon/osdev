/*
 * stdout.h - input output routines
 * Copyright (C) 2018 - Leonard LIMON, LIC
 */

#ifndef __STDIO__
#define __STDIO__

#include <drivers/screen.h>
//#include <drivers/keyboard.h>

#include <stdlib/strings.h>

/* === Output ==== */
void putc(char);

void print(char const*);
void printh(uint32_t);
void printi(int);

/* TODO: add a print function that accepts %d, %h and multiple parameters
 * (like printf)
 */

void set_cattr(uint8_t);

/* === Input === */
char getc(void);

#endif // __STDIO__ //
