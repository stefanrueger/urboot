/*
  pins_arduino.c - pin definitions for the Arduino board
  Part of Arduino / Wiring Lite

  Digispark-pro details provided by Stefan Rueger

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

  Created 19-06-2016 Stefan Rueger
*/

#ifndef Pins_Arduino_h
#define Pins_Arduino_h

#include <avr/pgmspace.h>

// Atmel ATtiny167
// see also https://digistump.com/wiki/digispark/tutorials/pinguidepro
// http://digispark.s3.amazonaws.com/DigisparkProDiagram2.png
// Note that this file swaps D3 and D4 around and makes A4 what was A3 (much more logical!)
//
//                                                   +--\/--+
//            (RXLIN, RXD, ADC0, PCINT0, A6=D6) PA0 1|     ~|20 PB0 (D0, PCINT8, OC1AU, DI, SDA)
//            (TXLIN, TXD, ADC1, PCINT1, A7=D7) PA1 2|     ~|19 PB1 (D1=LED, PCINT9, OC1BU, DO)
//        (CIPO, DO, OC0A, ADC2, PCINT2, A8=D8) PA2 3|~    ~|18 PB2 (D2, PCINT10, OC1AV, USCK, SCL)
//            (INT1, ISRC, ADC3, PCINT3, A9=D9) PA3 4|     ~|17 PB3 (D3, USB-, PCINT11, OC1BV)
//                                             AVCC 5|      |16 GND
//                                             AGND 6|      |15 VCC
// (COPI, SDA, DI, ICP1, ADC4, PCINT4, A10=D10) PA4 7|     x|14 PB4 (n/a, PCINT12, OC1AW, XTAL1, CLKI)
//  (SCK, SCL, USCK, T1, ADC5, PCINT5, A11=D11) PA5 8|     x|13 PB5 (n/a, PCINT13, ADC8, OC1BW, XTAL2, CLKO)
//           (!CS, AIN0, ADC6, PCINT6, A12=D12) PA6 9|     ~|12 PB6 (D4=A4, USB+, PCINT14, ADC9, OC1AX, INT0)
//     (AREF, XREF, AIN1, ADC7, PCINT7, A5=D5) PA7 10|     ~|11 PB7 (D13=A13=n/a, PCINT15, ADC10, OC1BX, !RESET, dW)
//                                                   +------+
// D0  = PB0
// D1  = PB1
// D2  = PB2
// D3  = PB3
// D4  = PB6
// D5  = PA7
// D6  = PA0
// D7  = PA1
// D8  = PA2
// D9  = PA3
// D10 = PA4
// D11 = PA5
// D12 = PA6
// D13 = PB7

#define NUM_DIGITAL_PINS     14 // D0, ..., D13
#define NUM_ANALOG_INPUTS    11 // A4, ..., A13 (that's 10, but ADC8 is used for XTAL and therefore not usable)

// map x from Ax to the digital pin number
#define analogInputToDigitalPin(p)  ((p) >= 4 && (p)<=13? (p): -1)

#define digitalPinHasPWM(p)         (((p) >= 0 && (p) <= 4) || (p) == 13 || (p) == 8)

static const uint8_t CS   = 12;
static const uint8_t COPI = 10;
static const uint8_t CIPO =  8;
static const uint8_t SCK  = 11;

static const uint8_t SDA  =  0;
static const uint8_t SCL  =  2;

#define LED_BUILTIN          1


// Ax constants can only be used for analogRead() - map numbers above NUM_DIGITAL_PINS
// keep the brackets - a meta-program converts the static const variables to #defines
static const uint8_t A4  = (14+9);
static const uint8_t A5  = (14+7);
static const uint8_t A6  = (14+0);
static const uint8_t A7  = (14+1);
static const uint8_t A8  = (14+2);
static const uint8_t A9  = (14+3);
static const uint8_t A10 = (14+4);
static const uint8_t A11 = (14+5);
static const uint8_t A12 = (14+6);
static const uint8_t A13 = (14+10);

#define analogPinToChannel(p) ((p)-14)

#define digitalPinToPCICR(p)    (&PCICR)
#define digitalPinToPCICRbit(p) ( ((p) >= 5 && (p) <= 12)? 0 /* PCIE0 */: 1 /* PCIE1 */ )
#define digitalPinToPCMSK(p)    ( ((p) >= 5 && (p) <= 12)? (&PCMSK0): (&PCMSK1) )
#define digitalPinToPCMSKbit(p) ( (p) >= 0 && (p) <= 3?  (p):     \
                                  (p) >= 6 && (p) <= 13? (p) - 6: \
                                  (p) == 4? 6: 7 /* pin 5 */)

#define digitalPinToInterrupt(p) ((p) == 4? 0: (p) == 9? 1: NOT_AN_INTERRUPT)


// below is from Digistump (smr)
// Core Configuration (used to be in core_build_options.h)

// If Software Serial communications doesn't work, run the TinyTuner sketch provided with the core to give you a calibrated OSCCAL value.
// Change the value here with the tuned value. By default this option uses the default value which the compiler will optimise out.
#define TUNED_OSCCAL_VALUE                        OSCCAL
// eg,
//#define TUNED_OSCCAL_VALUE                        0x57

// Choosing not to initialise saves power and flash. 1 = initialise.
#define INITIALIZE_ANALOG_TO_DIGITAL_CONVERTER    1
#define INITIALIZE_SECONDARY_TIMERS               0

#define TIMER_TO_USE_FOR_MILLIS                   0

#define HAVE_BOOTLOADER                           1

/*
  Where to put the software serial? (Arduino Digital pin numbers)
*/
// WARNING, if using software, TX is on AIN0, RX is on AIN1. Comparator is favoured to use its interrupt for the RX pin.
#define USE_SOFTWARE_SERIAL						  0
// Please define the port on which the analog comparator is found.
#define ANALOG_COMP_DDR				  DDRA
#define ANALOG_COMP_PORT			  PORTA
#define ANALOG_COMP_PIN				  PINA
#define ANALOG_COMP_AIN0_BIT			  6
#define ANALOG_COMP_AIN1_BIT			  7


//  Analog reference bit masks

// VCC used as analog reference, disconnected from PA0 (AREF)
#undef  DEFAULT
#define DEFAULT   0

// External voltage reference at PA0 (AREF) pin, internal reference turned off
#undef  EXTERNAL
#define EXTERNAL 1

// Internal 1.1V voltage reference
#undef  INTERNAL
#define INTERNAL 2



#ifdef ARDUINO_MAIN

// these arrays map port names (e.g. port B) to the
// appropriate addresses for various functions (e.g. reading
// and writing)
const uint16_t PROGMEM port_to_mode_PGM[] = {
	NOT_A_PORT,
	(uint16_t)&DDRA,
	(uint16_t)&DDRB,
};

const uint16_t PROGMEM port_to_output_PGM[] = {
	NOT_A_PORT,
	(uint16_t)&PORTA,
	(uint16_t)&PORTB,
};

const uint16_t PROGMEM port_to_input_PGM[] = {
	NOT_A_PORT,
	(uint16_t)&PINA,
	(uint16_t)&PINB,
};

const uint8_t PROGMEM digital_pin_to_port_PGM[] = {
	PB, /* 0 */
	PB,
	PB, /* 2 */
	PB, /* 3 */
	PB, /* 4 */
	PA,
	PA,
	PA,
	PA,
	PA,
	PA, /* 10 */
	PA,
	PA,
	PB, /* RESET */
};

const uint8_t PROGMEM digital_pin_to_bit_mask_PGM[] = {
	_BV(0), /* 0 */
	_BV(1),
	_BV(2), /* 2 */
	_BV(3), /* 3 */
	_BV(6), /* 4 */
	_BV(7),
	_BV(0),
	_BV(1),
	_BV(2),
	_BV(3),
	_BV(4), /* 10 */
	_BV(5),
	_BV(6),
	_BV(7),
};

const uint8_t PROGMEM digital_pin_to_timer_PGM[] = {
	TIMER1A,
	TIMER1B,
	TIMER1A,
	TIMER1B,
	TIMER1A,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	TIMER0A,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	TIMER1B,
};

#endif

#endif
