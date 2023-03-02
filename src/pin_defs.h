/*
 * pin_defs.h
 *
 * This header defines the default pin assignment for the LED of various chips
 * that are supported, though really, it's the boards that determine that, not the
 * chip. The defaults can be overwritten by make arguments such as LED=AtmelPB6
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

// Boards actually differ quite a bit re LED position
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



// Macros below are to determine uP classes for SWIO rx/tx timing
// Could maybe switch to sth like #define IS_XMEGA (defined(__AVR_XMEGA__) && __AVR_XMEGA__)

#define IS_XMEGA (defined(__AVR_ATxmega8E5__) || defined(__AVR_ATxmega16A4__) || \
  defined(__AVR_ATxmega16A4U__) || defined(__AVR_ATxmega16C4__) || \
  defined(__AVR_ATxmega16D4__) || defined(__AVR_ATxmega16E5__) || \
  defined(__AVR_ATxmega32C3__) || defined(__AVR_ATxmega32D3__) || \
  defined(__AVR_ATxmega32A4__) || defined(__AVR_ATxmega32A4U__) || \
  defined(__AVR_ATxmega32C4__) || defined(__AVR_ATxmega32D4__) || \
  defined(__AVR_ATxmega32E5__) || defined(__AVR_ATxmega64A1__) || \
  defined(__AVR_ATxmega64A1U__) || defined(__AVR_ATxmega64B1__) || \
  defined(__AVR_ATxmega64A3__) || defined(__AVR_ATxmega64A3U__) || \
  defined(__AVR_ATxmega64B3__) || defined(__AVR_ATxmega64C3__) || \
  defined(__AVR_ATxmega64D3__) || defined(__AVR_ATxmega64A4__) || \
  defined(__AVR_ATxmega64A4U__) || defined(__AVR_ATxmega64D4__) || \
  defined(__AVR_ATxmega128A1__) || defined(__AVR_ATxmega128A1revD__) || \
  defined(__AVR_ATxmega128A1U__) || defined(__AVR_ATxmega128B1__) || \
  defined(__AVR_ATxmega128A3__) || defined(__AVR_ATxmega128A3U__) || \
  defined(__AVR_ATxmega128B3__) || defined(__AVR_ATxmega128C3__) || \
  defined(__AVR_ATxmega128D3__) || defined(__AVR_ATxmega128A4__) || \
  defined(__AVR_ATxmega128A4U__) || defined(__AVR_ATxmega128D4__) || \
  defined(__AVR_ATxmega192A1__) || defined(__AVR_ATxmega192A3__) || \
  defined(__AVR_ATxmega192A3U__) || defined(__AVR_ATxmega192C3__) || \
  defined(__AVR_ATxmega192D3__) || defined(__AVR_ATxmega256A1__) || \
  defined(__AVR_ATxmega256A3__) || defined(__AVR_ATxmega256A3B__) || \
  defined(__AVR_ATxmega256A3BU__) || defined(__AVR_ATxmega256A3U__) || \
  defined(__AVR_ATxmega256C3__) || defined(__AVR_ATxmega256D3__) || \
  defined(__AVR_ATxmega384C3__) || defined(__AVR_ATxmega384D3__))

#define IS_REDUCED_CORE_TINY (defined(__AVR_ATtiny4__) || defined(__AVR_ATtiny5__) || \
 defined(__AVR_ATtiny9__) || defined(__AVR_ATtiny10__) || defined(__AVR_ATtiny20__) || \
 defined(__AVR_ATtiny40__) || defined(__AVR_ATtiny102__) || defined(__AVR_ATtiny104__))


// According to the data sheet these initialse SP to RAMEND on reset
#define UB_INIT_SP_IS_RAMEND (defined(__AVR_AT90USB1286__) || defined(__AVR_AT90USB1287__) || \
  defined(__AVR_AT90USB646__) || defined(__AVR_AT90USB647__) || defined(__AVR_AT90USB162__) || \
  defined(__AVR_AT90USB82__) || defined(__AVR_ATA5505__) || defined(__AVR_ATA6612C__) || \
  defined(__AVR_ATA6613C__) || defined(__AVR_ATA6614Q__) || defined(__AVR_ATA6616C__) || \
  defined(__AVR_ATA6617C__) || defined(__AVR_ATA664251__) || defined(__AVR_ATmega1280__) || \
  defined(__AVR_ATmega1281__) || defined(__AVR_ATmega640__) || defined(__AVR_ATmega2560__) || \
  defined(__AVR_ATmega2561__) || defined(__AVR_ATmega1284P__) || defined(__AVR_ATmega164A__) || \
  defined(__AVR_ATmega164P__) || defined(__AVR_ATmega164PA__) || defined(__AVR_ATmega324A__) || \
  defined(__AVR_ATmega324P__) || defined(__AVR_ATmega324PA__) || defined(__AVR_ATmega324PB__) || \
  defined(__AVR_ATmega644A__) || defined(__AVR_ATmega644P__) || defined(__AVR_ATmega644PA__) || \
  defined(__AVR_ATmega1284__) || defined(__AVR_ATmega644__) ||  \
  defined(__AVR_ATmega1284RFR2__) || defined(__AVR_ATmega128RFR2__) || \
  defined(__AVR_ATmega2564RFR2__) || defined(__AVR_ATmega256RFR2__) || \
  defined(__AVR_ATmega644RFR2__) || defined(__AVR_ATmega64RFR2__) || \
  defined(__AVR_ATmega128RFA1__) || defined(__AVR_ATmega16HVBrevB__) || \
  defined(__AVR_ATmega32HVB__) || defined(__AVR_ATmega32HVBrevB__) || \
  defined(__AVR_ATmega16HVB__) || defined(__AVR_ATmega8U2__) || defined(__AVR_ATmega16U2__) || \
  defined(__AVR_ATmega32U2__) || defined(__AVR_ATmega16U4__) || defined(__AVR_ATmega32U4__) || \
  defined(__AVR_ATmega32U6__) || defined(__AVR_ATmega168__) || defined(__AVR_ATmega168A__) || \
  defined(__AVR_ATmega168P__) || defined(__AVR_ATmega168PA__) || defined(__AVR_ATmega48__) || \
  defined(__AVR_ATmega48A__) || defined(__AVR_ATmega48P__) || defined(__AVR_ATmega48PA__) || \
  defined(__AVR_ATmega88__) || defined(__AVR_ATmega88A__) || defined(__AVR_ATmega88P__) || \
  defined(__AVR_ATmega88PA__) || defined(__AVR_ATmega328__) || defined(__AVR_ATmega328P__) || \
  defined(__AVR_ATmega328PB__) || defined(__AVR_ATmega16M1__) || defined(__AVR_ATmega32C1__) || \
  defined(__AVR_ATmega32M1__) || defined(__AVR_ATmega64C1__) || defined(__AVR_ATmega64M1__) || \
  defined(__AVR_ATtiny13__) || defined(__AVR_ATtiny13A__) || defined(__AVR_ATtiny1634__) || \
  defined(__AVR_ATtiny167__) || defined(__AVR_ATtiny87__) || defined(__AVR_ATtiny2313__) || \
  defined(__AVR_ATtiny2313A__) || defined(__AVR_ATtiny4313__) || defined(__AVR_ATtiny24__) || \
  defined(__AVR_ATtiny24A__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny44A__) || \
  defined(__AVR_ATtiny84__) || defined(__AVR_ATtiny84A__) || defined(__AVR_ATtiny25__) || \
  defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__) || defined(__AVR_ATtiny261__) || \
  defined(__AVR_ATtiny461__) || defined(__AVR_ATtiny861__) || defined(__AVR_ATtiny261A__) || \
  defined(__AVR_ATtiny461A__) || defined(__AVR_ATtiny861A__) || defined(__AVR_ATtiny441__) || \
  defined(__AVR_ATtiny841__) || defined(__AVR_ATtiny43U__) || defined(__AVR_ATtiny48__) || \
  defined(__AVR_ATtiny88__) || defined(__AVR_ATtiny828__))
