/**
 * SSD1306xLED - Library for the SSD1306 based OLED/PLED 128x64 displays
 * @author Neven Boyanov
 * This is part of the Tinusaur/SSD1306xLED project.
 * ----------------------------------------------------------------------------
 *  Copyright (c) 2023 Tinusaur (https://tinusaur.com). All rights reserved.
 *  Distributed as open source under the MIT License (see the LICENSE.txt file)
 *  Please, retain in your work a link to the Tinusaur project website.
 * ----------------------------------------------------------------------------
 * Source code available at: https://gitlab.com/tinusaur/ssd1306xled
 */

#ifndef SSD1306XLED_H
#define SSD1306XLED_H

// ============================================================================

// -----(+)-------------------->-----> [Vcc]	Pin 1 on the SSD1306 display board
// -----(-)-------------------->-----> [GND]	Pin 2 on the SSD1306 display board
#define SSD1306_SCL		PB2   // ----> [SCL]	Pin 3 on the SSD1306 display board
#define SSD1306_SDA		PB0   // ----> [SDA]	Pin 4 on the SSD1306 display board

#define SSD1306_SADDR	0x78  // Display IC2 slave address, default 0x78

// ----------------------------------------------------------------------------

#define ssd1306_clear() ssd1306_fill(0)
#define ssd1306_fill2(p1, p2) ssd1306_fill4(p1, p2, p1, p2)

// ----------------------------------------------------------------------------

void ssd1306_start_command(void);
void ssd1306_start_data(void);
void ssd1306_byte(uint8_t);
void ssd1306_stop(void);

// ----------------------------------------------------------------------------

void ssd1306_init(void);
void ssd1306_setpos(uint8_t, uint8_t);
void ssd1306_fill(uint8_t);
void ssd1306_fill4(uint8_t, uint8_t, uint8_t, uint8_t);

// ============================================================================

#endif
