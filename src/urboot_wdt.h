#ifndef URBOOT_WDT_H
#define URBOOT_WDT_H

/*
 * modified avr/wdt.h
 *
 *   - wdt_enable() is called isr_wdt_config()
 *   - removed code that disables interrupts as they are disabled in bootloader
 *   - argument of isr_wdt_config() is the bit code for the watchdog register:
 *     it is *not* the time code of wdt_enable()
 */

#if defined(WDTCSR)
#  define _WD_CONTROL_REG     WDTCSR
#elif defined(WDTCR)
#  define _WD_CONTROL_REG     WDTCR
#else
#  define _WD_CONTROL_REG     WDT
#endif

#if defined(WDTOE)
#define _WD_CHANGE_BIT      WDTOE
#else
#define _WD_CHANGE_BIT      WDCE
#endif


/* Copyright (c) 2002, 2004 Marek Michalkiewicz
   Copyright (c) 2005, 2006, 2007 Eric B. Weddington
   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:

   * Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.

   * Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in
     the documentation and/or other materials provided with the
     distribution.

   * Neither the name of the copyright holders nor the names of
     contributors may be used to endorse or promote products derived
     from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
  POSSIBILITY OF SUCH DAMAGE. */


#if defined(__AVR_TINY__)

#define isr_wdt_config(value) __asm__ __volatile__ ( \
  "wdr\n\t"  \
  "out %[CCPADDRESS],%[SIGNATURE]\n\t"  \
  "out %[WDTREG],%[WDVALUE]\n\t"  \
  : /* no outputs */  \
  : [CCPADDRESS] "I" (_SFR_IO_ADDR(CCP)),  \
    [SIGNATURE] "r" ((uint8_t)0xD8), \
    [WDTREG] "I" (_SFR_IO_ADDR(_WD_CONTROL_REG)), \
    [WDVALUE] "r" ((uint8_t) (value)) \
)

#elif defined(CCP)

static __inline__ __attribute__ ((__always_inline__)) void isr_wdt_config(const uint8_t value) {
  if (!_SFR_IO_REG_P (CCP) && !_SFR_IO_REG_P (_WD_CONTROL_REG)) {
    __asm__ __volatile__ (
      "wdr\n\t"
      "sts %[CCPADDRESS],%[SIGNATURE]\n\t"
      "sts %[WDTREG],%[WDVALUE]\n\t"
      : /* no outputs */
      : [CCPADDRESS] "n" (_SFR_MEM_ADDR(CCP)),
      [SIGNATURE] "r" ((uint8_t)0xD8),
      [WDTREG] "n" (_SFR_MEM_ADDR(_WD_CONTROL_REG)),
      [WDVALUE] "r" ((uint8_t) (value))
    );
  } else if (!_SFR_IO_REG_P (CCP) && _SFR_IO_REG_P (_WD_CONTROL_REG)) {
    __asm__ __volatile__ (
      "wdr\n\t"
      "sts %[CCPADDRESS],%[SIGNATURE]\n\t"
      "out %[WDTREG],%[WDVALUE]\n\t"
      : /* no outputs */
      : [CCPADDRESS] "n" (_SFR_MEM_ADDR(CCP)),
      [SIGNATURE] "r" ((uint8_t)0xD8),
      [WDTREG] "I" (_SFR_IO_ADDR(_WD_CONTROL_REG)),
      [WDVALUE] "r" ((uint8_t) (value))
    );
  } else if (_SFR_IO_REG_P (CCP) && !_SFR_IO_REG_P (_WD_CONTROL_REG)) {
    __asm__ __volatile__ (
      "wdr\n\t"
      "out %[CCPADDRESS],%[SIGNATURE]\n\t"
      "sts %[WDTREG],%[WDVALUE]\n\t"
      : /* no outputs */
      : [CCPADDRESS] "I" (_SFR_IO_ADDR(CCP)),
      [SIGNATURE] "r" ((uint8_t)0xD8),
      [WDTREG] "n" (_SFR_MEM_ADDR(_WD_CONTROL_REG)),
      [WDVALUE] "r" ((uint8_t) (value))
    );
  } else {
    __asm__ __volatile__ (
      "wdr\n\t"
      "out %[CCPADDRESS],%[SIGNATURE]\n\t"
      "out %[WDTREG],%[WDVALUE]\n\t"
      : /* no outputs */
      : [CCPADDRESS] "I" (_SFR_IO_ADDR(CCP)),
      [SIGNATURE] "r" ((uint8_t)0xD8),
      [WDTREG] "I" (_SFR_IO_ADDR(_WD_CONTROL_REG)),
      [WDVALUE] "r" ((uint8_t) (value))
    );
  }
}

#else

static __inline__ __attribute__ ((__always_inline__)) void isr_wdt_config (const uint8_t value) {
  if (_SFR_IO_REG_P (_WD_CONTROL_REG)) {
    __asm__ __volatile__ (
      "wdr\n\t"
      "out %0, %1\n\t"
      "out %0, %2" "\n \t"
      : /* no outputs */
      : "I" (_SFR_IO_ADDR(_WD_CONTROL_REG)),
      "r" ((uint8_t)(_BV(_WD_CHANGE_BIT) | _BV(WDE))),
      "r" ((uint8_t) (value))
    );
  } else {
    __asm__ __volatile__ (
      "wdr\n\t"
      "sts %0, %1\n\t"
      "sts %0, %2" "\n \t"
      : /* no outputs */
      : "n" (_SFR_MEM_ADDR(_WD_CONTROL_REG)),
      "r" ((uint8_t)(_BV(_WD_CHANGE_BIT) | _BV(WDE))),
      "r" ((uint8_t) (value))
    );
  }
}
#endif

#define wdt_reset() __asm__ __volatile__ ("wdr")

#endif
