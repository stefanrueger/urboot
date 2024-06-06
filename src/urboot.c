/*
 * Urboot bootloader for classic 8-bit AVR microprocessors
 *
 * published under GNU General Public License, version 3 (GPL-3.0)
 * author Stefan Rueger <stefan.rueger@urclocks.com>
 *
 * v8.0
 * 06.06.2024 (first version published in July 2016)
 *
 * Feature-rich bootloader that is fast and small
 *  - Tight code: most bootloaders fit into
 *     + 256 bytes albeit without EEPROM read/write support
 *     + 384 bytes with EEPROM read/write support
 *     + 512 bytes with dual-boot, EEPROM read/write support and Chip Erase capability
 *  - Highly modular feature set:
 *     + Dedicated protocol between programmer and bootloader (urprotocol)
 *     + EEPROM read/write support
 *     + Vector bootloader for devices without a boot section or to save space on devices with one
 *     + Software UART: no hardware UART required, but also useful when the chip's CPU frequency
 *       and the bootloader's desired baud rate are incompatible (eg, 8 MHz and 115,200 baud)
 *     + Subroutine pgm_write_page(sram, progmem) for applications so they can change themselves:
 *       on many MCUs the SPM write flash only works when called from the bootloader section
 *     + Dual programming from external SPI flash memory for over-the-air programming
 *     + Template bootloader with nops that will be replaced "on the fly" with code to manipulate
 *       the right LED/TX/RX/CS pins at bootloader-burning time (see accompanying urloader sketch)
 *     + Saves the reset flags in R2 for inspection by the application via the .init0 section
 *     + Bootloader protects itself from overwriting
 *     + Automatic host baud rate detection
 *     + Chip erase in bootloader (faster than -c urclock emulation)
 *  - Avrdude (from v7.1 onwards) supports urboot bootloaders with -c urclock
 *
 * Supported and tested (for v8.0)
 *  - ATmega328P (Urclock, UrSense, UrclockMini, Uno, Duemilanove, Anarduino, Moteino, Jeenode etc)
 *  - ATmega1284P (UrclockMega, MoteinoMega)
 *  - ATmega2560 (Mega R3)
 *  - ATtiny85 (Digispark)
 *  - ATtiny167 (Digispark Pro)
 *
 * Tested (for v7.7)
 *  - ATmega328P (Urclock, UrSense, UrclockMini, Uno, Duemilanove, Anarduino, Moteino, Jeenode etc)
 *  - ATmega1284P (UrclockMega, MoteinoMega)
 *  - ATmega2560 (Mega R3)
 *  - ATmega162
 *  - ATmega32 (MightyCore)
 *  - ATtiny88 (MH Tiny)
 *  - ATtiny85 (Digispark)
 *  - ATtiny167 (Digispark Pro)
 *  - ATtiny2313
 *  - ATtiny1634
 *  - ATtiny841
 *  - ATtiny13A
 *  - ATmega128RFA1
 *
 * Unsupported (without ever planning to support them)
 *  - AVRtiny core (ATtiny4, ATtiny5, ATtiny9, ATtiny10, ATtiny20, ATtiny40, ATtiny102, ATtiny104)
 *  - Minimal AVR1 core (AT90S1200, ATtiny11, ATtiny12, ATtiny15, ATtiny28)
 *  - AVR2 core without movw instructions (ATtiny22, ATtiny26, AT90S2313, AT90S2323, AT90S2333,
 *    AT90S2343, AT90S4414, AT90S4433, AT90S4434, AT90S8515, AT90C8534, AT90S8535)
 *  - AVR3 core without movw instructions (ATmega103, AT43USB320, AT43USB355, AT76C711)
 *
 * Unsupported within this source code file
 *  - ATxmega devices and newer AVR-DA/DB/DD/DU/EA/EB ... devices: these require different I/O
 *    code, different flash write code, different GPIO handling for LEDs & attached SPM memory
 *
 * Compiles but untested for the following -mmcu=... options
 *   at90can128 at90can32 at90can64 at90pwm1 at90pwm161 at90pwm2 at90pwm216 at90pwm2b at90pwm3
 *   at90pwm316 at90pwm3b at90pwm81 at90scr100 at90usb1286 at90usb1287 at90usb162 at90usb646
 *   at90usb647 at90usb82 ata5272 ata5505 ata5702m322 ata5782 ata5790 ata5790n ata5791 ata5795
 *   ata5831 ata6285 ata6286 ata6289 ata6612c ata6613c ata6614q ata6616c ata6617c ata664251 ata8210
 *   ata8510 atmega128 atmega1280 atmega1281 atmega1284 atmega1284rfr2 atmega128a
 *   atmega128rfr2 atmega16 atmega161 atmega163 atmega164a atmega164p atmega164pa atmega165
 *   atmega165a atmega165p atmega165pa atmega168 atmega168a atmega168p atmega168pa atmega168pb
 *   atmega169 atmega169a atmega169p atmega169pa atmega16a atmega16hva atmega16hva2 atmega16hvb
 *   atmega16hvbrevb atmega16m1 atmega16u2 atmega16u4 atmega2561 atmega2564rfr2 atmega256rfr2
 *   atmega323 atmega324a atmega324p atmega324pa atmega324pb atmega325 atmega3250
 *   atmega3250a atmega3250p atmega3250pa atmega325a atmega325p atmega325pa atmega328 atmega328pb
 *   atmega329 atmega3290 atmega3290a atmega3290p atmega3290pa atmega329a atmega329p atmega329pa
 *   atmega32a atmega32c1 atmega32hvb atmega32hvbrevb atmega32m1 atmega32u2 atmega32u4 atmega32u6
 *   atmega406 atmega48 atmega48a atmega48p atmega48pa atmega48pb atmega64 atmega640 atmega644
 *   atmega644a atmega644p atmega644pa atmega644rfr2 atmega645 atmega6450 atmega6450a atmega6450p
 *   atmega645a atmega645p atmega649 atmega6490 atmega6490a atmega6490p atmega649a atmega649p
 *   atmega64a atmega64c1 atmega64hve atmega64hve2 atmega64m1 atmega64rfr2 atmega8 atmega8515
 *   atmega8535 atmega88 atmega88a atmega88p atmega88pa atmega88pb atmega8a atmega8hva atmega8u2
 *   attiny13 attiny2313a attiny24 attiny24a attiny25 attiny261 attiny261a attiny4313 attiny43u
 *   attiny44 attiny441 attiny44a attiny45 attiny461 attiny461a attiny48 attiny828 attiny84
 *   attiny84a attiny861 attiny861a
 *
 * How the bootloader works
 *
 *   In common with bootloaders, it runs after every reset and tries to communicate with an
 *   external uploader program via a serial interface using a variant of the STK500v1 protocol.
 *   Hence, only the serial I/O needs to be connected from a Raspberry Pi/PC/laptop host to the
 *   board with the to-be-programmed MCU on it. After every reset the bootloader code determines
 *   its cause: if that was an external reset as opposed to power-up reset or watchdog timeout
 *   (WDT) it waits a small time for protocol initialisation bytes from an uploader program. If
 *   there was was a valid handshake, the bootloader carries out one of several possible tasks
 *   driven by the host's uploader such as receiving and burning a new sketch, downloading an
 *   existing sketch on the MCU, and, if compiled for it, reading or writing the EEPROM on the MCU.
 *   The bootloader finishes this part typically through a watchdog timeout reset that kicks in
 *   when initially no valid handshake could be detected, when a serial hardware or protocol error
 *   occurred during burning/verification or when the bootloader finished its task successfully.
 *   Watchdog timeout resets the MCU just like an external reset. When the bootloader is entered
 *   under this condition, though, it then normally jumps directly to the application. However,
 *   when the bootloader was compiled with dual-boot support, it first checks external SPI flash
 *   memory to see it contains a new sketch, in which case a copy of it is burned from external
 *   memory onto internal flash. The idea is that the application sketch could have been written to
 *   receive a new version via a radio and have placed it onto the external SPI flash memory before
 *   issuing a WDT reset. Hence, this dual-boot property is also called over the air programming,
 *   although one could conceivably also plug a new external SPI flash into a board for
 *   programming.
 *
 *
 * Assumptions, limitations and caveats of *any* bootloader
 *
 *   - Establishing communication invariably causes some small startup delay on external reset.
 *
 *   - For bootloaders on ISP MCUs there is no need for a physical programmer, though in order to
 *     burn the bootloader itself onto the MCU, an SPI header and an SPI programmer is needed at
 *     least once in the beginning. Burning the bootloader using SPI necessitates the MCU be in
 *     reset mode: all chip select lines of attached SPI hardware (external flash memory, radios
 *     etc) need therefore be pulled high through resistors to ensure all external SPI devices are
 *     inactive during SPI programming of the bootloader.
 *
 *   - In most cases the connection from the PC for uploading sketches via the bootloader will be
 *     through USB, which necessitates a USB to serial converter cable if the destination board
 *     does not have USB. Some PCs (most notably, the Raspberry Pi) have a native serial connection
 *     that also could be used for connecting to a board without USB.
 *
 *   - The bootloader sets the TX line of the UART or the designated TX pin of the software serial
 *     communication to output after every reset. If the bootloader is compiled to blink an LED, to
 *     output a square wave for debugging or to communicate via SPI with external memory for dual
 *     programming, there will be other lines that are set to output shortly after each reset. Not
 *     all projects can deal gracefully with these short flares of output activity on some of the
 *     pins. If you use a bootloader for production settings it is best to carefully consider the
 *     hardware implications on the circuit.
 *
 *   - As with all bootloaders they work best when the host has a way to reset the board, which is
 *     typically done via DTR/RTS lines of the host's serial port. Avrdude -c [arduino|urclock]
 *     does this automatically. The board needs to have hardware to facilitate the DTR/RTS
 *     connection for reset. Most boards do. If not, sometimes running a small dedicated reset
 *     program on the host just before running avrdude helps. This reset program somehow needs to
 *     pull the board's reset line low for a short time; on a Raspberry Pi external GPIO pins can
 *     be used for that. If that is not possible either, then setting the watchdog timeout to a
 *     long time (see the WDTO option below) may be helpful, so one can manually reset the board
 *     before calling avrdude. The default for the timeout is 500 ms. Only with the urclock
 *     programmer can smaller timeouts down to 128 ms be reliably utilised.
 *
 *
 * Assumptions, limitations, caveats, tips and tricks for *this* bootloader
 *
 *   - The uploading program is assumed to be avrdude with the urclock programmer (-c urclock)
 *
 *   - A bootloader with dual-boot support needs to know which port pin is assigned to the chip
 *     select of the SPI flash memory, which pin drives a blinking LED (if wanted), where the tx/rx
 *     lines are for serial communication, how long the watchdog timeout should be etc. This
 *     explodes the option space for this bootloader. Using the accompanying urloader sketch for
 *     burning this bootloader onto a board mitigates this somewhat by the use of "template"
 *     bootloaders (see TEMPLATE option). Urloader lets you interactively set at bootloader burn
 *     time which pins it should set/clear for LED, chip select, and RX/TX (for software serial
 *     I/O). Urloader knows some common boards and offers the right bootloaders for them.
 *
 *   - Remember that dual-boot bootloaders communicate with the SPI flash memory at all external
 *     and WDT resets. Therefore, all other attached SPI devices need their chip select pulled
 *     high. While in theory urboot.c could be changed so that it uses software pullup for these
 *     additional devices, this could cost additional code and will complicate the already too big
 *     option space. It is better practice to use hardware pullups in the board design.
 *
 *   - A dual-boot bootloader deals gracefully with a board that has no external SPI flash memory:
 *     it simply reads 0xff values and decides there is nothing interesting there. However, this
 *     causes unnecessary delay at each external reset and each WDT reset, and it toggles the SPI
 *     and chip select lines (see above); it is therefore not recommended to burn the most-feature
 *     rich urboot bootloader onto all your boards. Carefully determine what you actually need.
 *
 *   - When using external SPI memory on a board with a dual-boot bootloader, remember to reserve
 *     the first FLASHEND+1 bytes exclusively for sketches to be burned onto the MCU. These are
 *     recognised by the second byte being indicative of an rjmp or a jmp opcode. Placing  some
 *     random data in that area risks the MCU being programmed with just these random data.
 *
 *   - Vector bootloaders are great for devices with no dedicated boot section. They assume the
 *       + Vector table of the MCU, and therefore the reset vector, resides at address zero
 *       + Compiler puts either a jmp or an rjmp at address zero
 *       + Compiler does not zap/shorten the vector table if no or few interrupts are used
 *       + Compiler does not utilise unused interrupt vectors to place code there
 *
 *     This should be the case with all regular sketches produced by avr-gcc. Vector bootloaders
 *     are also useful for devices with boot section support to allow them to use less space than
 *     the smallest offered hardware-supported boot section. In this case ensure that the fuses are
 *     set to make the processor jump to the reset vector as opposed to the bootloader. And ensure
 *     that the protection bits in the lock byte actually allow code be written into the bootloader
 *     section with SPM instructions. Otherwise the extra space in the boot section that is freed
 *     by smaller vector bootloaders cannot be used. The urloader sketch sets fuses and lock bits
 *     appropriately when burning a vector bootloader onto your board. More on VBLs below.
 *
 *   - The code makes several assumptions that reduce the code size (eg, no interrupts can occur,
 *     SP points to RAMEND). They are true after a hardware reset, but will not necessarily be true
 *     if the bootloader is called directly. So don't.
 *
 *
 * Acknowledgements. Code builds on the following work
 *  - stk500boot.c          by Jason P. Kyle
 *  - Arduino bootloader    http://arduino.cc
 *  - Spiff's 1K bootloader http://spiffie.org/know/arduino_1k_bootloader/bootloader.shtml
 *  - avr-libc project      http://nongnu.org/avr-libc
 *  - Adaboot               http://www.ladyada.net/library/arduino/bootloader.html
 *  - AVR305                Atmel Application Note
 *
 * Copyright 2016-2022 for complete rewrite by Stefan Rueger (based on a 2016 optiboot version)
 * Copyright 2013-2015 previous versions by Bill Westfield
 * Copyright 2010 by Peter Knight
 *
 */

/*
 * Rewritten entirely by Stefan Rueger in 2016
 *
 *
 * There are now 6 main optional features (EEPROM support, vector bootloader, software serial code,
 * dual boot from external flash, on-the-fly setting of LED/TX/RX/CS pins at burn-time and a
 * pgm_write_page() function that can be called from the application code) plus a lot of fine-
 * grained compile-time options to control size and behaviour. The first three already existed, but
 * didn't necessarily fit into 512 bytes. Now most features fit into 512 bytes.
 *
 * Urboot version number in the object code
 *
 * I put my own spiel on version numbers so that they are encoded in one byte (bit packing) freeing
 * one byte for a description of the features of the specific bootloader such as VBL, DUAL, EEPROM,
 * ... This is useful, eg, to detect whether a bootloader is a VBL or provides the pgm_write_page()
 * function. The urboot-gcc wrapper makes use of that feature, as does the urloader program.
 *
 *
 * Condition to enter the bootloader
 *
 * Multiple reset flags can be set on startup: watchdog, brown out, power on, external and jtag.
 * When should the application be started, when the serial bootloader be run, and when the external
 * flash memory be checked for a viable program image? In this implementation the first decision is
 * between serial bootloader code and starting the app. If the latter then the external flash
 * memory is checked if and only if the watchdog reset flag was set; that check happens before the
 * actual app is started. Here the choices for the first decision:
 *
 *   if(ch != _BV(EXTRF))
 *
 * Only run serial bootloader on 'external reset exclusively' - anything else: run the application
 * (with a possible detour over checking dual booting from external SPI memory). This can lead to
 * problems when the application happens to trigger the watchdog early on, which makes the "reset
 * and nothing else" condition nearly impossible.
 *
 *   if(!(ch & _BV(EXTRF)))
 *
 * This runs the serial bootloader if and only if there was an external reset (irrespective of a
 * watchdog timeout), otherwise run the application with a possible detour over checking dual
 * booting from external SPI flash memory. My choice here.
 *
 *   if(ch & (_BV(WDRF) | _BV(BORF) | _BV(PORF)))
 *
 * This is the condition for starting the app in other bootloaders. It used to be that the serial
 * bootloader also would run when nothing caused the reset (all xxxRF == 0), so you could run it by
 * jumping direct to START from the application.
 *
 *
 * Edit History:
 *
 * May 2024
 * 8.0 smr: Remove -c arduino compatibility, make URPROTOCOL default (and remove it)
 * Nov 2022
 * 7.7 smr: added autobaud and, for vector bootloaders, reset vector protection
 * Jun 2022
 * 7.6 smr: refined the URPROTOCOL protocol implementation, now compiles for 184 MCUs
 * Dec 2021
 * 7.5 smr: added option URPROTOCOL that significantly reduces code by shifting a lot of work
 *     to avrdude (patching vector table for vector bootloader, removing avrdude requests for
 *     unnecessary info, etc). Also created a programmer urclock for avrdude
 * Apr 2021
 * 7.4 smr: changed condition entering bootloader to "external reset" from "only external reset"
 * 7.3 smr: added TEMPLATE feature
 *
 * Jul 2016
 * 7.2 smr: packed feature bits into urboot_version so that urloader displays them; new Atmel port
 *     port code (atmel_ports.h) so TX/RX/SFMCS all can be set same as LEDs; reformatted code;
 *     wrote urboot-gcc wrapper in perl and checked that code compiles for 110+ MCUs
 *
 * 7.1 smr: changed Makefile to work with new options; further reduced code size with careful
 *     recoding; rewrote eeprom access and moved address to r30; Adding 3-4 features keeps the code
 *     in under 512 byte for most MCUs
 *
 * Jun 2016
 * 7.0 smr: reviewed Bill Westfield's code; rewrote it reducing its size; introduced fine-grained
 *     compile time options to better control the size of bootloader; made vector bootloaders work;
 *     introduced dual boot, auto-increment (removed in v7.5), pgm_write_page()
 */


#include <inttypes.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <avr/eeprom.h>

#include <avr/boot.h>
#include "urboot_mcuid.h"       // MCU id (a small int constant sent to avrdude)

#include "urboot_wdt.h"         // Don't use <avr/wdt.h> owing to unwanted interrupt overhead
#include "urboot_ioregs.h"      // I/O register definitions; RAM and EEPROM sizes
#include "pin_defs.h"           // Definitions for LEDs and REG names (include after urboot_ioregs.h)
#include "errata.h"

#ifndef UARTNUM                 // Default UART to 0
#define UARTNUM 0
#endif

#ifndef UARTALT                 // Default UART ALT pin setting to 0 (ie, not active)
#define UARTALT 0
#endif

#include "ur_uart.h"            // Unified register and bit names for various UART types
#include "ur_uartnum.h"         // Generic UARTn names depending on UARTNUM/UARTALT

#ifndef VERSION
#define VERSION            0100 // v8.0 in octal, as minor version is in 3 least significant bits
#endif

#define MAJORVER (VERSION >> 3) // Max 31
#define MINORVER (VERSION & 7)  // Max 7


// Some sanity checks
#if SPM_PAGESIZE > 256          // Not applicable for classic parts, but check just in case
#error urboot.c cannot handle flash page sizes > 256
#endif

#if !defined(RAMSTART) || !defined(RAMEND)
#error Need to know RAMSTART and RAMEND
#endif
#if defined(RAMSIZE) && RAMEND-RAMSTART+1 != RAMSIZE
#error Inconsistent RAMSIZE/RAMSTART/RAMEND
#endif

#if EEPROM && (!defined(EESIZE) || EESIZE < 0)
#error Need to know EESIZE to generate bootloader EEPROM code
#endif



/*
 * Different flash memory size categories:
 *  -   8k or less: no jmp, only rjmp, small device
 *  -  64k or less: no need for RAMPZ, plain sailing, use lmp/smp
 *  - 128k or less: needs RAMPZ for elpm/spm, jmp targets
 *  - more than 128k: needs RAMPZ for elpm/spm, EIND for indirect jumps, jmp targets exceed 16-bit
 *  - more than 4M: low 16 bit of jmp opcode no longer 0x940c (currently only a theoretical issue)
 */

#define FLASHin8k      (FLASHEND <=     8191ul)
#define FLASHabove8k   (FLASHEND  >     8191ul)
#define FLASHin64k     (FLASHEND <=    65535ul)
#define FLASHabove64k  (FLASHEND  >    65535ul)
#define FLASHin128k    (FLASHEND <=  0x1fffful)
#define FLASHabove128k (FLASHEND  >  0x1fffful)
#define FLASHabove4M   (FLASHEND  > 0x3ffffful)

// Set to 1 iff flash size is power of 2 (All classic parts except ATmega406 with its 40 kB flash)
#ifndef FLASHWRAPS
#define FLASHWRAPS (!(FLASHEND & (FLASHEND+1UL)))
#endif


#ifndef DUAL
#define DUAL                  0
#endif

#ifndef VBL // Vector bootloader
#define VBL                   0
#endif

#if VBL > 1
#warning v8.0+ no longer caters for VBL 2 and 3, setting VBL to 1
#undef VBL
#define VBL 1
#endif

#if defined(SFMCS) || defined(LED)
#define TEMPLATE 0
#endif

#if (defined(BLINK) && BLINK) || (!defined(BLINK) && defined(FRILLS) && FRILLS >= 1)
#if !defined(LED) && !defined(TEMPLATE)
#define TEMPLATE              1
#elif !defined(LED) && defined(TEMPLATE) && ! TEMPLATE && defined(DEFLED)
#define LED DEFLED
#elif !defined(LED) && defined(TEMPLATE) && ! TEMPLATE
#error BLINK needs LED defined or TEMPLATE=1
#endif
#endif

#if !defined(SFMCS) && !defined(TEMPLATE)
#define TEMPLATE              1
#elif DUAL && !defined(SFMCS) && defined(TEMPLATE) && ! TEMPLATE
#error DUAL needs to know the chip select for external memory, eg, SFMCS=ArduinoPin8, or TEMPLATE=1
#endif

// TEMPLATE=1 creates nops for LED and CS, so the right pin can be dropped in at burn time
#ifndef TEMPLATE
#define TEMPLATE              0
#endif

#if DUAL

#if !defined(SPIF) && defined(SPIF0)
#define SPIF SPIF0
#endif
#if !defined(MSTR) && defined(MSTR0)
#define MSTR MSTR0
#endif
#if !defined(SPE) && defined(SPE0)
#define SPE SPE0
#endif

#if !defined(SPDR) || !defined(SPSR) || !defined(SPCR) || \
    !defined(SPIF) || !defined(MSTR) || !defined(SPE)
// Might be as simple as renaming registers... but then the part might not have SPI
#warning SPI communication not implemented for this MCU, switching off DUAL
#undef  DUAL
#define DUAL                  0
#endif

#if TEMPLATE
#define SAME_PORT_SFMCS_CS    0
#else
#define SAME_PORT_SFMCS_CS (UR_PORT_VALUE(SFMCS) == UR_PORT_VALUE(AtmelCS))
#endif

#endif // DUAL


#ifndef EEPROM
#define EEPROM                1
#endif

#ifndef PGMWRITEPAGE
#define PGMWRITEPAGE          1
#endif

#define RET_OPCODE       0x9508

#if !defined(RJMPWP) || !PGMWRITEPAGE || (RJMPWP&0xF000) != 0xC000 || RJMPWP == 0xCFFF
#undef  RJMPWP
#define RJMPWP       RET_OPCODE
#endif

#undef  PROTECTME               // From v7.7 enforce bootloader self-protection
#define PROTECTME             1

#if defined(PROTECTRESET) && PROTECTRESET && !FLASHWRAPS && VBL
#undef  PROTECTRESET
#define PROTECTRESET          0
#warning Cannot generate code to protect reset vector when flash is not a power of two
#endif

#ifndef PROTECTRESET
#if defined(_urboot_AVAILABLE) && VBL
#if FLASHabove64k && _urboot_AVAILABLE >= 18
#define PROTECTRESET          1
#define _urboot_AVAILABLE_r1 (_urboot_AVAILABLE - 18)
#elif !FLASHabove64k && _urboot_AVAILABLE >= 14
#define PROTECTRESET          1
#define _urboot_AVAILABLE_r1 (_urboot_AVAILABLE - 14)
#else
#define PROTECTRESET          0
#define _urboot_AVAILABLE_r1 _urboot_AVAILABLE
#endif
#else  // !(defined(_urboot_AVAILABLE) && VBL)
#define PROTECTRESET          0
#endif
#endif // PROTECTRESET


// 500 ms default watchdog timeout
#ifndef WDTO
#define WDTO              500MS
#endif
#define expandcat(x, y)    x##y
#define WATCHDOG_TIMEOUT(x) expandcat(WATCHDOG_, x)

#ifndef FRILLS
#define FRILLS                0
#endif

#ifndef PAGE_ERASE
#define PAGE_ERASE            0
#endif

// Some parts can only erase 4 pages at a time
#define FOUR_PAGE_ERASE (defined(__AVR_ATtiny441__) || defined(__AVR_ATtiny841__) || \
  defined(__AVR_ATtiny1634__) || defined(__AVR_ATtiny1634R__))

#ifndef CHIP_ERASE
#define CHIP_ERASE (FRILLS >= 7)
#endif

// Flash needs erasing before writing if write page function exported or there is no CE
#define ERASE_B4_WRITE (PGMWRITEPAGE || !CHIP_ERASE)

#ifndef UPDATE_FL
#if FOUR_PAGE_ERASE
#define UPDATE_FL             0
#else
#define UPDATE_FL (FRILLS >= 10? 4: FRILLS >= 9? 3: FRILLS >= 8? 2: FRILLS >= 6)
#endif
#endif // UPDATE_FL

#ifndef QEXITEND
#define QEXITEND  (FRILLS >= 5)
#endif
#ifndef QEXITERR
#define QEXITERR  (FRILLS >= 4)
#endif
#ifndef EXITFE
#define EXITFE    (FRILLS >= 3? 2: FRILLS >= 2)
#endif
#ifndef BLINK
#define BLINK     (FRILLS >= 1)
#endif

#ifndef AUTOBAUD
#define AUTOBAUD              0
#endif

#if FOUR_PAGE_ERASE && UPDATE_FL
#undef UPDATE_FL
#define UPDATE_FL             0
#warning Unsetting UPDATE_FL option on FOUR_PAGE_ERASE parts
#endif

#if defined(DEBUG_FREQ)
// Set LED pin as default if no frequency pin defined
#if !defined(FREQ_PIN) && defined(LED)
#define FREQ_PIN LED
#if !defined(FREQ_POLARITY) && defined(LEDPOLARITY)
#define FREQ_POLARITY LEDPOLARITY
#endif
#endif

#if !defined(FREQ_PIN)
#error you need to define FREQ_PIN for DEBUG_FREQ
#endif

#if defined(EXACT_DF) && EXACT_DF
#define _debug_freq_us(x) _delay_us(x)
#else
// Delay of microseconds that generates shorter code at the expense of being up to 5 CPU cycles shorter
#define _debug_freq_us(us) ({ \
  const double tus = (us); \
  uint32_t  loops, cycles = (uint32_t) (((F_CPU) / 1e6) * (tus>0? tus: 0)); \
  \
  if(83886082UL <= cycles && cycles <= 0xFFFFFFFFUL) { \
    loops = ((cycles - 9) / 6) + 1; \
    cycles = ((loops - 1) * 6) + 9; \
  } else if(262145UL <= cycles && cycles <= 83886081UL) { \
    loops = ((cycles - 7) / 5) + 1; \
    if(loops > 0xFFFFFFUL) \
      loops = 0xFFFFFFUL;  \
    cycles = ((loops - 1) * 5) + 7; \
  } else if(768 <= cycles && cycles <= 262144UL) { \
    loops = ((cycles - 5) / 4) + 1; \
    if(loops > 0xFFFF) \
      loops = 0xFFFF;  \
    cycles = ((loops - 1) * 4) + 5; \
  } else if(30 <= cycles && cycles <= 767) { \
    loops = cycles / 3; \
    if(loops > 255) \
      loops = 255;  \
    cycles = loops * 3; \
  } \
 \
  __builtin_avr_delay_cycles(cycles); \
})

#endif

#ifndef DEBUG_CYCLES
#define DEBUG_CYCLES 5
#endif

#endif // defined(DEBUG_FREQ)


#if TEMPLATE
// Different NOP codes (mov rN, rN) can be replaced on the fly by urloader
#define NOP_LED_SBIPORT() asm volatile("mov r0, r0\n")
#define NOP_LED_CBIPORT() asm volatile("mov r1, r1\n")
#define NOP_LED_SBIDDR()  asm volatile("mov r12, r12\n") // Once had asm("mov r2, %0\n") in code
#define NOP_LED_CLRPORT() asm volatile("mov r3, r3\n")
#define NOP_LED_CLRDDR()  asm volatile("mov r4, r4\n")

#define NOP_SFM_SBIPORT() asm volatile("mov r5, r5\n")
#define NOP_SFM_CBIPORT() asm volatile("mov r6, r6\n")
#define NOP_SFM_SBIDDR()  asm volatile("mov r7, r7\n")
#define NOP_SFM_CLRPORT() asm volatile("mov r8, r8\n")
#define NOP_SFM_CLRDDR()  asm volatile("mov r9, r9\n")

#define sfm_release()   NOP_SFM_SBIPORT()
#define sfm_assert()    NOP_SFM_CBIPORT()
#define sfm_setupcs()   NOP_SFM_SBIDDR()
#define sfm_resetddr()  NOP_SFM_CLRDDR()
#define sfm_resetport() NOP_SFM_CLRPORT()

#else // !TEMPLATE

#define sfm_release()   (UR_PORT(SFMCS) |=  UR_BV(SFMCS))
#define sfm_assert()    (UR_PORT(SFMCS) &= ~UR_BV(SFMCS))
#define sfm_setupcs()   (UR_DDR(SFMCS)  |=  UR_BV(SFMCS))
#define sfm_resetddr()  (UR_DDR(SFMCS)  = 0)
#define sfm_resetport() (UR_PORT(SFMCS) = 0)
#endif


#if BLINK

#if TEMPLATE
#define led_on()        NOP_LED_SBIPORT()
#define led_off()       NOP_LED_CBIPORT()
#define led_setup()     NOP_LED_SBIDDR()
#define led_resetddr()  NOP_LED_CLRDDR()
#define led_resetport() NOP_LED_CLRPORT()

#else // !TEMPLATE

#if defined(LEDPOLARITY) && LEDPOLARITY < 0
#define led_on()        (UR_PORT(LED) &= ~UR_BV(LED))
#define led_off()       (UR_PORT(LED) |=  UR_BV(LED))
#else
#define led_on()        (UR_PORT(LED) |=  UR_BV(LED))
#define led_off()       (UR_PORT(LED) &= ~UR_BV(LED))
#endif
#define led_setup()     (UR_DDR(LED) |= UR_BV(LED))
#if defined(DEBUG_FREQ) && defined(FREQ_PIN)
#if UR_PIN_VALUE(FREQ_PIN) == UR_PIN_VALUE(LED)
#undef led_setup
#define led_setup()     // Not needed as FREQ_PIN already set up
#endif
#endif
#define led_resetddr()  (UR_DDR(LED) = 0)
#define led_resetport() (UR_PORT(LED) = 0)
#endif // TEMPLATE

#else // !BLINK

#define led_on()
#define led_off()
#define led_setup()
#define led_resetddr()
#define led_resetport()
#endif


// I/O settings

#if UR_NUMUARTS == 0 && !defined(SWIO)
#define SWIO 1
#warning no uart: defaulting to SWIO=1
#endif

#ifndef SWIO
#define SWIO 0
#endif

#if SWIO
#ifndef RX
#warning SWIO set but not RX? Defaulting to AtmelPB0 (but this is unlikely what you want)
#define RX AtmelPB0
#endif
#ifndef TX
#warning SWIO set but not TX? Defaulting to AtmelPB1 (but this is unlikely what you want)
#define TX AtmelPB1
#endif
#endif

#if AUTOBAUD
#ifndef RX
#warning AUTOBAUD set but not RX? Defaulting to AtmelPB0 (but this is unlikely what you want)
#define RX AtmelPB0
#endif
#endif

#if AUTOBAUD && SWIO
#warning unable to perform AUTOBAUD for SWIO
#undef AUTOBAUD
#define AUTOBAUD 0
#endif


#if SWIO
#define getch_clobberlist "r25", "r18",
#define putch_clobberlist "r25", "r18",
#else
#define getch_clobberlist
#define putch_clobberlist "r25",
#define UisIOSPACE(reg) (UARTn_addr + reg##_off < 0x3f + __SFR_OFFSET)
#endif // !SWIO

#if !AUTOBAUD

// Set the baud rate defaults
#if !defined(BAUD_RATE)
#if   F_CPU >= 7800000L
#define BAUD_RATE       115200L // Generally works (but needs SWIO for ca 8 MHz)
#elif F_CPU >= 1000000L
#define BAUD_RATE         9600L // 19200 may exhibit significant error
#elif F_CPU >= 128000L
#define BAUD_RATE         4800L // Good for 128 kHz internal RC
#else
#define BAUD_RATE         1200L // Good even at 32768 Hz
#endif
#endif // !defined(BAUD_RATE)


#if !SWIO                       // ... and compute baud rate settings for various UART types
#if UR_UARTTYPE == UR_UARTTYPE_CLASSIC

// Baud setting and actual baud rate in UART 2x mode and normal mode
#define BAUD_SETTING2X (((F_CPU + 4L*BAUD_RATE)/(8L*BAUD_RATE)) < 1? 0: ((F_CPU + 4L*BAUD_RATE)/(8L*BAUD_RATE)) - 1)
#define BAUD_ACTUAL2X  (F_CPU/(8L*(BAUD_SETTING2X+1)))
#define BAUD_SETTING1X (((F_CPU + 8L*BAUD_RATE)/(16L*BAUD_RATE)) < 1? 0: ((F_CPU + 8L*BAUD_RATE)/(16L*BAUD_RATE)) -1 )
#define BAUD_ACTUAL1X  (F_CPU/(16L*(BAUD_SETTING1X+1)))

// Error per 1000, ie, in 0.1%
#define baud_error(act) ((1000L*(BAUD_RATE>=(act)? BAUD_RATE-(act): (act)-BAUD_RATE))/BAUD_RATE)

// Some classic parts cannot switch to 2x mode
#if !defined(A_U2Xn)
#undef UART2X
#define UART2X 0
#endif

// Select UART2X automatically
#if  !defined(UART2X) || UART2X == 1
#undef UART2X
/*
 * Using double speed mode UART costs 6 byte initialisation. Is it worth it? Switch to 2x mode if
 * error for normal mode is > 1.4% and error with 2x less than normal mode considering that
 * normal mode has higher tolerances than 2x speed mode
 */
#if 20*baud_error(BAUD_ACTUAL2X) < 15*baud_error(BAUD_ACTUAL1X) && baud_error(BAUD_ACTUAL1X) > 14
#define UART2X 1
#else
#define UART2X 0
#endif
#endif

#if UART2X
#define BAUD_SETTING BAUD_SETTING2X
#define BAUD_ACTUAL  BAUD_ACTUAL2X
#define BAUD_ERROR   baud_error(BAUD_ACTUAL2X)
#else
#define BAUD_SETTING BAUD_SETTING1X
#define BAUD_ACTUAL  BAUD_ACTUAL1X
#define BAUD_ERROR   baud_error(BAUD_ACTUAL1X)
#endif

#if   BAUD_ERROR > 50 && BAUD_ACTUAL < BAUD_RATE
#warning BAUD_RATE quantisation error exceeds -5%
#elif BAUD_ERROR > 50
#warning BAUD_RATE quantisation error greater than 5%
#elif  BAUD_ERROR > 25 && BAUD_ACTUAL < BAUD_RATE
#warning BAUD_RATE quantisation error exceeds -2.5%
#elif BAUD_ERROR > 25
#warning BAUD_RATE error quantisation greater than 2.5%
#endif

#if (defined(__AVR_ATmega161__) || defined(__AVR_AT94K__))
#if BAUD_SETTING > 4095
#error Unachievable baud rate (too slow)
#endif
#elif (!defined(UBRRnH) && BAUD_SETTING > 255) ||  BAUD_SETTING > 4095
#error Unachievable baud rate (too slow)
#elif BAUD_SETTING < 0
#error Unachievable baud rate (too fast)
#endif


#elif UR_UARTTYPE == UR_UARTTYPE_LIN

// 8-bit baud rate register given LBT value (number of samples in 8..63) and target baud rate bd
#define _linbrr(lbt, bd)  ({ const int _brrlbt = (lbt); const long _bd = (bd); \
  const long _brrret = (F_CPU + (_brrlbt)*(_bd)/2)/((_brrlbt)*(_bd)) - 1L; \
  _brrret < 0? 0L: _brrret > 255L? 255L: _brrret; \
})

// Baud rate given LBT value (number of samples in 8..63) and target baud rate bd
#define _linbaud(lbt, bd) ({ const int _baudlbt = (lbt); F_CPU/((_baudlbt)*(_linbrr(_baudlbt, bd)+1L)); })

// Baud rate quantisation error in 0.1 percent given LBT value and target baud rate bd
#define _linabserr(lbt, bd) ({ const long _aebdf = 1000L*(_linbaud(lbt, bd) - (bd)); _aebdf >= 0? _aebdf/(bd): -_aebdf/(bd); })

// Better of two LBT values: if the quantised(!) quantisation error is the same, the larger number of samples wins
#define _better2(l1, l2, bd) ({ const int _l1 = (l1), _l2 = (l2); long _e1 = _linabserr(_l1, bd), _e2 = _linabserr(_l2, bd); \
  _e1 < _e2? _l1: _e1 > _e2? _l2: _l1 > _l2? _l1: _l2; })
#define _better4(l1, l2, l3, l4, bd) _better2(_better2(l1, l2, bd), _better2(l3, l4, bd), bd)
#define _better8(l1, l2, l3, l4, l5, l6, l7, l8, bd) \
  _better2(_better4(l1, l2, l3, l4, bd), _better4(l5, l6, l7, l8, bd), bd)

// Best LBT value from 8..63
#define _linbestlbt(bd) ({ const int \
  _bestl2 =  _better8( 8,  9, 10, 11, 12, 13, 14, 15, bd), \
  _bestl3 =  _better8(16, 17, 18, 19, 20, 21, 22, 23, bd), \
  _bestl4 =  _better8(24, 25, 26, 27, 28, 29, 30, 31, bd), \
  _bestl5 =  _better8(32, 33, 34, 35, 36, 37, 38, 39, bd), \
  _bestl6 =  _better8(40, 41, 42, 43, 44, 45, 46, 47, bd), \
  _bestl7 =  _better8(48, 49, 50, 51, 52, 53, 54, 55, bd), \
  _bestl8 =  _better8(56, 57, 58, 59, 60, 61, 62, 63, bd); \
  _better8(8, _bestl2, _bestl3, _bestl4, _bestl5, _bestl6, _bestl7, _bestl8, bd); \
})

#define BAUD_LINLBT  _linbestlbt(BAUD_RATE)
#define BAUD_SETTING _linbrr(_linbestlbt(BAUD_RATE), BAUD_RATE)
#define UBRRnL LINBRRnL

// Most baud rates within below range can be generated, though there are some edge cases
#if F_CPU > BAUD_RATE*64L*256L  // Quantisation error 64/63, ie, ca 1.6%
#error LIN_UART BAUD_RATE too small for 8-bit LINBRR
#elif F_CPU < 79L*BAUD_RATE/10L // Quantisation error 79/80, ie, ca 1.25%
#error LIN_UART BAUD_RATE too big
#endif

#endif // UR_UARTTYPE

#endif // !SWIO

#endif // !AUTOBAUD



// Extended version word (from v 7.5 onwards)
#define UR_EXT_VBLVECTNUMMASK 0x7f00 // Mask for the vector bootloader vector number
#define UR_EXT_BLPAGESMASK 0x00ff // Mask for the number of bootloader pages

#define UR_EXT_VBLVECTNUM (VBL? VBL_VECT_NUM: 0) // For external program to patch the application
#define UR_EXT_BLPAGES ((FLASHEND+1UL-START+(SPM_PAGESIZE-1))/SPM_PAGESIZE) // # bootloader pages

#define UR_EXT_WORD(vectnum, blpages) ( (uint16_t) ( \
  (((vectnum)<<8) & UR_EXT_VBLVECTNUMMASK) | \
  ((blpages) & UR_EXT_BLPAGESMASK) \
))

#define ext_vblvectnum(ext) ((uint8_t) (((ext) & UR_EXT_VBLVECTNUMMASK)>>8))
#define ext_blpages(ext)    ((uint8_t) (((ext) & UR_EXT_BLPAGESMASK)>>0))



/*
 * Dedicated interrupt vector for VBL to store the jump to the application
 *
 *  - Set custom VBL_VECT_NUM in Makefile using vector name (or number)
 *
 *  - Defaults to SPM_READY_vect_num or, if that does not exist, to EE_READY_vect_num. If that
 *    does not exist either, the first unused vector in the table is taken or, failing that, the
 *    vector table is assumed to be expanded by one slot and that extra slot is taken
 *
 *  - If num is -1 it refers to the additional vbl interrupt vector; uploaded sketches need to
 *    have increased the vector table by one slot. This can be done in the Arduino IDE through
 *    adding the following to the end of the hardware/arduino/avr/cores/arduino/main.cpp source
 *    that will then automatically be included into every application:
 *
 *    // Additional vector in ISR vector table used by the urboot bootloader
 *    uint8_t __attribute__((used)) __attribute__((section(".vectors")))
 *    _vbl_jmp_to_application[FLASHEND+1UL > 8192? 4: 2] = {0x0C, 0x94};
 */

#define VBL_VECT_additional (_VECTORS_SIZE/(FLASHin8k? 2: 4))

#if defined(VBL_VECT_NUM)
#if VBL_VECT_NUM == -1
#undef VBL_VECT_NUM
#define VBL_VECT_NUM VBL_VECT_additional
#elif VBL_VECT_NUM <= 0 || VBL_VECT_NUM > VBL_VECT_additional
#error VBL_VECT_NUM has been set out of range for this MCU
#endif

#else // !defined(VBL_VECT_NUM)

#if !VBL
#define VBL_VECT_NUM 0
#elif         defined(SPMR_vect_num)
#define VBL_VECT_NUM (SPMR_vect_num)
#elif         defined(SPM_RDY_vect_num)
#define VBL_VECT_NUM (SPM_RDY_vect_num)
#elif         defined(SPM_Ready_vect_num)
#define VBL_VECT_NUM (SPM_Ready_vect_num)
#elif         defined(SPM_READY_vect_num)
#define VBL_VECT_NUM (SPM_READY_vect_num)
#elif         defined(SPMREADY_vect_num)
#define VBL_VECT_NUM (SPMREADY_vect_num)
#elif         defined(SPM_vect_num)
#define VBL_VECT_NUM (SPM_vect_num)
#elif         defined(NVM_SPM_vect_num)
#define VBL_VECT_NUM (NVM_SPM_vect_num)

#elif         defined(EEPROM_Ready_vect_num)
#define VBL_VECT_NUM (EEPROM_Ready_vect_num)
#elif         defined(EEPROM_READY_vect_num)
#define VBL_VECT_NUM (EEPROM_READY_vect_num)
#elif         defined(EE_RDY_vect_num)
#define VBL_VECT_NUM (EE_RDY_vect_num)
#elif         defined(EE_READY_vect_num)
#define VBL_VECT_NUM (EE_READY_vect_num)
#elif         defined(EEREADY_vect_num)
#define VBL_VECT_NUM (EEREADY_vect_num)
#elif         defined(ERDY_vect_num)
#define VBL_VECT_NUM (ERDY_vect_num)
#elif         defined(NVM_EE_vect_num)
#define VBL_VECT_NUM (NVM_EE_vect_num)

#else
#define VBL_VECT_NUM UB_VBLVECUU(__AVR_DEVICE_NAME__) // Suggested Vector
#if VBL_VECT_NUM <= 0 || VBL_VECT_NUM > VBL_VECT_additional
#error VBL_VECT_NUM has been assigned out of range for this MCU
#endif
#if VBL_VECT_NUM == VBL_VECT_additional // Warn user as they need to know about this decision
#warning Assigning a vector just past vector table for VBL_VECT_NUM
#endif
#endif
#endif // VBL_VECT_NUM


// Watchdog settings
#define WATCHDOG_OFF    (0)

#if !defined(WDP0) && defined(WDPS0)
/*
 * Timings are likely to be radically different, look up data sheet for those devices (ATA57xxM322,
 * ATA57xx, ATA5790N, ATA583x, ATA628x, ATA821x, ATA851x).
 */
#define WDP0              WDPS0
#define WDP1              WDPS1
#define WDP2              WDPS2
#endif

#if defined(WDP0)
#define WATCHDOG_16MS   (_BV(WDE))
#define WATCHDOG_32MS   (_BV(WDP0) | _BV(WDE))
#define WATCHDOG_64MS   (_BV(WDP1) | _BV(WDE))
#define WATCHDOG_125MS  (_BV(WDP1) | _BV(WDP0) | _BV(WDE))
#define WATCHDOG_250MS  (_BV(WDP2) | _BV(WDE))
#define WATCHDOG_500MS  (_BV(WDP2) | _BV(WDP0) | _BV(WDE))
#define WATCHDOG_1S     (_BV(WDP2) | _BV(WDP1) | _BV(WDE))
#define WATCHDOG_2S     (_BV(WDP2) | _BV(WDP1) | _BV(WDP0) | _BV(WDE))
#endif


#ifdef WDP3
#define WATCHDOG_4S     (_BV(WDP3) | _BV(WDE))
#define WATCHDOG_8S     (_BV(WDP3) | _BV(WDP0) | _BV(WDE))
#endif


// STK500 command definitions

#define STK_GET_PARAMETER  0x41
#define STK_SET_DEVICE     0x42
#define STK_SET_DEVICE_EXT 0x45
#define STK_ENTER_PROGMODE 0x50
#define STK_LEAVE_PROGMODE 0x51
#define STK_CHIP_ERASE     0x52
#define STK_LOAD_ADDRESS   0x55
#define STK_UNIVERSAL      0x56
#define STK_UNIVERSAL_EXT  0x4d
#define STK_UNIVERSAL_CE0  0xac
#define STK_UNIVERSAL_CE1  0x80
#define STK_PROG_PAGE      0x64
#define STK_READ_PAGE      0x74
#define STK_READ_SIGN      0x75

#define UR_PROG_PAGE_EE    0x00
#define UR_READ_PAGE_EE    0x01
#define UR_PROG_PAGE_FL    0x02
#define UR_READ_PAGE_FL    0x03
#define UR_PAGE_ERASE      0x04

// STK500 communication protocol

#define STK_INSYNC_C       0x14 // Original STK500 constant
#define STK_OK_C           0x10 // Original STK500 constant
#define STK_INSYNC STK_INSYNC_C // Possibly redefined further below
#define STK_OK         STK_OK_C // Possibly redefined further below
#define CRC_EOP            0x20


// Redefine STK_INSYNC and STK_OK so they inform avrdude about MCU type and 5 bit urboot features

#undef  STK_INSYNC
#undef  STK_OK

#define UB_N_MCU            2040 // MCU id is in 0..2039

// 5 bit urboot features
#define UB_RESERVED_1         1u
#define UB_RESERVED_2         2u

#define UB_READ_FLASH         4u // Urboot always can read flash

#if !ERASE_B4_WRITE              // No page erase during flash STK_PROG_PAGE
#define UB_FLASH_LL_NOR       8u // Flash programming with STK_PROG_PAGE looks like NAND memory
#else
#define UB_FLASH_LL_NOR       0u // Uploader needs to read flash first for sub-page modifications
#endif

#if CHIP_ERASE
#define UB_CHIP_ERASE        16u
#else
#define UB_CHIP_ERASE         0u
#endif

#if UB_MCUID(__AVR_DEVICE_NAME__) >= UB_N_MCU
#error MCU id out of range
#endif

#define UB_FEATURES  (UB_READ_FLASH | UB_FLASH_LL_NOR | UB_CHIP_ERASE)
#define UB_INFO (UB_FEATURES*UB_N_MCU + UB_MCUID(__AVR_DEVICE_NAME__))

// Reserve one slot for STK500 protocol and 256 more to make STK_OK and STK_INSYNC different
#if UB_INFO >= 255*256-1
#error Cannot work with this MCU/feature combination
#endif

// Put TMP_INSYNC in [0, 255] and TMP_OK1 in [0, 254]
#define TMP_INSYNC (UB_INFO / 255)
#define TMP_OK1    (UB_INFO % 255)

// TMP_OK is in [0, 255] and different from TMP_INSYNC
#define TMP_OK     ((TMP_OK1 < TMP_INSYNC)? TMP_OK1: TMP_OK1+1)

// Indistinguishable from STK500 protocol? Map to 255/254
#if TMP_INSYNC == STK_INSYNC_C && TMP_OK == STK_OK_C
#define STK_INSYNC 255
#define STK_OK     254
#else
#define STK_INSYNC TMP_INSYNC
#define STK_OK     TMP_OK
#endif


/*
 * Global variables and where they are stored
 *
 * For small code size it's useful to have a handful of global variables. The most important
 * variable that deals with addresses for lpm and spm opcodes to read and write flash is the
 * 16-bit zaddress. It is stored permanently in the Z register pair r31:30 throughout. This is
 * complemented by RAMPZ on larger devices. Where a Boolean variable is needed the code uses bits
 * from GPIOR0 or a similar, which results in shorter opcodes (sbi, cbi etc). SRAM is only needed
 * for a single copies of flash/eeprom page buffers for writing (max SPM_PAGESIZE bytes).
 *
 * register uint16_t zaddress asm("r30");
 *
 * For flash programming the strategy is to always use a 16 bit address and, if necessary, RAMPZ
 * to deal with larger addresses. The spm opcode uses RAMPZ:Z (Z=r31:30), elmp uses RAMPZ:Z, so
 * making the 16-bit address global and putting it in Z=r31:30 makes it "right at home". The only
 * trouble is that this register pair is call-clobbered, ie, is likely to be destroyed by library
 * function calls. Fortunately, this source does not use library calls. For the full application
 * binary interface and register usage of avr-gcc see https://gcc.gnu.org/wiki/avr-gcc
 *
 * As for EEPROM read/write, I don't think there are AVRs with EEPROM beyond 64k, certainly not the
 * target MCUs for urboot, so again 16 bit look OK for EEPROM read/write addresses.
 *
 * Executive summary: the asm("r30") is a nifty trick; safely ignore the call-clobbered warnings
 */

register uint16_t zaddress asm("r30");


/*
 * Contrary to normal C programs global variables are *not* initialised, as we drop the
 * initialisation code, saving PROGMEM. Urboot only needs one flash page worth of SRAM:
 *
 * uint8_t rambuffer[SPM_PAGESIZE];
 */

// Place rambuffer at 0x100 boundary if at least 32 bytes left for stack rcalls
#if (RAMSTART & 0xFF) && ((RAMSTART+255) & ~0xFF) + SPM_PAGESIZE + 30 < RAMSTART+RAMSIZE
#define RAMBUFFER ((RAMSTART+255) & ~0xFF)
#else
#define RAMBUFFER RAMSTART
#endif

#define rambuffer ((uint8_t *) RAMBUFFER)

// Hi byte of low word or opcode at reset position (is 0xCX for rjmp and 0x94 for jmp)
#define resethi8 rambuffer[1]
#define isRjmp(op16hi) (((op16hi) & 0xF0) == 0xC0)


#if FLASHabove64k
#define LPM "elpm "
typedef const uint_farptr_t progmem_t;
#else
#define LPM "lpm "
typedef const void *progmem_t;
#endif

/*
 * This bootloader does not accept page_read/page_write commands with more than SPM_PAGESIZE
 * bytes, even when dealing with EEPROM. For classic parts SPM_PAGESIZE ist at most 256, which can
 * be handled with uint8_t. Avrdude and any self-respecting programmer will specify lengths 1 ...
 * 256 (full page), never 0 bytes though. Note the low byte of 256 is zero, and len == 0 works for
 * do { ... } while(--len); loops when len is of type uint8_t. Magic, eh! Only use with these
 * types of loops, though.
 */

void writebufferX(void);

void ub_spm(uint8_t cmd);
#define ub_page_erase() ub_spm(__BOOT_PAGE_ERASE)

// Number of program pages below the boot section
#define SPM_NUMPAGES  (START/SPM_PAGESIZE)

#if SPM_NUMPAGES > 256          // Needs more than one byte
typedef uint16_t spm_pages_index_t;
#else
typedef uint8_t spm_pages_index_t; // Can handle SPM_NUMPAGES==256 with do { ... } while(--len);
#endif



// main() is in init9 (no interrupt vector table) and OS_main (no further entry or exit code)
int main(void) __attribute__ ((OS_main, section(".init9")));

void __attribute__ ((noinline)) putch(char);
uint8_t __attribute__ ((noinline)) getch(void);
void  __attribute__ ((noinline)) get_sync(void);

void __attribute__ ((noinline)) watchdogConfig(uint8_t x);

// Endless loop leading to a watchdog reset; the equivalent while(1); can produce larger code
#define bootexit() ({asm("rjmp .-2\n");  __builtin_unreachable (); })

#if QEXITERR
#define RJMPQEXIT "rjmp qexiterr\n"
#else
#define RJMPQEXIT "rjmp .-2\n"
#endif

// quickbootexit() tries to use existing code for quick exit, failing that it's bootexit()
#define quickbootexit() ({asm(RJMPQEXIT);  __builtin_unreachable (); })



#if SWIO
void bitDelay();

/*
 *
 * Software serial I/O follows largely the space-efficient algorithm in  Atmel's Applictaion Note
 * AVR305: Half Duplex Compact Software UART (Rev. 0952C-AVR-0905). The equation for the delay
 * counter SWIO_B_VALUE = (((F_CPU/BAUD_RATE)-23+3)/6) from AVR305 is correct for the Mega
 * architecture with 16 bit PC. Different 8-bit AVR architecture families have slightly different
 * timings. I list the number of clock cycles for 1 tx/rx bit delay for parameter b = SWIO_B_VALUE
 * below for tx and rx routines:
 *
 * clock cycles/bit  | tx       | rx      |
 * ---------------------------------------+
 * mega 16-bit PC    | 6b+14+9  | 6b+14+8 | baseline
 * mega 22-bit PC    | 6b+18+9  | 6b+18+8 | rcall/ret 1 clock longer (twice each)
 * xmega 16-bit PC   | 6b+12+8  | 6b+12+9 | rcall 1 clock shorter, tx sbi/cbi less, rx sbic more
 * xmega 22-bit PC   | 6b+16+8  | 6b+16+9 | as above but +4 clock because of longer rcall/ret
 * reduced core tiny | 6b+16+8  | 6b+16+8 | rcall longer, sbi/cbi shorter (tx)
 * ---------------------------------------+
 *
 * Timings for getch/putch code below are taken from Atmel's Instruction Set Manual
 * 0856J-AVR-07/2014. The required clock cycles per rx/tx bit are F_CPU/BAUD_RATE. The code
 * inserts nop (1 cycle) or rjmp .+0 (2 cycles) in the delay/putch/getch routines, if the baud
 * rate error warrants it to a) ensure tx and tx have the same timing and b) that the timing of
 * SWIO is as close as possible to the timing required by F_CPU/BAUD_RATE, so the max SWIO error
 * is |0.5/(F_CPU/BAUD_RATE)|. At 115200 baud and 16 MHz CPU frequency, the SWIO_B_VALUE is
 * b=19, and there are 137 clock cycles per bit for the mega 16-bit PC architecture. We need 16
 * MHz/115200 baud = 138.9 clock cycles. One extra clock cycle amounts to a slowdown of the baud
 * rate to the tune of 0.73%. That is significant as one needs to control the baud rate to about
 * 2.5%, which amounts to a drift of a quarter of a bit width for a single byte.
 */

// Cycles per bit
#define CPB ((F_CPU+BAUD_RATE/2)/BAUD_RATE)

// The bitDelay() function has a granularity of 6 cycles - want around 0.5% accuracy
#if CPB > 600
#define B_OFF                 3 // To centre error (max error is +/- 3 cycles)
#define B_EXTRA               0 // No further correction (3/600+ cycles < 0.5% error)
#else
#define B_OFF                 0 // Underestimate B_VALUE and insert opcodes for B_EXTRA cycles
#define B_EXTRA (CPB-CPB_B(SWIO_B_VALUE))
#endif

#ifndef SWIO_B_VALUE
#if IS_REDUCED_CORE_TINY        // Actually unsupported for bootloaders
#define SWIO_B_VALUE ((CPB-16-8+B_OFF+60)/6-10)
#define CPB_B(b) (6*(b)+16+9)
#define SWIO_B_DLYTX          0 // Delay tx timing so it's same as rx timing?
#define SWIO_B_DLYRX          0 // Delay rx timing so it's same as tx timing?

#elif IS_XMEGA && FLASHabove128k
#define SWIO_B_VALUE ((CPB-16-9+B_OFF+60)/6-10)
#define CPB_B(b) (6*(b)+16+9)
#define SWIO_B_DLYTX          1
#define SWIO_B_DLYRX          0

#elif IS_XMEGA
#define SWIO_B_VALUE ((CPB-12-9+B_OFF+60)/6-10)
#define CPB_B(b) (6*(b)+12+9)
#define SWIO_B_DLYTX          1
#define SWIO_B_DLYRX          0

#elif FLASHabove128k
#define SWIO_B_VALUE ((CPB-18-9+B_OFF+60)/6-10)
#define CPB_B(b) (6*(b)+18+9)
#define SWIO_B_DLYTX          0
#define SWIO_B_DLYRX          1

#else
#define SWIO_B_VALUE ((CPB-14-9+B_OFF+60)/6-10)
#define CPB_B(b) (6*(b)+14+9)
#define SWIO_B_DLYTX          0
#define SWIO_B_DLYRX          1
#endif
#endif // SWIO_B_VALUE

#if SWIO_B_VALUE > 256          // Sic! 256 is still OK
#error Baud rate too slow for SWIO
#elif SWIO_B_VALUE < 0
#error Baud rate too fast for SWIO
#endif

#if B_EXTRA > 5 || B_EXTRA < 0
# error Baud rate incompatible with F_CPU for SWIO
#endif

#endif // SWIO


#if CHIP_ERASE

#if FOUR_PAGE_ERASE             // CE_SIZE <= 256 (parts with 4-page erase have small page sizes)
#define CE_SIZE (4*SPM_PAGESIZE)
#else
#define CE_SIZE SPM_PAGESIZE
#endif

#if START % CE_SIZE != 0
#error START must be a multiple of CE_SIZE
#endif

static void chip_erase() {
  asm volatile(
    ldi(r30, lo8(%[top]))       // Load RAMPZ:Z with START-CE_SIZE
    ldi(r31, hi8(%[top]))
#if FLASHabove64k
    ldi(r25, hh8(%[top]))
    out_rampz(r25)
#endif
  "1: "
    "wdr\n"                     // Reset WDT as CE can take a long time

#if UPDATE_FL >= 2
    "movw r18, r30\n"
#if CE_SIZE == 256              // Determine loop end comparison reg r23
    "mov  r23, r30\n"
#else
     ldi(r23, lo8(CE_SIZE))
    "add  r23, r30\n"
#endif
  "2: "
    LPM "r22, Z+\n"             // Might increment RAMPZ
    cpi(r22, 0xff)
    "brne 3f\n"                 // Leave loop at first non-0xff
    "cpse r30, r23\n"           // Cpse does not change SREG (and its Z-bit)
    "rjmp 2b\n"
  "3: "
    "movw r30, r18\n"
#if FLASHabove64k
    out_rampz(r25)
#endif
    "breq 4f\n"                 // Skip page erase if the page was all 0xff
#endif // UPDATE_FL

    ldi(r24, __BOOT_PAGE_ERASE)
    "rcall ub_spm\n"            // Erase page
#if defined(RWWSRE)
    "rcall rww_enable\n"
#endif
 "4: "
#if CE_SIZE == 256
    subi(r31, 1)
#elif CE_SIZE <= 32
    sbiw(r30, CE_SIZE)
#else
    subi(r30, CE_SIZE)
    "sbc r31, r1\n"
#endif
#if FLASHabove64k
    "sbc r25, r1\n"
    out_rampz(r25)             // Decrement RAMPZ:Z by CE_SIZE
#endif
    "brcc 1b\n"
  : "=z"(zaddress)
  : [top] "n"((uint32_t) (START-CE_SIZE))
  :
#if FLASHabove64k
    "r25",
#endif
#if UPDATE_FL >= 2
    "r23", "r22", "r18", "r19",
#endif
    "r24"
  );
}

#endif // CHIP_ERASE


// Opcode for jmp START (jmp uses a word address, hence shift one extra bit on byte address START)
#define  JMP_START (((((uint32_t) START >> 1) & 0xffff)<<16) | \
  (0x940c | ((((uint32_t) START >> 18) & 31)<<4) | ((((uint32_t) START >> 17) &  1)<<0)))

// Opcode for jmp from address 0 *forward* to START (for up to 8k parts, wraps around at 4k of 8k)
#define RJMP_FWD_START (((((uint16_t) START >> 1) - 1) & 0xFFF) | 0xC000)

// Opcode for rjmp from address 0 *backwards* to START (for *all* parts where memory wraps around)
#define RJMP_BWD_START (0xC000 | (((uint16_t)((START-FLASHEND-3UL)/2))&0xFFF))


// Location of the application start vector in bytes
#if VBL && FLASHin8k
#define appstart_vec_loc (VBL_VECT_NUM*2)
#elif VBL
#define appstart_vec_loc (VBL_VECT_NUM*4)
#else // !VBL
#define appstart_vec_loc 0
#endif


#if DUAL

// SPI flash memory
// Universal SPI flash memory commands
#define SFM_C_READ         0x03 // Read memory
#define SFM_C_WREN         0x06 // Write enable
#define SFM_C_SE4k         0x20 // Sector erase (4k)

// SPI communication
static uint8_t spi_transfer(uint8_t data) {
  asm volatile(
    out_spdr(%[data])           // SPDR = data;
 "1: "
    sbxs_spsr(%[data], SPIF)    // while(!(SPSR & _BV(SPIF)))
    "rjmp 1b\n"                 //   continue;
    in_spdr(%[data])            // data = SPDR;
  : [data] "=r"(data) : "0"(data)
  );
  return data;
}

/*
 * Read a memory page from external SPI flash. 16 bit address is OK as 3rd byte of addr is kept in
 * global RAMPZ. Note this is a trick that we can *only* deploy because external SPI flash memory
 * and internal to-be-burned flash are byte for byte parallel at the same addresses. There cannot
 * be a header (an imaginative 0xc0de magic number, the size of the program or anything else) to
 * introduce the binary executable in external memory: it has to start right at byte 0 if you want
 * to play the RAMPZ trick. It's also a much simpler proposition to the user of "over the air"
 * programming: tell them to drop the new executable at the beginning of the external flash and
 * trigger a WDT reset. Simple! There is a modicum of sanity checking that the first word is a
 * jmp/rjmp instruction (put by the compiler into the reset vector), so not all data that are put
 * there by accident will trigger a rewrite of MCU flash.
 */

static void sfm_read_page() {
  sfm_assert();
  spi_transfer(SFM_C_READ);
  // 24 bit address
#if FLASHabove64k
  spi_transfer(RAMPZ);
#else
  spi_transfer(0);
#endif
  spi_transfer(zaddress >> 8);
  spi_transfer(zaddress);

  // Copy one MCU flash page of data from SPI flash memory to SRAM
  asm volatile(
    ldi(r26, lo8(RAMBUFFER))    // uint8_t *bufp = rambuffer;
    ldi(r27, hi8(RAMBUFFER))    // uint8_t n = SPM_PAGESIZE;
  "1: "
    ldi(r24, 0)                 // do {
    "rcall spi_transfer\n"      //   *bufp++ = spi_transfer(0);
    "st X+, r24\n"              // } while(--n);
     cpi(r26, lo8(RAMBUFFER + SPM_PAGESIZE))
    "brne 1b\n"
#if SPM_PAGESIZE == 256
    "subi  r27, 1\n"           // X = X - 256
#elif (RAMBUFFER+SPM_PAGESIZE)/256 == RAMBUFFER/256
     ldi(r26, lo8(RAMBUFFER))
#else
     ldi(r26, lo8(RAMBUFFER))
     ldi(r27, hi8(RAMBUFFER))
#endif
    ::: getch_clobberlist "r26", "r27", "r24"
  );

  sfm_release();
}
#endif


#if DUAL

#if FLASHabove64k
#define inczaddresspage() ({ zaddress += SPM_PAGESIZE; if(!zaddress) RAMPZ++; })
#else
#define inczaddresspage() ({ zaddress += SPM_PAGESIZE; })
#endif

static void dual_boot() {
 /*
  * Define relevant SPI pins as outputs - CS must be output for SPI to work in master mode. Pull
  * up CS to deactivate any attached device (eg, RFM module); pull up the chip select CS for the
  * external memory; and pull up POCI, so a stream of 1's is seen without an external SPI flash.
  * There is a slight code saving when CS is on the same port as SFMCS of the SPI interface. The
  * code makes the reasonable(?) assumption that the lines for SPI are all on the same port.
  */

#if SAME_PORT_SFMCS_CS
  UR_PORT(AtmelCS) = UR_BV(SFMCS) | UR_BV(AtmelCS) | UR_BV(AtmelPOCI);
  UR_DDR(AtmelCS)  = UR_BV(SFMCS) | UR_BV(AtmelCS) | UR_BV(AtmelPICO) | UR_BV(AtmelSCK);
#else
  UR_PORT(AtmelCS) = UR_BV(AtmelCS) | UR_BV(AtmelPOCI);
  UR_DDR(AtmelCS) = UR_BV(AtmelCS) | UR_BV(AtmelPICO) | UR_BV(AtmelSCK);
  sfm_release();
  sfm_setupcs();
#endif

  SPCR = _BV(MSTR) | _BV(SPE);  // Init SPI: Master, F_CPU/4, MSBFIRST, SPI_MODE0

  zaddress = 0;                 // RAMPZ will be 0 here
  spm_pages_index_t numpages = (spm_pages_index_t) SPM_NUMPAGES;
  do {
    // Read in a full memory page from external flash
    sfm_read_page();

#if FLASHabove64k
    if(RAMPZ == 0)
#endif
      if(zaddress == 0) {
        // On first (vector) page check whether reset vector is an rjmp or jmp instruction
        if(
#if FLASHabove4M                // jmp for an MCU with more than 4M flash (should be so lucky :)
          (resethi8 >> 1) != (0x94U >> 1) &&
#elif FLASHabove8k              // Otherwise jmp opcode has 0x94 as high byte
          resethi8 != 0x94U &&
#endif
          !isRjmp(resethi8) )
          goto startingapp;     // No AVR program in SPI flash: directly start the application
      }

    writebufferX();             // Returns with original zaddress/RAMPZ intact
    inczaddresspage();
  } while(--numpages);

  // Erase first 4 kB using SFM_C_SE4k command and reboot via WDT reset
  sfm_assert();
  spi_transfer(SFM_C_WREN);
  sfm_release();
  sfm_assert();

  asm volatile(                 // Looks a bit convoluted but saves 4 bytes
    "ldi   r16, 4\n"
     ldi(r24, SFM_C_SE4k)
 "1: rcall spi_transfer\n"      // spi_transfer(SFM_C_SE4k);
    "ldi   r24, 0\n"            // spi_transfer(0);
    "subi  r16, 1\n"            // spi_transfer(0);
    "brne  1b\n"                // spi_transfer(0);
    ::: "r24", "r16"
  );

  sfm_release();

  // Let sector erase finish before reset - 1 s should be ample
  watchdogConfig(WATCHDOG_1S);
  bootexit();                   // Deleted the first 4 kB sector on the SPI flash: reset via WDT

startingapp:
  // Crude initialisation before starting the app (WDT reset would result in an endless loop)
  SPCR = 0;                     // Always switch off SPI

#ifndef DUAL_NO_SPI_RESET
  UR_DDR(AtmelCS) = 0;
  UR_PORT(AtmelCS) = 0;
#if !SAME_PORT_SFMCS_CS
  sfm_resetddr();
  sfm_resetport();
#endif
#endif // DUAL_NO_SPI_RESET
}
#endif // DUAL

#if EEPROM

static void write_sram(uint8_t length) { // Write data from host into SRAM and reload X to RAMBUFFER
  uint8_t xend = length;

  asm volatile(
    ldi(r26, lo8(RAMBUFFER))
    ldi(r27, hi8(RAMBUFFER))
#if RAMBUFFER & 0xff
    "add   %[xend], r26\n"
#endif
  "1: rcall getch\n"
    "st    X+, r24\n"
    "cpse  %[xend], r26\n"
    "rjmp  1b\n"
    ldi(r26, lo8(RAMBUFFER))
    ldi(r27, hi8(RAMBUFFER))
#if RAMBUFFER & 0xff            // Tell gcc that xend will be changed
   : [xend] "=r"(xend) : "0"(xend)
#else
   :: [xend] "r"(xend)
#endif
   : getch_clobberlist "r26", "r27", "r24"
  );
}

#else

static void write_sram_spm_pagesize(void) { // Also resets X to rambuffer at end
  asm volatile(
     ldi(r26, lo8(RAMBUFFER))
     ldi(r27, hi8(RAMBUFFER))
 "1: rcall getch\n"
    "st    X+, r24\n"
     cpi(r26, lo8(RAMBUFFER + SPM_PAGESIZE))
    "brne  1b\n"
#if SPM_PAGESIZE == 256
    "subi  r27, 1\n"           // X = X - 256
#elif (RAMBUFFER+SPM_PAGESIZE)/256 == RAMBUFFER/256
     ldi(r26, lo8(RAMBUFFER))
#else
     ldi(r26, lo8(RAMBUFFER))
     ldi(r27, hi8(RAMBUFFER))
#endif
    ::: getch_clobberlist "r26", "r27", "r24"
  );
}

#endif

static uint8_t getaddrlength() {
  asm volatile(             // Compiler is not great at setting address from next two/three bytes
    "rcall getch\n"
    "mov %A0, r24\n"
    "rcall getch\n"
    "mov %B0, r24\n"
#if FLASHabove64k
    "rcall getch\n"
    out_rampz(r24)
#endif
    : "=r"(zaddress) :: getch_clobberlist "r24"
  );

  return getch();
}


#if QEXITEND
static void leave_progmode() {
  // Shorten watchdog timeout to start application pretty soon
  watchdogConfig(WATCHDOG_16MS);
  get_sync();
}
#endif




/*
 * Urboot layout of top six bytes
 *
 * FLASHEND-5: numblpags, only from v7.5: 1 byte number 1..127 of bootloader flash pages
 * FLASHEND-4: vblvecnum, only from v7.5: 1 byte vector number 1..127 for vector bootloader
 * FLASHEND-3: 2 byte rjmp opcode to bootloader pgm_write_page(sram, flash) or ret opcode
 * FLASHEND-1: capability byte of bootloader
 * FLASHEND-0: version number of bootloader: 5 msb = major version, 3 lsb = minor version
 */

/*
 * Capability byte of bootloader from version 7.2 onwards
 *
 * The PGMWRITEPAGE bit has been redundant from v7.5 as one can tell presence of the page write
 * routine by checking the rjmp opcode against the ret opcode. From v7.7 this bit has been
 * allocated for autobaud detection. Similarly, from v7.7 the PROTECTME option has been forced to
 * 1, and the RESETFLAGS presevation has been on from at least v7.6. These two bits have also been
 * reallocated in v7.7 to represent even stronger protection (bootloader and reset vector) and chip
 * erase capability, respectively.
 */

#define UR_AUTOBAUD         128 // Bootloader has autobaud detection (v7.7+)
#define UR_EEPROM            64 // EEPROM read/write support
#define UR_UPDATE_FL         32 // Check flash page before writing it (from v8.0)
#define UR_DUAL              16 // Dual boot
#define UR_VBLMASK           12 // Mask for two vector bootloader bits
#define UR_VBLPATCHVERIFY    12 // No longer used in v8.0+
#define UR_VBLPATCH           8 // No longer used in v8.0+
#define UR_VBLJUMP            4 // Start application via interrupt vector instead of reset
#define UR_NO_VBL             0 // Not a vector bootloader, must set fuses to HW bootloader support
#define UR_PROTECTRESET       2 // Bootloader enforces reset vector points to bootloader (v7.7+)
#define UR_HAS_CE             1 // Bootloader has Chip Erase (v7.7+)

#define UR_AFLAG  (AUTOBAUD? UR_AUTOBAUD: 0)
#define UR_EFLAG  (EEPROM? UR_EEPROM: 0)
#define UR_UFLAG  (UPDATE_FL? UR_UPDATE_FL: 0)
#define UR_DFLAG  (DUAL? UR_DUAL: 0)
#define UR_VFLAGS ((VBL & 3) * (UR_VBLMASK & -UR_VBLMASK)) // Shift VBL into mask position
#define UR_PFLAG  (PROTECTRESET && VBL? UR_PROTECTRESET: 0)
#define UR_CFLAG  (CHIP_ERASE? UR_HAS_CE: 0)

#define UR_TYPE (UR_AFLAG | UR_EFLAG | UR_UFLAG | UR_DFLAG | UR_VFLAGS | UR_PFLAG | UR_CFLAG)

/*
 * Version section just under FLASHEND has 6 bytes (4 bytes in versions 7.2 to 7.4)
 *  - extended version word (the VBL vector number and number of bootloader pages from version 7.5)
 *  - rjmp pgm_write_page code at 4 bytes below FLASHEND+1UL
 *  - one byte bitfield that encodes features of this urboot compilation
 *  - one byte for major (5 most sig bits) and minor (3 least sig bits) version numbers
 *
 * This resides at the *end* rather than the start of the bootloader because it's clear that all
 * boot sections should end at FLASHEND, but an application wouldn't necessarily know where the
 * start of the boot section is. This is a bit awkward, as RJMPWP is computed by the urboot-gcc
 * perl script. I simply couldn't figure out how to let the linker do this (relative jumps between
 * sections are above my pay grade - smr).
 */

unsigned const int __attribute__ ((section(".version"))) urboot_version[] = {
  UR_EXT_WORD(UR_EXT_VBLVECTNUM, UR_EXT_BLPAGES),
  RJMPWP,                       // Opcode for rjmp to pgm_write_page() or ret if not there
  (VERSION<<8) + UR_TYPE,
};



// Normal app start: jump to reset vector or dedicated VBL vector and tell compiler mcusr is needed
#if FLASHin8k || FLASHWRAPS
#define jmpToAppOpcode() asm("rjmp urboot_version+%0\n" :: \
  "n"(sizeof urboot_version+appstart_vec_loc), "r"(mcusr))
#else
#define jmpToAppOpcode() asm("jmp %0\n" :: "n"(appstart_vec_loc), "r"(mcusr))
#endif


// Main is put by linker at the start of the bootloader (same as compile-time constant START)
int main(void) {
  register uint8_t mcusr asm("r2"); // Ask compiler to allocate r2 for reset flags

  asm volatile(" eor r1, r1");  // Clear temporary register r1 to zero
#if !UB_INIT_SP_IS_RAMEND       // Some MCUs initialise SP to 0, not RAMEND, after reset
  SP = RAMEND;
#endif

  // Copy reset flags and clear them
  mcusr = UB_MCUSR;
  UB_MCUSR = 0;
  watchdogConfig(WATCHDOG_OFF);

  // Unless there was an external reset jump to application
#if !DUAL && !(defined(__AVR_ERRATA_SKIP_JMP_CALL__) && __AVR_ERRATA_SKIP_JMP_CALL__ && \
  !FLASHin8k && !FLASHWRAPS)
  // Skip next instruction if EXTRF set (compiler doesn't know length of asm is one instruction)
  asm volatile("sbrs %[ms], %[extrf]\n" :: [ms] "r"(mcusr), [extrf] "I"(EXTRF));
    jmpToAppOpcode();
#else

  if(!(mcusr & _BV(EXTRF))) {
#if DUAL                        // Check external SPI flash, then start the application
    if(mcusr & _BV(WDRF))       // Reset by watchdog? Check for dual boot from external flash
      dual_boot();
#endif
     jmpToAppOpcode();
    __builtin_unreachable ();
  }
#endif


// Start of bootloader

  // Mark location of WDTO setting, so urloader can change the 500 ms default externally
  asm volatile(".global watchdog_setting\nwatchdog_setting:\n\t" :::);
  watchdogConfig(WATCHDOG_TIMEOUT(WDTO));


#if SWIO && !defined(TX)
#error you need to define TX for SWIO
#elif SWIO

  // Set TX pin as output
  UR_DDR(TX) |= UR_BV(TX);

#else // !SWIO

// Initialise the UART

// Frame 8N1: this is the default in all data sheets that I've read: do not initialise UCSRnC
#ifndef UB_INIT_UCSRnC
#define UB_INIT_UCSRnC         0
#endif

#if defined(C_UCSZn0) &&  defined(C_UCSZn1)
#if defined(C_URSELn)            // Needed by some MCUs for shared I/O locations of UBRRH and UCSRC
#define  UCSRnC_val (_BV(C_URSELn) | _BV(C_UCSZn0) | _BV(C_UCSZn1))
#else
#define  UCSRnC_val (_BV(C_UCSZn0) | _BV(C_UCSZn1))
#endif
#endif

// Only ATtiny441 or ATtiny841 can remap UART0 RX/TX pins
#if UR_UARTTYPE == UR_UARTTYPE_CLASSIC && defined(U_REMAP) && defined(U0MAP) && UARTNUM == 0 && UARTALT == 1
  U_REMAP = _BV(U0MAP);
#endif

#if AUTOBAUD                    // 18 bytes to measure host baudrate vs 2 bytes to load constant BR

// Measure cycles/rxbit = F_CPU/baudrate and set UBBRnL
// Loop 2 below modified from https://github.com/nerdralph/picoboot/

#ifndef UART2X                  // Autobaud defaults to 2x speed for higher baud rate spectrum
#define UART2X                1
#endif

#if UART2X || UR_UARTTYPE == UR_UARTTYPE_LIN
#define auto_inc (256/8)        // Add 0.125 to X, ie, 256/8 to XL, in 8-bit fixed point rep
#else
#define auto_inc (256.16)       // Add 0.0625 to X, ie, 256/16 to XL, in 8-bit fixed point rep
#endif

#define autobaud(store_brrl) \
   "ldi  r26, 0x7f\n"           /* Initialise X with -0.5 in 8-bit fixed point representation */ \
   "ldi  r27, 0xff\n"           /* Want XH = UBRRnL = F_CPU/(8*baudrate)-1 = cycles/(8*bits)-1 */ \
 "1: " \
   "sbic %[RXPin], %[RXBit]\n"  /* Wait for falling start bit edge of 0x30=STK_GET_SYNC */ \
   "rjmp 1b\n" \
 "2: " \
    adiw(r26, auto_inc)         /* Increment r26:27 so that final value of r27 is BRRL divisor */ \
   "sbis %[RXPin], %[RXBit]\n"  /* Loop as long as rx bit is low */ \
   "rjmp 2b\n"                  /* 5-cycle loop for 5 low bits (start bit + 4 lsb of 0x30) */ \
    store_brrl                  /* Store r27 to BRRL register */ \
 "3: " \
   "sbiw r26, 1\n"              /* Drain input: run down X for 25.6 rx bits (256/8 * 4 c/5 c) */ \
   "brne 3b\n"                  /* 4-cycle loop */

#endif // AUTOBAUD


#if UR_UARTTYPE == UR_UARTTYPE_CLASSIC

// Number of USART registers /not/ in I/O space (ignore shared BRRH for rare cases)
#define SHARED_BRRH (!AUTOBAUD && UARTNUM==1 && (defined(__AVR_ATmega161__) || defined(__AVR_AT94K__)) && BAUD_SETTING > 255)
#define N_not_IO_BRRL !UisIOSPACE(UBRRnL)
#if !AUTOBAUD && !SHARED_BRRH && defined(UBRRnH) && BAUD_SETTING > 255
#define N_not_IO_BRRH !UisIOSPACE(UBRRnH)
#else
#define N_not_IO_BRRH 0
#endif
#if UART2X
#define N_not_IO_CSRA !UisIOSPACE(UCSRnA)
#else
#define N_not_IO_CSRA 0
#endif
#define N_not_IO_CSRB !UisIOSPACE(UCSRnB)
#if UB_INIT_UCSRnC && defined(UCSRnC_val)
#define N_not_IO_CSRC !UisIOSPACE(UCSRnC)
#else
#define N_not_IO_CSRC 0
#endif

// Less than 3 regs to be initialised outside I/O space? Assign all directly
#if N_not_IO_BRRL+N_not_IO_BRRH+N_not_IO_CSRA+N_not_IO_CSRB+N_not_IO_CSRC < 3

  asm volatile(

#if AUTOBAUD
    autobaud(out_ubrrnl(r27))
#else // !AUTOBAUD
#if SHARED_BRRH
    ldi(r24, %[shared_setting]) // Sic! UART0's high BRR nibble serves as bit 9:11 of UART1's
    out_ubrr0h(r24)             // U_UBRR0H = (BAUD_SETTING>>8) << 4;
#elif defined(UBRRnH) && BAUD_SETTING > 255
    ldi(r24, hi8(%[baud_setting]))
    out_ubrrnh(r24)             // UBRRnH = BAUD_SETTING>>8;
#endif
 ".global ldi_baud\nldi_baud: " // Mark location of baud setting
    ldi(r24, lo8(%[baud_setting]))
    out_ubrrnl(r24)             // UBRRnL = BAUD_SETTING & 0xff;
#endif // AUTOBAUD

#if UART2X
    ldi(r24, _BV(A_U2Xn))
    out_ucsrna(r24)             // UCSRnA = _BV(A_U2Xn);
#endif
    ldi(r24, (_BV(B_RXENn) | _BV(B_TXENn)))
    out_ucsrnb(r24)             // UCSRnB = (_BV(B_RXENn) | _BV(B_TXENn));
#if UB_INIT_UCSRnC && defined(UCSRnC_val)
    ldi(r24, UCSRnC_val)
    out_ucsrnc(r24)             // UCSRnC = UCSRnC_val;
#endif
  ::
#if !AUTOBAUD
    [baud_setting] "n"(BAUD_SETTING),
    [shared_setting] "n"((uint8_t) ((BAUD_SETTING>>8)<<4)),
#endif
    [RXPin] "I"(_SFR_IO_ADDR(UR_PIN(RX))), [RXBit] "I"(UR_BIT(RX))
  : "r26", "r27", "r24"
  );

#else // 3 or more registers to be initialised: assign via store indirect Z+offset

  asm volatile(
    "ldi r30, lo8(%[base])\n"   // Load Z for st Z+offset, r27
    "ldi r31, hi8(%[base])\n"

#if AUTOBAUD
    autobaud("std Z+%[brrl_off], r27\n")
#else
#if defined(UBRRnH_off) && BAUD_SETTING > 255
    "ldi r27, hi8(%[brrl_val])\n"
    "std Z+%[brrh_off], r27\n"
#endif
  ".global ldi_baud\nldi_baud: " // Mark location of baud setting
    "ldi r27, lo8(%[brrl_val])\n"
    "std Z+%[brrl_off], r27\n"
#endif // AUTOBAUD

#if UART2X
    "ldi r27, lo8(%[sra_val])\n"
    "std Z+%[sra_off], r27\n"
#endif
    "ldi r27, lo8(%[srb_val])\n"
    "std Z+%[srb_off], r27\n"
#if UB_INIT_UCSRnC && defined(UCSRnC_val)
    "ldi r27, lo8(%[src_val])\n"
    "std Z+%[src_off], r27\n"
#endif
  ::
    [brrl_off] "I"(UBRRnL_off),
#if defined(UBRRnH_off)
    [brrh_off] "I"(UBRRnH_off),
#endif
#if AUTOBAUD
    [RXPin] "I"(_SFR_IO_ADDR(UR_PIN(RX))), [RXBit] "I"(UR_BIT(RX)),
#else
    [brrl_val] "n"(BAUD_SETTING),
#endif
    [sra_off] "I"(UCSRnA_off),
    [sra_val] "n"(_BV(A_U2Xn)),
    [srb_off] "I"(UCSRnB_off),
    [srb_val] "n"(_BV(B_RXENn) | _BV(B_TXENn)),
#if UB_INIT_UCSRnC && defined(UCSRnC_val)
    [src_off] "I"(UCSRnC_off),
    [src_val] "n"(UCSRnC_val),
#endif
    [base] "n"(_SFR_MEM_ADDR(*UARTn_base))
    : "r30", "r31", "r26", "r27"
  );
#endif // Less than 3 regs to be initialised outside I/O space?

#elif UR_UARTTYPE == UR_UARTTYPE_LIN


// LINBTRn = _BV(S_LDISRn) | (_sblbt << S_LBTn0);
// LINBRRnL = _linbrr(_sblbt, Baud);
// LINCRn = _BV(C_LENAn) | _BV(C_LCMDn2) | _BV(C_LCMDn1) | _BV(C_LCMDn0);

  asm volatile(
    "ldi r30, lo8(%[base])\n"   // Load Z for st Z+offset, r27
    "ldi r31, hi8(%[base])\n"
  ".global ldi_linlbt\nldi_linlbt: " // Mark location of LINLBT setting
    "ldi r27, %[btr_val]\n"
    "std Z+%[btr_off], r27\n"
#if AUTOBAUD
    autobaud("std Z+%[brrl_off], r27\n")
#else
  ".global ldi_linbaud\nldi_linbaud: " // Mark location of LIN baud setting
    "ldi r27, %[brrl_val]\n"
    "std Z+%[brrl_off], r27\n"
#endif
    "ldi r27, %[lincr_val]\n"
    "std Z+%[lincr_off], r27\n"
  ::
    [brrl_off] "I"(LINBRRnL_off),
    [btr_off] "I"(LINBTRn_off),
#if AUTOBAUD
    [RXPin] "I"(_SFR_IO_ADDR(UR_PIN(RX))), [RXBit] "I"(UR_BIT(RX)),
    [btr_val] "n"(_BV(S_LDISRn) | (8 << S_LBTn0)),
#else
    [btr_val] "n"(_BV(S_LDISRn) | (BAUD_LINLBT << S_LBTn0)),
    [brrl_val] "n"(BAUD_SETTING),
#endif
    [lincr_off] "I"(LINCRn_off),
    [lincr_val] "n"(_BV(C_LENAn) | _BV(C_LCMDn2) | _BV(C_LCMDn1) | _BV(C_LCMDn0)),
    [base] "n"(_SFR_MEM_ADDR(*UARTn_base))
    : "r30", "r31", "r26", "r27"
  );
#endif // UR_UARTTYPEs
#endif // !SWIO


// Swing a square wave of 5 periods on pin FREQ_PIN (defaults to LED) to enable exact F_CPU measurement
#if defined(DEBUG_FREQ)

#if UB_CANNOT_TOGGLE_PORT
#define UB_FREQ_PIN_Toggle() (UR_PORT(FREQ_PIN) ^= UR_BV(FREQ_PIN))
#define UB_DEBUG_OHEAD (6e6/F_CPU)     // Subtract 6 CPU cycles for loop overhead
#else
#define UB_FREQ_PIN_Toggle() (UR_PIN(FREQ_PIN) |= UR_BV(FREQ_PIN))
#if IS_XMEGA
#define UB_DEBUG_OHEAD (4e6/F_CPU)
#else
#define UB_DEBUG_OHEAD (5e6/F_CPU)
#endif
#endif

  UR_DDR(FREQ_PIN) |= UR_BV(FREQ_PIN); // Set FREQ_PIN pin as output
  uint8_t rn = 2*DEBUG_CYCLES;
  do {
#if defined(FREQ_POLARITY) && FREQ_POLARITY < 0
    _debug_freq_us(0.5e6/DEBUG_FREQ - UB_DEBUG_OHEAD);
    UB_FREQ_PIN_Toggle();
#else
    UB_FREQ_PIN_Toggle();
    _debug_freq_us(0.5e6/DEBUG_FREQ - UB_DEBUG_OHEAD);
#endif
  } while(--rn);
#endif

  // Exit loop by causing WDT reset - which will initialise the MCU
  for(;;) {
    uint8_t ch = getch();

    if(0) {


#if CHIP_ERASE
    } else if(ch == STK_CHIP_ERASE) {
      get_sync();               // First sync before chip_erase which can take long
      chip_erase();
#endif

#if PAGE_ERASE
    } else if(ch == UR_PAGE_ERASE) {
      (void) getaddrlength();
      get_sync();               // First sync before page_erase which can take long
      ub_page_erase();
#endif

#if EEPROM
    } else if(ch == UR_PROG_PAGE_FL || ch == UR_PROG_PAGE_EE) {
      uint8_t length = getaddrlength();
      write_sram(length);
      get_sync();
      if(ch == UR_PROG_PAGE_EE) { asm volatile(
      "1: "
#if EESIZE > 256
        out_eearh(r31)
#endif                          // do {
        out_eearl(r30)          //   EEAR = zaddress;
        "ld  r24, X+\n"         //   EEDR = *bufp++;
        out_eedr(r24)            //
        "sbi %[eecr], %[eempe]\n" // EECR |= _BV(EEMPE); // EEPROM master write enable
        "sbi %[eecr], %[eepe]\n"  // EECR |= _BV(EEPE);  // EEPROM write enable
        "adiw r30, 1\n"         //   zaddress++;
      "2: "
        sbxc_eecr(xx, EEPE)     //   while(EECR & _BV(EEPE))
        "rjmp 2b\n"             //     continue;
        "subi %[len], 1\n"
        "brne 1b\n"             // } while(--length);
      : /*
         * No need to tell gcc that length or zaddress is changed; they
         * won't be needed and the compiler doesn't know what they are.
         */
      : [len] "d"(length), "z"(zaddress),
        [eecr] "I"(_SFR_IO_ADDR(EECR)), [eempe] "I"(EEMPE), [eepe] "I"(EEPE)
      : "r24", "r26", "r27");
      } else {
        writebufferX();
      }
#else
    } else if(ch == UR_PROG_PAGE_FL) {
      (void) getaddrlength();
      write_sram_spm_pagesize();
      get_sync();
      writebufferX();
#endif

    } else if(ch == UR_READ_PAGE_FL) {
      uint8_t length = getaddrlength();
      get_sync();
      asm volatile(
      "1: "
        LPM  "r24, Z+\n"        // do {
        "rcall putch\n"         //   putch(*zaddress++);
        "subi %[len], 1\n"      // } while(--length);
        "brne 1b\n"
      : // Again, don't tell gcc length has changed
      : [len] "d"(length), "z"(zaddress)
      : putch_clobberlist "r24"
      );

#if EEPROM
    } else if(ch == UR_READ_PAGE_EE) {
      uint8_t length = getaddrlength();
      get_sync();
      asm volatile(
      "1: "
#if EESIZE > 256
        out_eearh(r31)
#endif                          // do {
        out_eearl(r30)          //   EEAR = zaddress;
        "sbi %[eecr], %[eere]\n" //  EECR |= _BV(EERE); // Start EEPROM read
        in_eedr(r24)            //   putch(EEDR);
        "rcall putch\n"         //
        "adiw r30, 1\n"         //   zaddress++;
        "subi %[len], 1\n"      // } while(--length);
        "brne 1b\n"
      : // Don't tell gcc length has changed
      : [len] "d"(length), "z"(zaddress),
        [eecr] "I"(_SFR_IO_ADDR(EECR)), [eere] "I"(EERE)
      : putch_clobberlist "r24"
      );
#endif


#if QEXITEND
    } else if(ch == STK_LEAVE_PROGMODE) {
      leave_progmode();
#endif

    } else {
#if !EEPROM
#if QEXITEND
      if(ch == UR_READ_PAGE_EE || ch == UR_PROG_PAGE_EE)
        quickbootexit();
#else
      while(ch == UR_READ_PAGE_EE || ch == UR_PROG_PAGE_EE)
        continue;
#endif
#endif
      get_sync();               // Covers the rest, eg, STK_ENTER_PROGMODE, STK_GET_SYNC, ...
    }

    putch(STK_OK);
  }
}


void putch(char chr) {
  asm volatile(
#if !SWIO

  "1: "
#if UR_UARTTYPE == UR_UARTTYPE_CLASSIC
    sbxs_ucsrna(r25, A_UDREn)   // while(!(UCSRnA & _BV(A_UDREn)))
    "rjmp 1b\n"                 //   continue;
    out_udrn(%[chr])            // UDRn = chr;
#elif UR_UARTTYPE == UR_UARTTYPE_LIN
    sbxc_linsirn(r25, A_LBUSYn) // while(LINSIRn & _BV(A_LBUSYn))
    "rjmp 1b\n"                 //   continue;
    out_lindatn(%[chr])         // LINDATn = chr;
#endif
  :: [chr] "r"(chr) : "r25"

#else
  // By and large follows Atmel's AN AVR305
    "ldi  r18, 10\n"            // Start bit, 8 data bits, 1 stop bit
    "com  %[chr]\n"             // One's complement
    "sec\n"                     // Set carry (for start bit)
  "1: "
    "brcc 2f\n"
  ".global cbi_tx\ncbi_tx: "    // Mark location of cbi tx opcode
    "cbi %[TXPort], %[TXBit]\n" // Set carry puts line low
    "rjmp 3f\n"
  "2: "
    "sbi %[TXPort], %[TXBit]\n" // Clear carry puts line high
    "nop\n"
  "3: "
    "rcall bitDelay\n"
#if SWIO_B_DLYTX == 1           // Add 1 cycle to adjust timing to be same as getch()
    "nop\n"
#endif
    "lsr %[chr]\n"              // Push lsb into carry, on empty byte carry is clear (stop bit)
    "dec r18\n"
    "brne 1b\n"
    "ret\n"

  "bitDelay: \n"
#if B_EXTRA == 1 || B_EXTRA == 2
    ".global swio_extra\nswio_extra: " // Mark location of SWIO extra delay
#endif
#if B_EXTRA & 1
    "nop\n"
#endif
#if B_EXTRA == 2 || B_EXTRA == 3
    "rjmp .+0\n"
#endif
    "rcall halfBitDelay\n"
    "halfBitDelay: "
#if SWIO_B_VALUE > 0
    ".global ldi_bvalue\nldi_bvalue: " // Mark location of ldi SWIO_B_VALUE
    "ldi r25, %[bvalue]\n"
  "1: "
    "dec r25\n"
    "brne 1b\n"
#endif
#if B_EXTRA == 4 || B_EXTRA == 5
    "rjmp .+0\n"
#endif
  ::
    [chr] "r"(chr), [TXPort] "I"(_SFR_IO_ADDR(UR_PORT(TX))),
    [TXBit] "I"(UR_BIT(TX)), [bvalue] "M"(SWIO_B_VALUE & 0xff)
  : "r18", "r25"
#endif
  );
}


uint8_t getch(void) {
  uint8_t chr;

  led_on();
  led_setup();

  asm volatile(
#if !SWIO

#if UR_UARTTYPE == UR_UARTTYPE_CLASSIC
  "1: "
    sbxs_ucsrna(%[chr], A_RXCn)
    "rjmp 1b\n"

    // Watchdog reset wdr (possibly skipped when rx error bit is set in uart status reg USRAn)
#if EXITFE==2
    sbRc_ucsrna(%[chr], A_FEn)  // Skip 2-byte exit code if error bit is clear in uart status register
    RJMPQEXIT
#elif EXITFE==1
    sbRs_ucsrna(%[chr], A_FEn)  // Skip wdr if error bit is set in uart status register
#endif
    "wdr\n"
    in_udrn(%[chr])             // Load UART data register

#elif UR_UARTTYPE == UR_UARTTYPE_LIN
  "1: "
    sbxs_linsirn(%[chr], A_LRXOKn)
    "rjmp 1b\n"

    // Watchdog reset wdr (possibly skipped when rx error bit is set in uart status reg USRAn)
#if EXITFE==2
    sbRc_linsirn(%[chr], A_LERRn) // Skip 2-byte exit code if error bit is clear in uart status register
    RJMPQEXIT
#elif EXITFE==1
    sbRs_linsirn(%[chr], A_LERRn) // Skip wdr if error bit is set in uart status register#endif
#endif
    "wdr\n"
    in_lindatn(%[chr])          // Load UART data register

#endif // UR_UARTTYPE
    : [chr] "=r"(chr)

#else // SWIO

    "ldi   r18, 9\n"            // 8 bit + 1 stop bit
  ".global sbic_rx\nsbic_rx: "  // Mark location of sbic rx opcode
  "1: "
    "sbic  %[RXPin], %[RXBit]\n" // Wait for falling edge of start bit
    "rjmp  1b\n"
    "rcall halfBitDelay\n"      // Get to middle of start bit
  "2: "
    "rcall bitDelay\n"          // Wait 1 bit & sample
    "clc\n"
    "sbic  %[RXPin], %[RXBit]\n"
    "sec\n"
    "dec   r18\n"
    "breq  3f\n"
    "ror   %[chr]\n"
#if SWIO_B_DLYRX == 1           // Add 1 cycle so timing is same as putch()
    "nop\n"
#endif
    "rjmp  2b\n"
  "3: "
#if EXITFE==2                   // Hard exit on frame error
#if QEXITERR
    "brcs .+2\n"                // Jump to wdt when OK (carry contains stop bit, set = no FE)
    RJMPQEXIT                   // Two-byte relative jump to quick exit
#else
    "brcc .-2\n"                // WDT reset
#endif
#elif EXITFE==1                 // Don't reset the watchdog timer on frame error
    "brcc .+2\n"                // Jump over wdt on frame error (carry is stop bit, clear = FE)
#endif
    "   wdr\n"
  : [chr] "=r"(chr)
  : [RXPin] "I"(_SFR_IO_ADDR(UR_PIN(RX))), [RXBit] "I"(UR_BIT(RX))
  : "r25", "r18" // r25 is clobbered by bitDelay

#endif // !SWIO
  );

  led_off();

  return chr;
}


void get_sync(void) {
  uint8_t last = getch();

#if QEXITERR
  if(last != CRC_EOP) {
    asm volatile("qexiterr:\n\t" :::);
    watchdogConfig(WATCHDOG_16MS); // Shorten WD timeout
    bootexit();
  }
#else
  asm volatile(
    cpi(%[last], CRC_EOP)       // if(last != CRC_EOP)
    "brne .-2\n"                //   bootexit();
    :: [last] "r"(last)
  );
#endif

  putch(STK_INSYNC);
}


void watchdogConfig(uint8_t x) {
  isr_wdt_config(x);            // same as wdt_reset(); WDTCSR = _BV(WDCE) | _BV(WDE); WDTCSR = x;
}


// Optimise backing up and restoring Z below
#if SPM_PAGESIZE == 256 // Subtract 256
#define backupZ
#define restoreZ "subi r31, 1\n"
#define clobberZ

#elif SPM_PAGESIZE <= 32
#define backupZ
#define restoreZ sbiw(r30, SPM_PAGESIZE)
#define clobberZ

#elif UPDATE_FL || DUAL         // Either requires a full backup/restore
#define backupZ  "movw r18, r30\n"
#define restoreZ "movw r30, r18\n"
#define clobberZ "r19", "r18",

#else
#define backupZ
#define restoreZ "sbiw r30, 2\n" // Just get Z back to previous page
#define clobberZ
#endif

/*
 * Rather than using the <avr/boot.h> macros I roll my own: it's worth it (saves 16-24 bytes)
 *
 * zaddress  r31:30 global
 * X         r27:26 local pointer (scans through sram)
 * sram      r25:24 argument as input
 *           r25    used as Boolean variable for UPDATE_FL
 * argument  r24    used for calling ub_spm and as temporary register
 * pgm       r23:22 argument if flash <= 64 kB (otherwise high 16 bit of pgm)
 *           r21:20 argument low 16 bit of pgm if flash > 64 kB
 *           r20    old byte from PROGMEM (UPDATE_FL only)
 *           r21    new to-be-written byte from SRAM (UPDATE_FL only)
 * origRAMPZ r22    third byte of 32 bit pgm argument if flash > 64 kB
 * xloopend  r23    lo8(X) at end of loop (for comparison)
 * origzaddr r19:18 copy of global address in Z (when needed, see clobberZ)
 *           r1     null on return (compiler relies on r1 being 0)
 * tmp       r0
 *
 * The spm command is called 3 times with an address in flash; the erase and write spm call should
 * operate on the same address including RAMPZ to avoid erasing one page and writing to another.
 * The fill buffer spm call ignores the page the address belongs to but uses the lower bits for
 * position in buffer. The caller can pass to writebuffer() a pointer to /any/ location in the page
 * and the algorithm will write that page.
 *
 */

#if PGMWRITEPAGE

void pgm_write_page(void *sram, progmem_t pgm) {
  asm volatile(                 // Copy sram to X and pgm to Z
#if FLASHabove64k
     out_rampz(r22)             // RAMPZ = pgm>>16;
    "movw r30, r20\n"           // zaddress = pgm & 0xffff, r21:20 is free now
#else
    "movw r30, r22\n"           // zaddress = pgm (pgm is a 2-byte pointer)
#endif
    "movw r26, r24\n"           // X = sram, r25:24 is free now
                                // Fall through to void writebufferX()
  "writebufferX:\n"

#else // !PGMWRITEPAGE

void writebufferX(void) {       // Write buffer from sram at X to PROGMEM at RAMPZ:Z
  asm volatile(                 // ) }
#endif // PGMWRITEPAGE

    andi(r30, lo8(~(SPM_PAGESIZE-1))) // Ensure Z aligns with page boundary

/*
 * Compute low byte of the sram address at end of loop that reads SPM_PAGESIZE bytes, so we can
 * compare with cpse. If either
 *   - the low byte of SPM_PAGESIZE is zero or
 *   - no PGMWRITEPAGE (then the sram address is the constant rambuffer)
 * it is a mov/ldi, otherwise add low byte of sram to low byte of SPM_PAGESIZE for end condition.
 * The loop is for no more than 256 bytes, so single byte arithmetic is OK.
 */
#if SPM_PAGESIZE == 256         // Determine loop end comparison reg r23
    "mov  r23, r26\n"           // No need to add, just copy low byte of sram address
#elif !PGMWRITEPAGE             // No page write? This routine is only used for sram=RAMBUFFER
     ldi(r23, lo8(RAMBUFFER + SPM_PAGESIZE))
#else
     ldi(r23, lo8(SPM_PAGESIZE))
    "add  r23, r26\n"           // r23 = lo8(len + sram) for loop end comparison with lo8(X)
#endif

    backupZ
#if FLASHabove64k && (PROTECTME || (VBL && PROTECTRESET))
     in_rampz(r22)              // Also copy RAMPZ
#endif

#if PROTECTME
#if START & 0xff
    "cpi  r30, lo8(%[start])\n" // zaddress >= START?
    "ldi  r24, hi8(%[start])\n"
    "cpc  r31, r24\n"
#else                           // Low byte of START zero, easier comparison
    "cpi  r31, hi8(%[start])\n" // zaddress >= START?
#endif
#if FLASHabove64k
    "ldi  r24, hh8(%[start])\n"
    "cpc  r22, r24\n"           // RAMPZ
#endif
    "brcc ub_ret\n"             // Return if so
#endif // PROTECTME

#if VBL && PROTECTRESET && FLASHWRAPS // Store "rjmp START" opcode at addr 0 to protect bootloader
#if FLASHabove64k
    "cpi   r22, 0\n"            // RAMZ != 0?
    "brne  not_reset_page\n"
#endif
    "adiw  r30, 0\n"            // zaddress != 0?
    "brne  not_reset_page\n"
    "ldi   r24, lo8(%[rjmp_bwd_start])\n" // Store rjmp to bootloader at address 0
    "st    X+, r24\n"
    "ldi   r24, hi8(%[rjmp_bwd_start])\n"
    "st    X, r24\n"
    "sbiw  r26, 1\n"            // Restore X pointer to ram
  "not_reset_page: "
#endif

#if UPDATE_FL >= 1              // Return if flash page already has desired contents
    ldi(r25, 0)                 // Set all Booleans to 0
  "3: "
     LPM  "r20, Z+\n"           // Load existing byte from PROGMEM, might increment RAMPZ
    "ld    r21, X+\n"           // Load new byte from ram page
#if !ERASE_B4_WRITE
    "and   r21, r20\n"          // New byte can only delete existing 1's in NOR memory
#endif
    "cpse  r20, r21\n"          // Does a page write make a difference?
    "ori r25, 1<<%[differ]\n"

#if ERASE_B4_WRITE && UPDATE_FL >= 4
    cpi(r21, 0xff)              // Are any of the to-be-written bytes not 0xff?
    "breq .+2\n"
    "ori r25, 1<<%[notff]\n"
#endif

#if ERASE_B4_WRITE && UPDATE_FL >= 3
    "and   r20, r21\n"          // Check if new byte from ram would set a bit
    "cpse  r20, r21\n"
    "ori r25, 1<<%[clearpg]\n"  // If so PROGMEM needs erasing before write
#endif

    "cpse  r26, r23\n"          // Finished?
    "rjmp  3b\n"

    restoreZ
#if FLASHabove64k
    out_rampz(r22)              // Restore RAMPZ should it have gone over 64 kB boundary
#endif
#if SPM_PAGESIZE == 256         // Restore sram
    "dec r27\n"
#elif SPM_PAGESIZE <= 32
    sbiw(r26, SPM_PAGESIZE)
#elif !PGMWRITEPAGE && (RAMBUFFER+SPM_PAGESIZE)/256 == RAMBUFFER/256
    ldi(r26, lo8(RAMBUFFER))    // sram was RAMBUFFER if we don't offer pgm_write_page()
#else                           // Any sram, subtracy SPM_PAGESIZE to go to start
    subi(r26, SPM_PAGESIZE)
    "sbc r27, r1\n"
#endif

    "sbrs r25, %[differ]\n"
    "rjmp ub_ret\n"             // Return straight away if page write not needed

#if ERASE_B4_WRITE && UPDATE_FL >= 3
    "sbrs r25, %[clearpg]\n"
    "rjmp skip_erase\n"         // Can skip erase when no bit in page needs setting
#endif
#endif // UPDATE_FL

#if ERASE_B4_WRITE
#if FOUR_PAGE_ERASE             // 3*SPM_PAGESIZE fits in one byte for those
     ldi(r24, 3*SPM_PAGESIZE)   // if(!(zaddress & 3*SPM_PAGESIZE))
    "and  r24, r30\n"           //   ub_page_erase();
    "brne skip_erase\n"
#endif
    ldi(r24, __BOOT_PAGE_ERASE)
    "rcall ub_spm\n"

#if UPDATE_FL >= 4              // All data 0xff? Page just erased no need to write
#if defined(RWWSRE)
    "sbrs r25, %[notff]\n"
    "rjmp rww_enable\n"
#else
    "sbrs r25, %[notff]\n"
    "rjmp ub_ret\n"
#endif
#endif // UPDATE_FL >= 4
#endif // ERASE_B4_WRITE

  "skip_erase: "
     ldi(r24, __BOOT_PAGE_FILL)
  "1: "
    "ld    r0,  X+\n"
    "ld    r1,  X+\n"           // Data word for spm page fill is in r1:0
    "rcall ub_spm\n"
    "adiw  r30, 2\n"            // zaddress += 2, RAMPZ stays the same
    "cpse  r26, r23\n"          // Finished?
    "rjmp  1b\n"

    restoreZ
    ldi(r24, __BOOT_PAGE_WRITE)
#if defined(RWWSRE)
    "rcall ub_spm\n"
  "rww_enable: "
    ldi(r24, __BOOT_RWW_ENABLE)
#endif                          // Fall through to ub_spm and return

  "ub_spm: "
    out_spmcr(r24)
    "spm\n"
#if defined(__AVR_ATmega161__) || defined(__AVR_ATmega163__) || defined(__AVR_ATmega323__)
   ".word 0xffff\n"            // Above microprocessors need this code pipeline after SPM
   "nop\n"
#endif
  "2: "
   sbxc_spmcr(r1, __SPM_ENABLE)
    "rjmp 2b\n"
  "ub_ret: "
    "eor r1, r1\n"              // R1 will also have been destroyed on spm for fill page
  ::
#if UPDATE_FL
    [differ] "I"(0),            // Do SRAM and PROGMEM effecively differ? (bit 0 in r25)
#if ERASE_B4_WRITE
#if UPDATE_FL >=4
    [notff] "I"(1),             // Is one of the new bytes in SRAM 0xff?
#endif
#if UPDATE_FL >= 3
    [clearpg] "I"(2),           // Does the page need erasing?
#endif
#endif
#endif
    [start] "n"((uint32_t) START),
    [rjmp_bwd_start] "n"(RJMP_BWD_START)
  : clobberZ "r31", "r30", "r27", "r26", "r25", "r24", "r23", "r22",
#if UPDATE_FL
    "r21", "r20",
#endif
    "r0"
  );
}
