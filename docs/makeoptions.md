# Make options

This repository should work out of the box on a typical PC under Linux. Just `cd src` and type
```
  make atmega328p_autobaud_ur
```
to test whether compilation works in principle. If successful, a
bootloader `atmega328p_autobaud_ur.hex` should have been generated. The
system needs `perl` installed and quite possibly the following perl
modules:
```
  File::Basename
  Capture::Tiny
  POSIX
  Scalar::Util
  Getopt::Long
```
They can normally be installed using `cpan install <module>`. On the first run, `cpan` may need
to be configured. Follow the recommendations there; my personal choice for the way cpan installs
perl modules is via `sudo`, but it should not matter what is chosen.

Urboot bootloaders have many configuration options for avr-gcc. With the notable exception of
`-DSTART` and `-DRJMPWP` (which are set by the `avr-gcc` wrapper `urboot-gcc`, see below) almost
all bootloader generation options can be put on the command line for make without the `-D`. For
example, issuing the command

```
 $ make MCU=attiny45 F_CPU=123456L SWIO=1 RX=AtmelPB0 TX=AtmelPB1 NAME=myboard
```

creates a bootloader file `myboard.hex` for a board with the MCU ATtiny45 and the CPU frequency
1.23456 MHz using software I/O on the pins `PB0` and `PB1` for uploading new applications.

Below is a list of options that are currently supported by the urboot project;
unless an option can only be issued to `avr-gcc` this help file will leave the leading `-D` away.

## Essentials

 - `MCU=<mcu>`

   This selects the AVR part, eg, atmega328p (the default `make` option), for which the bootloader
   is compiled. The name must be in all lower case.

 - `F_CPU=<num>L`

    This option selects the CPU frequency of the destination board in Hertz as a long constant
    number, eg, `F_CPU=16000000L` for 16 MHz, which is also the default `make` option. Unless
    the option `AUTOBAUD` is selected, the bootloader code needs to know the CPU frequency as the
    serial communication baud rate is derived from it.

 - `NAME=<name>`

   This is a make-only option that determines the base name of the three files that `make` creates:
     + `<name>.hex` is an Intel hex file of the bootloader
     + `<name>.elf` is the executable and linkable format file
     + `<name>.lst` is a crude ASCII listing of an assembler file

   In the absence of this option `make` invents its own name that may or may not be agreeable. For
   backward compatibility `MOVETO=<name>` does the same thing.

## Serial communication

 - `UARTNUM=<n> UARTALT=<n> UART2X=<0|1> AUTOBAUD=<0|1> RX=...`
 - `UARTNUM=<n> UARTALT=<n> UART2X=<0|1|2> BAUD_RATE=<baud>`
 - `SWIO=<0|1> RX=... TX=... BAUD_RATE=<baud>`

   These three different settings determine how the serial communication is implemented. When the
   MCU has a hardware UART using that will normally create shorter code, and this is the default.
   In case the MCU has more than one UART, then `UARTNUM` specifies which one to use (0, 1, ...).
   Note that `UARTNUM` always counts from 0, irrespective of whether the only UART is actually
   called `USART1` or whether the UARTs have a different naming scheme as with the XMEGA UARTs
   that are named after the port, eg, `USARTC0`. `UARTALT`, if given, specifies an alternative
   pin assignment for RX/TX. It is usually a small number that aligns with the available
   alternative assignments lined out in the data sheet.

   If `AUTOBAUD` is set then the generated bootloader will try to initialise the UART with the
   host communication speed that the bootloader augurs from the first byte that is expected from
   the host at runtime; the code utilises that the lower 5 bit of that byte (`0x30` =
   `STK_GET_SYNC`) are `0x10`. `AUTOBAUD` is a bit of a misnomer, as urboot only offers 256
   different baud rates. Moreover, these must be compatible with `F_CPU`, namely `F_CPU/8`,
   `F_CPU/16`, `F_CPU/24`, ..., `F_CPU/2048`. The set of compatible baud rates comes with a narrow
   tolerance band of +/-1.5%. As a consequence, a 16 MHz MCU can only safely communicate in the
   following 33 baud rate intervals (the last one is the union of 224 overlapping individual baud
   rate intervals):

   ```
     [1970000, 2030000] = [0.985*F_CPU/(8*1), 1.015*F_CPU/(8*1)], F_CPU = 16 MHz
     [ 985000, 1015000] = [0.985*F_CPU/(8*2), 1.015*F_CPU/(8*2)]
     [ 656667,  676667]
     [ 492500,  507500]
     [ 394000,  406000]
     [ 328333,  338333]
     [ 281429,  290000]
     [ 246250,  253750]
     [ 218889,  225556]
     [ 197000,  203000]
     [ 179091,  184545]
     [ 164167,  169167]
     [ 151538,  156154]
     [ 140714,  145000]
     [ 131333,  135333]
     [ 123125,  126875]
     [ 115882,  119412] <-- 115200 baud is out of specs for a 16 MHz MCU
     [ 109444,  112778]
     [ 103684,  106842]
     [  98500,  101500]
     [  93810,   96667]
     [  89545,   92273]
     [  85652,   88261]
     [  82083,   84583]
     [  78800,   81200]
     [  75769,   78077]
     [  72963,   75185]
     [  70357,   72500]
     [  67931,   70000]
     [  65667,   67667]
     [  63548,   65484]
     [  61563,   63437]
     [  7695,    61515] = [0.985*F_CPU/(8*256), 1.015*F_CPU/(8*33)]
   ```

   Notice that the ubiquitous 115,200 baud is actually out of specs for popular 16 MHz MCU builds.
   In practice, tolerances of up to 2.5% may actually work, though that is not recommended. Also
   signal noise and cable length play a big role in whether communication is stable. The maximum
   permitted baud rate of `F_CPU/8` may not always work. In general, lower baud rates are expected
   to work better, at least they will exhibit a lower quantisation error. If the chosen oscillator
   source has an unknown but high deviation from the nominal frequency (as is the case for the
   internal 8 MHz oscillators of a typical MCU) then it is safer to choose a host computer baud
   rate in the last interval of the above table, ie, a baud rate of less than `F_CPU/260`. In the
   case of an 8 MHz internal oscillator with an autobaud bootloader this would be, eg, 28,800 baud,
   though 57,600 baud are likely to work and 115,200 baud might work. External oscillators usually
   have a good tolerance, even a resonator with a 2000 ppm error will only produce an additional
   deviation error of 0.2% in addition to the quantisation error caused by limited baud rate
   division choices for autobaud bootloaders.

   The above set of baud rates is generated by setting `UART2X` to 1, the default when `AUTOBAUD` has
   been set. Setting `UART2X=0` saves 4 bytes of code and has a higher tolerance of 2% error but will
   restrict available baud rates to half of the above baud rates: `F_CPU/16`, `F_CPU/32`, `F_CPU/48`,
   ..., `F_CPU/4096`.

   In order for the `AUTOBAUD` feature to work, urboot will need to (and usually does) know the rx
   line of the UART so it can measure the host's baud rate. In addition, the rx line needs to
   reside on a port that is within the bit-addressable bytes of the MCU, which are typically
   located in [0x20, 0x3f]. Therefore UARTs for which the rx pin is located on Port `H` and above
   are not available for `AUTOBAUD`, eg, UART2 and UART3 of the ATmega2560.

   If `AUTOBAUD` is not set or if it is set to 0, the code automatically selects reasonably good
   settings to match the desired fixed baud rate `BAUD_RATE`. In the absence of `AUTOBAUD` if
   `UART2X` is set to 1 or unset, then the code decides about whether or not to switch to the
   higher baud rate set based on the respective error rates. Setting `UART2X` to 2 will always
   utilise the set of higher baud rates only, setting it to 0 will always utilise the set of lower
   baud rates to match the desired BAUD_RATE.

   Some classic parts, eg the ATmega103, do not have a way to set double UART speed. Setting
   `UART2X` to 1 has no effect for these, and the set of available `AUTOBAUD` baud rates is as if
   `UART2X` had been set to 0.

   A dozen or so AVR parts support the automotive Local Interconnect Network that can be used as
   ordinary UART. This includes the LIN/UARTs of ATtiny167/87. They allow the setting the number of
   samples per transferred bit to 8..63 in addition to the baud rate divisor. Most classic UARTs
   only can set the number of samples to 16 or 8 (the latter is actually what `UART2x=1` does). As
   a consequence, LIN/UARTs can create almost all baud rates well within 1% of quantisation error
   with far fewer blind spots. Urboot bootloaders for these parts make use of that automagically.
   The setting of `UART2X` takes no effect for these parts when working with fixed baud rates.
   `AUTOBAUD` though works exactly as for the other classic parts, ie, the number of samples will be
   either set to 8 or 16 depending on the setting of `UART2X` as described above.

   `SWIO=1` creates code for software I/O. That is not only useful for those MCUs that don't have
   a UART, but also for those combinations of `F_CPU` and `BAUD_RATE` where a UART would create
   too large quantisation errors for the baud rate. It might also be necessary to resort to
   software I/O when the board uses the MCU's rx and tx lines otherwise. The options `RX` and `TX`
   specify which pins are used for software I/O. The syntax is the same as with `LED` and `SFMCS`:
   Use any pin descriptor in Atmel format such as `AtmelPB2` or as Arduino pin number as in
   `TX=ArduinoPin2`. The default baud rate depends on the CPU frequency of the board and is 115,200
   for 8 MHz or above. Again, the ports of the rx and tx line need to be below Port `H` for
   software I/O to work.

   Admissible baud rates also depend on the CPU frequency. The table below details the approximate
   boundaries as implemented in urboot bootloaders:

   | Baud rate | Lowest | Highest | Min @ F<sub>CPU</sub> = 16 MHz | Max @ F<sub>CPU</sub> = 16 MHz |
   | --: | --: | --: | --: | --: |
   | Classic `UART2X=0` | F<sub>CPU</sub>/65536 | F<sub>CPU</sub>/16 | 244 baud | 1,000,000 baud |
   | Classic `UART2X=1` | F<sub>CPU</sub>/65536 | F<sub>CPU</sub>/8 | 244 baud | 2,000,000 baud |
   | Classic `UART2X=2` | F<sub>CPU</sub>/32768 | F<sub>CPU</sub>/8 | 488 baud | 2,000,000 baud |
   | LIN UART | F<sub>CPU</sub>/16384 | F<sub>CPU</sub>/8 | 980 baud | 2,000,000 baud |
   | `AUTOBAUD` `UART2x=0` | F<sub>CPU</sub>/4096 | F<sub>CPU</sub>/16 | 3,900 baud | 1,000,000 baud |
   | `AUTOBAUD` `UART2x=1` | F<sub>CPU</sub>/2048 | F<sub>CPU</sub>/8 | 7,800 baud | 2,000,000 baud |
   | `SWIO=1` (16-bit PC) | F<sub>CPU</sub>/1560 | F<sub>CPU</sub>/23 | 10,300 baud | 700,000 baud |
   | `SWIO=1` (22-bit PC) | F<sub>CPU</sub>/1560 | F<sub>CPU</sub>/27 | 10,300 baud | 600,000 baud |

   Note that the highest values are theoretical limits that are normally quite difficult to achieve
   in practice owing to the fact that the bootloader needs to do work in between receiving bytes,
   owing to typically higher quantisation errors at the high end of baud rates and owing to
   physical limitations given by cable length and line noise. Even within those boundaries, some
   baud rates are out of specs, particularly with classic UARTs as detailed above.

## Main functional features

 - `URPROTOCOL=<0|1>`

   When set to 0, this option implements a skeleton of the STK500v1 protocol and generates extra
   code that is needed for avrdude's arduino programmer to work with the bootloader. The generated
   code at places does not do anything useful except keeping the STK500 v1 protocol going: `avrdude
   -c arduino` is a full-fledged `stk500v1` programmer, which issues a couple of requests to set
   the device that are not needed here and wants to read fuses. With `URPROTOCOL=0` the generated
   code gracefully accepts these requests but ignores them or, worse, returns a value that is just
   not true: for example, the bootloader pretends the fuses are `0xff`.

   `URPROTOCOL=1` works with a version of avrdude that offers an urclock programmer. The protocol
   is different to STK500v1, so that an MCU id and a few other features of this bootloader are sent
   to avrdude. Generally `URPROTOCOL=1` requires the bootloader to do much less work and shifts
   some of the work to the programmer. In this case, it is needed to run `avrdude -c urclock` for
   programming and `-c arduino` will not work. Setting `URPROTOCOL=1` saves between 76 and 112
   bytes.

   For historical reasons the default is `URPROTOCOL=0` though that is rarely useful. Once Avrdude
   v7.1 with its urclock programmer is reasonably well distributed, it is likely that `URPROTOCOL=1`
   will become the default. At some point `URPROTOCOL=0` will be withdrawn altogether to
   simplify `urboot.c`.

 - `WDTO=<timeout>` (watchdog timeout)

   Timeout for watchdog that governs how long after reset urboot waits for characters from the
   programmer before starting the application. Possible values are: `16MS`, `32MS`, `64MS`,
   `125MS`, `250MS`, `500MS`, `1S` and `2S`. Most MCUs also provide `4S` and `8S`. When the device
   has a reset circuit that can be triggered into reset by pulling the DTR or RTS line of the
   associated USB-to-serial converter chip then small times such as `500MS` (the default in the
   `urboot.c` file) or `1S` (the default in the Makefile) are useful. If there is no such reset
   circuit and upload involves manual synchronisation then higher values such as `2S` are
   recommended. Very small values up to `125MS` are unlikely to provide enough time for a handshake
   with the uploading program.

 - `EEPROM=<1|0>`

   With `EEPROM=1`, avrdude's option `-U eeprom:r:file.eep:i` saves the eeprom contents of the
   MCU onto a file `file.eep` and `-U eeprom:w:file.eep:i` will write the file contents to the
   MCU's eeprom. It is a useful property to be able to manage non-volatile data and programs
   independently. The default for this option is `EEPROM=1`.

 - `VBL=<0|1|2|3> [VBL_VECT_NUM=<num>]`

   This option is valid for parts that do not have a `UPDI` interface. It determines whether or not
   a vector bootloader is generated. The default for this option is `VBL=0` for devices that have
   hardware boot section support.

   `VBL=1` creates a vector bootloader: The AVR interrupt vector table consists of 4-byte `jmp`
   instructions or 2-byte `rjmp` instructions (if the MCU has no more than 8 kB flash it is always
   a `rjmp`). The reset vector is the first in the table, and contains a jump instruction to the
   start of the application. A vector bootloader (`VBL=1`, `2`, or `3`) is a hook between the reset
   vector and the application, where the reset vector has been made to point to the bootloader, and
   the bootloader - once finished - jumps to a dedicated but otherwise unused interrupt vector that
   has been made to point to the application. This trick provides a bootloader for boards or MCUs
   without boot section support (think Trinket, Digispark, ATmega48, ...). On MCUs with boot
   section support one can utilise a vector bootloader, too, by changing the fuses to jump to
   `0x0000` on external reset. This way the boot section can start at any multiple of
   `SPM_PAGESIZE` in high flash space, which can considerably reduce the space taken by the
   bootloader. For example, an ATmega328P system can then use 384 bytes (3 pages) rather than the
   otherwise minimum 512 bytes boot section. Likewise, those MCUs with 1 kB minimum boot size
   (ATmega1284P, eg) might save considerably more space that would otherwise needlessly be used for
   the bootloader.

   By default, a vector bootloader loses one interrupt vector that no program ever can use on that
   device. Urboot chooses a "rare" vector of the target MCU to store a jump instruction to the
   application start address. The default for that rarely used interrupt vector is `SPM_RDY` or, if
   that does not exist, `EE_RDY`, but it can be set by `VBL_VECT_NUM=<n>` with <n> = 1, 2, 3, ...

   If reserving an interrupt vector for the bootloader is out of question, there is also the option
   of enlarging the processor's interrupt vector table by one additional slot. This can be done in
   the Arduino IDE through adding the following to the end of the
   `hardware/arduino/avr/cores/arduino/main.cpp` source that will then automatically be included
   into every application:
   ```
   // Additional vector in ISR vector table used by the urboot bootloader
   uint8_t __attribute__((used)) __attribute__((section(".vectors")))
     _vbl_jmp_to_application[FLASHEND+1UL > 8192? 4: 2] = {0x0C, 0x94};
   ```
   The initialisation of the vector with the lower half of the 4-byte `jmp` instruction matters for
   MCUs with flash memory between more than 8k and 128k inclusively as urboot `VBL=2` or `3` only
   patches the destination address in the missing upper half of the 4-byte `jmp`. This increases
   the size of each sketch by 4 or 2 bytes, respectively. Compiling the bootloader with the option
   `VBL_VECT_NUM=-1` tells it to use this additional vector (which, eg, would translate to 26 for
   the ATmega328P, 35 for the ATmega1284P, 57 for the ATmega2560, 15 for the ATtiny85 and 20 for
   the ATtiny167).

   `VBL=1` creates a vector bootloader with otherwise zero bytes overhead; the only change in the
   bootloader is that it jumps to the application through the designated interrupt vector rather
   than the reset vector. Note that the interrupt vector table of every application burned with
   this bootloader needs to be patched before being uploaded: the designated interrupt vector needs
   to be a copy of the original reset vector with the application start, and the reset vector then
   needs to be modified to jump to the start of the bootloader. In the case of relative jumps (for
   MCUs with up to 8 kB flash) the relative destination needs adapting, too. Patching the
   application for upload can be done separately as an extra step between compiling and uploading.
   Alternatively, avrdude's urclock programmer will patch the application automatically. It knows
   about this bootloader, which encodes its version number and main capabilities in a table at the
   top of the boot section (see below), so that `avrdude -c urclock` can and will apply the patch
   for upload and verification auto-magically.

   If neither avrdude nor external patching of applications fits the workflow, one can alternatively
   compile a bootloader with `VBL=2` or `3`, so it patches the vectors at the cost of enlarged code
   size. Options `VBL=2` or `3` both create a vector bootloader that copies the reset vector of a
   program during upload into an unused interrupt vector and replaces the reset vector with a jump
   to the bootloader. Once the bootloader has modified the uploaded program, `avrdude -c arduino`
   would notice the changes in the verification step and complain. `VBL=3` creates code that
   specifically alters verification of the first page at address zero, which contains the vector
   table, so a copy of the uploaded vector page is returned. This way, everything except the page
   with the modified vector table is being verified. A serial communication error on the first page
   would still be discovered as the bootloader returns what it has read. Note that verification of
   the vector page is altered only when immediately done after programming, ie, when using
   avrdude's implicit verify option. If the program flash is read in a separate avrdude call then
   the real contents is returned as it should be. From version 7.5 onwards the `VBL=2` or `3` code
   checks whether the reset vector already points to the bootloader, in which case the patch is not
   executed. Consequently, verification of the page at zero is not altered. This is done to allow
   both un-patched and patched applications be uploaded with a vector bootloader when VBL is set to
   2 or 3.

   `VBL=1` is recommended over `VBL=2` or `3` as the bootloader code to patch the incoming
   application on the device makes more assumptions than avrdude's urclock programmer. Once avrdude
   v7.1 with its urclock programmer is reasonably well distributed, it is likely that options
   `VBL=2` and `VBL=3` are going to be deprecated as patching the application vectors in *every*
   bootloader is not a good use of flash if that an be done by the uploader program. Removing these
   options will simplify the source code `urboot.c`.

   Burning a VBL-enabled urboot onto an MCU should in theory also require the jump to boot section
   be burned to the reset vector. The accompanying `urloader` sketch does that.  However, one will
   get away with not burning the reset jump address into the vector table, if the MCU flash is
   erased otherwise: erased words read `0xffff`, and although this is not an official opcode, it
   behaves as `sbrs r31, 7` (skip one instruction if bit 7 in R31 is set). Any reset to `0x0000` on
   an otherwise erased flash will therefore eventually run into the start of the bootloader.

 - `DUAL=<0|1> [SFMCS=<AtmelPxx|ArduinoPinX> | TEMPLATE=1]`

   `DUAL=1` provides dual-boot functionality: When the boot sector is entered through a
   Watchdog timer (WDT) reset, a dual-boot enabled urboot reads the first page of an attached
   external SPI flash memory. If that is compatible with a `jmp`/`rjmp` vector table as judged by
   the first few bytes, it is assumed that the SPI flash memory contains a new program to be burned
   onto the MCU. As there is no size associated with the copy of the program the maximum number of
   flash pages is being copied onto the MCU right up to START, where the bootloader sits. It then
   runs the new application. This is a crucial stepping stone for "over the air" programming: The
   application program, when run normally, recognises "new program" packets that it gets from
   somewhere (radio, WiFi, magic, ...) and drops them onto the start of external memory. When
   finished, the application triggers a watchdog reset, urboot burns the new program, and the
   application has just reprogrammed itself: Voila! Useful for those wireless projects that are
   installed where they are kind of hard to get to. The default is `DUAL=0`.


   The `SFMCS=<AtmelPxx|ArduinoPinX>` option encodes the pin number of the SPI flash memory chip
   select line: either use a constant such as `AtmelPB0` (port B, pin 0) or, eg, `ArduinoPin8`,
   which uses the Arduino Pin numbers that are available for some boards/MCUs.

   `TEMPLATE=1` creates a template bootloader with different nop opcodes (`mov rN,rN`) where the
   `SFMCS` pin needs to be manipulated. These template bootloaders cannot be used as they are, but
   the accompanying urloader sketch (to burn a template bootloader) allows the user to select the
   CS pin of the flash memory. This will then replace the `mov rN, rN` opcodes with the right
   `sbi`/`cbi` opcodes in the right places on the fly when burning the bootloader.


 - `PROTECTME=<1|0>`

   Setting `PROTECTME=1` prevents the bootloader from overwriting itself. This can only in rare
   circumstances be safely switched off, eg, when no `pgm_write_page()` function is exported, the
   bootloader uses hardware boot section support and the lock bits are set to protect the
   bootloader. As the protection code is only between 4 and 10 bytes and the user might forget
   protecting the hardware-supported boot section through lockbits, this protection can no longer
   be switched off from v7.7 onwards.


 - `PROTECTRESET=<1|0>`

   Setting `PROTECTRESET=1` protects a vector bootloader from overwriting the reset vector.
   Overwriting the reset vector has the potential to "brick" the bootloader. Programming with
   `avrdude -c urclock` looks after the reset vector, so incidents where it can be overwritten are
   rare even without protection: the principal cause of this happening is when the user calls the
   `pgm_write_page(sram, flash)` function from the application *and* overwrites the reset vector at
   flash address zero; another cause can be a user overwriting the bootloader size with a wrong
   number using the extended avrdude option `-xbootsize=<n>` when uploading a file. Setting
   `PROTECTRESET=1` will ensure that a backward `rjmp` from address 0 to the  bootloader will be
   written to flash whenever the user writes the first flash memory page. As an aside, whenever the
   function `pgm_write_page(sram, flash)` is called by the user for `flash=0` then the first two
   bytes of the memory that the pointer sram points to are also altered with the `rjmp` to the
   bootloader. If `PROTECTRESET` is unset, ie, neither 0 or 1, then this option will be set if
   there is still space in the bootloader to accommodate the protection code.

 - `PGMWRITEPAGE=<1|0>`

   `PGMWRITEPAGE=1` makes available  a bootloader function
   ```
     void pgm_write_page(void *sram, progmem_t pgm);
   ```
   to applications. This function writes `SPM_PAGESIZE` bytes from SRAM memory into flash at
   location `pgm`. When the device has more than 64 kB flash, and hence `RAMPZ` exists, then
   `progmem_t` is a type that holds 32-bit addresses, else `progmem_t` is a 16-bit address `(void
   *)`. The caller has to take care that `pgm` is aligned with memory pages of `SPM_PAGESIZE`
   bytes. `sram` is always 16-bit. Normally, application code cannot write to flash, as this has to
   be done from the boot section, so this option provides applications with the opportunity to
   treat unused flash a bit like EEPROM. There are library functions `urstoreWrite()` and
   `urstoreRead()` that make use of this interface. As exporting this function in the bootloader
   comes at a small cost of some 4-6 bytes overhead to call the anyway needed `writebuffer()`
   function, the recommendation is to always  set this option. From a security point of view, there
   are certain risks involved, as this option allows the application to modify itself. If you are
   concerned about this, I recommend using external programming using a programmer instead of using
   any bootloader at all (rendering urboot moot) and/or setting the fuses very restrictively (which
   renders this option obsolete).

   ```
     ((void (*)(void *, progmem_t))((FLASHEND+1UL-4UL)/2))(sram, pgm);
   ```

   Division by 2 reflects the fact that avr-gcc has PC word addresses. `pgm` needs to be a 32-bit
   type if the MCU has more that 64k flash, a 16-bit type otherwise. Here is a full definition of
   the `pgm_write_page()` function to be used in the application sketch (cut and paste or use the
   `ur/libraries/urboot` library):

   ```
   #if FLASHEND > 0xFFFFUL
     typedef const uint_farptr_t progmem_t;
   #else
     typedef const void *progmem_t;
   #endif


   /*
    * Copy a single page from SRAM to page-aligned PROGMEM utilising smr's urboot
    *  - Takes ca 8.7 ms on an 16 MHz ATmega328p
    *  - Remember to flush serial buffers before calling
    *  - The bootloader alters the first bytes of where sram points to if it has
    *    reset protection and Page 0 is overwritten
    *  - See https://gcc.gnu.org/onlinedocs/gcc/AVR-Options.html re EIND, RAMPZ
    */
   void urbootPageWrite(void *sram, progmem_t pgm) {
     cli();
   #if FLASHEND > 0x1FFFFUL        // Set the correct memory segment for indirect jump
     EIND = ((FLASHEND+1UL-4UL)/2) >> 16;
   #endif
     // Call the bootloader routine pgm_write_page(sram, pgm)
     ((void (*)(void *sram, progmem_t pgm))((FLASHEND+1UL-4UL)/2))(sram, pgm);
   #if FLASHEND > 0xFFFFUL
     RAMPZ = 0;
   #endif
   #if FLASHEND > 0x1FFFFUL
     EIND = 0;
   #endif
     sei();
   }
   ```

   This function and other utility functions are available in
   `urboot-utils.cpp` in the directory
   [`urboot-utils`](https://github.com/stefanrueger/urboot/blob/main/urboot-utils)

## Frills

The options below are frills, ie, not really essential for the functionality of the bootloader.

 - `CHIP_ERASE=<0|1>`

   With this option set to 1, an STK_CHIP_ERASE command will erase all flash below the
   bootloader; otherwise, `STK_CHIP_ERASE` commands are ignored. If the `URPROTOCOL` option is set
   then this bootloader will inform `avrdude -c urclock` whether or not it has the ability to
   perform a chip erase, so that avrdude can emulate a chip erase if needed by erasing unset bytes
   in the input file for flash to `0xff`. Avrdude's chip erase emulation is much slower than the
   bootloader's chip erase, as it carries the overhead of sending `0xff` bytes over the serial
   interface. Setting this option will cost around 16-38 bytes code.

 - `RETSWVERS=<0|1>` (return correct software version)

   Option `RETSWVERS=1` creates code to tell the avrdude or otherwise programmer truthfully the
   current software version. The space used for this code is probably not worthwhile. Without it,
   urboot pretends to be on software version `X.X` and hardware version `X`, where `X` is the major
   software version (currently 7). `RETSWVERS=0` saves 28 bytes for `URPROTOCOL=0`. With
   `URPROTOCOL=1` this option has no effect, as the bootloader does not expect, nor respond to,
   `STK_SW_MAJOR/MINOR` requests.

 - `EXITFE=<0|1|2>` (exit on frame errors)

   Option `EXITFE=2` creates code that exits on frame errors in the serial communication. The
   rationale is that frame errors indicate a wrong bit-rate, which might be owing to an external
   device wanting to communicate with the application. Setting EXITFE to 1 cares about frame errors
   in the sense that the watchdog timer is only reset on bytes that have no frame error. Not
   resetting the watchdog timer hastens the bootloader's reset to the application. Setting this
   option to 0 ignores frame errors, which ultimately saves a few bytes of code and delays the
   inevitable exit from the bootloader that will be caused anyway through protocol errors.

 - `QEXITERR=<0|1>` (quick exit on error)

   Option QEXITERR=1 creates code so that when there is a protocol error during the bootloader
   communication, the application is started within 16 ms as opposed to the default WDT timeout. If
   QEXITERR is set, then other exit on error situations also make use of this code for quick exit.

 - `BLINK=<0|1> [LED=<pin> [LEDPOLARITY=<1|-1>] | TEMPLATE=1]`

   Light effects: `BLINK=1` toggles the activity LED during serial data I/O. The Option
   `LED=<pin>` sets the LED pin in terms of Atmel port and bit number as in, eg, `AtmelPB1` or
   `ArduinoPin9`. `LEDPOLARITY` specifies whether the LED is assumed to be low active
   (`LEDPOLARITY=-1`) or high active otherwise (`LEDPOLARITY` undefined or set to 1). Using
   `TEMPLATE=1` is an alternative to specifying the LED pin at compile time. This creates a
   template bootloader with different nop opcodes (`mov rN,rN`) instead of code that operates the
   LED. The urloader sketch to burn a template bootloader lets the user select/change the LED pin
   and polarity. This will then replace the `mov rN, rN` opcodes with the right `sbi`/`cbi` opcodes
   in the right places on the fly when burning the bootloader.

 - `QEXITEND=<0|1>` (quick exit on end)

   Option `QEXITEND=1` creates code so that when finished programming the application is started
   within 16 ms as opposed to the default WDT timeout.


 - `FRILLS=<level>`

   Shortcut for setting defaults for some of the options; the default level is 0; if the below
   options are undefined they will be defined as
   | Option | default |
   | :--    | :-- |
   |`CHIP_ERASE` | `FRILLS >= 7` |
   |`RETSWVERS`  | `FRILLS >= 6` |
   |`QEXITEND`   | `FRILLS >= 5` |
   |`QEXITERR`   | `FRILLS >= 4` |
   |`EXITFE`     | `FRILLS >= 3? 2: FRILLS >= 2` |
   |`BLINK`      | `FRILLS >= 1` |


 - `AUTOFRILLS=<level-list>`

   In absence of `FRILLS=<level>`, this option asks `urboot-gcc` to try out the first `<level>`
   of the comma-separated `<level-list>` of `FRILLS` levels, to then determine the
   the highest `FRILLS` level of the list that fits into the usage occupied by the first level.
   For example `AUTOFRILLS=0,7,6,1,2,3,4` first determines the space usage of a bootloader with
   `FRILLS=0` and then figures out which of the other `FRILLS` levels also fit into that space.
   The finally compiled bootloader uses the biggest of these. This option is only seen by
   the `avr-gcc` wrapper `urboot-gcc` and not passed on to the actual compiler.


## Debug options

 - `DEBUG_FREQ=<fq> [FREQ_PIN=<pin> [FREQ_POLARITY=<1|-1>]] [EXACT_DF=<0|1>] [DEBUG_CYCLES=<n>]`

   If `DEBUG_FREQ` is set, the bootloader initially swings a square wave of `DEBUG_CYCLES` periods
   (default 5) on pin `FREQ_PIN` with a frequency of `fq` Hz, where `fq` can be a real number.
   `FREQ_PIN` can be the same as the LED pin, and in fact defaults to the `LED` pin if the latter
   is defined and the former is not. This option is useful of one wants to measure the frequency
   `F_CPU` of the MCU on the FREQ_PIN (using another MCU or a scope). This is not meant for
   production, only for debugging. 50 Hz is a good frequency as this delays bootloader comms by
   only 100 ms. Any lower frequency might be too much delay for default synchronisation with
   avrdude, though `avrdude -c urclock` has a `-xdelay=<ms>` parameter that can be used to
   compensate for the extra startup time of this option. `FREQ_POLARITY=1` starts the square wave
   with a high level after reset, and `-1` with a low level after reset. After the `n` periods the
   signal on `FREQ_PIN` stays low, irrespective of `FREQ_POLARITY`. The duration of one period of
   the square wave might be too short by up to 5 clock cycles, which is normally of no concern. If
   precision of this debug square wave is of importance, then `EXACT_DF` can be set to `1`, in
   which case the quantisation error of one period is less than 1 clock cycle at the expense of up
   to 4 bytes extra code.

 - `FLASHWRAPS=<1|0>`

   This option informs `urboot.c` whether or not flash memory wraps around. When the bootloader
   finally starts the application it will jump to a location in the vector table (either the reset
   vector or, if it is a vector bootloader, to the dedicated vector). If flash memory wraps around,
   then the bootloader can execute a short 2-byte `rjmp` over the end of flash to the vector table
   rather than a absolute `jmp` thus saving 2 bytes of code. The default is `FLASHWRAPS=1` iff the
   size of flash memory is a power of 2, but can be overwritten should this assumption be wrong for
   a particular MCU. This option has no effect in MCUs with up to 8k flash memory as they always
   use `rjmp`.


## `urboot-gcc` wrapper

`urboot-gcc` is an `avr-gcc` wrapper for urboot bootloader compilation; it computes the correct start
address for the bootloader and the correct `rjmp` opcode to the `pgm_write_page(sram, flash)` function
in the bootloader and injects both as `-DSTART=<addr>` and `-DRJMPWP=<opcode>`, respectively, when
calling `avr-gcc`.

 - `-DSTART=<address>`

   There are several places in the code of the bootloader where it needs to know its eventual place
   in flash memory, eg, for code to protect overwriting the bootloader itself, for patching
   applications in vector bootloaders (see below) and for copying the right amount of external SPI
   flash memory to microprocessor flash in dual boot. Bootloaders normally sit in high flash memory
   (except in AVR parts with `UPDI` interface). The `-DSTART=0x...UL` option informs `urboot.c`
   about the byte address where the linker will be told to put the bootloader. Coincidently,
   `FLASHEND-START+1UL` is the size of the bootloader. `START` is actually also the address of the
   main function but using the symbol main instead of the START compile-time constant enlarges the
   code without need. The accompanying perl script `urboot-gcc` acts as `avr-gcc` wrapper; it knows
   a lot of MCUs, can compute the "right" START and auto-magically runs the avr-gcc appropriately
   with the correct `-DSTART-...` option.


 - `-DRJMPWP=0xCxxx`

   The compiler option `-DRJMPWP=0xCxxx` stores the `rjmp pgm_write_page` opcode just below the two
   version bytes, so that this function can be called at the address `FLASHEND+1UL-4`. The
   accompanying perl compiler wrapper `urboot-gcc` computes `RJMPWP` automatically (iff the
   `pgm_write_page` and `main` symbols both exist in the .elf file) and passes this option on to
   `avr-gcc`. In case that `PGMWRITEPAGE=0` or that the `-DRJMPWP=..`. opcode has not been supplied
   to `urboot.c` by `urboot-gcc` or the user then the `ret` opcode will be stored instead of the
   `rjmp`, so that calling this interface returns empty-handed. Use as follows in your application:


```
$ urboot-gcc -\?
Syntax: urboot-gcc [-toolchain=<v>] {avr-gcc opts} urboot.c
Function: compiles urboot with the right addresses until compilation converges
Option:
  -toolchain=<v> uses ./avr-toolchain/<v>/bin
                 or avr-gcc version in $PATH on -toolchain=system or -toolchain=
                 or GCCROOT environment variable
                 or ./avr-toolchain/4.8.1/bin
                 (show available list with -toolchain=?)
```

Finding the right address for the bootloader is a two-step, sometimes three-step process: On first
compilation a START address is used that works with the target mcu, but will be too low. Once the
bootloader is complied and linked, urboot-gcc compiles again using size info and urboot version bits
from the newly created .elf file. The bootloader placement depends on the type of bootloader
support that the mcu offers, and on whether the bootloader is a vector bootloader (VBL - as encoded
in the version bits), which requires no bootloader support by the mcu. For example, you might have
compiled a bootloader for the ATmega1284P with VBL support that fits into 512 bytes. This will be
placed in the topmost 512 bytes of the flash space. However, if compiled as normal bootloader
without VBL support, it must be placed in the top 1024 bytes, because that's the smallest boot
section that this mcu offers. urboot-gcc aligns the bootloader with the correct SPM page size and/or
boot section size. In a slight complication of things, it may happen that the size of the
bootloader changes after the second step. This is because the bootloader address is used as
compile-time constant, which when changed, can be used in a different way by the code or optimiser.
This is normally not a problem, but when the new size ticks over the boot section or page size
limit, this will trigger a linker error after the second step. To resolve this, urboot-gcc must
compile and link a third time, this time adding a few bytes of slack to the size. In case you
requested the export of pgm_write_page() in the bootloader source, urboot-gcc computes the
appropriate 2-byte rjmp call and places it 4 bytes under the end of the bootloader, ie, at the end
of flash just before the two version bytes.

In addition to figuring out the correct start address of the bootloader and the correct rjmp to the
pgm_write_page(sram, flash) bootloader routine, urboot-gcc also runs avr-gcc one more final time
with -D_urboot_AVAILABLE=<num_bytes> to indicate how many free bytes are available for desirable,
but not essential features. Currently, urboot.c uses this feature for the generation of reset
vector protection code for vector bootloaders.

## hexls

`hexls` is a perl script that lists urboot bootloader hex files with size, version, features and
name.

```
$ hexls -\?
Syntax: hexls {<bootloader>.hex}
Function: list urboot bootloader hex files with size, usage, version, features and name
Options:
  -version     print version (1.05) and exit
  -md=url      print neat markdown table with header (files use url)
  -check       check rjmp op code at end (if pgm_page_write-enabled and .lst file there)
  -sort        sort rows according to use, size, version, features, name
  -use         print number of bytes occupied by bootloader and exit
```

Example:
```
$ hexls -sort -md *.hex
```

|Size|Usage|Version|Features|Hex file|
|:-:|:-:|:-:|:-:|:--|
|252|256|u7.7|`w-u-jpr--`|atmega328p_min.hex|
|254|256|u7.7|`w-u-jpra-`|atmega328p_amin.hex|
|358|384|u7.7|`weu-jPr-c`|atmega328p_ur.hex|
|374|384|u7.7|`weu-jPrac`|atmega328p_autobaud_ur.hex|
|376|384|u7.7|`weu-jPr-c`|atmega328p_8000khz_swio_ur.hex|
|460|512|u7.7|`wes-hpr-c`|atmega328p.hex|
|508|512|u7.7|`weudhprac`|atmega328p_adur.hex|

- **Size:** Bootloader code size including small table at top end
- **Usage:** How many bytes of flash are needed, ie, HW boot section or a multiple of the page size
- **Version:** For example, u7.6 is an urboot version, o5.2 is an optiboot version
- **Features:**
  + `w` bootloader provides `pgm_write_page(sram, flash)` for the application at `FLASHEND-4+1`
  + `e` EEPROM read/write support
  + `u` uses urprotocol requiring `avrdude -c urclock` for programming
  + `s` uses skeleton of STK500v1 protocol (deprecated); `-c urclock` and `-c arduino` both work
  + `d` dual boot (over-the-air programming from external SPI flash)
  + `h` hardware boot section: make sure fuses are set for reset to jump to boot section
  + `j` vector bootloader: applications *need to be patched externally*, eg, using `avrdude -c urclock`
  + `p` bootloader protects itself from being overwritten
  + `P` vector bootloader only: protects itself and reset vector from being overwritten
  + `r` preserves reset flags for the application in the register R2
  + `a` autobaud detection (f_cpu/8n using discrete divisors, n = 1, 2, ..., 256)
  + `c` bootloader provides chip erase functionality (recommended for large MCUs)
  + `-` corresponding feature not present

## Production options

 - `TOOLVER=<4.8.1|4.9.2|5.4.0|7.4.0|system>`

   This selects the version of the avr toolchain that should be used for compilation. Older toolchains
   tend produce slightly smaller code. The `Makefile` selects good versions automatically depending on
   the chosen options and the part.

 - `GCCROOT=<dir>`

   Alternatively, `GCCROOT`, the directories with the binaries of the toolchain, can be specified
   directly; the default is `./avr-toolchain/$(TOOLVER)/bin/`.

   Below is a comparison table that has been compiled from different toolchain versions. Urboot is the
   one where the `Makefile` automatically selects between the available ones in this repository, and
   with rare exceptions the best choice. Later versions tend to have larger code still.

   Features | Name | urboot | 4.8.1 | 5.4.0 | 7.4.0
   --: | :-- | --: | --:  | --: | --:
   `w-u-jpr--` | attiny2313_min.hex | 224 | 224 | 222 | 244
   `w-u-hpr--` | atmega8_min.hex | 244 | 244 | 246 | 254
   `w-u-jpr--` | atmega32_min.hex | 244 | 244 | 246 | 254
   `w-u-jpr--` | atmega1284p_min.hex | 248 | 248 | 248 | 248
   `w-u-jpr--` | atmega2560_min.hex | 248 | 248 | 248 | 248
   `w-u-jpr--` | atmega644p_min.hex | 250 | 250 | 250 | 250
   `w-u-jpr--` | atmega88_min.hex | 252 | 252 | 252 | 252
   `w-u-jpr--` | attiny167_min.hex | 252 | 252 | 252 | 252
   `w-u-jpra-` | atmega328p_amin.hex | 254 | 254 | 254 | 254
   `w-u-jpr--` | attiny84_min.hex | 256 | 256 | 276 | 284
   `weu-jpr--` | atmega88_ur.hex | 310 | 310 | 310 | 310
   `weu-jpr--` | atmega8_ur.hex | 314 | 314 | 314 | 314
   `weu-jPr-c` | atmega32_ur.hex | 354 | 354 | 354 | 354
   `weu-jPr-c` | atmega328p_ur.hex | 358 | 358 | 358 | 358
   `weu-jPr-c` | attiny167_ur.hex | 372 | 372 | 372 | 372
   `weu-jpr-c` | luminet_baud9600_ur.hex | 372 | 372 | 372 | 372
   `weu-jPrac` | atmega328p_autobaud_ur.hex | 374 | 374 | 374 | 374
   `weu-jPr-c` | atmega328p_8000khz_swio_ur.hex | 376 | 376 | 376 | 376
   `weu-jPr-c` | attiny85_ur.hex | 376 | 376 | 376 | 376
   `weu-jPr-c` | atmega328p_7875khz_swio_ur.hex | 378 | 378 | 378 | 378
   `weu-jPr-c` | atmega2560_ur.hex | 386 | 386 | 386 | 386
   `wes-hpr-c` | atmega8.hex | 432 | 432 | 438 | 456
   `wes-hpr-c` | atmega32.hex | 434 | 434 | 440 | 458
   `wes-hpr-c` | pro_20mhz.hex | 440 | 440 | 440 | 456
   `wes-hpr-c` | atmega328p.hex | 444 | 444 | 444 | 460
   `wes-hpr-c` | pro_8mhz.hex | 462 | 462 | 462 | 478
   `weudhpr-c` | urclock_cs8_dur.hex | 484 | 484 | 484 | 484
   `weudhpr-c` | atmega328p_dur.hex | 492 | 492 | 492 | 492
   `weudjpr--` | atmega2560_dur.hex | 496 | 496 | 496 | 496
   `weudjpr-c` | attiny167_dur.hex | 500 | 500 | 500 | 500
   `wes-vpr--` | atmega1284p_v2.hex | 504 | 504 | 504 | 522
   `weudhprac` | atmega328p_adur.hex | 508 | 508 | 508 | 508


## Examples

### Generating bootloaders with `make`

```
make MCU=attiny2313 URPROTOCOL=1 EEPROM=0 VBL=1 FRILLS=2 VBL_VECT_NUM=EEPROM_READY_vect_num NAME=attiny2313_min
make MCU=atmega328p URPROTOCOL=1 EEPROM=0 VBL=1 FRILLS=6 NAME=atmega328p_min
make MCU=atmega328p AUTOBAUD=1 URPROTOCOL=1 EEPROM=0 VBL=1 FRILLS=3 NAME=atmega328p_amin
make MCU=atmega328p AUTOBAUD=1 URPROTOCOL=1 VBL=1 FRILLS=7 NAME=atmega328p_autobaud_ur
make MCU=atmega328p URPROTOCOL=1 VBL=1 FRILLS=7 NAME=atmega328p_ur
make MCU=atmega328p F_CPU=8000000L URPROTOCOL=1 SWIO=1 RX=ArduinoPin0 TX=ArduinoPin1 VBL=1 FRILLS=7 NAME=atmega328p_8000khz_swio_ur
make MCU=atmega328p AUTOBAUD=1 URPROTOCOL=1 VBL=1 LED=AtmelPB5 FRILLS=7 NAME=promini_led13_ur
make MCU=atmega168 F_CPU=8000000L URPROTOCOL=1 SWIO=1 RX=ArduinoPin0 TX=ArduinoPin1 VBL=1 FRILLS=7 NAME=lilypad_ur
make MCU=atmega2560 URPROTOCOL=1 EEPROM=0 VBL=1 FRILLS=3 NAME=atmega2560_min
make MCU=atmega2560 URPROTOCOL=1 VBL=1 FRILLS=7 NAME=atmega2560_ur
```

### Generating bootloaders with `urboot-gcc`

```
./urboot-gcc -toolchain=4.8.1 -g -Wundef -Wall -Os -fno-split-wide-types -mrelax -mmcu=attiny2313 -DF_CPU=16000000L -Wno-clobbered -DWDTO=1S -DDUAL=0 -DEEPROM=0 -DURPROTOCOL=1 -DFRILLS=2 -DVBL=1 -DVBL_VECT_NUM=EEPROM_READY_vect_num -Wl,--relax -nostartfiles -nostdlib -o attiny2313_min.elf urboot.c
./avr-toolchain/4.8.1/bin/avr-objcopy -j .text -j .data -j .version --set-section-flags .version=alloc,load -O ihex attiny2313_min.elf attiny2313_min.hex
./avr-toolchain/4.8.1/bin/avr-objdump -h -S attiny2313_min.elf > attiny2313_min.lst

./urboot-gcc -toolchain=5.4.0 -g -Wundef -Wall -Os -fno-split-wide-types -mrelax -mmcu=atmega328p -DF_CPU=16000000L -Wno-clobbered -DWDTO=1S -DAUTOBAUD=1 -DDUAL=0 -DEEPROM=1 -DURPROTOCOL=1 -DFRILLS=7 -DVBL=1 -Wl,--relax -nostartfiles -nostdlib -o atmega328p_autobaud_ur.elf urboot.c
./avr-toolchain/5.4.0/bin/avr-objcopy -j .text -j .data -j .version --set-section-flags .version=alloc,load -O ihex atmega328p_autobaud_ur.elf atmega328p_autobaud_ur.hex
./avr-toolchain/5.4.0/bin/avr-objdump -h -S atmega328p_autobaud_ur.elf > atmega328p_autobaud_ur.lst
```

### Generating bootloaders with `avr-gcc`

```
./avr-toolchain/4.8.1/bin/avr-gcc -DSTART=0x720UL -DRJMPWP=0xcfe3 -Wl,--section-start=.text=0x720 -Wl,--section-start=.version=0x7fa -g -Wundef -Wall -Os -fno-split-wide-types -mrelax -mmcu=attiny2313 -DF_CPU=16000000L -Wno-clobbered -DWDTO=1S -DDUAL=0 -DEEPROM=0 -DURPROTOCOL=1 -DFRILLS=2 -DVBL=1 -DVBL_VECT_NUM=EEPROM_READY_vect_num -Wl,--relax -nostartfiles -nostdlib -o attiny2313_min.elf urboot.c
./avr-toolchain/4.8.1/bin/avr-objcopy -j .text -j .data -j .version --set-section-flags .version=alloc,load -O ihex attiny2313_min.elf attiny2313_min.hex
./avr-toolchain/4.8.1/bin/avr-objdump -h -S attiny2313_min.elf > attiny2313_min.lst

./avr-toolchain/5.4.0/bin/avr-gcc -DSTART=0x3fe00UL -DRJMPWP=0xcf98 -Wl,--section-start=.text=0x3fe00 -Wl,--section-start=.version=0x3fffa -D_urboot_AVAILABLE=144 -g -Wundef -Wall -Os -fno-split-wide-types -mrelax -mmcu=atmega2560 -DF_CPU=16000000L -Wno-clobbered -DWDTO=1S -DDUAL=0 -DEEPROM=1 -DURPROTOCOL=1 -DFRILLS=7 -DVBL=1 -Wl,--relax -nostartfiles -nostdlib -o atmega2560_16mhz_115200bps_ev1f7_wdto1s_ur.elf urboot.c
./avr-toolchain/5.4.0/bin/avr-objcopy -j .text -j .data -j .version --set-section-flags .version=alloc,load -O ihex atmega2560_16mhz_115200bps_ev1f7_wdto1s_ur.elf atmega2560_16mhz_115200bps_ev1f7_wdto1s_ur.hex
./avr-toolchain/5.4.0/bin/avr-objdump -h -S atmega2560_16mhz_115200bps_ev1f7_wdto1s_ur.elf > atmega2560_16mhz_115200bps_ev1f7_wdto1s_ur.lst
```

