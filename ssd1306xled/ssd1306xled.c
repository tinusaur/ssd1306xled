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
// ACKNOWLEDGEMENTS:
// - Some code and ideas initially based on "IIC_wtihout_ACK" 
//   by http://www.14blog.com/archives/1358 (defunct)
// - Init sequence used info from Adafruit_SSD1306.cpp init code.
// ============================================================================

#include <stdlib.h>
#include <avr/io.h>
#include <avr/pgmspace.h>

#include "ssd1306xled.h"

// ----------------------------------------------------------------------------

void ssd1306_start_command(void);	// Initiate transmission of command
void ssd1306_start_data(void);	// Initiate transmission of data
void ssd1306_data_byte(uint8_t);	// Transmission 1 byte of data
void ssd1306_stop(void);	// Finish transmission

// ----------------------------------------------------------------------------

const uint8_t ssd1306_init_sequence [] PROGMEM = {	// Initialization Sequence

	0xAE,			// Set Display ON/OFF - AE=OFF, AF=ON
	0xD5, 0xF0,		// Set display clock divide ratio/oscillator frequency, set divide ratio
	0xA8, 0x3F,		// Set multiplex ratio (1 to 64) ... (height - 1)
	0xD3, 0x00,		// Set display offset. 00 = no offset
	0x40 | 0x00,	// Set start line address, at 0.
	0x8D, 0x14,		// Charge Pump Setting, 14h = Enable Charge Pump
	0x20, 0x00,		// Set Memory Addressing Mode - 00=Horizontal, 01=Vertical, 10=Page, 11=Invalid
	0xA0 | 0x01,	// Set Segment Re-map
	0xC8,			// Set COM Output Scan Direction
	0xDA, 0x12,		// Set COM Pins Hardware Configuration - 128x32:0x02, 128x64:0x12
	0x81, 0x3F,		// Set contrast control register
	0xD9, 0x22,		// Set pre-charge period (0x22 or 0xF1)
	0xDB, 0x20,		// Set Vcomh Deselect Level - 0x00: 0.65 x VCC, 0x20: 0.77 x VCC (RESET), 0x30: 0.83 x VCC
	0xA4,			// Entire Display ON (resume) - output RAM to display
	0xA6,			// Set Normal/Inverse Display mode. A6=Normal; A7=Inverse
	0x2E,			// Deactivate Scroll command
	0xAF,			// Set Display ON/OFF - AE=OFF, AF=ON
	//
	0x22, 0x00, 0x3f,	// Set Page Address (start,end)
	0x21, 0x00,	0x7f,	// Set Column Address (start,end)
	//
	// 0xD6, 0x01,		// Set Zoom In, 0=disabled, 1=enabled
/*
	0xAE,			// Display OFF (sleep mode)
	0x20, 0x00,		// Set Memory Addressing Mode - 00=Horizontal, 01=Vertical, , 10=Page, 11=Invalid
	0xB0,			// Set Page Start Address for Page Addressing Mode, 0-7
	0xC8,			// Set COM Output Scan Direction
	0x00,			// ---set low column address
	0x10,			// ---set high column address
	0x40,			// --set start line address
	0x81, 0x3F,		// Set contrast control register
	0xA1,			// Set Segment Re-map. A0=address mapped; A1=address 127 mapped.
	0xA6,			// Set display mode. A6=Normal; A7=Inverse
	0xA8, 0x3F,		// Set multiplex ratio(1 to 64)
	0xA4,			// Output RAM to Display - 0xA4=Output follows RAM content; 0xA5,Output ignores RAM content
	0xD3, 0x00,		// Set display offset. 00 = no offset
	0xD5, 0xF0,		// --set display clock divide ratio/oscillator frequency
	0xD9, 0x22,		// Set pre-charge period
	0xDA, 0x12,		// Set com pins hardware configuration
	0xDB, 0x20,		// --set vcomh, 0x20,0.77xVcc
	0x8D, 0x14,		// Charge Pump Setting, 14h = Enable Charge Pump
	0xAF,			// Display ON in normal mode
*/
};

// ============================================================================

// NOTE: These functions are separate sub-library for handling I2C simplified output.
// NAME: I2CSW - I2C Simple Writer.
// Convenience definitions for manipulating PORTB pins
// NOTE: These definitions are used only internally by the I2CSW library
#define I2CSW_HIGH(PORT) PORTB |= (1 << PORT)
#define I2CSW_LOW(PORT) PORTB &= ~(1 << PORT)

// ----------------------------------------------------------------------------

void i2csw_start(void);
void i2csw_stop(void);
void i2csw_byte(uint8_t byte);

// ----------------------------------------------------------------------------

void i2csw_start(void) {
	DDRB |= (1 << SSD1306_SDA);	// Set port as output
	DDRB |= (1 << SSD1306_SCL);	// Set port as output
	I2CSW_HIGH(SSD1306_SCL);	// Set to HIGH
	I2CSW_HIGH(SSD1306_SDA);	// Set to HIGH
	I2CSW_LOW(SSD1306_SDA);		// Set to LOW
	I2CSW_LOW(SSD1306_SCL);		// Set to LOW
}

void i2csw_stop(void) {
	I2CSW_LOW(SSD1306_SCL);		// Set to LOW
	I2CSW_LOW(SSD1306_SDA);		// Set to LOW
	I2CSW_HIGH(SSD1306_SCL);	// Set to HIGH
	I2CSW_HIGH(SSD1306_SDA);	// Set to HIGH
	DDRB &= ~(1 << SSD1306_SDA);// Set port as input
}

void i2csw_byte(uint8_t byte) {
	uint8_t i;
	for (i = 0; i < 8; i++) {
		if ((byte << i) & 0x80)
			I2CSW_HIGH(SSD1306_SDA);
		else
			I2CSW_LOW(SSD1306_SDA);
		I2CSW_HIGH(SSD1306_SCL);
		I2CSW_LOW(SSD1306_SCL);
	}
	I2CSW_HIGH(SSD1306_SDA);
	I2CSW_HIGH(SSD1306_SCL);
	I2CSW_LOW(SSD1306_SCL);
}

// ============================================================================

void ssd1306_start_command(void) {
	i2csw_start();
	i2csw_byte(SSD1306_SADDR);	// Slave address: R/W(SA0)=0 - write
	i2csw_byte(0x00);			// Control byte: D/C=0 - write command
}

void ssd1306_start_data(void) {
	i2csw_start();
	i2csw_byte(SSD1306_SADDR);	// Slave address, R/W(SA0)=0 - write
	i2csw_byte(0x40);			// Control byte: D/C=1 - write data
}

void ssd1306_data_byte(uint8_t b) {
	i2csw_byte(b);
}

void ssd1306_stop(void) {
	i2csw_stop();
}

// ============================================================================

void ssd1306_init(void) {
	ssd1306_start_command();	// Initiate transmission of command
	for (uint8_t i = 0; i < sizeof (ssd1306_init_sequence); i++) {
		ssd1306_data_byte(pgm_read_byte(&ssd1306_init_sequence[i]));	// Send the command out
	}
	ssd1306_stop();	// Finish transmission
}

void ssd1306_setpos(uint8_t x, uint8_t y) {
	ssd1306_start_command();
	ssd1306_data_byte(0xb0 | (y & 0x07));	// Set page start address
	ssd1306_data_byte(x & 0x0f);			// Set the lower nibble of the column start address
	ssd1306_data_byte(0x10 | (x >> 4));		// Set the higher nibble of the column start address
	ssd1306_stop();	// Finish transmission
}

void ssd1306_fill4(uint8_t p1, uint8_t p2, uint8_t p3, uint8_t p4) {
	ssd1306_setpos(0, 0);
	ssd1306_start_data();	// Initiate transmission of data
	for (uint16_t i = 0; i < 128 * 8 / 4; i++) {
		ssd1306_data_byte(p1);
		ssd1306_data_byte(p2);
		ssd1306_data_byte(p3);
		ssd1306_data_byte(p4);
	}
	ssd1306_stop();	// Finish transmission
}

// ============================================================================
