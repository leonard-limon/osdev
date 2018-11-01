
/*
 * strings.h - everything related to strings
 * Copyright (c) 2018 - Leonard Limon, LIC
 */

#ifndef __STRINGS__
#define __STRINGS__

#include <cpu/types.h>

#include <stdlib/stdio.h>

void int_to_str(int n, char *const str);
void hex_to_str(uint32_t n, char *const str);
void reverse(char *str);

uint32_t strlen(const char *str);

void str_append(char *str, char c);
void str_soft_append(char *str, char c);
int strcmp(const char *str1, const char *str2);

#endif
