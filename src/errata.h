/*
 * errata.h
 *
 * published under GNU General Public License, version 3 (GPL-3.0)
 * author Stefan Rueger <stefan.rueger@urclocks.com>
 *
 * v1.3
 * 26.03.2022
 *
 * Different avr-libc versions have had different errors in the past. My preference is still
 * avr-gcc v 4.8.1 owing to the tight code it produces, so need to tend to past (and current)
 * errors in avr-libc.
 *
 * See urboot.c for details.
 */

#ifndef ERRATA_H
#define ERRATA_H

#if defined(__AVR_ATtiny4__) || defined(__AVR_ATtiny5__)
#undef  SIGNATURE_1
#define SIGNATURE_1 0x8F

#elif defined(__AVR_AT90S4434__)
#undef  SIGNATURE_1
#undef  SIGNATURE_2
#define SIGNATURE_1 0x92
#define SIGNATURE_2 0x02

#elif defined(__AVR_ATmega164A__)
#undef  SIGNATURE_2
#define SIGNATURE_2 0x0F

#elif defined(__AVR_ATmega324A__)
#undef  SIGNATURE_2
#define SIGNATURE_2 0x15

#elif defined(__AVR_ATmega169A__)
#undef  SIGNATURE_2
#define SIGNATURE_2 0x11

#elif defined(__AVR_ATmega329P__) || defined(__AVR_ATmega329PA__)
#undef  SIGNATURE_2
#define SIGNATURE_2 0x0B

#elif defined(__AVR_ATmega3290P__) ||  defined(__AVR_ATmega6490P__)
#undef  SIGNATURE_2
#define SIGNATURE_2 0x0C

#elif defined(__AVR_ATmega325P__) ||  defined(__AVR_ATmega645P__)
#undef  SIGNATURE_2
#define SIGNATURE_2 0x0D

#elif defined(__AVR_ATmega3250P__) ||  defined(__AVR_ATmega6450P__)
#undef  SIGNATURE_2
#define SIGNATURE_2 0x0E

#elif defined(__AVR_ATA664251__)
#undef  SPM_PAGESIZE
#undef  FLASHEND
#undef  SIGNATURE_1
#undef  SIGNATURE_2

#define SPM_PAGESIZE 128
#define FLASHEND 0x3FFF
#define SIGNATURE_1 0x94
#define SIGNATURE_2 0x87

#elif defined(__AVR_ATmega165__)
#undef  SIGNATURE_2
#define SIGNATURE_2 0x10

#elif defined(__AVR_ATmega165A__)
#undef  SIGNATURE_2
#define SIGNATURE_2 0x10

#elif defined(__AVR_ATmega328__)
#undef  SIGNATURE_2
#define SIGNATURE_2 0x14

#elif defined(__AVR_ATtiny48__)
#undef  SPM_PAGESIZE
#define SPM_PAGESIZE 64

#elif defined(__AVR_ATtiny88__)
#undef  SPM_PAGESIZE
#define SPM_PAGESIZE 64

#elif defined(__AVR_ATxmega64A1U__)
#undef  SPM_PAGESIZE
#define SPM_PAGESIZE 256
#undef  BOOT_SECTION_PAGE_SIZE
#define BOOT_SECTION_PAGE_SIZE 256
#undef  BOOT_SECTION_START
#define BOOT_SECTION_START 0x10000
#undef  APPTABLE_SECTION_START
#define APPTABLE_SECTION_START 0xF000
#undef  APP_SECTION_PAGE_SIZE
#define APP_SECTION_PAGE_SIZE 256
#undef  FUSE_MEMORY_SIZE
#define FUSE_MEMORY_SIZE 6

#elif defined(__AVR_ATxmega16E5__)
#undef  BOOT_SECTION_START
#define BOOT_SECTION_START 0x4000

#elif defined(__AVR_ATxmega128A1U__)
#undef  FUSE_MEMORY_SIZE
#define FUSE_MEMORY_SIZE 6

#elif defined(__AVR_AT90PWM1__)
#undef  SIGNATURE_0
#undef  SIGNATURE_1
#undef  SIGNATURE_2
#define SIGNATURE_0 0x1E
#define SIGNATURE_1 0x93
#define SIGNATURE_2 0x83

#elif defined(__AVR_AT90PWM2__)
#undef  SIGNATURE_0
#undef  SIGNATURE_1
#undef  SIGNATURE_2
#define SIGNATURE_0 0x1E
#define SIGNATURE_1 0x93
#define SIGNATURE_2 0x81

#elif defined(__AVR_AT90PWM3__)
#undef  SIGNATURE_0
#undef  SIGNATURE_1
#undef  SIGNATURE_2
#define SIGNATURE_0 0x1E
#define SIGNATURE_1 0x93
#define SIGNATURE_2 0x81

#elif defined(__AVR_AT90S2333__)
#undef  SIGNATURE_0
#undef  SIGNATURE_1
#undef  SIGNATURE_2
#define SIGNATURE_0 0x1E
#define SIGNATURE_1 0x91
#define SIGNATURE_2 0x05

#elif defined(__AVR_AT90USB82__)
#undef  SIGNATURE_0
#undef  SIGNATURE_1
#undef  SIGNATURE_2
#define SIGNATURE_0 0x1E
#define SIGNATURE_1 0x93
#define SIGNATURE_2 0x82

#elif defined(__AVR_ATmega8HVA__)
#undef  SIGNATURE_0
#undef  SIGNATURE_1
#undef  SIGNATURE_2
#define SIGNATURE_0 0x1E
#define SIGNATURE_1 0x93
#define SIGNATURE_2 0x10

#elif defined(__AVR_ATmega32HVBrevB__) || defined(__AVR_ATmega32HVBREVB__)
#undef  SIGNATURE_0
#undef  SIGNATURE_1
#undef  SIGNATURE_2
#define SIGNATURE_0 0x1E
#define SIGNATURE_1 0x95
#define SIGNATURE_2 0x10

#endif

#endif
