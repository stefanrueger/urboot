/*
 * pin_defs.h
 *
 * This header defines the default pin assignments (DEFLED, TX, RX, ...) for the various chips
 * that are supported, though really, it's the boards that determine the default pins, not the
 * chip. The defaults can be overwritten by make arguments such as TX=AtmelPB6
 *
 * Quite a bit modified 2016 and 2022-23 by Stefan Rueger
 * Copyright 2013-2015 by Bill Westfield
 * Copyright 2010 by Peter Knight
 * This software is licensed under version 2 of the Gnu Public Licence.
 * See urboot.c for details.
 */

#include "atmel_ports.h"

// EEPROM support (older uPs lack the EEPE/EEMPE bit definitions)
#if !defined(EEMPE) && defined(EEMWE)
#define EEMPE EEMWE
#define EEPE  EEWE
#endif

// MCU status register names vary
#if defined(MCUCSR)
#define UB_MCUSR         MCUCSR

#else                            // need #else/#if as MCUSR is poisoned in ATmega128's iom128.h
#if defined(MCUSR)
#define UB_MCUSR          MCUSR

#elif defined(RST_STATUS)
#define UB_MCUSR     RST_STATUS

#define BORF  RST_BORF_bp       // Brown-out Reset Flag
#define EXTRF RST_EXTRF_bp      // External Reset Flag
#define PDIRF RST_PDIRF_bp      // Programming and Debug Interface Reset Flag
#define PORF  RST_PORF_bp       // Power-on Reset Flag
#define SDRF  RST_SDRF_bp       // Spike Detection Reset Flag
#define SRF   RST_SRF_bp        // Software Reset Flag
#define WDRF  RST_WDRF_bp       // Watchdog Reset Flag

#elif defined(RSTFLR)           // (unsupported) ATtiny4 ATtiny5 ATtiny9 ATtiny10 ATtiny20 ATtiny40
#define UB_MCUSR         RSTFLR
#endif
#endif

#if !defined(BORF) && defined(BODRF)
#define BORF BODRF
#endif

#if defined(__AVR_ATmega161__) && !defined(EXTRF)
#define EXTRF 1
#endif


// Watchdog support
#if !defined(WDTCSR) && defined(WDTCR)
#define WDTCSR WDTCR
#endif
#if !defined (WDCE) && defined(WDTOE)
#define WDCE WDTOE
#endif

// Boards actually differ quite a bit re LED
// Arduino NG, Diecimila, Duemilanove, Jeelabs, Moteino, Anarduino, Timeduino, Urclock, UrSense ...
#if defined(__AVR_ATmega168__) || defined(__AVR_ATmega168P__) || defined(__AVR_ATmega328P__) || \
    defined(__AVR_ATmega88__) || defined(__AVR_ATmega8__)
#ifndef DEFLED
#define DEFLED AtmelPB5
#endif

// Luminet support
#elif defined(__AVR_ATtiny84__)
#ifndef DEFLED
#define DEFLED AtmelPA4
#endif

// Digispark support
#elif defined(__AVR_ATtiny85__)
#ifndef DEFLED
#define DEFLED AtmelPB1
#endif

// Digispark Pro support
#elif defined(__AVR_ATtiny167__)
#ifndef DEFLED
#define DEFLED AtmelPB1
#endif

// Sanguino support (and other 40pin DIP cpus) -- for MoteinoMega DEFLED is at PD7, though
#elif defined(__AVR_ATmega644P__) || defined(__AVR_ATmega1284P__) || defined(__AVR_ATmega32__) || \
     defined (__AVR_ATmega16__)
#ifndef DEFLED
#define DEFLED AtmelPB0
#endif

// Mega support
#elif defined(__AVR_ATmega1280__)
#ifndef DEFLED
#define DEFLED AtmelPB7
#endif
#endif


// From the ATDF files: assign RX/TX of the (first) USART [todo: many are missing in older ATDFs]

#if defined(__AVR_ATmega1608__) || defined(__AVR_ATmega1609__) || defined(__AVR_ATmega3208__) || \
    defined(__AVR_ATmega3209__) || defined(__AVR_ATmega4808__) || defined(__AVR_ATmega4809__) || \
    defined(__AVR_ATmega808__) || defined(__AVR_ATmega809__) || defined(__AVR_AVR128DA28__) || \
    defined(__AVR_AVR128DA32__) || defined(__AVR_AVR128DA48__) || defined(__AVR_AVR128DA64__) || \
    defined(__AVR_AVR128DB28__) || defined(__AVR_AVR128DB32__) || defined(__AVR_AVR128DB48__) || \
    defined(__AVR_AVR128DB64__) || defined(__AVR_AVR16DD14__) || defined(__AVR_AVR16DD20__) || \
    defined(__AVR_AVR16DD28__) || defined(__AVR_AVR16DD32__) || defined(__AVR_AVR32DA28__) || \
    defined(__AVR_AVR32DA32__) || defined(__AVR_AVR32DA48__) || defined(__AVR_AVR32DB28__) || \
    defined(__AVR_AVR32DB32__) || defined(__AVR_AVR32DB48__) || defined(__AVR_AVR32DD14__) || \
    defined(__AVR_AVR32DD20__) || defined(__AVR_AVR32DD28__) || defined(__AVR_AVR32DD32__) || \
    defined(__AVR_AVR64DA28__) || defined(__AVR_AVR64DA32__) || defined(__AVR_AVR64DA48__) || \
    defined(__AVR_AVR64DA64__) || defined(__AVR_AVR64DB28__) || defined(__AVR_AVR64DB32__) || \
    defined(__AVR_AVR64DB48__) || defined(__AVR_AVR64DB64__) || defined(__AVR_AVR64DD14__) || \
    defined(__AVR_AVR64DD20__) || defined(__AVR_AVR64DD28__) || defined(__AVR_AVR64DD32__) || \
    defined(__AVR_AVR64EA28__) || defined(__AVR_AVR64EA32__) || defined(__AVR_AVR64EA48__)

#ifndef RX
#define RX AtmelPA1
#endif
#ifndef TX
#define TX AtmelPA0
#endif

#endif


#if defined(__AVR_ATtiny441__) || defined(__AVR_ATtiny841__)

#ifndef RX
#define RX AtmelPA4
#endif
#ifndef TX
#define TX AtmelPA5
#endif

#endif


#if defined(__AVR_ATtiny202__) || defined(__AVR_ATtiny212__) || \
    defined(__AVR_ATtiny402__) || defined(__AVR_ATtiny412__)

#ifndef RX
#define RX AtmelPA7
#endif
#ifndef TX
#define TX AtmelPA6
#endif

#endif


#if defined(__AVR_ATtiny102__) || defined(__AVR_ATtiny104__) || defined(__AVR_ATtiny1604__) || \
    defined(__AVR_ATtiny1606__) || defined(__AVR_ATtiny1607__) || defined(__AVR_ATtiny1614__) || \
    defined(__AVR_ATtiny1616__) || defined(__AVR_ATtiny1617__) || defined(__AVR_ATtiny1624__) || \
    defined(__AVR_ATtiny1626__) || defined(__AVR_ATtiny1627__) || defined(__AVR_ATtiny204__) || \
    defined(__AVR_ATtiny214__) || defined(__AVR_ATtiny3216__) || defined(__AVR_ATtiny3217__) || \
    defined(__AVR_ATtiny3224__) || defined(__AVR_ATtiny3226__) || defined(__AVR_ATtiny3227__) || \
    defined(__AVR_ATtiny404__) || defined(__AVR_ATtiny406__) || defined(__AVR_ATtiny414__) || \
    defined(__AVR_ATtiny416auto__) || defined(__AVR_ATtiny416__) || defined(__AVR_ATtiny417__) || \
    defined(__AVR_ATtiny424__) || defined(__AVR_ATtiny426__) || defined(__AVR_ATtiny427__) || \
    defined(__AVR_ATtiny804__) || defined(__AVR_ATtiny806__) || defined(__AVR_ATtiny807__) || \
    defined(__AVR_ATtiny814__) || defined(__AVR_ATtiny816__) || defined(__AVR_ATtiny817__) || \
    defined(__AVR_ATtiny824__) || defined(__AVR_ATtiny826__) || defined(__AVR_ATtiny827__)

#ifndef RX
#define RX AtmelPB3
#endif
#ifndef TX
#define TX AtmelPB2
#endif

#endif


#if defined(__AVR_ATxmega128A1U__) || defined(__AVR_ATxmega128A3U__) || \
    defined(__AVR_ATxmega128A4U__) || defined(__AVR_ATxmega128B1__) || \
    defined(__AVR_ATxmega128B3__) || defined(__AVR_ATxmega128C3__) || \
    defined(__AVR_ATxmega128D3__) || defined(__AVR_ATxmega128D4__) || \
    defined(__AVR_ATxmega16A4U__) || defined(__AVR_ATxmega16C4__) || \
    defined(__AVR_ATxmega16D4__) || defined(__AVR_ATxmega16E5__) || \
    defined(__AVR_ATxmega192A3U__) || defined(__AVR_ATxmega192D3__) || \
    defined(__AVR_ATxmega256A3BU__) || defined(__AVR_ATxmega256A3U__) || \
    defined(__AVR_ATxmega256C3__) || defined(__AVR_ATxmega256D3__) || \
    defined(__AVR_ATxmega32A4U__) || defined(__AVR_ATxmega32C3__) || \
    defined(__AVR_ATxmega32C4__) || defined(__AVR_ATxmega32D3__) || \
    defined(__AVR_ATxmega32D4__) || defined(__AVR_ATxmega32E5__) || \
    defined(__AVR_ATxmega384C3__) || defined(__AVR_ATxmega384D3__) || \
    defined(__AVR_ATxmega64A1U__) || defined(__AVR_ATxmega64A3U__) || \
    defined(__AVR_ATxmega64A4U__) || defined(__AVR_ATxmega64B1__) || \
    defined(__AVR_ATxmega64B3__) || defined(__AVR_ATxmega64C3__) || \
    defined(__AVR_ATxmega64D3__) || defined(__AVR_ATxmega64D4__) || defined(__AVR_ATxmega8E5__) || \
    /* Manually added below */ \
    defined(__AVR_ATtiny828__)


#ifndef RX
#define RX AtmelPC2
#endif
#ifndef TX
#define TX AtmelPC3
#endif

#endif


#if defined(__AVR_ATmega1284P__) || defined(__AVR_ATmega1284__) || defined(__AVR_ATmega164A__) || \
    defined(__AVR_ATmega164PA__) || defined(__AVR_ATmega164P__) || defined(__AVR_ATmega168A__) || \
    defined(__AVR_ATmega168PA__) || defined(__AVR_ATmega168PB__) || defined(__AVR_ATmega168P__) ||\
    defined(__AVR_ATmega168__) || defined(__AVR_ATmega16A__) || defined(__AVR_ATmega16__) || \
    defined(__AVR_ATmega324A__) || defined(__AVR_ATmega324PA__) || defined(__AVR_ATmega324PB__) ||\
    defined(__AVR_ATmega324P__) || defined(__AVR_ATmega328PB__) || defined(__AVR_ATmega328P__) || \
    defined(__AVR_ATmega328__) || defined(__AVR_ATmega32A__) || defined(__AVR_ATmega32__) || \
    defined(__AVR_ATmega48A__) || defined(__AVR_ATmega48PA__) || defined(__AVR_ATmega48PB__) || \
    defined(__AVR_ATmega48P__) || defined(__AVR_ATmega48__) || defined(__AVR_ATmega644A__) || \
    defined(__AVR_ATmega644PA__) || defined(__AVR_ATmega644P__) || defined(__AVR_ATmega644__) || \
    defined(__AVR_ATmega88A__) || defined(__AVR_ATmega88PA__) || defined(__AVR_ATmega88PB__) || \
    defined(__AVR_ATmega88P__) || defined(__AVR_ATmega88__) || defined(__AVR_ATmega8A__) || \
    defined(__AVR_ATmega8__) || \
    /* Manually added below */ \
    defined(__AVR_ATmega161__) ||  defined(__AVR_ATmega162__) ||  defined(__AVR_ATmega163__) || \
    defined(__AVR_ATmega323__) ||  defined(__AVR_ATmega8515__) ||  defined(__AVR_ATmega8535__) || \
    defined(__AVR_ATtiny2313__) ||  defined(__AVR_ATtiny2313A__) ||  defined(__AVR_ATtiny4313__)

#ifndef RX
#define RX AtmelPD0
#endif
#ifndef TX
#define TX AtmelPD1
#endif

#endif


#if defined(__AVR_AT90CAN128__) || defined(__AVR_AT90CAN32__) || defined(__AVR_AT90CAN64__) || \
    defined(__AVR_ATmega16U2__) || defined(__AVR_ATmega16U4__) || defined(__AVR_ATmega32U2__) ||  \
    defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega8U2__)

#ifndef RX
#define RX AtmelPD2
#endif
#ifndef TX
#define TX AtmelPD3
#endif

#endif


#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega1281__) || defined(__AVR_ATmega128A__) || \
    defined(__AVR_ATmega128__) || defined(__AVR_ATmega169A__) || defined(__AVR_ATmega169PA__) || \
    defined(__AVR_ATmega169P__) || defined(__AVR_ATmega2560__) || defined(__AVR_ATmega2561__) || \
    defined(__AVR_ATmega329PA__) || defined(__AVR_ATmega329P__) || defined(__AVR_ATmega329__) || \
    defined(__AVR_ATmega640__) || defined(__AVR_ATmega649A__) || defined(__AVR_ATmega649P__) || \
    defined(__AVR_ATmega649__) || defined(__AVR_ATmega64A__) || defined(__AVR_ATmega64__) || \
    /* Manually added below */ \
    defined(__AVR_ATmega1284RFR2__) || defined(__AVR_ATmega128RFA1__) || \
    defined(__AVR_ATmega128RFR2__) || defined(__AVR_ATmega165__) || \
    defined(__AVR_ATmega165A__) || defined(__AVR_ATmega165P__) || \
    defined(__AVR_ATmega165PA__) || defined(__AVR_ATmega169__) || \
    defined(__AVR_ATmega2564RFR2__) || defined(__AVR_ATmega256RFR2__) || \
    defined(__AVR_ATmega325__) || defined(__AVR_ATmega3250__) || \
    defined(__AVR_ATmega3250A__) || defined(__AVR_ATmega3250P__) || \
    defined(__AVR_ATmega3250PA__) || defined(__AVR_ATmega325A__) || \
    defined(__AVR_ATmega325P__) || defined(__AVR_ATmega325PA__) || \
    defined(__AVR_ATmega3290__) || defined(__AVR_ATmega3290A__) || \
    defined(__AVR_ATmega3290P__) || defined(__AVR_ATmega3290PA__) || \
    defined(__AVR_ATmega329A__) || defined(__AVR_ATmega644RFR2__) || \
    defined(__AVR_ATmega645__) || defined(__AVR_ATmega6450__) || \
    defined(__AVR_ATmega6450A__) || defined(__AVR_ATmega6450P__) || \
    defined(__AVR_ATmega645A__) || defined(__AVR_ATmega645P__) || \
    defined(__AVR_ATmega6490__) || defined(__AVR_ATmega6490A__) || \
    defined(__AVR_ATmega6490P__) || defined(__AVR_ATmega64RFR2__)

#ifndef RX
#define RX AtmelPE0
#endif
#ifndef TX
#define TX AtmelPE1
#endif

#endif


/* Manually added */

#if defined(__AVR_ATtiny1634__)

#ifndef RX
#define RX AtmelPA7
#endif
#ifndef TX
#define TX AtmelPB0
#endif

#endif



// Macros below are to determine uP classes for SWIO rx/tx timing;
// could maybe switch to sth like #define IS_XMEGA (defined(__AVR_XMEGA__) && __AVR_XMEGA__)

#define IS_XMEGA ( \
  defined(__AVR_ATxmega128A1U__) || defined(__AVR_ATxmega128A1__) || defined(__AVR_ATxmega128A3U__) || \
  defined(__AVR_ATxmega128A3__) || defined(__AVR_ATxmega128A4U__) || defined(__AVR_ATxmega128B1__) || \
  defined(__AVR_ATxmega128B3__) || defined(__AVR_ATxmega128C3__) || defined(__AVR_ATxmega128D3__) || \
  defined(__AVR_ATxmega128D4__) || defined(__AVR_ATxmega16A4U__) || defined(__AVR_ATxmega16A4__) || \
  defined(__AVR_ATxmega16C4__) || defined(__AVR_ATxmega16D4__) || defined(__AVR_ATxmega192A3U__) || \
  defined(__AVR_ATxmega192A3__) || defined(__AVR_ATxmega192C3__) || defined(__AVR_ATxmega192D3__) || \
  defined(__AVR_ATxmega256A3BU__) || defined(__AVR_ATxmega256A3B__) || defined(__AVR_ATxmega256A3U__) || \
  defined(__AVR_ATxmega256A3__) || defined(__AVR_ATxmega256C3__) || defined(__AVR_ATxmega256D3__) || \
  defined(__AVR_ATxmega32A4U__) || defined(__AVR_ATxmega32A4__) || defined(__AVR_ATxmega32C4__) || \
  defined(__AVR_ATxmega32D4__) || defined(__AVR_ATxmega384C3__) || defined(__AVR_ATxmega384D3__) || \
  defined(__AVR_ATxmega64A1U__) || defined(__AVR_ATxmega64A1__) || defined(__AVR_ATxmega64A3U__) || \
  defined(__AVR_ATxmega64A3__) || defined(__AVR_ATxmega64A4U__) || defined(__AVR_ATxmega64B1__) || \
  defined(__AVR_ATxmega64B3__) || defined(__AVR_ATxmega64C3__) || defined(__AVR_ATxmega64D3__) || \
  defined(__AVR_ATxmega64D4__) \
)

#define IS_MEGA ( \
  defined(__AVR_AT90CAN128__) || defined(__AVR_AT90CAN32__) || defined(__AVR_AT90CAN64__) || \
  defined(__AVR_AT90PWM161__) || defined(__AVR_AT90PWM1__) || defined(__AVR_AT90PWM216__) || \
  defined(__AVR_AT90PWM2B__) || defined(__AVR_AT90PWM316__) || defined(__AVR_AT90PWM3B__) || \
  defined(__AVR_AT90PWM81__) || defined(__AVR_AT90USB1286__) || defined(__AVR_AT90USB1287__) || \
  defined(__AVR_AT90USB162__) || defined(__AVR_AT90USB646__) || defined(__AVR_AT90USB647__) || \
  defined(__AVR_AT90USB82__) || defined(__AVR_ATA6285__) || defined(__AVR_ATA6286__) || \
  defined(__AVR_ATmega1280__) || defined(__AVR_ATmega1281__) || defined(__AVR_ATmega1284P__) || \
  defined(__AVR_ATmega1284__) || defined(__AVR_ATmega128A__) || defined(__AVR_ATmega128RFA1__) || \
  defined(__AVR_ATmega128__) || defined(__AVR_ATmega162__) || defined(__AVR_ATmega164A__) || \
  defined(__AVR_ATmega164PA__) || defined(__AVR_ATmega164P__) || defined(__AVR_ATmega165A__) || \
  defined(__AVR_ATmega165PA__) || defined(__AVR_ATmega165P__) || defined(__AVR_ATmega168A__) || \
  defined(__AVR_ATmega168PA__) || defined(__AVR_ATmega168P__) || defined(__AVR_ATmega168__) || \
  defined(__AVR_ATmega169A__) || defined(__AVR_ATmega169PA__) || defined(__AVR_ATmega169P__) || \
  defined(__AVR_ATmega16A__) || defined(__AVR_ATmega16HVB__) || defined(__AVR_ATmega16M1__) || \
  defined(__AVR_ATmega16U2__) || defined(__AVR_ATmega16U4__) || defined(__AVR_ATmega16__) || \
  defined(__AVR_ATmega2560__) || defined(__AVR_ATmega2561__) || defined(__AVR_ATmega324A__) || \
  defined(__AVR_ATmega324PA__) || defined(__AVR_ATmega324P__) || defined(__AVR_ATmega3250A__) || \
  defined(__AVR_ATmega3250PA__) || defined(__AVR_ATmega3250P__) || defined(__AVR_ATmega3250__) || \
  defined(__AVR_ATmega325A__) || defined(__AVR_ATmega325PA__) || defined(__AVR_ATmega325P__) || \
  defined(__AVR_ATmega325__) || defined(__AVR_ATmega328P__) || defined(__AVR_ATmega328__) || \
  defined(__AVR_ATmega3290A__) || defined(__AVR_ATmega3290PA__) || defined(__AVR_ATmega3290P__) || \
  defined(__AVR_ATmega3290__) || defined(__AVR_ATmega329A__) || defined(__AVR_ATmega329PA__) || \
  defined(__AVR_ATmega329P__) || defined(__AVR_ATmega329__) || defined(__AVR_ATmega32A__) || \
  defined(__AVR_ATmega32C1__) || defined(__AVR_ATmega32HVB__) || defined(__AVR_ATmega32M1__) || \
  defined(__AVR_ATmega32U2__) || defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega32__) || \
  defined(__AVR_ATmega48A__) || defined(__AVR_ATmega48PA__) || defined(__AVR_ATmega48P__) || \
  defined(__AVR_ATmega48__) || defined(__AVR_ATmega640__) || defined(__AVR_ATmega644A__) || \
  defined(__AVR_ATmega644PA__) || defined(__AVR_ATmega644P__) || defined(__AVR_ATmega644__) || \
  defined(__AVR_ATmega6450A__) || defined(__AVR_ATmega6450P__) || defined(__AVR_ATmega6450__) || \
  defined(__AVR_ATmega645A__) || defined(__AVR_ATmega645P__) || defined(__AVR_ATmega645__) || \
  defined(__AVR_ATmega6490A__) || defined(__AVR_ATmega6490P__) || defined(__AVR_ATmega6490__) || \
  defined(__AVR_ATmega649A__) || defined(__AVR_ATmega649P__) || defined(__AVR_ATmega649__) || \
  defined(__AVR_ATmega64A__) || defined(__AVR_ATmega64C1__) || defined(__AVR_ATmega64M1__) || \
  defined(__AVR_ATmega64__) || defined(__AVR_ATmega8515__) || defined(__AVR_ATmega8535__) || \
  defined(__AVR_ATmega88A__) || defined(__AVR_ATmega88PA__) || defined(__AVR_ATmega88P__) || \
  defined(__AVR_ATmega88__) || defined(__AVR_ATmega8A__) || defined(__AVR_ATmega8U2__) || \
  defined(__AVR_ATmega8__) || \
  defined(__AVR_AT43USB320__) || defined(__AVR_AT43USB355__) || defined(__AVR_AT76C711__) || \
  defined(__AVR_AT90SCR100__) || defined(__AVR_AT94K__) || defined(__AVR_ATA5505__) || \
  defined(__AVR_ATA5702M322__) || defined(__AVR_ATA5782__) || defined(__AVR_ATA5790__) || \
  defined(__AVR_ATA5790N__) || defined(__AVR_ATA5791__) || defined(__AVR_ATA5795__) || \
  defined(__AVR_ATA5831__) || defined(__AVR_ATA6613C__) || defined(__AVR_ATA6614Q__) || \
  defined(__AVR_ATA6617C__) || defined(__AVR_ATA664251__) || defined(__AVR_ATA8210__) || \
  defined(__AVR_ATA8510__) || defined(__AVR_ATmega103__) || defined(__AVR_ATmega1284RFR2__) || \
  defined(__AVR_ATmega128RFR2__) || defined(__AVR_ATmega161__) || defined(__AVR_ATmega163__) || \
  defined(__AVR_ATmega165__) || defined(__AVR_ATmega168PB__) || defined(__AVR_ATmega169__) || \
  defined(__AVR_ATmega16HVA__) || defined(__AVR_ATmega16HVA2__) || defined(__AVR_ATmega16HVBrevB__) || \
  defined(__AVR_ATmega16HVBREVB__) || defined(__AVR_ATmega2564RFR2__) || defined(__AVR_ATmega256RFR2__) || \
  defined(__AVR_ATmega323__) || defined(__AVR_ATmega328PB__) || defined(__AVR_ATmega32HVBrevB__) || \
  defined(__AVR_ATmega32HVBREVB__) || defined(__AVR_ATmega32U6__) || defined(__AVR_ATmega406__) || \
  defined(__AVR_ATmega644RFR2__) || defined(__AVR_ATmega64HVE__) || defined(__AVR_ATmega64HVE2__) || \
  defined(__AVR_ATmega64RFR2__) || defined(__AVR_ATtiny1634__) || defined(__AVR_ATtiny167__) || \
  defined(__AVR_M3000__) \
)

#define IS_TINY ( \
  defined(__AVR_ATtiny10__) || defined(__AVR_ATtiny13A__) || defined(__AVR_ATtiny13__) || \
  defined(__AVR_ATtiny1634__) || defined(__AVR_ATtiny167__) || defined(__AVR_ATtiny20__) || \
  defined(__AVR_ATtiny2313A__) || defined(__AVR_ATtiny2313__) || defined(__AVR_ATtiny24A__) || \
  defined(__AVR_ATtiny24__) || defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny261A__) || \
  defined(__AVR_ATtiny261__) || defined(__AVR_ATtiny26__) || defined(__AVR_ATtiny28__) || \
  defined(__AVR_ATtiny40__) || defined(__AVR_ATtiny4313__) || defined(__AVR_ATtiny43U__) || \
  defined(__AVR_ATtiny44A__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny45__) || \
  defined(__AVR_ATtiny461A__) || defined(__AVR_ATtiny461__) || defined(__AVR_ATtiny48__) || \
  defined(__AVR_ATtiny4__) || defined(__AVR_ATtiny5__) || defined(__AVR_ATtiny828__) || \
  defined(__AVR_ATtiny84A__) || defined(__AVR_ATtiny84__) || defined(__AVR_ATtiny85__) || \
  defined(__AVR_ATtiny861A__) || defined(__AVR_ATtiny861__) || defined(__AVR_ATtiny87__) || \
  defined(__AVR_ATtiny88__) || defined(__AVR_ATtiny9__) \
)

#define IS_REDUCED_CORE_TINY ( \
  defined(__AVR_ATtiny4__) || defined(__AVR_ATtiny5__) || defined(__AVR_ATtiny9__) || \
  defined(__AVR_ATtiny10__) \
)

#define IS_328 ( \
  defined(__AVR_ATmega328__) || defined(__AVR_ATmega328P__) || \
  defined(__AVR_ATmega168A__) || defined(__AVR_ATmega168PA__) || \
  defined(__AVR_ATmega168__) || defined(__AVR_ATmega168P__) || \
  defined(__AVR_ATmega88A__) || defined(__AVR_ATmega88PA__) || \
  defined(__AVR_ATmega88__) || defined(__AVR_ATmega88P__) || \
  defined(__AVR_ATmega48A__) || defined(__AVR_ATmega48PA__) || \
  defined(__AVR_ATmega48__) || defined(__AVR_ATmega48P__) \
)

#define IS_2560 ( \
  defined(__AVR_ATmega2560__) || defined(__AVR_ATmega2561__) || \
  defined(__AVR_ATmega1280__) || defined(__AVR_ATmega1281__) || \
  defined(__AVR_ATmega640__) \
)

#define IS_1284 ( \
  defined(__AVR_ATmega1284__) || defined(__AVR_ATmega1284P__) || \
  defined(__AVR_ATmega644A__) || defined(__AVR_ATmega644PA__) || \
  defined(__AVR_ATmega644__) || defined(__AVR_ATmega644P__) || \
  defined(__AVR_ATmega324A__) || defined(__AVR_ATmega324PA__) || \
  defined(__AVR_ATmega164A__) || defined(__AVR_ATmega164PA__) \
)


#define IS_8 ( \
    defined(__AVR_ATmega8__) \
)

#define IS_TINY4 ( \
  defined(__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__) \
)

#define IS_TINY5 ( \
  defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__) \
)

#define IS_TINY167 ( \
  defined(__AVR_ATtiny87__) || defined(__AVR_ATtiny167__) \
)


