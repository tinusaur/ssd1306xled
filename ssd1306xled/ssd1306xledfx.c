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

#include "ssd1306xled.h"
#include "ssd1306xledfx.h"

// ----------------------------------------------------------------------------

extern void i2csw_start(void);
extern void i2csw_stop(void);
extern void i2csw_byte(uint8_t byte);

// ----------------------------------------------------------------------------

void ssd1306_draw_bmp(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, const uint8_t bitmap[]) {
	uint16_t j = 0;
	uint8_t y;
	if (y1 % 8 == 0) y = y1 / 8;
	else y = y1 / 8 + 1;
	for (y = y0; y < y1; y++) {
		// TODO: QUESTION - Is this correct: ssd1306_setpos() and then just i2csw_byte() ???
		ssd1306_setpos(x0, y);
		i2csw_start();
		for (uint8_t x = x0; x < x1; x++) {
			i2csw_byte(pgm_read_byte(&bitmap[j++]));
		}
		i2csw_stop();
	}
}

// ============================================================================
