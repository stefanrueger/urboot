/*
  pins_arduino.c - pin definitions for the Arduino board
  Part of Arduino / Wiring Lite

  Copyright (c) 2005 David A. Mellis

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General
  Public License along with this library; if not, write to the
  Free Software Foundation, Inc., 59 Temple Place, Suite 330,
  Boston, MA  02111-1307  USA

  Modified 12-06-2016 to conform to other pins_arduino.h S.Rueger
  Modified 28-08-2009 for attiny84 R.Wiersma
  Modified 09-10-2009 for attiny45 A.Saporetti
*/

#ifndef Pins_Arduino_h
#define Pins_Arduino_h

#include <avr/pgmspace.h>

// Defines to make D4 support PWM
// See for more info: http://forums.adafruit.com/viewtopic.php?f=52&t=43951

//#define TCCR1A GTCCR
//#define WGM10  PWM1B

// Atmel ATtiny45/85 ARDUINO
//
//                                                       +-\/-+
//               (PCINT5, !RESET, ADC0, dW, D5=A0) PB5  1|    |8  VCC
// (PCINT3, XTAL1, CLKI, !OC1B, ADC3, USB+, D3=A3) PB3  2|-   |7  PB2 (D2=A1, SCK, USCK, SCL, ADC1, T0, INT0, PCINT2)
//  (PCINT4, XTAL2, CLKO, OC1B, ADC2, USB-, D4=A2) PB4  3|~  ~|6  PB1 (D1=LED, CIPO, DO, AIN1, OC0B, OC1A, PCINT1)
//                                                 GND  4|   -|5  PB0 (D0, COPI, DI, SDA, AIN0, OC0A, !OC1A, AREF, PCINT0
//                                                       +----+

#define NUM_DIGITAL_PINS            6
#define NUM_ANALOG_INPUTS           4
// map x from Ax to digital pin number
#define analogInputToDigitalPin(p)  ((p) == 0? 5: (p) == 1? 2: (p) == 2? 4: (p) == 3? 3: -1)

static const uint8_t CS   = 3;
static const uint8_t COPI = 0;
static const uint8_t CIPO = 1;
static const uint8_t SCK  = 2;

static const uint8_t SDA =  0;
static const uint8_t SCL =  2;

#define LED_BUILTIN 	    1

// map to above NUM_DIGITAL_PINS, but in a way it's easy to compute the channel number
static const uint8_t A0 = 6;
static const uint8_t A1 = 7;
static const uint8_t A2 = 8;
static const uint8_t A3 = 9;

#define digitalPinToPCICR(p)     (((p) >= 0 && (p) <= 5)? (&GIMSK): ((uint8_t *)0))
#define digitalPinToPCICRbit(p)  (PCIE)
#define digitalPinToPCMSK(p)     (((p) <= 5)? (&PCMSK): ((uint8_t *)0))
#define digitalPinToPCMSKbit(p)  (p)

#define digitalPinToInterrupt(p) ((p) == 2? 0: NOT_AN_INTERRUPT)

#define analogPinToChannel(p)    ((p) < 6? (p): (p)-6)

#define TCCR1A GTCCR

#ifdef ARDUINO_MAIN

void initVariant() {
	GTCCR |= (1 << PWM1B);
}

// these arrays map port names (e.g. port B) to the
// appropriate addresses for various functions (e.g. reading
// and writing)
const uint16_t PROGMEM port_to_mode_PGM[] = {
	NOT_A_PORT,
	NOT_A_PORT,
	(uint16_t) &DDRB,
};

const uint16_t PROGMEM port_to_output_PGM[] = {
	NOT_A_PORT,
	NOT_A_PORT,
	(uint16_t) &PORTB,
};

const uint16_t PROGMEM port_to_input_PGM[] = {
	NOT_A_PIN,
	NOT_A_PIN,
	(uint16_t) &PINB,
};

const uint8_t PROGMEM digital_pin_to_port_PGM[] = {
	PB, // 0
	PB,
	PB,
	PB,
	PB,
	PB, // 5
};

const uint8_t PROGMEM digital_pin_to_bit_mask_PGM[] = {
	_BV(0),
	_BV(1),
	_BV(2),
	_BV(3),
	_BV(4),
	_BV(5),
};

const uint8_t PROGMEM digital_pin_to_timer_PGM[] = {
	TIMER0A, /* OC0A */
	TIMER0B,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	TIMER1B,
	NOT_ON_TIMER,
};

#endif

#endif
