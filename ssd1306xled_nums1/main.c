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

// NOTE: About F_CPU - it should be set in either (1) Makefile; or (2) in the IDE.

#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>

#include "tinyavrlib/cpufreq.h"

#include "ssd1306xled/ssd1306xled.h"
#include "ssd1306xled/ssd1306xledtx.h"
#include "ssd1306xled/font6x8.h"
// #include "ssd1306xled/font8x16.h"

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//                 ATtiny85
//               +----------+   (-)--GND--
//       (RST)---+ PB5  Vcc +---(+)--VCC--
// ---[OWOWOD]---+ PB3  PB2 +---[TWI/SCL]-
// --------------+ PB4  PB1 +-------------
// --------(-)---+ GND  PB0 +---[TWI/SDA]-
//               +----------+
//              Tinusaur Board
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
	ssd1306xled_font6x8 = ssd1306xled_font6x8data;
	// ssd1306xled_font8x16 = ssd1306xled_font8x16data;

	// Small delay is necessary if ssd1306_init is the first operation in the application.
	_delay_ms(40);
	ssd1306_init();

	// ---- Main Loop ----
	for (;;) {
		ssd1306_clear();

		// ---- Fill out screen with patters ----
		ssd1306_fill2(0x55, 0xAA);	// Dots
		_delay_ms(TESTING_DELAY);

		// NOTE: Screen width - 128, that is 21 symbols per row.

		// ---- Print some text on the screen ----
		ssd1306_setpos(4, 1);
		ssd1306_string_font6x8(" Numbers in a table ");

		// ---- Print some small numbers on the screen ----
		uint16_t n1 = 0;
		for (uint8_t j = 3; j < 7; j++) {
			ssd1306_setpos(12, j);
			for (uint8_t i = 0; i < 6; i++) {
				ssd1306_numdec(n1++);
				ssd1306_string(" ");
			}
		}
		_delay_ms(TESTING_DELAY << 2);

		// ---- Print some large numbers on the screen ----
		uint16_t n2 = 199;
		for (uint8_t j = 3; j < 7; j++) {
			ssd1306_setpos(12, j);
			for (uint8_t i = 0; i < 3; i++) {
				ssd1306_numdecp(n2);
				ssd1306_string(" ");
				n2 += 567;
			}
		}
		_delay_ms(TESTING_DELAY << 2);

		// ---- Print some variables on the screen ----
		ssd1306_setpos(18, 3);
		ssd1306_string("----------------");
		ssd1306_setpos(18, 6);
		ssd1306_string("----------------");
		uint16_t n3 = 0;
		for (uint8_t i = 0; i < 163; i++) {
			ssd1306_setpos(18, 4);
			ssd1306_string("a = ");
			ssd1306_numdecp(n3);
			ssd1306_string(" -> ");
			ssd1306_setpos(18, 5);
			ssd1306_string("b = ");
			ssd1306_numdecp(0xffff - n3);
			ssd1306_string(" -> ");
			n3 += (n3 * 3) / 33 + 1;
		}
		_delay_ms(TESTING_DELAY << 4);
	}

	return 0; // Return the mandatory result value. It is "0" for success.
}

// ============================================================================
