/*
 * stdout.c - input output routines
 * Copyright (C) 2018 - Leonard LIMON, LIC
 */

#include "stdio.h"

/* TODO: declare variable at a more apropriate location */
/* FIXME: Bugs with this variable */
uint8_t curs_follows_print = YES;

void print(char const* str) {

	while (*str) {
		screen_putc(*str++);
	}

	// move cursors if asked to
	if (curs_follows_print) {
		screen_move_curs();
	}

}

void putc(char c) {

	screen_putc(c);

	// move cursors if asked to
	if (curs_follows_print) {
		screen_move_curs();
	}

}

void printi(int n) {

	char str[11];
	int_to_str(n, str);
	print(str);

}

void printh(uint32_t n) {

	char str[11];
	hex_to_str(n, str);
	print(str);

}

void set_cattr(uint8_t attr) {

	screen_set_attr(attr);

}

char getc(void) {

	return 0;
    //return kbd_getc();

}
