
/*
 * timer.h - all functions related to timer
 * Copyright (c) 2018 - Leonard Limon, LIC
 */

#ifndef __TIMER__
#define __TIMER__

#include <cpu/types.h>
#include <cpu/int_handlers.h>

#include <kernel/scheduler.h>

/* === setup and timer related routines === */
extern uint32_t timer_get_tick();
extern void timer_init();
extern void timer_set_fq(uint32_t fq);
extern void timer_sleep(uint32_t t);

/* === interrupt routine === */
static void timer_callback(registers_t);

#endif
