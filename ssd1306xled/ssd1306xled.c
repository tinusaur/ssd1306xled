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

#include <stdlib.h>
#include <avr/io.h>
#include <avr/pgmspace.h>

#include "ssd1306xled.h"

// ----------------------------------------------------------------------------

const uint8_t ssd1306_init_sequence [] PROGMEM = {	// Initialization Sequence
	0xAE,			// Display OFF (sleep mode)
	0x20, 0b00,		// Set Memory Addressing Mode
					// 00=Horizontal Addressing Mode; 01=Vertical Addressing Mode;
					// 10=Page Addressing Mode (RESET); 11=Invalid
	0xB0,			// Set Page Start Address for Page Addressing Mode, 0-7
	0xC8,			// Set COM Output Scan Direction
	0x00,			// ---set low column address
	0x10,			// ---set high column address
	0x40,			// --set start line address
	0x81, 0x3F,		// Set contrast control register
	0xA1,			// Set Segment Re-map. A0=address mapped; A1=address 127 mapped. 
	0xA6,			// Set display mode. A6=Normal; A7=Inverse
	0xA8, 0x3F,		// Set multiplex ratio(1 to 64)
	0xA4,			// Output RAM to Display
					// 0xA4=Output follows RAM content; 0xA5,Output ignores RAM content
	0xD3, 0x00,		// Set display offset. 00 = no offset
	0xD5,			// --set display clock divide ratio/oscillator frequency
	0xF0,			// --set divide ratio
	0xD9, 0x22,		// Set pre-charge period
	0xDA, 0x12,		// Set com pins hardware configuration		
	0xDB,			// --set vcomh
	0x20,			// 0x20,0.77xVcc
	0x8D, 0x14,		// Set DC-DC enable
	0xAF			// Display ON in normal mode
};

// ============================================================================

// TODO: These functions could become separate library for handling I2C simplified output.
// IDEA: SI2CW - Simplified I2C Writer

// Some code based on "IIC_wtihout_ACK" by http://www.14blog.com/archives/1358

// These definitions are used only internally by the library
// Convenience definitions for PORTB
#define DIGITAL_WRITE_HIGH(PORT) PORTB |= (1 << PORT)
#define DIGITAL_WRITE_LOW(PORT) PORTB &= ~(1 << PORT)

// ----------------------------------------------------------------------------

void ssd1306_xfer_start(void);
void ssd1306_xfer_stop(void);
void ssd1306_send_byte(uint8_t byte);
void ssd1306_send_command(uint8_t command);
void ssd1306_send_data_start(void);
void ssd1306_send_data_stop(void);

// ----------------------------------------------------------------------------

void ssd1306_xfer_start(void)
{
	DIGITAL_WRITE_HIGH(SSD1306_SCL);	// Set to HIGH
	DIGITAL_WRITE_HIGH(SSD1306_SDA);	// Set to HIGH
	DIGITAL_WRITE_LOW(SSD1306_SDA);		// Set to LOW
	DIGITAL_WRITE_LOW(SSD1306_SCL);		// Set to LOW
}

void ssd1306_xfer_stop(void)
{
	DIGITAL_WRITE_LOW(SSD1306_SCL);		// Set to LOW
	DIGITAL_WRITE_LOW(SSD1306_SDA);		// Set to LOW
	DIGITAL_WRITE_HIGH(SSD1306_SCL);	// Set to HIGH
	DIGITAL_WRITE_HIGH(SSD1306_SDA);	// Set to HIGH
}

void ssd1306_send_byte(uint8_t byte)
{
	uint8_t i;
	for (i = 0; i < 8; i++)
	{
		if ((byte << i) & 0x80)
			DIGITAL_WRITE_HIGH(SSD1306_SDA);
		else
			DIGITAL_WRITE_LOW(SSD1306_SDA);
		
		DIGITAL_WRITE_HIGH(SSD1306_SCL);
		DIGITAL_WRITE_LOW(SSD1306_SCL);
	}
	DIGITAL_WRITE_HIGH(SSD1306_SDA);
	DIGITAL_WRITE_HIGH(SSD1306_SCL);
	DIGITAL_WRITE_LOW(SSD1306_SCL);
}

void ssd1306_send_command_start(void) {
	ssd1306_xfer_start();
	ssd1306_send_byte(SSD1306_SA);  // Slave address, SA0=0
	ssd1306_send_byte(0x00);	// write command
}

void ssd1306_send_command_stop(void) {
	ssd1306_xfer_stop();
}

void ssd1306_send_command(uint8_t command)
{
	ssd1306_send_command_start();
	ssd1306_send_byte(command);
	ssd1306_send_command_stop();
}

void ssd1306_send_data_start(void)
{
	ssd1306_xfer_start();
	ssd1306_send_byte(SSD1306_SA);
	ssd1306_send_byte(0x40);	//write data
}

void ssd1306_send_data_stop(void)
{
	ssd1306_xfer_stop();
}

// ============================================================================

void ssd1306_init(void)
{
	DDRB |= (1 << SSD1306_SDA);	// Set port as output
	DDRB |= (1 << SSD1306_SCL);	// Set port as output
	
	for (uint8_t i = 0; i < sizeof (ssd1306_init_sequence); i++) {
		ssd1306_send_command(pgm_read_byte(&ssd1306_init_sequence[i]));
	}
}

void ssd1306_setpos(uint8_t x, uint8_t y)
{
	ssd1306_send_command_start();
	ssd1306_send_byte(0xb0 + y);
	ssd1306_send_byte(((x & 0xf0) >> 4) | 0x10); // | 0x10
/* TODO: Verify correctness */	ssd1306_send_byte((x & 0x0f)); // | 0x01
	ssd1306_send_command_stop();
}

void ssd1306_fill4(uint8_t p1, uint8_t p2, uint8_t p3, uint8_t p4) {
	ssd1306_setpos(0, 0);
	ssd1306_send_data_start();
	for (uint16_t i = 0; i < 128 * 8 / 4; i++) {
		ssd1306_send_byte(p1);
		ssd1306_send_byte(p2);
		ssd1306_send_byte(p3);
		ssd1306_send_byte(p4);
	}
	ssd1306_send_data_stop();
}

// ----------------------------------------------------------------------------

void ssd1306_byte(uint8_t b) {
	ssd1306_send_data_start();
	ssd1306_send_byte(b);
	ssd1306_send_data_stop();
}

// ============================================================================
