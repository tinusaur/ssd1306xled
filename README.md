# SSD1306xLED

SSD1306xLED - Library for the SSD1306 based OLED/PLED displays and ATtiny85 microcontrollers.

SSD1306xLED is a small C library for working with the SSD1306 display driver to control dot-matrix OLED/PLED 128x64 displays. The library is intended to be used with the Tinusaur boards but should work with any other board based on ATtiny85 or a similar microcontroller.

    Copyright (c) 2021 Tinusaur (https://tinusaur.com). All rights reserved.
    Distributed as open source under the MIT License (see the LICENSE.txt file).
    Please, retain in your work a link to the Tinusaur project website.

- SSD1306xLED source code:   https://gitlab.com/tinusaur/ssd1306xled  
  GitHub mirror at: https://github.com/tinusaur/ssd1306xled
- SSD1306xLED project page:  http://tinusaur.org/projects/ssd1306xled

NOTE: This project was developed for and tested on the ATtiny85 microcontrollers. It might or might not work in other environments.

- Tinusaur website: https://tinusaur.org
- Tinusaur on Twitter: https://twitter.com/tinusaur
- Tinusaur on Facebook: https://www.facebook.com/tinusaur

-------------------------------------------------------------------------------

Folders and modules:

- ssd1306xled          - the SSD1306xLED source code library
	- font6x8.h          - small 6x8 font
	- font8x16.h         - large 8x16 font
	- ssd1306xled.c      - library implementation
	- ssd1306xled.h      - library headers
	- ssd1306xled8x16.c  - large font 8x16 implementation
	- ssd1306xled8x16.h  - large font 8x16 headers
- ssd1306xled_cn       - additional functions for handling some Chinese characters (currently does not work)
- ssd1306xled_test     - simple testing program for the library
	- img0_128x64c1.h    - sample bitmap image 128x64
	- img1_128x64c1.h    - sample bitmap image 128x64
	- main.c             - sample code using the library
- ssd1306xled_test_cn  - test for Chinese functions (currently does not work)

## How does it works?

	TO-DO

## Compatibility

- MCU: This library was developed and tested with the following microcontrollers: ATtiny85, ATtiny45, ATtiny25.
- CPU FREQUENCY: This library has support and was tested work at 1 MHz and 8 MHz CPU clock mode.
