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
		uint8_t p; // Pattern
		ssd1306_clear();	// Clear the screen.
		// ---- Fill out screen line by line ----
		p = 0xff;
		for (uint8_t i = 0; i < 8; i++) {
			p >>= 1;
			ssd1306_fill(~p);
		}
		_delay_ms(TESTING_DELAY);
		// ---- Fill out screen with shifting byte values ----
		p = 0xff;
		ssd1306_setpos(0, 0);
		ssd1306_start_data();	// Initiate transmission of data
		for (uint16_t i = 128 * 8; i > 0; i--) {
			ssd1306_byte(p);
			p >>= 1;
			if (p == 0) p = 0xff;
		}
		ssd1306_stop();
		_delay_ms(TESTING_DELAY);
		// ---- Fill out screen with random byte values ----
		static uint16_t r;
		ssd1306_setpos(0, 0);
		ssd1306_start_data();	// Initiate transmission of data
		for (uint16_t i = 128 * 8; i > 0; i--) {
			// r = (r * 17 + 83) % 0x3fff;	// Generate a pseudo random number.
			r = ((r << 4) + r + 83) & 0x3fff;	// Optimized (above)
			ssd1306_byte(r >> 6);
		}
		ssd1306_stop();
		_delay_ms(TESTING_DELAY);
	}

	return 0; // Return the mandatory for the "main" function int value - "0" for success.
}

// ============================================================================
