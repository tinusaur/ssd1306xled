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

extern void i2csw_start(void);
extern void i2csw_stop(void);
extern void i2csw_byte(uint8_t byte);

// ----------------------------------------------------------------------------

const uint8_t *ssd1306tx_font_src;
uint8_t ssd1306tx_font_char_base;

// ----------------------------------------------------------------------------

void ssd1306tx_init(const uint8_t *fron_src, uint8_t char_base) {
	ssd1306tx_font_src = fron_src;
	ssd1306tx_font_char_base = char_base;
}

// ----------------------------------------------------------------------------

void ssd1306tx_char(char ch) {
	uint8_t c = ch - 32; // Convert ASCII code to font data index.
	i2csw_start();
	for (uint8_t i = 0; i < 6; i++) {
		i2csw_byte(pgm_read_byte(&ssd1306tx_font_src[c * 6 + i]));	// TODO: Optimize this.
	}
	i2csw_stop();
}

void ssd1306tx_string(char *s) {
	while (*s) {
		ssd1306tx_char(*s++);
	}
}

// ----------------------------------------------------------------------------

char ssd1306_numdec_buffer[USINT2DECASCII_MAX_DIGITS + 1];

void ssd1306tx_numdec(uint16_t num) {
	ssd1306_numdec_buffer[USINT2DECASCII_MAX_DIGITS] = '\0';   // Terminate the string.
	uint8_t digits = usint2decascii(num, ssd1306_numdec_buffer);
	ssd1306tx_string(ssd1306_numdec_buffer + digits);
}

void ssd1306tx_numdecp(uint16_t num) {
	ssd1306_numdec_buffer[USINT2DECASCII_MAX_DIGITS] = '\0';   // Terminate the string.
	usint2decascii(num, ssd1306_numdec_buffer);
	ssd1306tx_string(ssd1306_numdec_buffer);
}

// ----------------------------------------------------------------------------

void ssd1306tx_stringxy(const uint8_t *fron_src, uint8_t x, uint8_t y, const char s[]) {
	uint8_t ch, j = 0;
	while (s[j] != '\0') {
		ch = s[j] - 32; // Convert ASCII code to font data index.
		if (x > 120) {
			x = 0;    // Go to the next line.
			y++;
		}
		ssd1306_setpos(x, y);
		i2csw_start();
		for (uint8_t i = 0; i < 8; i++) {
			i2csw_byte(pgm_read_byte(&fron_src[ch * 16 + i]));
		}
		i2csw_stop();
		ssd1306_setpos(x, y + 1);
		i2csw_start();
		for (uint8_t i = 0; i < 8; i++) {
			i2csw_byte(pgm_read_byte(&fron_src[ch * 16 + i + 8]));
		}
		i2csw_stop();
		x += 8;
		j++;
	}
}

// ============================================================================
