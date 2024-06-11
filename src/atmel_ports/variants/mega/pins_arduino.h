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

/*
 * The Arduino Mega does not use all the 86 I/O pins of the ATmega2560
 * processor; consider creating a different file for your own extended I/O
 * definitions if you have a PCB that utilises these extra pins, see
 * https://stackoverflow.com/questions/21417521/arduino-use-all-ports-atmega2560
 */

#define NUM_DIGITAL_PINS            70
#define NUM_ANALOG_INPUTS           16
#define analogInputToDigitalPin(p)  ((p) < 16? (p)+54: -1)
#define digitalPinHasPWM(p)         (((p) >= 2 && (p) <= 13) || ((p) >= 44 && (p)<= 46))

static const uint8_t CS   = 53;
static const uint8_t PICO = 51;
static const uint8_t POCI = 50;
static const uint8_t SCK  = 52;

static const uint8_t SDA = 20;
static const uint8_t SCL = 21;
#define LED_BUILTIN 13

static const uint8_t A0 = 54;
static const uint8_t A1 = 55;
static const uint8_t A2 = 56;
static const uint8_t A3 = 57;
static const uint8_t A4 = 58;
static const uint8_t A5 = 59;
static const uint8_t A6 = 60;
static const uint8_t A7 = 61;
static const uint8_t A8 = 62;
static const uint8_t A9 = 63;
static const uint8_t A10 = 64;
static const uint8_t A11 = 65;
static const uint8_t A12 = 66;
static const uint8_t A13 = 67;
static const uint8_t A14 = 68;
static const uint8_t A15 = 69;

/*
 * A majority of the pins are *not* PCINTs, so be warned (i.e. you cannot use them as receive pins for
 * dyss/yass libraries). Out of the 24 pins that have PCINTs 5 are unused in this PCB design (PJ2...PJ6),
 * this leaves 19 PCINT pins: 0, 15, 14, 10, 11, 12, 13,  50, 51, 52, 53,  62, 63, 64, 65, 66, 67, 68, 69
 */

#define digitalPinToPCICR(p)    ( (((p) >= 10) && ((p) <= 13)) || (((p) >= 50) && ((p) <= 53)) || \
                                  (p) == 0 || (p) == 15 || (p) == 14 || \
                                  (((p) >= 62) && ((p) <= 69))? (&PCICR): ((uint8_t *)0) )

#define digitalPinToPCICRbit(p) ( (((p) >= 10) && ((p) <= 13)) || (((p) >= 50) && ((p) <= 53))? 0: \
                                  (p) == 0 || (p) == 15 || (p) == 14? 1: \
                                  ((p) >= 62) && ((p) <= 69)? 2: 0 )

#define digitalPinToPCMSK(p)    ( (((p) >= 10) && ((p) <= 13)) || (((p) >= 50) && ((p) <= 53))? (&PCMSK0): \
                                  (p) == 0 || (p) == 15 || (p) == 14? (&PCMSK1): \
                                  (((p) >= 62) && ((p) <= 69))? (&PCMSK2): ((uint8_t *)0) )

#define digitalPinToPCMSKbit(p) ( (((p) >= 10) && ((p) <= 13))? ((p) - 6): \
                                  ((p) == 50)? 3: \
                                  ((p) == 51)? 2: \
                                  ((p) == 52)? 1: \
                                  ((p) == 53)? 0: \
                                  ((p) ==  0)? 0: \
                                  ((p) == 15)? 1: \
                                  ((p) == 14)? 2: \
                                  (((p) >= 62) && ((p) <= 69))? ((p) - 62): 0 )

#define digitalPinToInterrupt(p) ((p) == 2? 0: ((p) == 3? 1: ((p) >= 18 && (p) <= 21? 23 - (p): NOT_AN_INTERRUPT)))

#ifdef ARDUINO_MAIN

/*
 * ATmega640/1280/2560 pinout
 *
 *       0
 *       0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6
 *       1 9 9 9 9 9 9 9 9 9 9 8 8 8 8 8 8 8 8 8 8 7 7 7 7
 *       | | | | | | | | | | | | | | | | | | | | | | | | |
 *      +------------------------------------------------+
 *  ~1 -|                                                |- 75
 *   2 -|  O                                             |- 74
 *   3 -|                                                |- 73
 *   4 -|                                                |- 72
 *  ~5 -|                                                |- 71
 *  ~6 -|                                                |- 70
 *  ~7 -|                                                |- 69
 *   8 -|                                                |- 68
 *   9 -|                                                |- 67
 *  10 -|                                                |- 66
 *  11 -|                                                |- 65
 *  12 -|                                                |- 64
 *  13 -|                                                |- 63
 *  14 -|                                                |- 62
 * ~15 -|                                                |- 61
 * ~16 -|                                                |- 60
 * ~17 -|                                                |- 59
 * ~18 -|                                                |- 58
 *  19 -|                                                |- 57
 *  20 -|                                                |- 56
 *  21 -|                                                |- 55
 *  22 -|                                                |- 54
 * ~23 -|                                                |- 53
 * ~24 -|                                                |- 52
 * ~25 -|                                                |- 51
 *      +------------------------------------------------+
 *       | | | | | | | | | | | | | | | | | | | | | | | | |
 *       6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0
 *       2 2 2 2 3 3 3 3 3 3 3 3 3 3 4 4 4 4 4 4 4 4 4 4 5
 *       ~                       ~ ~ ~
 *
 *
 *  ~1 PG5 = D4, OC0B
 *   2 PE0 = D0 (RX0), RXD0, PCINT8
 *   3 PE1 = D1 (TX0), TXD0
 *   4 PE2 = n/a, XCK0, AIN0
 *  ~5 PE3 = D5, OC3A, AIN1
 *  ~6 PE4 = D2, OC3B, INT4
 *  ~7 PE5 = D3, OC3C, INT5
 *   8 PE6 = n/a, T3, INT6
 *   9 PE7 = n/a, CLKO, ICP3, INT7
 *  10 VCC
 *  11 GND
 *  12 PH0 = D17 (RX2), RXD2
 *  13 PH1 = D16 (TX2), TXD2
 *  14 PH2 = n/a, XCK2
 * ~15 PH3 = D6, OC4A
 * ~16 PH4 = D7, OC4B
 * ~17 PH5 = D8, OC4C
 * ~18 PH6 = D9, OC2B
 *  19 PB0 = D53 (CS), CS, PCINT0
 *  20 PB1 = D52 (SCK), SCK, PCINT1
 *  21 PB2 = D51 (PICO), PICO, PCINT2
 *  22 PB3 = D50 (POCI), POCI, PCINT3
 * ~23 PB4 = D10, OC2A, PCINT4
 * ~24 PB5 = D11, OC1A, PCINT5
 * ~25 PB6 = D12, OC1B, PCINT6
 * ~26 PB7 = D13, OC0A, OC1C, PCINT7
 *  27 PH7 = n/a, T4
 *  28 PG3 = n/a, TOSC2
 *  29 PG4 = n/a, TOSC1
 *  30 RESET
 *  31 VCC
 *  32 GND
 *  33 XTAL2
 *  34 XTAL1
 *  35 PL0 = D49, ICP4
 *  36 PL1 = D48, ICP5
 *  37 PL2 = D47, T5
 * ~38 PL3 = D46, OC5A
 * ~39 PL4 = D45, OC5B
 * ~40 PL5 = D44, OC5C
 *  41 PL6 = D43
 *  42 PL7 = D42
 *  43 PD0 = D21 (SCL), SCL, INT0
 *  44 PD1 = D20 (SDA), SDA, INT1
 *  45 PD2 = D19 (RX1), RXD1, INT2
 *  46 PD3 = D18 (TX1), TXD1, INT3
 *  47 PD4 = n/a, ICP1
 *  48 PD5 = n/a, XCK1
 *  49 PD6 = n/a, T1
 *  50 PD7 = D38, T0
 *  51 PG0 = D41, WR
 *  52 PG1 = D40, RD
 *  53 PC0 = D37, A8
 *  54 PC1 = D36, A9
 *  55 PC2 = D35, A10
 *  56 PC3 = D34, A11
 *  57 PC4 = D33, A12
 *  58 PC5 = D32, A13
 *  59 PC6 = D31, A14
 *  60 PC7 = D30, A15
 *  61 VCC
 *  62 GND
 *  63 PJ0 = D15 (RX3), RXD3, PCINT9
 *  64 PJ1 = D14 (TX3), TXD3, PCINT10
 *  65 PJ2 = n/a, XCK3, PCINT11
 *  66 PJ3 = n/a, PCINT12
 *  67 PJ4 = n/a, PCINT13
 *  68 PJ5 = n/a, PCINT14
 *  69 PJ6 = n/a, PCINT15
 *  70 PG2 = D39, ALE
 *  71 PA7 = D29, AD7
 *  72 PA6 = D28, AD6
 *  73 PA5 = D27, AD5
 *  74 PA4 = D26, AD4
 *  75 PA3 = D25, AD3
 *  76 PA2 = D24, AD2
 *  77 PA1 = D23, AD1
 *  78 PA0 = D22, AD0
 *  79 PJ7 = n/a
 *  80 VCC
 *  81 GND
 *  82 PK7 = A15, ADC15, PCINT23
 *  83 PK6 = A14, ADC14, PCINT22
 *  84 PK5 = A13, ADC13, PCINT21
 *  85 PK4 = A12, ADC12, PCINT20
 *  86 PK3 = A11, ADC11, PCINT19
 *  87 PK2 = A10, ADC10, PCINT18
 *  88 PK1 = A9, ADC9, PCINT17
 *  89 PK0 = A8, ADC8, PCINT16
 *  90 PF7 = A7, ADC7
 *  91 PF6 = A6, ADC6
 *  92 PF5 = A5, ADC5, TMS
 *  93 PF4 = A4, ADC4, TMK
 *  94 PF3 = A3, ADC3
 *  95 PF2 = A2, ADC2
 *  96 PF1 = A1, ADC1
 *  97 PF0 = A0, ADC0
 *  98 AREF
 *  99 GND
 * 100 AVCC
 *
 * and again, by signal or port name
 *
 *  98 AREF
 * 100 AVCC
 *  11 GND
 *  32 GND
 *  62 GND
 *  81 GND
 *  99 GND
 *  78 PA0 = D22, AD0
 *  77 PA1 = D23, AD1
 *  76 PA2 = D24, AD2
 *  75 PA3 = D25, AD3
 *  74 PA4 = D26, AD4
 *  73 PA5 = D27, AD5
 *  72 PA6 = D28, AD6
 *  71 PA7 = D29, AD7
 *  19 PB0 = D53 (CS), CS, PCINT0
 *  20 PB1 = D52 (SCK), SCK, PCINT1
 *  21 PB2 = D51 (PICO), PICO, PCINT2
 *  22 PB3 = D50 (POCI), POCI, PCINT3
 * ~23 PB4 = D10, OC2A, PCINT4
 * ~24 PB5 = D11, OC1A, PCINT5
 * ~25 PB6 = D12, OC1B, PCINT6
 * ~26 PB7 = D13, OC0A, OC1C, PCINT7
 *  53 PC0 = D37, A8
 *  54 PC1 = D36, A9
 *  55 PC2 = D35, A10
 *  56 PC3 = D34, A11
 *  57 PC4 = D33, A12
 *  58 PC5 = D32, A13
 *  59 PC6 = D31, A14
 *  60 PC7 = D30, A15
 *  43 PD0 = D21 (SCL), SCL, INT0
 *  44 PD1 = D20 (SDA), SDA, INT1
 *  45 PD2 = D19 (RX1), RXD1, INT2
 *  46 PD3 = D18 (TX1), TXD1, INT3
 *  47 PD4 = n/a, ICP1
 *  48 PD5 = n/a, XCK1
 *  49 PD6 = n/a, T1
 *  50 PD7 = D38, T0
 *   2 PE0 = D0 (RX0), RXD0, PCINT8
 *   3 PE1 = D1 (TX0), TXD0
 *   4 PE2 = n/a, XCK0, AIN0
 *  ~5 PE3 = D5, OC3A, AIN1
 *  ~6 PE4 = D2, OC3B, INT4
 *  ~7 PE5 = D3, OC3C, INT5
 *   8 PE6 = n/a, T3, INT6
 *   9 PE7 = n/a, CLKO, ICP3, INT7
 *  97 PF0 = A0, ADC0
 *  96 PF1 = A1, ADC1
 *  95 PF2 = A2, ADC2
 *  94 PF3 = A3, ADC3
 *  93 PF4 = A4, ADC4, TMK
 *  92 PF5 = A5, ADC5, TMS
 *  91 PF6 = A6, ADC6
 *  90 PF7 = A7, ADC7
 *  51 PG0 = D41, WR
 *  52 PG1 = D40, RD
 *  70 PG2 = D39, ALE
 *  28 PG3 = n/a, TOSC2
 *  29 PG4 = n/a, TOSC1
 *  ~1 PG5 = D4, OC0B
 *  12 PH0 = D17 (RX2), RXD2
 *  13 PH1 = D16 (TX2), TXD2
 *  14 PH2 = n/a, XCK2
 * ~15 PH3 = D6, OC4A
 * ~16 PH4 = D7, OC4B
 * ~17 PH5 = D8, OC4C
 * ~18 PH6 = D9, OC2B
 *  27 PH7 = n/a, T4
 *  63 PJ0 = D15 (RX3), RXD3, PCINT9
 *  64 PJ1 = D14 (TX3), TXD3, PCINT10
 *  65 PJ2 = n/a, XCK3, PCINT11
 *  66 PJ3 = n/a, PCINT12
 *  67 PJ4 = n/a, PCINT13
 *  68 PJ5 = n/a, PCINT14
 *  69 PJ6 = n/a, PCINT15
 *  79 PJ7 = n/a
 *  89 PK0 = A8, ADC8, PCINT16
 *  88 PK1 = A9, ADC9, PCINT17
 *  87 PK2 = A10, ADC10, PCINT18
 *  86 PK3 = A11, ADC11, PCINT19
 *  85 PK4 = A12, ADC12, PCINT20
 *  84 PK5 = A13, ADC13, PCINT21
 *  83 PK6 = A14, ADC14, PCINT22
 *  82 PK7 = A15, ADC15, PCINT23
 *  35 PL0 = D49, ICP4
 *  36 PL1 = D48, ICP5
 *  37 PL2 = D47, T5
 * ~38 PL3 = D46, OC5A
 * ~39 PL4 = D45, OC5B
 * ~40 PL5 = D44, OC5C
 *  41 PL6 = D43
 *  42 PL7 = D42
 *  30 RESET
 *  10 VCC
 *  31 VCC
 *  61 VCC
 *  80 VCC
 *  34 XTAL1
 *  33 XTAL2
 *
 *
 * ... and the 70 used I/O lines by Arduino Name. Note that "= A8" means
 * the Arduino name (analogue pin) and ", A8" means the ATmega name
 * (External Memory interface address bit 8). The Arduino pin names are a
 * bit unfortunate. Port A, B, C, F, K and L are fully available. F and K
 * double up as analogue ports.
 *
 *  97 PF0 = A0, ADC0
 *  96 PF1 = A1, ADC1
 *  95 PF2 = A2, ADC2
 *  94 PF3 = A3, ADC3
 *  93 PF4 = A4, ADC4, TMK
 *  92 PF5 = A5, ADC5, TMS
 *  91 PF6 = A6, ADC6
 *  90 PF7 = A7, ADC7
 *  89 PK0 = A8, ADC8, PCINT16
 *  88 PK1 = A9, ADC9, PCINT17
 *  87 PK2 = A10, ADC10, PCINT18
 *  86 PK3 = A11, ADC11, PCINT19
 *  85 PK4 = A12, ADC12, PCINT20
 *  84 PK5 = A13, ADC13, PCINT21
 *  83 PK6 = A14, ADC14, PCINT22
 *  82 PK7 = A15, ADC15, PCINT23
 *   2 PE0 = D0 (RX0), RXD0, PCINT8
 *   3 PE1 = D1 (TX0), TXD0
 *  ~6 PE4 = D2, OC3B, INT4
 *  ~7 PE5 = D3, OC3C, INT5
 *  ~1 PG5 = D4, OC0B
 *  ~5 PE3 = D5, OC3A, AIN1
 * ~15 PH3 = D6, OC4A
 * ~16 PH4 = D7, OC4B
 * ~17 PH5 = D8, OC4C
 * ~18 PH6 = D9, OC2B
 * ~23 PB4 = D10, OC2A, PCINT4
 * ~24 PB5 = D11, OC1A, PCINT5
 * ~25 PB6 = D12, OC1B, PCINT6
 * ~26 PB7 = D13, OC0A, OC1C, PCINT7
 *  64 PJ1 = D14 (TX3), TXD3, PCINT10
 *  63 PJ0 = D15 (RX3), RXD3, PCINT9
 *  13 PH1 = D16 (TX2), TXD2
 *  12 PH0 = D17 (RX2), RXD2
 *  46 PD3 = D18 (TX1), TXD1, INT3
 *  45 PD2 = D19 (RX1), RXD1, INT2
 *  44 PD1 = D20 (SDA), SDA, INT1
 *  43 PD0 = D21 (SCL), SCL, INT0
 *  78 PA0 = D22, AD0
 *  77 PA1 = D23, AD1
 *  76 PA2 = D24, AD2
 *  75 PA3 = D25, AD3
 *  74 PA4 = D26, AD4
 *  73 PA5 = D27, AD5
 *  72 PA6 = D28, AD6
 *  71 PA7 = D29, AD7
 *  60 PC7 = D30, A15
 *  59 PC6 = D31, A14
 *  58 PC5 = D32, A13
 *  57 PC4 = D33, A12
 *  56 PC3 = D34, A11
 *  55 PC2 = D35, A10
 *  54 PC1 = D36, A9
 *  53 PC0 = D37, A8
 *  50 PD7 = D38, T0
 *  70 PG2 = D39, ALE
 *  52 PG1 = D40, RD
 *  51 PG0 = D41, WR
 *  42 PL7 = D42
 *  41 PL6 = D43
 * ~40 PL5 = D44, OC5C
 * ~39 PL4 = D45, OC5B
 * ~38 PL3 = D46, OC5A
 *  37 PL2 = D47, T5
 *  36 PL1 = D48, ICP5
 *  35 PL0 = D49, ICP4
 *  22 PB3 = D50 (POCI), POCI, PCINT3
 *  21 PB2 = D51 (PICO), PICO, PCINT2
 *  20 PB1 = D52 (SCK), SCK, PCINT1
 *  19 PB0 = D53 (CS), CS, PCINT0
 *
 *
 * List of the 16 unused ATmega2560 pins:
 *
 *  47 PD4 = n/a, ICP1
 *  48 PD5 = n/a, XCK1
 *  49 PD6 = n/a, T1
 *   4 PE2 = n/a, XCK0, AIN0
 *   8 PE6 = n/a, T3, INT6
 *   9 PE7 = n/a, CLKO, ICP3, INT7
 *  28 PG3 = n/a, TOSC2
 *  29 PG4 = n/a, TOSC1
 *  14 PH2 = n/a, XCK2
 *  27 PH7 = n/a, T4
 *  65 PJ2 = n/a, XCK3, PCINT11
 *  66 PJ3 = n/a, PCINT12
 *  67 PJ4 = n/a, PCINT13
 *  68 PJ5 = n/a, PCINT14
 *  69 PJ6 = n/a, PCINT15
 *  79 PJ7 = n/a
 *
 */

const uint16_t PROGMEM port_to_mode_PGM[] = {
	NOT_A_PORT,
	(uint16_t) &DDRA,
	(uint16_t) &DDRB,
	(uint16_t) &DDRC,
	(uint16_t) &DDRD,
	(uint16_t) &DDRE,
	(uint16_t) &DDRF,
	(uint16_t) &DDRG,
	(uint16_t) &DDRH,
	NOT_A_PORT,
	(uint16_t) &DDRJ,
	(uint16_t) &DDRK,
	(uint16_t) &DDRL,
};

const uint16_t PROGMEM port_to_output_PGM[] = {
	NOT_A_PORT,
	(uint16_t) &PORTA,
	(uint16_t) &PORTB,
	(uint16_t) &PORTC,
	(uint16_t) &PORTD,
	(uint16_t) &PORTE,
	(uint16_t) &PORTF,
	(uint16_t) &PORTG,
	(uint16_t) &PORTH,
	NOT_A_PORT,
	(uint16_t) &PORTJ,
	(uint16_t) &PORTK,
	(uint16_t) &PORTL,
};

const uint16_t PROGMEM port_to_input_PGM[] = {
	NOT_A_PIN,
	(uint16_t) &PINA,
	(uint16_t) &PINB,
	(uint16_t) &PINC,
	(uint16_t) &PIND,
	(uint16_t) &PINE,
	(uint16_t) &PINF,
	(uint16_t) &PING,
	(uint16_t) &PINH,
	NOT_A_PIN,
	(uint16_t) &PINJ,
	(uint16_t) &PINK,
	(uint16_t) &PINL,
};

const uint8_t PROGMEM digital_pin_to_port_PGM[] = {
	// PORTLIST		
	// -------------------------------------------		
	PE	, // PE 0 ** 0 ** USART0_RX	
	PE	, // PE 1 ** 1 ** USART0_TX	
	PE	, // PE 4 ** 2 ** PWM2	
	PE	, // PE 5 ** 3 ** PWM3	
	PG	, // PG 5 ** 4 ** PWM4	
	PE	, // PE 3 ** 5 ** PWM5	
	PH	, // PH 3 ** 6 ** PWM6	
	PH	, // PH 4 ** 7 ** PWM7	
	PH	, // PH 5 ** 8 ** PWM8	
	PH	, // PH 6 ** 9 ** PWM9	
	PB	, // PB 4 ** 10 ** PWM10	
	PB	, // PB 5 ** 11 ** PWM11	
	PB	, // PB 6 ** 12 ** PWM12	
	PB	, // PB 7 ** 13 ** PWM13	
	PJ	, // PJ 1 ** 14 ** USART3_TX	
	PJ	, // PJ 0 ** 15 ** USART3_RX	
	PH	, // PH 1 ** 16 ** USART2_TX	
	PH	, // PH 0 ** 17 ** USART2_RX	
	PD	, // PD 3 ** 18 ** USART1_TX	
	PD	, // PD 2 ** 19 ** USART1_RX	
	PD	, // PD 1 ** 20 ** I2C_SDA	
	PD	, // PD 0 ** 21 ** I2C_SCL	
	PA	, // PA 0 ** 22 ** D22	
	PA	, // PA 1 ** 23 ** D23	
	PA	, // PA 2 ** 24 ** D24	
	PA	, // PA 3 ** 25 ** D25	
	PA	, // PA 4 ** 26 ** D26	
	PA	, // PA 5 ** 27 ** D27	
	PA	, // PA 6 ** 28 ** D28	
	PA	, // PA 7 ** 29 ** D29	
	PC	, // PC 7 ** 30 ** D30	
	PC	, // PC 6 ** 31 ** D31	
	PC	, // PC 5 ** 32 ** D32	
	PC	, // PC 4 ** 33 ** D33	
	PC	, // PC 3 ** 34 ** D34	
	PC	, // PC 2 ** 35 ** D35	
	PC	, // PC 1 ** 36 ** D36	
	PC	, // PC 0 ** 37 ** D37	
	PD	, // PD 7 ** 38 ** D38	
	PG	, // PG 2 ** 39 ** D39	
	PG	, // PG 1 ** 40 ** D40	
	PG	, // PG 0 ** 41 ** D41	
	PL	, // PL 7 ** 42 ** D42	
	PL	, // PL 6 ** 43 ** D43	
	PL	, // PL 5 ** 44 ** D44	
	PL	, // PL 4 ** 45 ** D45	
	PL	, // PL 3 ** 46 ** D46	
	PL	, // PL 2 ** 47 ** D47	
	PL	, // PL 1 ** 48 ** D48	
	PL	, // PL 0 ** 49 ** D49	
	PB	, // PB 3 ** 50 ** SPI_POCI	
	PB	, // PB 2 ** 51 ** SPI_PICO	
	PB	, // PB 1 ** 52 ** SPI_SCK	
	PB	, // PB 0 ** 53 ** SPI_CS	
	PF	, // PF 0 ** 54 ** A0	
	PF	, // PF 1 ** 55 ** A1	
	PF	, // PF 2 ** 56 ** A2	
	PF	, // PF 3 ** 57 ** A3	
	PF	, // PF 4 ** 58 ** A4	
	PF	, // PF 5 ** 59 ** A5	
	PF	, // PF 6 ** 60 ** A6	
	PF	, // PF 7 ** 61 ** A7	
	PK	, // PK 0 ** 62 ** A8	
	PK	, // PK 1 ** 63 ** A9	
	PK	, // PK 2 ** 64 ** A10	
	PK	, // PK 3 ** 65 ** A11	
	PK	, // PK 4 ** 66 ** A12	
	PK	, // PK 5 ** 67 ** A13	
	PK	, // PK 6 ** 68 ** A14	
	PK	, // PK 7 ** 69 ** A15	
};

const uint8_t PROGMEM digital_pin_to_bit_mask_PGM[] = {
	// PIN IN PORT		
	// -------------------------------------------		
	_BV( 0 )	, // PE 0 ** 0 ** USART0_RX	
	_BV( 1 )	, // PE 1 ** 1 ** USART0_TX	
	_BV( 4 )	, // PE 4 ** 2 ** PWM2	
	_BV( 5 )	, // PE 5 ** 3 ** PWM3	
	_BV( 5 )	, // PG 5 ** 4 ** PWM4	
	_BV( 3 )	, // PE 3 ** 5 ** PWM5	
	_BV( 3 )	, // PH 3 ** 6 ** PWM6	
	_BV( 4 )	, // PH 4 ** 7 ** PWM7	
	_BV( 5 )	, // PH 5 ** 8 ** PWM8	
	_BV( 6 )	, // PH 6 ** 9 ** PWM9	
	_BV( 4 )	, // PB 4 ** 10 ** PWM10	
	_BV( 5 )	, // PB 5 ** 11 ** PWM11	
	_BV( 6 )	, // PB 6 ** 12 ** PWM12	
	_BV( 7 )	, // PB 7 ** 13 ** PWM13	
	_BV( 1 )	, // PJ 1 ** 14 ** USART3_TX	
	_BV( 0 )	, // PJ 0 ** 15 ** USART3_RX	
	_BV( 1 )	, // PH 1 ** 16 ** USART2_TX	
	_BV( 0 )	, // PH 0 ** 17 ** USART2_RX	
	_BV( 3 )	, // PD 3 ** 18 ** USART1_TX	
	_BV( 2 )	, // PD 2 ** 19 ** USART1_RX	
	_BV( 1 )	, // PD 1 ** 20 ** I2C_SDA	
	_BV( 0 )	, // PD 0 ** 21 ** I2C_SCL	
	_BV( 0 )	, // PA 0 ** 22 ** D22	
	_BV( 1 )	, // PA 1 ** 23 ** D23	
	_BV( 2 )	, // PA 2 ** 24 ** D24	
	_BV( 3 )	, // PA 3 ** 25 ** D25	
	_BV( 4 )	, // PA 4 ** 26 ** D26	
	_BV( 5 )	, // PA 5 ** 27 ** D27	
	_BV( 6 )	, // PA 6 ** 28 ** D28	
	_BV( 7 )	, // PA 7 ** 29 ** D29	
	_BV( 7 )	, // PC 7 ** 30 ** D30	
	_BV( 6 )	, // PC 6 ** 31 ** D31	
	_BV( 5 )	, // PC 5 ** 32 ** D32	
	_BV( 4 )	, // PC 4 ** 33 ** D33	
	_BV( 3 )	, // PC 3 ** 34 ** D34	
	_BV( 2 )	, // PC 2 ** 35 ** D35	
	_BV( 1 )	, // PC 1 ** 36 ** D36	
	_BV( 0 )	, // PC 0 ** 37 ** D37	
	_BV( 7 )	, // PD 7 ** 38 ** D38	
	_BV( 2 )	, // PG 2 ** 39 ** D39	
	_BV( 1 )	, // PG 1 ** 40 ** D40	
	_BV( 0 )	, // PG 0 ** 41 ** D41	
	_BV( 7 )	, // PL 7 ** 42 ** D42	
	_BV( 6 )	, // PL 6 ** 43 ** D43	
	_BV( 5 )	, // PL 5 ** 44 ** D44	
	_BV( 4 )	, // PL 4 ** 45 ** D45	
	_BV( 3 )	, // PL 3 ** 46 ** D46	
	_BV( 2 )	, // PL 2 ** 47 ** D47	
	_BV( 1 )	, // PL 1 ** 48 ** D48	
	_BV( 0 )	, // PL 0 ** 49 ** D49	
	_BV( 3 )	, // PB 3 ** 50 ** SPI_POCI	
	_BV( 2 )	, // PB 2 ** 51 ** SPI_PICO	
	_BV( 1 )	, // PB 1 ** 52 ** SPI_SCK	
	_BV( 0 )	, // PB 0 ** 53 ** SPI_CS	
	_BV( 0 )	, // PF 0 ** 54 ** A0	
	_BV( 1 )	, // PF 1 ** 55 ** A1	
	_BV( 2 )	, // PF 2 ** 56 ** A2	
	_BV( 3 )	, // PF 3 ** 57 ** A3	
	_BV( 4 )	, // PF 4 ** 58 ** A4	
	_BV( 5 )	, // PF 5 ** 59 ** A5	
	_BV( 6 )	, // PF 6 ** 60 ** A6	
	_BV( 7 )	, // PF 7 ** 61 ** A7	
	_BV( 0 )	, // PK 0 ** 62 ** A8	
	_BV( 1 )	, // PK 1 ** 63 ** A9	
	_BV( 2 )	, // PK 2 ** 64 ** A10	
	_BV( 3 )	, // PK 3 ** 65 ** A11	
	_BV( 4 )	, // PK 4 ** 66 ** A12	
	_BV( 5 )	, // PK 5 ** 67 ** A13	
	_BV( 6 )	, // PK 6 ** 68 ** A14	
	_BV( 7 )	, // PK 7 ** 69 ** A15	
};

const uint8_t PROGMEM digital_pin_to_timer_PGM[] = {
	// TIMERS		
	// -------------------------------------------		
	NOT_ON_TIMER	, // PE 0 ** 0 ** USART0_RX	
	NOT_ON_TIMER	, // PE 1 ** 1 ** USART0_TX	
	TIMER3B	, // PE 4 ** 2 ** PWM2	
	TIMER3C	, // PE 5 ** 3 ** PWM3	
	TIMER0B	, // PG 5 ** 4 ** PWM4	
	TIMER3A	, // PE 3 ** 5 ** PWM5	
	TIMER4A	, // PH 3 ** 6 ** PWM6	
	TIMER4B	, // PH 4 ** 7 ** PWM7	
	TIMER4C	, // PH 5 ** 8 ** PWM8	
	TIMER2B	, // PH 6 ** 9 ** PWM9	
	TIMER2A	, // PB 4 ** 10 ** PWM10	
	TIMER1A	, // PB 5 ** 11 ** PWM11	
	TIMER1B	, // PB 6 ** 12 ** PWM12	
	TIMER0A	, // PB 7 ** 13 ** PWM13	
	NOT_ON_TIMER	, // PJ 1 ** 14 ** USART3_TX	
	NOT_ON_TIMER	, // PJ 0 ** 15 ** USART3_RX	
	NOT_ON_TIMER	, // PH 1 ** 16 ** USART2_TX	
	NOT_ON_TIMER	, // PH 0 ** 17 ** USART2_RX	
	NOT_ON_TIMER	, // PD 3 ** 18 ** USART1_TX	
	NOT_ON_TIMER	, // PD 2 ** 19 ** USART1_RX	
	NOT_ON_TIMER	, // PD 1 ** 20 ** I2C_SDA	
	NOT_ON_TIMER	, // PD 0 ** 21 ** I2C_SCL	
	NOT_ON_TIMER	, // PA 0 ** 22 ** D22	
	NOT_ON_TIMER	, // PA 1 ** 23 ** D23	
	NOT_ON_TIMER	, // PA 2 ** 24 ** D24	
	NOT_ON_TIMER	, // PA 3 ** 25 ** D25	
	NOT_ON_TIMER	, // PA 4 ** 26 ** D26	
	NOT_ON_TIMER	, // PA 5 ** 27 ** D27	
	NOT_ON_TIMER	, // PA 6 ** 28 ** D28	
	NOT_ON_TIMER	, // PA 7 ** 29 ** D29	
	NOT_ON_TIMER	, // PC 7 ** 30 ** D30	
	NOT_ON_TIMER	, // PC 6 ** 31 ** D31	
	NOT_ON_TIMER	, // PC 5 ** 32 ** D32	
	NOT_ON_TIMER	, // PC 4 ** 33 ** D33	
	NOT_ON_TIMER	, // PC 3 ** 34 ** D34	
	NOT_ON_TIMER	, // PC 2 ** 35 ** D35	
	NOT_ON_TIMER	, // PC 1 ** 36 ** D36	
	NOT_ON_TIMER	, // PC 0 ** 37 ** D37	
	NOT_ON_TIMER	, // PD 7 ** 38 ** D38	
	NOT_ON_TIMER	, // PG 2 ** 39 ** D39	
	NOT_ON_TIMER	, // PG 1 ** 40 ** D40	
	NOT_ON_TIMER	, // PG 0 ** 41 ** D41	
	NOT_ON_TIMER	, // PL 7 ** 42 ** D42	
	NOT_ON_TIMER	, // PL 6 ** 43 ** D43	
	TIMER5C	, // PL 5 ** 44 ** D44	
	TIMER5B	, // PL 4 ** 45 ** D45	
	TIMER5A	, // PL 3 ** 46 ** D46	
	NOT_ON_TIMER	, // PL 2 ** 47 ** D47	
	NOT_ON_TIMER	, // PL 1 ** 48 ** D48	
	NOT_ON_TIMER	, // PL 0 ** 49 ** D49	
	NOT_ON_TIMER	, // PB 3 ** 50 ** SPI_POCI	
	NOT_ON_TIMER	, // PB 2 ** 51 ** SPI_PICO	
	NOT_ON_TIMER	, // PB 1 ** 52 ** SPI_SCK	
	NOT_ON_TIMER	, // PB 0 ** 53 ** SPI_CS	
	NOT_ON_TIMER	, // PF 0 ** 54 ** A0	
	NOT_ON_TIMER	, // PF 1 ** 55 ** A1	
	NOT_ON_TIMER	, // PF 2 ** 56 ** A2	
	NOT_ON_TIMER	, // PF 3 ** 57 ** A3	
	NOT_ON_TIMER	, // PF 4 ** 58 ** A4	
	NOT_ON_TIMER	, // PF 5 ** 59 ** A5	
	NOT_ON_TIMER	, // PF 6 ** 60 ** A6	
	NOT_ON_TIMER	, // PF 7 ** 61 ** A7	
	NOT_ON_TIMER	, // PK 0 ** 62 ** A8	
	NOT_ON_TIMER	, // PK 1 ** 63 ** A9	
	NOT_ON_TIMER	, // PK 2 ** 64 ** A10	
	NOT_ON_TIMER	, // PK 3 ** 65 ** A11	
	NOT_ON_TIMER	, // PK 4 ** 66 ** A12	
	NOT_ON_TIMER	, // PK 5 ** 67 ** A13	
	NOT_ON_TIMER	, // PK 6 ** 68 ** A14	
	NOT_ON_TIMER	, // PK 7 ** 69 ** A15	
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
#define SERIAL_PORT_MONITOR         Serial
#define SERIAL_PORT_HARDWARE        Serial
#define SERIAL_PORT_HARDWARE1       Serial1
#define SERIAL_PORT_HARDWARE2       Serial2
#define SERIAL_PORT_HARDWARE3       Serial3
#define SERIAL_PORT_HARDWARE_OPEN   Serial1
#define SERIAL_PORT_HARDWARE_OPEN1  Serial2
#define SERIAL_PORT_HARDWARE_OPEN2  Serial3

#endif
