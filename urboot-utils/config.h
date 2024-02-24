/*
 * config.h
 *
 * Compatibility functions for urclock IDE
 *
 * published under GNU General Public License, version 3 (GPL-3.0)
 * author Stefan Rueger
 * v 1.0
 * 18.12.2022
 */

#ifndef CONFIG_H
#define CONFIG_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <limits.h>
#include <math.h>

#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
#include <util/atomic.h>
#include <util/crc16.h>
#include <util/parity.h>
#include <util/delay.h>
#include <avr/eeprom.h>

#ifndef sprintf_q
#define sprintf_q(buf, fmt, ...) sprintf_P(buf, PSTR(fmt), ## __VA_ARGS__)
#define printf_q(fmt, ...) printf_P(PSTR(fmt), ## __VA_ARGS__)
#endif

#define DEBUG_TIMING 0

// Provide your own I/O functions for your system
#define stdio_flush()
#define printf_P(x, ...)

#endif
