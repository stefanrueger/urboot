/*
 * urboot_bool.h
 *
 * Provide 8 Boolean variables through an I/O register such as GPIOR0, so the
 * compiler can make use of efficient cbi/sbi bit clear and bit set operations
 * as well as conditional skip opcodes sbic and sbis. Not available for
 * ATtiny13/13A, ATmega64/64A/128/128A, ATmega161/162 and ATmega8515.
 *
 * Usage:
 *
 * #define mybool0 urboot_bool(0)
 * #define mybool1 urboot_bool(1)
 *
 * etc and use mybool0, mybool1 etc as (global) Boolean variable in code.
 *
 * Published under GNU General Public License, version 3 (GPL-3.0)
 * Author Stefan Rueger <stefan.rueger@urclocks.com>
 *
 * v 1.0
 * 12.05.2024
 *
 */

#ifndef URBOOT_BOOL_H
#define URBOOT_BOOL_H

typedef struct {
  uint8_t b0:1; uint8_t b1:1; uint8_t b2:1; uint8_t b3:1;
  uint8_t b4:1; uint8_t b5:1; uint8_t b6:1; uint8_t b7:1;
} Urboot_boolpool;

#ifndef UB_BOOL

#undef  _MMIO_BYTE
#define _MMIO_BYTE(mem_addr) (mem_addr)
#define isIO(x) (x <= 0x1F + __SFR_OFFSET)
// #define _SFR_IO8(io_addr) _MMIO_BYTE((io_addr) + __SFR_OFFSET)
// #define _SFR_MEM8(mem_addr) _MMIO_BYTE(mem_addr)

#if defined(GPIOR0) && isIO(GPIOR0) // Most classic parts
#define UB_BOOL GPIOR0

#elif defined(GPIOR) && isIO(GPIOR) // ATA5790, ATA5790N, ATA5791
#define UB_BOOL GPIOR

#elif defined(TWBR) && isIO(TWBR)   // ATmega8/8A/16/16A/32/32A, ATmega163, ATmega323, ATmega8535
#define UB_BOOL TWBR
#endif

#undef  _MMIO_BYTE
#define _MMIO_BYTE(mem_addr) (*(volatile uint8_t *)(mem_addr))
#undef  isIO

#endif // UB_BOOL

#ifdef UB_BOOL
#define urboot_bool(n) (((volatile Urboot_boolpool *)_SFR_MEM_ADDR(UB_BOOL))->b ## n)
#else                           // ATtiny13/13A, ATmega64/64A/128/128A, ATmega161/162, ATmega8515
// No suitable register in IO space for these
#endif

#endif
