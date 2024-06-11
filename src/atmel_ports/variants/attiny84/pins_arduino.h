/*
  pins_arduino.c - pin definitions for the Arduino board
  Stefan Rueger, 2016

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
*/

#ifndef Pins_Arduino_h
#define Pins_Arduino_h

#include <avr/pgmspace.h>

// Atmel ATtiny84
//
//                                         +-\/-+
//                                    Vcc 1|    |14 Gnd
//              (D10, XTAL1, PCINT8) PB0  2|    |13 PA0 (PCINT0, D0=A0, Aref)
//               (D9, XTAL2, PCINT9) PB1  3|    |12 PA1 (PCINT1, D1=A1)
//          (D11 or !Reset, PCINT11) PB3  4|    |11 PA2 (PCINT2, D2=A2)
//         (OC0A, INT0, D8, PCINT10) PB2  5|    |10 PA3 (PCINT3, D3=A3)
//             (OC0B, A7=D7, PCINT7) PA7  6|    |9  PA4 (PCINT4, D4=A4, SCK, SCL, T1)
//  (OC1A, SDA, PICO, A6=D6, PCINT6) PA6  7|    |8  PA5 (PCINT5, D5=A5, POCI, OC1B)
//                                         +----+

#define NUM_DIGITAL_PINS           12 // D11 == !Reset
#define NUM_ANALOG_INPUTS           8
#define analogInputToDigitalPin(p)  ((p) <8? (p): -1)

static const uint8_t CS   = 7;
static const uint8_t PICO = 6;
static const uint8_t POCI = 5;
static const uint8_t SCK  = 4;

static const uint8_t SDA = 6;
static const uint8_t SCL = 4;
#define LED_BUILTIN  what_do_I_know

static const uint8_t A0 = 0;
static const uint8_t A1 = 1;
static const uint8_t A2 = 2;
static const uint8_t A3 = 3;
static const uint8_t A4 = 4;
static const uint8_t A5 = 5;
static const uint8_t A6 = 6;
static const uint8_t A7 = 7;

#define digitalPinToPCICR(p)    ((p) >= 0 && (p) <= 11? (&GIMSK): ((uint8_t *)0))
#define digitalPinToPCICRbit(p) ((p) >= 0 && (p) <=  7? 0: 1)
#define digitalPinToPCMSK(p)    ((p) >= 0 && (p) <=  7? (&PCMSK0): ((p) >= 8 && (p) <= 11)? (&PCMSK1): ((uint8_t *)0))
#define digitalPinToPCMSKbit(p) ((p)>=0 && (p)<=7? (p): (p)==8? 2: (p)==9? 1: (p)==10? 0: 3 /* Pin 11 = Reset */)

#define digitalPinToInterrupt(p) ((p) == 8? 0 : NOT_AN_INTERRUPT)

#define analogPinToChannel(p)   (p)

#define TCCR1A GTCCR

#ifdef ARDUINO_MAIN

// these arrays map port names (e.g. port B) to the
// appropriate addresses for various functions (e.g. reading
// and writing)
const uint16_t PROGMEM port_to_mode_PGM[] = {
	NOT_A_PORT,
	(uint16_t) &DDRA,
	(uint16_t) &DDRB,
};

const uint16_t PROGMEM port_to_output_PGM[] = {
	NOT_A_PORT,
	(uint16_t) &PORTA,
	(uint16_t) &PORTB,
};

const uint16_t PROGMEM port_to_input_PGM[] = {
	NOT_A_PIN,
	(uint16_t) &PINA,
	(uint16_t) &PINB,
};

const uint8_t PROGMEM digital_pin_to_port_PGM[] = {
	PA, // PA0 = A0 = D0
	PA,
	PA,
	PA,
	PA,
	PA,
	PA,
	PA, // PA7 = A7 = D7
	PB, // PB2 = D8
	PB, // PB1 = D9
	PB, // PB0 = D10
	PB, // PB3 = D11 or Reset
};

const uint8_t PROGMEM digital_pin_to_bit_mask_PGM[] = {
	_BV(0),
	_BV(1),
	_BV(2),
	_BV(3),
	_BV(4),
	_BV(5),
	_BV(6),
	_BV(7),
	_BV(2), // PB2 = D8
	_BV(1), // PB1 = D9
	_BV(0), // PB0 = D10
	_BV(3), // PB3 = D11 or Reset
};

const uint8_t PROGMEM digital_pin_to_timer_PGM[] = {
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	TIMER1B,
	TIMER1A,
	TIMER0B,
	TIMER0A,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
};

#endif

#endif
