/**
 * SSD1306xLED - Drivers for SSD1306 controlled dot matrix OLED/PLED 128x64 displays
 *
 * @author Neven Boyanov
 *
 * This is part of the Tinusaur/SSD1306xLED project.
 *
 * Copyright (c) 2017 Neven Boyanov, The Tinusaur Team. All Rights Reserved.
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

#include "ssd1306xled.h"
#include "ssd1306xled8x16.h"
#include "font8x16.h"

// ----------------------------------------------------------------------------

// TODO: Merge this code with the main library

// NOTE: These definitions are copy from the main library.
void ssd1306_xfer_start(void);
void ssd1306_xfer_stop(void);
void ssd1306_send_byte(uint8_t byte);
void ssd1306_send_command(uint8_t command);
void ssd1306_send_data_start(void);
void ssd1306_send_data_stop(void);

// ----------------------------------------------------------------------------

void ssd1306_string_font8x16xy(uint8_t x, uint8_t y, const char s[]) {
	uint8_t ch, j = 0;
	while (s[j] != '\0') {
		ch = s[j] - 32; // Convert ASCII code to font data index.
		if (x > 120) {
			x = 0;
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
