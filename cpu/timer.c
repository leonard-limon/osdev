
/*
 * timer.c - all functions related to timer
 * Copyright (c) 2018 - Leonard Limon, LIC
 */

#include "timer.h"

extern volatile uint32_t tick;

static void timer_callback(registers_t r) {

	tick++;
	scheduler();

}

uint32_t timer_get_tick() {

	return tick;

}

void timer_init() {

	interrupt_handlers[0x20] = timer_callback;
	timer_set_fq(1000);

}

void timer_set_fq(uint32_t fq) {

	uint32_t divisor = 1193180 / fq;

	// send command to PIT
	outb(0x43, 0x36);

	uint8_t low =  (uint8_t)(divisor & 0xff);
	uint8_t high = (uint8_t)((divisor >> 8) & 0xff);

	// send the fq divisor to PIT
	outb(0x40, low);
	outb(0x40, high);

}
