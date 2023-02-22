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

#include "ssd1306xled.h"
#include "ssd1306xledfx.h"

// ----------------------------------------------------------------------------

extern void ssd1306_start_data(void);	// Initiate transmission of data
extern void ssd1306_byte(uint8_t);	// Transmission 1 byte of data
extern void ssd1306_stop(void);			// Finish transmission

// ----------------------------------------------------------------------------

void ssd1306_draw_bmp(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, const uint8_t bitmap[]) {
	uint16_t j = 0;
	uint8_t y;
	if (y1 % 8 == 0) y = y1 / 8;
	else y = y1 / 8 + 1;
	for (y = y0; y < y1; y++) {
		// TODO: QUESTION - Is this correct: ssd1306_setpos() and then just ssd1306_byte() ???
		ssd1306_setpos(x0, y);
		ssd1306_start_data();
		for (uint8_t x = x0; x < x1; x++) {
			ssd1306_byte(pgm_read_byte(&bitmap[j++]));
		}
		ssd1306_stop();
	}
}

// ============================================================================
