/**
 * SSD1306xLED - Drivers for SSD1306 controlled dot matrix OLED/PLED 128x64 displays
 *
 * @created: 2014-08-08
 * @author: Neven Boyanov
 *
 * This is part of the Tinusaur/SSD1306xLED project.
 *
 * Copyright (c) 2016 Neven Boyanov, Tinusaur Team. All Rights Reserved.
 * Distributed as open source software under MIT License, see LICENSE.txt file.
 * Please, as a favor, retain the link http://tinusaur.org to The Tinusaur Project.
 *
 * Source code available at: https://bitbucket.org/tinusaur/ssd1306xled
 *
 */

// ============================================================================

// #define F_CPU 1000000UL
// NOTE: The F_CPU (CPU frequency) should not be defined in the source code.
//       It should be defined in either (1) Makefile; or (2) in the IDE. 

#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>

//
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//                ATtiny
//               25/45/85
//              +----------+   (-)-------
//      (RST)---+ PB5  Vcc +---(+)-------
// --[OWOWOD]---+ PB3  PB2 +---[TWI/SCL]-
//           ---+ PB4  PB1 +---
// -------(-)---+ GND  PB0 +---[TWI/SDA]-
//              +----------+
//
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// -----(+)--------------->	// Vcc,	Pin 1 on SSD1306 Board
// -----(-)--------------->	// GND,	Pin 2 on SSD1306 Board
#define SSD1306_SCL		PB2	// SCL,	Pin 3 on SSD1306 Board
#define SSD1306_SDA		PB0	// SDA,	Pin 4 on SSD1306 Board

#define SSD1306_SA		0x78	// Slave address

// ----------------------------------------------------------------------------

#include "cpufreq.h"

#include "../ssd1306xled/ssd1306xled.h"
#include "../ssd1306xled/ssd1306xled8x16.h"

// ----------------------------------------------------------------------------

#include "img0_128x64c1.h"
#include "img1_128x64c1.h"

// ----------------------------------------------------------------------------

int main(void) {

	// ---- Initialization ----
	
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

	// Small delay is necessary if ssd1306_init is the first operation in the application.
	_delay_ms(40);
	ssd1306_init();

	// ---- Main Loop ----

	for (;;) {
		
		ssd1306_clear(); _delay_ms(200);

		// ---- Fill out screen with stripes ----
		uint8_t p = 0xff;
		for (uint8_t i = 0; i < 8; i++) {
			p = (p >> 1);
			ssd1306_fill(~p);
		}
		_delay_ms(400);

		// ---- Fill out screen with patters ----
		ssd1306_fill(0xAA); _delay_ms(400);
		ssd1306_fill2(0x55, 0xAA); _delay_ms(400);
		ssd1306_fill4(0xCC, 0xCC, 0x33, 0x33); _delay_ms(400);
		_delay_ms(1000);

		// ---- Print some small numbers on the screen ----
		uint16_t n1 = 0;
		for (uint8_t j = 0; j < 8; j++) {
			ssd1306_setpos(0, j);
			for (uint8_t i = 0; i < 7; i++) {
				ssd1306_numdec(n1++);
				ssd1306_string(" ");
			}
		}
		_delay_ms(1000);
		
		// ---- Print some large numbers on the screen ----
		ssd1306_fill2(0xAA, 0x55);	// Fill screen
		uint16_t n2 = 199;
		for (uint8_t j = 1; j < 7; j++) {
			ssd1306_setpos(10, j);
			for (uint8_t i = 0; i < 3; i++) {
				ssd1306_numdecp(n2);
				ssd1306_string(" ");
				n2 += 567;
			}
		}
		_delay_ms(1000);
		
		// ---- Print some variables on the screen ----
		ssd1306_fill4(0xC0, 0x030, 0x0C, 0x03); _delay_ms(400);
		uint16_t n3 = 0;
		for (uint8_t i = 0; i < 163; i++) {
			ssd1306_setpos(44, 3);
			ssd1306_string("a=");
			ssd1306_numdecp(n3);
			ssd1306_setpos(44, 4);
			ssd1306_string("b=");
			ssd1306_numdecp(0xffff - n3);
			n3 += (n3 * 3) / 33 + 1;
		}
		_delay_ms(1000);

		// ---- Print some small and large text on the screen ----
		ssd1306_clear(); _delay_ms(200);
		ssd1306_setpos(34, 0);	ssd1306_string_font6x8("This is the");
		ssd1306_string_font8x16xy(10, 1, "Tinusaur");
		ssd1306_setpos(80, 2);	ssd1306_string_font6x8("project");
		ssd1306_setpos(0, 4);	ssd1306_string_font6x8("The quick start       platform for your    next awesome project");
		ssd1306_setpos(8, 7);	ssd1306_string_font6x8("http://tinusaur.org");
		_delay_ms(6000);
		
		// ---- Draw bitmap on the screen ----
		ssd1306_draw_bmp(0,0,128,8, img1_128x64c1);
		_delay_ms(4000);

		// ---- Draw bitmap on the screen ----
		ssd1306_draw_bmp(0,0,128,8, img0_128x64c1);
		_delay_ms(6000);
	}
	
	return 0;
}

// ============================================================================
