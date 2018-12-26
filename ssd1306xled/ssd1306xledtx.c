/**
 * SSD1306xLED - Library for the SSD1306 based OLED/PLED 128x64 displays
 *
 * @author Neven Boyanov
 *
 * This is part of the Tinusaur/SSD1306xLED project.
 *
 * Copyright (c) 2018 Neven Boyanov, The Tinusaur Team. All Rights Reserved.
 * Distributed as open source software under MIT License, see LICENSE.txt file.
 * Retain in your source code the link http://tinusaur.org to the Tinusaur project.
 *
 * Source code available at: https://bitbucket.org/tinusaur/ssd1306xled
 *
 */

// ============================================================================

#include <stdlib.h>
#include <avr/io.h>
#include <avr/pgmspace.h>

#include "tinyavrlib/num2str.h"

#include "ssd1306xled.h"
#include "ssd1306xledtx.h"

// ============================================================================

void ssd1306tx_init(void) {

}

// ----------------------------------------------------------------------------

uint8_t *ssd1306xled_font6x8 = NULL;
// NOTE: If the function below is used then the font should be defined.

void ssd1306_char_font6x8(char ch) {
	uint8_t c = ch - 32; // Convert ASCII code to font data index.
	ssd1306_send_data_start();
	for (uint8_t i = 0; i < 6; i++) {
		ssd1306_send_byte(pgm_read_byte(&ssd1306xled_font6x8[c * 6 + i]));	// TODO: Optimize this.
	}
	ssd1306_send_data_stop();
}

void ssd1306_string_font6x8(char *s) {
	while (*s) {
		ssd1306_char_font6x8(*s++);
	}
}

char ssd1306_numdec_buffer[USINT2DECASCII_MAX_DIGITS + 1];

void ssd1306_numdec_font6x8(uint16_t num) {
	ssd1306_numdec_buffer[USINT2DECASCII_MAX_DIGITS] = '\0';   // Terminate the string.
	uint8_t digits = usint2decascii(num, ssd1306_numdec_buffer);
	ssd1306_string_font6x8(ssd1306_numdec_buffer + digits);
}

void ssd1306_numdecp_font6x8(uint16_t num) {
	ssd1306_numdec_buffer[USINT2DECASCII_MAX_DIGITS] = '\0';   // Terminate the string.
	usint2decascii(num, ssd1306_numdec_buffer);
	ssd1306_string_font6x8(ssd1306_numdec_buffer);
}

// ----------------------------------------------------------------------------

// NOTE: If the function below is used then the font should be defined.
uint8_t *ssd1306xled_font8x16 = NULL;

void ssd1306_string_font8x16xy(uint8_t x, uint8_t y, const char s[]) {
	uint8_t ch, j = 0;
	while (s[j] != '\0') {
		ch = s[j] - 32; // Convert ASCII code to font data index.
		if (x > 120) {
			x = 0;    // Go to the next line.
			y++;
		}
		ssd1306_setpos(x, y);
		ssd1306_send_data_start();
		for (uint8_t i = 0; i < 8; i++) {
			ssd1306_send_byte(pgm_read_byte(&ssd1306xled_font8x16[ch * 16 + i]));
		}
		ssd1306_send_data_stop();
		ssd1306_setpos(x, y + 1);
		ssd1306_send_data_start();
		for (uint8_t i = 0; i < 8; i++) {
			ssd1306_send_byte(pgm_read_byte(&ssd1306xled_font8x16[ch * 16 + i + 8]));
		}
		ssd1306_send_data_stop();
		x += 8;
		j++;
	}
}

// ============================================================================
