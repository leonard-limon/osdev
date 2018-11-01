
/*
 * keyboard.c - drivers for keyboard
 * Copyright (c) 2018 - Leonard Limon, LIC
 */

#include "keyboard.h"

static bool print_kbd;
static volatile char last_char;
static volatile bool waiting_for_char;

extern char kbd_getc() {

	waiting_for_char = 1;
	while (waiting_for_char);
	return last_char;

}

extern void kbd_init() {

	interrupt_handlers[0x21] = kbd_int_hdl;

	print_kbd = 1;
	waiting_for_char = 0;

}

static void kbd_int_hdl(registers_t r) {

	bool full_buff;
	uchar_t c;

	static bool lshift_en	= 0;
	static bool rshift_en	= 0;
	static bool alt_en		= 0;
	static bool ctrl_en		= 0;
	static bool caps_lock	= 0;

	do {
		full_buff = inb(0x64);
	} while ((full_buff & 0x01) == 0); 	// wait for kbd buffer to be full

	// fetch depressed or released key
	if ((c = inb(0x60)) < 0x80) { // key pressed

		switch (c) {

		case 0x1d: // l ctrl
			ctrl_en = 1;
			break;

		case 0x38: // l alt
			alt_en = 1;
			break;

		case 0x3a: // caps lock
			caps_lock = !caps_lock;
			break;

		case 0x36: // r shift
			rshift_en = 1;
			break;

		case 0x2a: // l shift
			lshift_en = 1;
			break;

		case 0xe0: // multimedia
			break;

		default:
			waiting_for_char = 0;
			last_char = kbd_map[c * 4 + (lshift_en || rshift_en || caps_lock)];

			// print the typed character if asked to
			if (print_kbd) {
				putc(last_char);
			}

			break;

		}

	} else {		// key released

		switch (c -= 0x80) {

		case 0x1d: // l ctrl
			ctrl_en = 0;
			break;

		case 0x38: // l alt
			alt_en = 0;
			break;

		case 0x36: // r shift
			rshift_en = 0;
			break;

		case 0x2a: // l shift
			lshift_en = 0;
			break;

		default:
			break;

		}

	}

}
