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

#ifndef NUM2STR_H
#define NUM2STR_H

// ----------------------------------------------------------------------------

#include <stdint.h>

// ----------------------------------------------------------------------------

#define USINT2DECASCII_MAX_DIGITS 5

// ----------------------------------------------------------------------------

uint8_t usint2decascii(uint16_t, char *);

// ----------------------------------------------------------------------------

#endif

// ----------------------------------------------------------------------------
