/*
 * screen.h - screen driver
 * Copyright (C) 2018 - Leonard LIMON, LIC
 */

#ifndef __SCREEN__
#define __SCREEN__

#include <cpu/io.h>
#include <cpu/types.h>

/* === screen memory addresses === */
#define VIDEO_MEM 				0xb8000
#define MAX_ROW					24
#define MAX_COL					80
#define VIDEO_MEM_LIM 			(VIDEO_MEM + MAX_COL*MAX_ROW*2)

/* === Screen char attributes codes === */
#define BLACK_FORGND        	0x00
#define BLUE_FORGND         	0x01
#define GREEN_FORGND        	0x02
#define CYAN_FORGND         	0x03
#define RED_FORGND          	0x04
#define MAGENTA_FORGND      	0x05
#define BROWN_FORGND        	0x06
#define LIGHT_GRAY_FORGND   	0x07

#define DARK_GRAY_FORGND     	0x08
#define LIGHT_BLUE_FORGND    	0x09
#define LIGHT_GREEN_FORGND   	0x0A
#define LIGHT_CYAN_FORGND    	0x0B
#define LIGHT_RED_FORGND     	0x0C
#define LIGHT_MAGENTA_FORGND 	0x0D
#define YELLOW_FORGND        	0x0E
#define WHITE_FORGND         	0x0F

#define BLACK_BACKGND        	0x00
#define BLUE_BACKGND         	0x10
#define GREEN_BACKGND        	0x20
#define CYAN_BACKGND         	0x30
#define RED_BACKGND          	0x40
#define MAGENTA_BACKGND      	0x50
#define BROWN_BACKGND        	0x60
#define LIGHT_GRAY_BACKGND 	 	0x70

#define DARK_GRAY_BACKGND     	0x80
#define LIGHT_BLUE_BACKGND    	0x90
#define LIGHT_GREEN_BACKGND   	0xA0
#define LIGHT_CYAN_BACKGND    	0xB0
#define LIGHT_RED_BACKGND     	0xC0
#define LIGHT_MAGENTA_BACKGND 	0xD0
#define YELLOW_BACKGND        	0xE0
#define WHITE_BACKGND         	0xF0

/* === Screen i/o ports === */
#define SCREEN_CTRL_PORT 		0x3d4
#define SCREEN_DATA_PORT 		0x3d5

/* === screen control === */
void screen_clear(void);
void screen_scroll(void);

void screen_setc(uint16_t pos, char c, uint8_t attr);
void screen_putc(char c);
void screen_movc(uint16_t pos);
void screen_set_attr(uint8_t attr);

/* === cursor handling === */
void screen_move_curs(void);
void screen_hide_curs(void);
uint16_t screen_curs_pos(void);

#endif // __SCREEN__ //
