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

#define F_CPU 1000000UL

#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>

// ----------------------------------------------------------------------------

// ---------------------	// Vcc,	Pin 1 on SSD1306 Board
// ---------------------	// GND,	Pin 2 on SSD1306 Board
#define SSD1306_SCL		PB0	// SCL,	Pin 3 on SSD1306 Board
#define SSD1306_SDA		PB1	// SDA,	Pin 4 on SSD1306 Board

#define SSD1306_SA		0x78	// Slave address

// ----------------------------------------------------------------------------

#include "../ssd1306xled/ssd1306xled.h"
#include "../ssd1306xled_cn/font1616cn.h"

// ----------------------------------------------------------------------------

int main(void)
{
	// ---- Initialization ----
	
	// Small delay is necessary if ssd1306_initis the first operation in the application.
	_delay_ms(40);
	ssd1306_init();

	// ---- Main Loop ----

	for (;;) {

		uint8_t p = 0xff;
		for (uint8_t i = 0; i < 4; i++)
		{
			p = (p >> i);
			ssd1306_fillscreen(~p);
			_delay_ms(100);
		}

		ssd1306_fillscreen(0x00);
		
		ssd1306_char_f16x16(0,5,0);
		ssd1306_char_f16x16(16,5,1);
		ssd1306_char_f16x16(32,5,2);
		ssd1306_char_f16x16(48,5,3);
		ssd1306_char_f16x16(64,5,4);
		_delay_ms(4000);
	}
	
	return 0; // Return the mandatory result value. It is "0" for success.
}

// ============================================================================
