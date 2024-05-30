/*
  pins_arduino.h - Pin definition functions for Arduino
  Part of Arduino - http://www.arduino.cc/

  Copyright (c) 2007 David A. Mellis

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

#define NUM_DIGITAL_PINS           21 // D0 .. D13 digital only, D14 ... D19 are same as A0 ... A5, D20 is RESET
#define NUM_ANALOG_INPUTS           6
#define analogInputToDigitalPin(p)  ((p) < 6? (p)+14: -1)

#if defined(__AVR_ATmega8__)
#define digitalPinHasPWM(p)         ((p) == 9 || (p) == 10 || (p) == 11)
#else
#define digitalPinHasPWM(p)         ((p) == 3 || (p) == 5 || (p) == 6 || (p) == 9 || (p) == 10 || (p) == 11)
#endif

static const uint8_t CS   = 10;
static const uint8_t PICO = 11;
static const uint8_t POCI = 12;
static const uint8_t SCK  = 13;

static const uint8_t SDA = 18;
static const uint8_t SCL = 19;
#define LED_BUILTIN        13

static const uint8_t A0 = 14;
static const uint8_t A1 = 15;
static const uint8_t A2 = 16;
static const uint8_t A3 = 17;
static const uint8_t A4 = 18;
static const uint8_t A5 = 19;
static const uint8_t A6 = 20; // cannot use A6 and A7 for digital r/w as not associated to a port
static const uint8_t A7 = 21; // and only available on TQFP and MLF square SFD chips

// 0 .. 20 are our 21 digital pins: D20 is !RESET (and not the same as A6, though it's the same number)
#define digitalPinToPCICR(p)    ((p) >= 0 && (p) <= 20? (&PCICR): ((uint8_t *)0))
#define digitalPinToPCICRbit(p) ((p) <= 7? 2: (p) <= 13? 0: 1)
#define digitalPinToPCMSK(p)    ((p) <= 7? (&PCMSK2): (p) <= 13? (&PCMSK0): (p) <= 20? (&PCMSK1): ((uint8_t *)0))
#define digitalPinToPCMSKbit(p) ((p) <= 7? (p): (p) <= 13? (p) - 8: (p) - 14)

#define digitalPinToInterrupt(p)  ((p) == 2? 0: (p) == 3? 1: NOT_AN_INTERRUPT)

#ifdef ARDUINO_MAIN

// On the Arduino board, digital pins are also used for the analog output (software PWM).
//  Analog input pins are a separate set.
//
// Atmel ATMEGA8/168/328 Arduino
//
//                                   +--\/--+
//      (PCINT14, !RESET, D20) PC6  1|      |28  PC5 (D19=A5, ADC5, SCL, PCINT13)
//          (PCINT16, RXD, D0) PD0  2|      |27  PC4 (D18=A4, ADC4, SDA, PCINT12)
//          (PCINT17, TXD, D1) PD1  3|      |26  PC3 (D17=A3, ADC3, PCINT11)
//         (PCINT18, INT0, D2) PD2  4|      |25  PC2 (D16=A2, ADC2, PCINT10)
//   (PCINT19, OC2B, INT1, D3) PD3  5|~+    |24  PC1 (D15=A1, ADC1, PCINT9)
//      (PCINT20, XCK, T0, D4) PD4  6|      |23  PC0 (D14=A0, ADC0, PCINT8)
//                             VCC  7|      |22  GND
//                             GND  8|      |21  AREF
// (PCINT6, XTAL1, TOSC1, n/a) PB6  9|x     |20  AVCC
// (PCINT7, XTAL2, TOSC2, n/a) PB7 10|x     |19  PB5 (D13=LED, SCK, PCINT5)
//     (PCINT21, OC0B, T1, D5) PD5 11|~+    |18  PB4 (D12, POCI, PCINT4)
//   (PCINT22, OC0A, AIN0, D6) PD6 12|~+   ~|17  PB3 (D11, PICO, OC2A, PCINT3)
//         (PCINT23, AIN1, D7) PD7 13|     ~|16  PB2 (D10, !CS, OC1B, PCINT2)
//    (PCINT0, CLKO, ICP1, D8) PB0 14|     ~|15  PB1 (D9, OC1A, PCINT1)
//                                   +------+
//
// D20 = PC6 can be configured as Reset (needed for SPI programming) or additional digital pin
// x used for XTAL, not usable in boards with an external crystal
// ~ PWM
// ~+ indicates the additional PWM pins on the ATmega168 and ATmega328


// these arrays map port names (e.g. port B) to the
// appropriate addresses for various functions (e.g. reading
// and writing)
const uint16_t PROGMEM port_to_mode_PGM[] = {
	NOT_A_PORT,
	NOT_A_PORT,
	(uint16_t) &DDRB,
	(uint16_t) &DDRC,
	(uint16_t) &DDRD,
};

const uint16_t PROGMEM port_to_output_PGM[] = {
	NOT_A_PORT,
	NOT_A_PORT,
	(uint16_t) &PORTB,
	(uint16_t) &PORTC,
	(uint16_t) &PORTD,
};

const uint16_t PROGMEM port_to_input_PGM[] = {
	NOT_A_PORT,
	NOT_A_PORT,
	(uint16_t) &PINB,
	(uint16_t) &PINC,
	(uint16_t) &PIND,
};

const uint8_t PROGMEM digital_pin_to_port_PGM[] = {
	PD, // D0
	PD,
	PD,
	PD,
	PD,
	PD,
	PD,
	PD,
	PB, // D8
	PB,
	PB,
	PB,
	PB,
	PB,
	PC, // D14 = A0
	PC,
	PC,
	PC,
	PC,
	PC,
	PC,
};

const uint8_t PROGMEM digital_pin_to_bit_mask_PGM[] = {
	_BV(0), // D0 = PD0 on port D
	_BV(1),
	_BV(2),
	_BV(3),
	_BV(4),
	_BV(5),
	_BV(6),
	_BV(7),
	_BV(0), // D8 = PB0 on port B
	_BV(1),
	_BV(2),
	_BV(3),
	_BV(4),
	_BV(5),
	_BV(0), // D14 = A0 = PC0 on port C
	_BV(1),
	_BV(2),
	_BV(3),
	_BV(4),
	_BV(5), // D19 = A5
	_BV(6), // D20 = !RESET = PC6 on port C
};

const uint8_t PROGMEM digital_pin_to_timer_PGM[] = {
	NOT_ON_TIMER, // D0, port D
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	// on the ATmega168/328, digital pin 3 has hardware pwm
#if defined(__AVR_ATmega8__)
	NOT_ON_TIMER,
#else
	TIMER2B,
#endif
	NOT_ON_TIMER,
	// on the ATmega168/328, digital pins 5 and 6 have hardware pwm
#if defined(__AVR_ATmega8__)
	NOT_ON_TIMER,
	NOT_ON_TIMER,
#else
	TIMER0B,
	TIMER0A,
#endif
	NOT_ON_TIMER,
	NOT_ON_TIMER, // D8, port B
	TIMER1A,
	TIMER1B,
#if defined(__AVR_ATmega8__)
	TIMER2,
#else
	TIMER2A,
#endif
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER, // D14, port C
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
};

#endif

// These serial port names are intended to allow libraries and architecture-neutral
// sketches to automatically default to the correct port name for a particular type
// of use.  For example, a GPS module would normally connect to SERIAL_PORT_HARDWARE_OPEN,
// the first hardware serial port whose RX/TX pins are not dedicated to another use.
//
// SERIAL_PORT_MONITOR        Port which normally prints to the Arduino Serial Monitor
//
// SERIAL_PORT_USBVIRTUAL     Port which is USB virtual serial
//
// SERIAL_PORT_LINUXBRIDGE    Port which connects to a Linux system via Bridge library
//
// SERIAL_PORT_HARDWARE       Hardware serial port, physical RX & TX pins.
//
// SERIAL_PORT_HARDWARE_OPEN  Hardware serial ports which are open for use.  Their RX & TX
//                            pins are NOT connected to anything by default.
#define SERIAL_PORT_MONITOR   Serial
#define SERIAL_PORT_HARDWARE  Serial

#endif
