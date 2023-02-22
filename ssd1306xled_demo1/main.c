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

// NOTE: About F_CPU - it should be set in either (1) Makefile; or (2) in the IDE.

#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>

#include "tinyavrlib/cpufreq.h"

#include "ssd1306xled/ssd1306xled.h"
#include "ssd1306xled/ssd1306xledtx.h"
#include "ssd1306xled/ssd1306xledfx.h"

#include "ssd1306xled/font6x8.h"
#include "ssd1306xled/font8x16.h"

#include "demo128x64.h"

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

// ---- CPU Frequency Setup ----
#if F_CPU == 1000000UL
#pragma message "F_CPU=1MHZ"
	CLKPR_SET(CLKPR_1MHZ);
#elif F_CPU == 8000000UL
#pragma message "F_CPU=8MHZ"
	CLKPR_SET(CLKPR_8MHZ);
#else
#pragma message "F_CPU=????"
#error "CPU frequency should be either 1 MHz or 8 MHz"
#endif

	// ---- Initialization ----
	_delay_ms(40);	// Small delay might be necessary if ssd1306_init is the first operation in the application.
	ssd1306_init();
	ssd1306tx_init(ssd1306xled_font6x8data, ' ');

	// ---- Main Loop ----
	ssd1306_clear();	// Clear the screen.
	ssd1306_draw_bmp(0, 0, 128, 8, demo128x64);
	ssd1306tx_stringxy(ssd1306xled_font8x16data, 32, 0, "Tinusaur");
	ssd1306_setpos(3, 2); ssd1306tx_numdec(78); ssd1306tx_string("'C");
	// ssd1306_setpos(3, 3); ssd1306tx_string("----");
	ssd1306_setpos(3, 4); ssd1306tx_string("x:"); ssd1306tx_numdec(12);
	ssd1306_setpos(3, 5); ssd1306tx_string("y:"); ssd1306tx_numdec(34);
	ssd1306_setpos(3, 6); ssd1306tx_string("z:"); ssd1306tx_numdec(56);
	for (;;) {
		// ---- Draw bitmap on the screen ----
		_delay_ms(TESTING_DELAY << 2);
		// ---- Fill out screen with random byte values ----
		static uint16_t r; // NOTE: For more "real" random numbers this should be 32bit.
		for (uint8_t c = 255; c > 0; c--) {
			for (uint16_t j = 3; j <= 6; j++) {
				ssd1306_setpos(32, j);
				ssd1306_start_data();	// Initiate transmission of data
				for (uint8_t i = 64; i > 0; i--) {
					// r = (r * 17 + 83) % 0x3fff;	// Generate a pseudo random number.
					r = ((r << 4) + r + 83) & 0xffff;	// Optimized (above)
					ssd1306_byte(r >> 8);
				}
				ssd1306_stop();
			}
			ssd1306_setpos(100, 2); ssd1306tx_string("P:"); ssd1306tx_numdec((r >> 8) & 0x3f);
		}
		_delay_ms(TESTING_DELAY);
	}

	return 0; // Return the mandatory for the "main" function int value - "0" for success.
}

// ============================================================================
