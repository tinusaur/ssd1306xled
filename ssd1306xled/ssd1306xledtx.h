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

#ifndef SSD1306XLEDTX_H
#define SSD1306XLEDTX_H

// ============================================================================

#define ssd1306_char(c) ssd1306_char_font6x8(c)
#define ssd1306_string(s) ssd1306_string_font6x8(s)
#define ssd1306_numdec(n) ssd1306_numdec_font6x8(n)
#define ssd1306_numdecp(n) ssd1306_numdecp_font6x8(n)

// ----------------------------------------------------------------------------

extern uint8_t *ssd1306xled_font6x8;
extern uint8_t *ssd1306xled_font8x16;

// ----------------------------------------------------------------------------

void ssd1306tx_init(void);

void ssd1306_char_font6x8(char ch);
void ssd1306_string_font6x8(char *s);
void ssd1306_numdec_font6x8(uint16_t num);
void ssd1306_numdecp_font6x8(uint16_t num);

void ssd1306_string_font8x16xy(uint8_t x, uint8_t y, const char s[]);

// ============================================================================

#endif
