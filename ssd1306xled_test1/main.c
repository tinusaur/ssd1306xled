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

#define TESTING_DELAY 500

int main(void) {
	// ---- Initialization ----
	ssd1306_init();

	// ---- Main Loop ----
	for (;;) {
		ssd1306_clear();	// Clear the screen.
		// ---- Fill out the screen with patters ----
		ssd1306_fill(0xAA);	// Horizontal lines
		_delay_ms(TESTING_DELAY);
		ssd1306_fill2(0XFF, 0x00);	// Vertical lines
		_delay_ms(TESTING_DELAY);
		ssd1306_fill2(0x55, 0xAA);	// Dots
		_delay_ms(TESTING_DELAY);
		ssd1306_fill4(0xCC, 0xCC, 0x33, 0x33);	// Small squares
		_delay_ms(TESTING_DELAY);
		ssd1306_fill4(0xC0, 0x30, 0x0C, 0x03);	// Slashes, sloping lines
		_delay_ms(TESTING_DELAY);
		ssd1306_fill4(0x30, 0xC0, 0x03, 0x0C);	// Slashes, sloping lines
		_delay_ms(TESTING_DELAY);
		ssd1306_fill(0XFF);	// Solid
		_delay_ms(TESTING_DELAY);
	}

	return 0; // Return the mandatory for the "main" function int value - "0" for success.
}

// ============================================================================
