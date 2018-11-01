/*
 * screen.c - screen driver
 * Copyright (C) 2018 - Leonard LIMON, LIC
 */

#include "screen.h"

static char		*tptr = (char *)VIDEO_MEM;
static uint8_t	cattr = BLACK_BACKGND | LIGHT_GRAY_FORGND;

void screen_clear(void) {

	tptr = (char *)VIDEO_MEM_LIM;

	do {
		*(--tptr) = BLACK_BACKGND | LIGHT_GRAY_FORGND;
		*(--tptr) = 0;
	} while (tptr > (char *)VIDEO_MEM);

	cattr = BLACK_BACKGND | LIGHT_GRAY_FORGND;

}

void screen_setc(uint16_t pos, char c, uint8_t attr) {

	((char *)VIDEO_MEM)[2*pos] 		= c;
	((char *)VIDEO_MEM)[2*pos + 1] 	= attr;

}

void screen_scroll(void) {

	char *ptr = (char *)VIDEO_MEM;
	char *tmp = ptr + 2*MAX_COL;

	while (tmp < (char *)VIDEO_MEM_LIM) {
		*ptr++ = *tmp++;
	}

	while (ptr < (char *)VIDEO_MEM_LIM) {
		*ptr++ = 0;
		*ptr++ = LIGHT_GRAY_FORGND | BLACK_BACKGND;
	}

	tptr -= 2*MAX_COL;

}

void screen_putc(char c) {

	// TODO: handle special characters like
	// CR, LN, Backspace, Delete, TAB

    if (c == 8) { // backspace

        if ((tptr -= 2) < (char *)VIDEO_MEM) {
            tptr = (char *)VIDEO_MEM;
        }

        tptr[0] = 0;
        tptr[1] = LIGHT_GRAY_FORGND | BLACK_BACKGND;

    }

	else if (c == 9) { // Tab
		tptr += 8 - ((uint32_t)tptr - VIDEO_MEM) % 8;
	}

	else if (c == 10) { // LF
		tptr += (2*MAX_COL) - ((uint32_t)tptr - VIDEO_MEM) % (2*MAX_COL);
	}

	else if (c == 13) { // CR
		tptr -= ((uint32_t)tptr - VIDEO_MEM) % (2*MAX_COL);
	}

	// only print printable characters
	else if (c >= 32 && c < 127) {
		*(tptr++) = c;
		*(tptr++) = cattr;
	}

    else if (c == 127) { // backspace

        if ((tptr -= 2) < (char *)VIDEO_MEM) {
            tptr = (char *)VIDEO_MEM;
        }

        tptr[0] = 0;
        tptr[1] = LIGHT_GRAY_FORGND | BLACK_BACKGND;

    }

	// text reached end of screen
	if (tptr >= (char *)VIDEO_MEM_LIM) {
		screen_scroll();
	}

}

void screen_movc(uint16_t pos) {

	tptr = (char *)VIDEO_MEM + 2*pos;

	if (tptr >= (char *)VIDEO_MEM_LIM) {
		tptr = (char *)VIDEO_MEM;
	}

}

void screen_set_attr(uint8_t attr) {

	cattr = attr;

}

uint16_t curr_cpos(void) {

	return (uint32_t)tptr - VIDEO_MEM;

}

void screen_move_curs(void) {
	//screen_setc(0, 'S', BLUE_BACKGND | GREEN_FORGND);
	uint16_t pos = ((uint32_t)tptr - VIDEO_MEM) / 2;

	outb(SCREEN_CTRL_PORT, 0x0f);
	outb(SCREEN_DATA_PORT, (uint8_t)pos);
	outb(SCREEN_CTRL_PORT, 0x0e);
	outb(SCREEN_DATA_PORT, (uint8_t)(pos >> 8));

}

void screen_hide_curs(void) {

	outb(SCREEN_CTRL_PORT, 0x0f);
	outb(SCREEN_DATA_PORT, 0xff);
	outb(SCREEN_CTRL_PORT, 0x0e);
	outb(SCREEN_DATA_PORT, 0xff);

}
