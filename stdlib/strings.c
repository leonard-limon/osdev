/*
 * strings.c - everything related to strings
 * Copyright (c) 2018 - Leonard Limon, LIC
 */


#include "strings.h"

void int_to_str(int n, char *const str) {

	bool sign;
	uint32_t i = 0;

    if (sign = (n < 0)) n = -n;

    do {
        str[i++] = n % 10 + '0';
    } while ((n /= 10) > 0);

    if (sign) str[i++] = '-';
    str[i] = '\0';

    reverse(str);

}

void hex_to_str(uint32_t n, char *const str) {

	uint32_t i = 0;

	while (i < 8) {
		uint32_t tmp = n & 0x0f;
		str[i++] = tmp + (tmp < 0x0a? '0': 'a' - 0x0a);
		n >>= 4;
	}

	str[i++] = 'x';
	str[i++] = '0';
	str[i++] = 0;

	reverse(str);

}

void reverse(char *str) {

	char tmp;
	uint32_t i = 0, j = strlen(str) - 1;

	while (i < j) {
		tmp = str[i];
		str[i++] = str[j];
		str[j--] = tmp;
	}

}

uint32_t strlen(const char *str) {

	uint32_t len = 0;

	while (str[len]) {
		len++;
	}

	return len;

}

void str_soft_append(char *str, char c) {

	uint32_t len = strlen(str);

	if (c == '\b') { if (len) str[len - 1] = '\0'; }  // backspace
	else if (c == 9 || c == 10 || c == 13 || (c >= 32 && c < 127)) {

		str[len] = c;

	}

}

void str_append(char *str, char c) {

	uint32_t len = strlen(str);

	if (c == '\b') { if (len) str[len - 1] = '\0'; }  // backspace
	else if (c == 9 || c == 10 || c == 13 || (c >= 32 && c < 127)) {

		str[len] = c;
		str[len + 1] = 0;

	}

}

int strcmp(const char *str1, const char *str2) {

	uint32_t i = 0;

	while (str1[i] == str2[i] && str1[i] && str2[i]) {
		++i;
	}

	return str1[i] - str2[i];

}
