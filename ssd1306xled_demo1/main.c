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

/**
 * Draw vertical short line.
 * Short line is between 0 and 8 pixels in height.
 */
void demo_vsline(uint8_t x, uint8_t r, uint8_t h, uint8_t w) {
	ssd1306_setpos(x, r);
	ssd1306_start_data(); // Initiate transmission of data
	/*
	uint8_t t[] = {
			0b00000000,	// 0
			0b10000000,	// 1
			0b11000000,	// 2
			0b11100000,	// 3
			0b11110000,	// 4
			0b11111000,	// 5
			0b11111100,	// 6
			0b11111110,	// 7
		//	0b11111111,	// 8
	};
		b = t[h];
	*/
	uint8_t b = 0xff;
	if (h < 8) b = b << (8 - h);
	for (; w > 0; w--)
		ssd1306_byte(b);
	ssd1306_stop(); // Finish transmission of data
}

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
	// ---- Draw bitmap on the screen ----
	ssd1306_draw_bmp(0, 0, 128, 8, demo128x64);
	ssd1306tx_stringxy(ssd1306xled_font8x16data, 32, 0, "Tinusaur");
	ssd1306_setpos(3, 2); ssd1306tx_numdec(78); ssd1306tx_string("'C");
	// ssd1306_setpos(3, 3); ssd1306tx_string("----");
	ssd1306_setpos(3, 4); ssd1306tx_string("x:"); ssd1306tx_numdec(12);
	ssd1306_setpos(3, 5); ssd1306tx_string("y:"); ssd1306tx_numdec(34);
	ssd1306_setpos(3, 6); ssd1306tx_string("z:"); ssd1306tx_numdec(56);
	
	for (uint8_t h = 8; h > 0; h--) {
		demo_vsline(102, 5, h, 3);
		demo_vsline(111, 5, h, 3);
		demo_vsline(120, 5, h, 3);
		_delay_ms(100);
	}

	uint8_t x = 20;
	uint8_t y = 20;
	uint8_t z = 20;
	for (;;) {
		// ---- Fill out screen with random byte values ----
		static uint16_t r; // NOTE: For more "real" random numbers this should be 32bit.
		uint8_t rand;
		for (uint8_t c = 255; c > 0; c--) {
			for (uint16_t j = 3; j <= 6; j++) {
				ssd1306_setpos(32, j);
				ssd1306_start_data();	// Initiate transmission of data
				for (uint8_t i = 64; i > 0; i--) {
					// r = (r * 17 + 83) % 0x3fff;	// Generate a pseudo random number.
					r = ((r << 4) + r + 83) & 0xffff;	// Optimized (above)
					rand = r >> 8;
					ssd1306_byte(rand);
				}
				ssd1306_stop();
			}
			if (rand & 0b10000000) {
				if (rand & 0b00100000) x++;
				if (rand & 0b00010000) x--;
				if (rand & 0b00001000) y++;
				if (rand & 0b00000100) y--;
				if (rand & 0b00000010) z++;
				if (rand & 0b00000001) z--;
				ssd1306_setpos(3, 4); ssd1306tx_string("x:"); ssd1306tx_numdec(x);
				ssd1306_setpos(3, 5); ssd1306tx_string("y:"); ssd1306tx_numdec(y);
				ssd1306_setpos(3, 6); ssd1306tx_string("z:"); ssd1306tx_numdec(z);
				demo_vsline(102, 5, 24 - x, 3);
				demo_vsline(111, 5, 24 - y, 3);
				demo_vsline(120, 5, 24 - z, 3);
				ssd1306_setpos(100, 2); ssd1306tx_string("P:"); ssd1306tx_numdec(rand & 0x3f);
			}
		}
		_delay_ms(TESTING_DELAY);
	}

	return 0; // Return the mandatory for the "main" function int value - "0" for success.
}

// ============================================================================
