/**
 * SSD1306xLED - Library for the SSD1306 based OLED/PLED 128x64 displays
 * @author Neven Boyanov
 * This is part of the Tinusaur/SSD1306xLED project.
 * ----------------------------------------------------------------------------
 *  Copyright (c) 2023 Tinusaur (https://tinusaur.com). All rights reserved.
 *  Distributed as open source under the MIT License (see the LICENSE.txt file)
 *  Please, retain in your work a link to the Tinusaur project website.
 * ----------------------------------------------------------------------------
 * Source code available at: https://gitlab.com/tinusaur/ssd1306xled
 */

// ============================================================================

#include <stdlib.h>
#include <avr/io.h>
#include <avr/pgmspace.h>

#include "tinyavrlib/num2str.h"

#include "ssd1306xled.h"
#include "ssd1306xledtx.h"

// ----------------------------------------------------------------------------

extern void ssd1306_start_data(void);	// Initiate transmission of data
extern void ssd1306_byte(uint8_t);	// Transmission 1 byte of data
extern void ssd1306_stop(void);			// Finish transmission

// ----------------------------------------------------------------------------

const uint8_t *ssd1306tx_font_src;
uint8_t ssd1306tx_font_char_base;

// ----------------------------------------------------------------------------

void ssd1306tx_init(const uint8_t *font_src, uint8_t char_base) {
	ssd1306tx_font_src = font_src;
	ssd1306tx_font_char_base = char_base;
}

// ----------------------------------------------------------------------------
// TODO: Use the char_base to calculate the font data index.
void ssd1306tx_char(char ch) {
	uint16_t j = (ch << 2) + (ch << 1) - 192; // Equiv.: j=(ch-32)*6 <== Convert ASCII code to font data index.
	ssd1306_start_data();
	for (uint8_t i = 0; i < 6; i++) {
		ssd1306_byte(pgm_read_byte(&ssd1306tx_font_src[j + i]));
	}
	ssd1306_stop();
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

void ssd1306tx_stringxy(const uint8_t *font_src, uint8_t x, uint8_t y, const char s[]) {
	uint16_t j, k = 0;
	while (s[k] != '\0') {
		j = s[k] * 16 - (32 * 16); // Convert ASCII code to font data index. NOTE: (x*16) already optimized to (x<<4).
		if (x > 120) {
			x = 0;    // Go to the next line.
			y++;
		}
		ssd1306_setpos(x, y);
		ssd1306_start_data();
		for (uint8_t i = 0; i < 8; i++) {
			ssd1306_byte(pgm_read_byte(&font_src[j + i]));
		}
		ssd1306_stop();
		ssd1306_setpos(x, y + 1);
		ssd1306_start_data();
		for (uint8_t i = 0; i < 8; i++) {
			ssd1306_byte(pgm_read_byte(&font_src[j + i + 8]));
		}
		ssd1306_stop();
		x += 8;
		k++;
	}
}

// ============================================================================
