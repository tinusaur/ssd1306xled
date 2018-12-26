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

	_delay_ms(40);	// Small delay might be necessary if ssd1306_init is the first operation in the application.
	ssd1306_init();

	// ---- Main Loop ----
	for (;;) {
		ssd1306_clear();	// Clear the screen.

		// ---- Fill out screen with random bytes values ----
		uint16_t r = 100;
		ssd1306_setpos(0, 0);
		for (uint16_t i = 0; i < 128 * 8; i++) {
			// rand=(rand*109+89)%251; // Ref: https://www.avrfreaks.net/forum/random-number-generation-0
			r = (r * 109 + 89) % 521; // Generate a pseudo random number.
			ssd1306_byte(r);
		}
		_delay_ms(TESTING_DELAY);

		// ---- Fill out screen with sequential bytes values ----
		ssd1306_setpos(0, 0);
		for (uint16_t i = 0; i < 128 * 8; i++) {
			ssd1306_byte(i);
		}
		_delay_ms(TESTING_DELAY);

		// ---- Fill out screen line by line ----
		uint8_t p = 0xff;
		for (uint8_t i = 0; i < 8; i++) {
			p = (p >> 1);
			ssd1306_fill(~p);
		}
		_delay_ms(TESTING_DELAY);

		// ---- Fill out screen with patters ----
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

	return 0; // Return the mandatory result value. It is "0" for success.
}

// ============================================================================
