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
#include "ssd1306xled/ssd1306xledtx.h"
#include "ssd1306xled/font6x8.h"

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
	ssd1306tx_init(ssd1306xled_font6x8data, ' ');

	// ---- Main Loop ----
	for (;;) {
		ssd1306_clear();	// Clear the screen.

		// ---- Fill out screen with patters ----
		ssd1306_fill2(0x80, 0x00);
		_delay_ms(TESTING_DELAY);

		// NOTE: Screen width - 128, that is 21 symbols per row.
		// ---- Print some text on the screen ----
		ssd1306_setpos(0, 0);
		ssd1306tx_string("Hello, World! Testing");
		ssd1306_setpos(7, 1);
		ssd1306tx_string("SSD1306xLED Library");
		ssd1306_setpos(0, 2);
		ssd1306tx_string("!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[5]^_`abcdefghijklmnopqrstuvwxyz~");
		ssd1306_setpos(0, 6);
		ssd1306tx_string(" This is the TINUSUR ");
		ssd1306_setpos(3, 7);
		ssd1306tx_string("https://tinusaur.com");
		_delay_ms(TESTING_DELAY << 4);
	}

	return 0; // Return the mandatory for the "main" function int value - "0" for success.
}

// ============================================================================
