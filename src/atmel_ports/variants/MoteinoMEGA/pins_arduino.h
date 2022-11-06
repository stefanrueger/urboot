// Pins definitions for Atmega644/1284P
// Original file from Maniacbug's Mighty-1284 core and mods by Leonardo Miliani
// Modded by Felix Rusu - LowPowerLab.com

#ifndef Pins_Arduino_h
#define Pins_Arduino_h

#include <avr/pgmspace.h>

//                         ATMEL ATMEGA644P/1284P
//
//                               +---\/---+
//    PCINT8/XCK0/T0 (D0 ) PB0  1|        |40  PA0 (A0/D24) PCINT0
//    PCINT9/CLKO/T1 (D1 ) PB1  2|        |39  PA1 (A1/D25) PCINT1
// PCINT10/INT2/AIN0 (D2 ) PB2  3|        |38  PA2 (A2/D26) PCINT2
// PCINT11/OC0A/AIN1 (D3 ) PB3  4|~       |37  PA3 (A3/D27) PCINT3
//   PCINT12/OC0B/CS (D4 ) PB4  5|~       |36  PA4 (A4/D28) PCINT4
// PCINT13/ICP3/COPI (D5 ) PB5  6|        |35  PA5 (A5/D29) PCINT5
// PCINT14/OC3A/CIPO (D6 ) PB6  7|~*      |34  PA6 (A6/D30) PCINT6
//  PCINT15/OC3B/SCK (D7 ) PB7  8|~*      |33  PA7 (A7/D31) PCINT7
//                         RST  9|        |32  AREF
//                         VCC 10|        |31  GND
//                         GND 11|        |30  AVCC
//                       XTAL2 12|        |29  PC7 (D23) TOSC2/PCINT23
//                       XTAL1 13|        |28  PC6 (D22) TOSC1/PCINT22
//   PCINT24/RX0/T3* (D8 ) PD0 14|        |27  PC5 (D21) TDI/PCINT21
//       PCINT25/TX0 (D9 ) PD1 15|        |26  PC4 (D20) TDO/PCINT20
//  PCINT26/INT0/RX1 (D10) PD2 16|        |25  PC3 (D19) TMS/PCINT19
//  PCINT27/INT1/TX1 (D11) PD3 17|        |24  PC2 (D18) TCK/PCINT18
// PCINT28/XCK1/OC1B (D12) PD4 18|~       |23  PC1 (D17) SDA/PCINT17
//      PCINT29/OC1A (D13) PD5 19|~       |22  PC0 (D16) SCL/PCINT16
//  PCINT30/OC2B/ICP (D14) PD6 20|~      ~|21  PD7 (D15) OC2A/PCINT31
//                               +--------+
//
// *: T3, OC3A, OC3B (and corresponding PWMs) are only present on Atmega1284
//
// PCINT15-8: D7-0    : bit 1
// PCINT31-24: D15-8  : bit 3
// PCINT23-16: D23-16 : bit 2
// PCINT7-0: D31-24   : bit 0

#define NUM_DIGITAL_PINS            32
#define NUM_ANALOG_INPUTS           8
#define analogInputToDigitalPin(p)  ((p) < NUM_ANALOG_INPUTS? (p)+24 : -1)

#if defined(__AVR_ATmega1284P__)
#define digitalPinHasPWM(p)         ((p) == 3 || (p) == 4 || (p) == 6 || (p) == 7 || (p) == 12 || (p) == 13 || (p) == 14 || (p) == 15)
#else
#define digitalPinHasPWM(p)         ((p) == 3 || (p) == 4 || (p) == 12 || (p) == 13 || (p) == 14 || (p) == 15)
#endif

static const uint8_t CS   = 4;
static const uint8_t COPI = 5;
static const uint8_t CIPO = 6;
static const uint8_t SCK  = 7;

static const uint8_t SDA = 17;
static const uint8_t SCL = 16;
//static const uint8_t LED = 15;
#define LED_BUILTIN 15

static const uint8_t A0 = 24;
static const uint8_t A1 = 25;
static const uint8_t A2 = 26;
static const uint8_t A3 = 27;
static const uint8_t A4 = 28;
static const uint8_t A5 = 29;
static const uint8_t A6 = 30;
static const uint8_t A7 = 31;


#define digitalPinToPCICR(p)    (((p) >= 0 && (p) < NUM_DIGITAL_PINS)? (&PCICR): ((uint8_t *)0))
#define digitalPinToPCICRbit(p) (((p) <= 7)?        1:  ((p) <= 15)?        3:  ((p) <= 23)?        2:         0)
#define digitalPinToPCMSK(p)    (((p) <= 7)? (&PCMSK1): ((p) <= 15)? (&PCMSK3): ((p) <= 23)? (&PCMSK2): (&PCMSK0))
#define digitalPinToPCMSKbit(p) ((uint8_t)(p) % 8)

#define digitalPinToInterrupt(p) ((p) == 10? 0: (p) == 11? 1: (p) == 2? 2: NOT_AN_INTERRUPT)


#ifdef ARDUINO_MAIN

#define PA 1
#define PB 2
#define PC 3
#define PD 4

// these arrays map port names (e.g. port B) to the
// appropriate addresses for various functions (e.g. reading
// and writing)
const uint16_t PROGMEM port_to_mode_PGM[] =
{
	NOT_A_PORT,
	(uint16_t) &DDRA,
	(uint16_t) &DDRB,
	(uint16_t) &DDRC,
	(uint16_t) &DDRD,
};

const uint16_t PROGMEM port_to_output_PGM[] =
{
	NOT_A_PORT,
	(uint16_t) &PORTA,
	(uint16_t) &PORTB,
	(uint16_t) &PORTC,
	(uint16_t) &PORTD,
};

const uint16_t PROGMEM port_to_input_PGM[] =
{
	NOT_A_PORT,
	(uint16_t) &PINA,
	(uint16_t) &PINB,
	(uint16_t) &PINC,
	(uint16_t) &PIND,
};

const uint8_t PROGMEM digital_pin_to_port_PGM[] =
{
	PB, /* 0 */
	PB,
	PB,
	PB,
	PB,
	PB,
	PB,
	PB,
	PD, /* 8 */
	PD,
	PD,
	PD,
	PD,
	PD,
	PD,
	PD,
	PC, /* 16 */
	PC,
	PC,
	PC,
	PC,
	PC,
	PC,
	PC,
	PA, /* 24 */
	PA,
	PA,
	PA,
	PA,
	PA,
	PA,
	PA  /* 31 */
};

const uint8_t PROGMEM digital_pin_to_bit_mask_PGM[] =
{
	_BV(0), /* 0, port B */
	_BV(1),
	_BV(2),
	_BV(3),
	_BV(4),
	_BV(5),
	_BV(6),
	_BV(7),
	_BV(0), /* 8, port D */
	_BV(1),
	_BV(2),
	_BV(3),
	_BV(4),
	_BV(5),
	_BV(6),
	_BV(7),
	_BV(0), /* 16, port C */
	_BV(1),
	_BV(2),
	_BV(3),
	_BV(4),
	_BV(5),
	_BV(6),
	_BV(7),
	_BV(0), /* 24, port A */
	_BV(1),
	_BV(2),
	_BV(3),
	_BV(4),
	_BV(5),
	_BV(6),
	_BV(7)
};

const uint8_t PROGMEM digital_pin_to_timer_PGM[] =
{
	NOT_ON_TIMER, 	/* 0  - PB0 */
	NOT_ON_TIMER, 	/* 1  - PB1 */
	NOT_ON_TIMER, 	/* 2  - PB2 */
	TIMER0A,		/* 3  - PB3 */
	TIMER0B,		/* 4  - PB4 */
	NOT_ON_TIMER, 	/* 5  - PB5 */
#if defined(__AVR_ATmega1284P__)
	TIMER3A,		/* 6  - PB6 */
	TIMER3B,		/* 7  - PB7 */
#else
	NOT_ON_TIMER,   /* 6  - PB6 */
	NOT_ON_TIMER,   /* 7  - PB7 */
#endif
	NOT_ON_TIMER, 	/* 8  - PD0 */
	NOT_ON_TIMER, 	/* 9  - PD1 */
	NOT_ON_TIMER, 	/* 10 - PD2 */
	NOT_ON_TIMER, 	/* 11 - PD3 */
	TIMER1B,     	/* 12 - PD4 */
	TIMER1A,     	/* 13 - PD5 */
	TIMER2B,     	/* 14 - PD6 */
	TIMER2A,     	/* 15 - PD7 */
	NOT_ON_TIMER, 	/* 16 - PC0 */
	NOT_ON_TIMER,   /* 17 - PC1 */
	NOT_ON_TIMER,   /* 18 - PC2 */
	NOT_ON_TIMER,   /* 19 - PC3 */
	NOT_ON_TIMER,   /* 20 - PC4 */
	NOT_ON_TIMER,   /* 21 - PC5 */
	NOT_ON_TIMER,   /* 22 - PC6 */
	NOT_ON_TIMER,   /* 23 - PC7 */
	NOT_ON_TIMER,   /* 24 - PA0 */
	NOT_ON_TIMER,   /* 25 - PA1 */
	NOT_ON_TIMER,   /* 26 - PA2 */
	NOT_ON_TIMER,   /* 27 - PA3 */
	NOT_ON_TIMER,   /* 28 - PA4 */
	NOT_ON_TIMER,   /* 29 - PA5 */
	NOT_ON_TIMER,   /* 30 - PA6 */
	NOT_ON_TIMER    /* 31 - PA7 */
};

#endif // ARDUINO_MAIN

#endif // Pins_Arduino_h
// vim:ai:cin:sts=2 sw=2 ft=cpp