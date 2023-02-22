/**
 * SSD1306xLED - Testing scripts
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

#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>

#include "ssd1306xled/ssd1306xled.h"
#include "ssd1306xled/ssd1306xledfx.h"

// #include "pixelart_bitmap_128x64_01.h"
#include "pixelart_bitmap_128x64_02.h"
#include "pixelart_bitmap_128x64_03.h"
#include "pixelart_bitmap_128x64_04.h"
#include "pixelart_bitmap_128x64_05.h"
#include "pixelart_bitmap_128x64_06.h"
#include "pixelart_bitmap_128x64_07.h"
#include "pixelart_bitmap_128x64_08.h"

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//                 ATtiny85
//               +----------+    (-)--GND--
//      (RST)--> + PB5  Vcc +----(+)--VCC--
// ---[OWOWOD]---+ PB3  PB2 +----[TWI/SCL]-
// --------------+ PB4  PB1 +--------------
// --------(-)---+ GND  PB0 +----[TWI/SDA]-
//               +----------+
//                 Tinusaur
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// NOTE: If you want to reassign the SCL and SDA pins and the I2C address
// do that in the library source code and recompile it so it will take affect.

// ----------------------------------------------------------------------------

#define TESTING_DELAY 4000

int main(void) {
	// ---- Initialization ----
	ssd1306_init();

	// ---- Main Loop ----
	ssd1306_clear();	// Clear the screen.
	for (;;) {
		// ---- Draw bitmap on the screen ----
		// ssd1306_draw_bmp(0, 0, 128, 8, pixelart_bitmap_128x64_01);
		// _delay_ms(TESTING_DELAY);
		// ---- Draw bitmap on the screen ----
		ssd1306_draw_bmp(0, 0, 128, 8, pixelart_bitmap_128x64_02);
		_delay_ms(TESTING_DELAY);
		// ---- Draw bitmap on the screen ----
		ssd1306_draw_bmp(0, 0, 128, 8, pixelart_bitmap_128x64_03);
		_delay_ms(TESTING_DELAY);
		// ---- Draw bitmap on the screen ----
		ssd1306_draw_bmp(0, 0, 128, 8, pixelart_bitmap_128x64_04);
		_delay_ms(TESTING_DELAY);
		// ---- Draw bitmap on the screen ----
		ssd1306_draw_bmp(0, 0, 128, 8, pixelart_bitmap_128x64_05);
		_delay_ms(TESTING_DELAY);
		// ---- Draw bitmap on the screen ----
		ssd1306_draw_bmp(0, 0, 128, 8, pixelart_bitmap_128x64_06);
		_delay_ms(TESTING_DELAY);
		// ---- Draw bitmap on the screen ----
		ssd1306_draw_bmp(0, 0, 128, 8, pixelart_bitmap_128x64_07);
		_delay_ms(TESTING_DELAY);
		// ---- Draw bitmap on the screen ----
		ssd1306_draw_bmp(0, 0, 128, 8, pixelart_bitmap_128x64_08);
		_delay_ms(TESTING_DELAY);
	}

	return 0; // Return the mandatory for the "main" function int value - "0" for success.
}

// ============================================================================
