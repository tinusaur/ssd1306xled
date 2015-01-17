/*
 * NUM2STR - Functions to handle the conversion of numeric vales to strings.
 *
 * @created	2014-12-18
 * @author	Neven Boyanov
 * @version	Last Modified 2015-01-17
 *
 * This is part of the TinyAVRLib project.
 *
 * Copyright (c) 2015 Neven Boyanov, Tinusaur Team. All Rights Reserved.
 * Distributed as open source software under MIT License, see LICENSE.txt file.
 * Please, as a favour, retain the link http://tinusaur.org to The Tinusaur Project.
 *
 * Source code available at: https://bitbucket.org/tinusaur/tinyavrlib
 *
 */

// ----------------------------------------------------------------------------

#include "num2str.h"

// ----------------------------------------------------------------------------

// NOTE: This implementation is borrowed from the LCDDDD library.
// Original source code at: https://bitbucket.org/boyanov/avr/src/default/lcdddd/src/lcdddd/lcdddd.h

uint8_t usint2decascii(uint16_t num, char* buffer)
{
	const unsigned short powers[] = { 10000u, 1000u, 100u, 10u, 1u }; // The "const unsigned short" combination gives shortest code.
	char digit; // "digit" is stored in a char array, so it should be of type char.
	uint8_t digits = USINT2DECASCII_MAX_DIGITS - 1;
	for (uint8_t pos = 0; pos < 5; pos++) // "pos" is index in array, so should be of type int.
	{
		digit = 0;
		while (num >= powers[pos])
		{
			digit++;
			num -= powers[pos];
		}

		// ---- CHOOSE (1), (2) or (3) ----

		// CHOICE (1) Fixed width, zero padded result.
		/*
		buffer[pos] = digit + '0';	// Convert to ASCII
		*/

		// CHOICE (2) Fixed width, zero padded result, digits offset.
		/*
		buffer[pos] = digit + '0';	// Convert to ASCII
		// Note: Determines the offset of the first significant digit.
		if (digits == -1 && digit != 0) digits = pos;
		// Note: Could be used for variable width, not padded, left aligned result.
		*/

		// CHOICE (3) Fixed width, space (or anything else) padded result, digits offset.
		// Note: Determines the offset of the first significant digit.
		// Note: Could be used for variable width, not padded, left aligned result.
		if (digits == USINT2DECASCII_MAX_DIGITS - 1)
		{
			if (digit == 0)
			{
				if (pos < USINT2DECASCII_MAX_DIGITS - 1)	// Check position, so single "0" will be handled properly.
					digit = -16;	// Use: "-16" for space (' '), "-3" for dash/minus ('-'), "0" for zero ('0'), etc. ...
			}
			else
			{
				digits = pos;
			}
		}
		buffer[pos] = digit + '0';	// Convert to ASCII
		
	}
	
	// NOTE: The resulting ascii text should not be terminated with '\0' here.
	//       The provided buffer maybe part of a larger text in both directions.
	
	return digits;
}

// ----------------------------------------------------------------------------

