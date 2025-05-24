# Urboot
**Feature-rich small AVR bootloader using
[urprotocol](https://github.com/stefanrueger/urboot/blob/main/urprotocol.md)**

 - Tight code: most bootloaders fit into
     + 256 bytes albeit normally without EEPROM read/write support
     + 384 bytes with EEPROM read/write support
     + 512 bytes with dual-boot, EEPROM read/write support and Chip Erase capability
 - Highly modular feature set:
     + Dedicated protocol between programmer and bootloader (urprotocol)
     + EEPROM read/write support
     + Vector bootloader for devices without a boot section or to save space on devices with one
     + Software UART: no hardware UART required, but also useful when the chip's CPU frequency and
       the bootloader's desired baud rate are incompatible (eg, 8 MHz and 115,200 baud)
     + Subroutine `pgm_write_page(sram, progmem)` for applications so they can change themselves:
       on many MCUs the SPM write flash only works when called from the bootloader section (default on)
     + Dual programming from external SPI flash memory for over-the-air programming
     + [Template](https://github.com/stefanrueger/urboot/blob/main/docs/makeoptions.md#template_sfm)
       bootloader with nops that can be replaced with opcodes to set/clear the right
       [LED](https://github.com/stefanrueger/urboot/blob/main/docs/makeoptions.md#template_led) or
       [CS pins](https://github.com/stefanrueger/urboot/blob/main/docs/makeoptions.md#template_sfm)
     + Saves the reset flags in R2 for inspection by the application via the .init0 section (default on)
     + Bootloader protects itself from overwriting (default on)
     + Automatic host baud rate detection
     + Chip erase in bootloader (faster than -c urclock emulation)
     + Update flash feature that tries to wear out flash less (and is faster)
 - Avrdude (from v7.1 onwards) supports urboot bootloaders with `-c urclock`

Urboot bootloaders can be hundreds of bytes shorter than
[optiboot](https://github.com/Optiboot/optiboot) with the same
functionality, see
[below](https://github.com/stefanrueger/urboot/blob/main/README.md#comparison).

<p id="background"></p>

**Background.** More information on how bootloaders work with tips, tricks
and limitations [here](https://github.com/stefanrueger/urboot/blob/main/docs/background.md).

<p id="compiling"></p>

**Compiling an urboot bootloader.** The small sizes of `urboot` bootloaders depend on many a trick
of the trade, most of which only work with the avr-gcc toolchain used for compilation. It turns out
that two older avr-toolchains, version 4.8.1 and 5.4.0, produce pretty tight code. Linux binaries of
these are located in the `src/avr-toolchain` directory. This project uses helper programs in perl:
an avr-gcc wrapper `urboot-gcc` and `hexls` that shows stats of the produced `.hex` files. So, all
things considered, compiling this project is easiest on Linux with perl installed (and some
required perl libraries, too; install them with `cpan`). All going well `make all` should produce
`.hex`, `.elf` and `.lst` files of a number of bootloaders. The created .hex files should coincide
with the corresponding ones in the directory
[`src/all`](https://github.com/stefanrueger/urboot/tree/main/src/all). Once all these hurdles are
taken, it is easy to create own bootloaders with commands such as
```
 $ make MCU=atmega328p AUTOBAUD=1 VBL=1 AUTOFRILLS=5..10 NAME=atmega328p_a
```

Alternatively, the Dockerfile may be built and used to build binaries on other systems:
```
 $ docker run --platform linux/amd64 -v "$(pwd)/src":/src --rm -it $(docker build -q .) \
   MCU=atmega328p AUTOBAUD=1 VBL=1 AUTOFRILLS=5..10 NAME=atmega328p_a
```
More detailed information here: [`make` options](https://github.com/stefanrueger/urboot/blob/main/docs/makeoptions.md)

<p id="pre-compiled"></p>

**Pre-compiled bootloaders.** If compiling bootloaders is not feasible, feel free to try out one of
the 1,283,806 *different* pre-compiled bootloaders in the directory tree for [`MCU
parts`](https://github.com/stefanrueger/urboot.hex/tree/main/mcus/). You can find bootloaders for
popular boards in a corresponding directory for
[boards](https://github.com/stefanrueger/urboot.hex/tree/main/boards/), for example, autobaud
bootloaders for the venerable ATmega328P based
[Uno](https://github.com/stefanrueger/urboot.hex/blob/main/boards/uno/atmega328p/watchdog_1_s/autobaud//uart0_rxd0_txd1/led+b5/README.md)
with a LED on PB5, the [Arduino Pro
Mini](https://github.com/stefanrueger/urboot.hex/tree/main/boards/promini/atmega328p/watchdog_1_s/autobaud/uart0_rxd0_txd1/led+b5/README.md)
(same bootloaders), the
[Jeenode](https://github.com/stefanrueger/urboot.hex/tree/main/boards/jeenode/atmega328p/watchdog_1_s/autobaud/uart0_rxd0_txd1/led-b1/README.md)
with a low-active LED on PB1, the ATmega1284P based [Moteino
Mega](https://github.com/stefanrueger/urboot.hex/tree/main/boards/moteinomega/atmega1284p/watchdog_1_s/autobaud/uart0_rxd0_txd1/led+d7/README.md)
and the ATmega2560 [Mega R3
board](https://github.com/stefanrueger/urboot.hex/tree/main/boards/mega-r3/atmega2560/watchdog_1_s/autobaud/uart0_rxe0_txe1/led+b7/README.md);
or, eg, some 16 MHz and 115,200 baud bootloaders for the [ATtiny167 based Digispark
Pro](https://github.com/stefanrueger/urboot.hex/tree/main/boards/digispark-pro/attiny167/watchdog_1_s/external_oscillator_x/16m000000_hz/+115k2_baud/uart0_rxa0_txa1/led+b1/README.md),
the [ATtiny85 based
Disgispark](https://github.com/stefanrueger/urboot.hex/tree/main/boards/digispark/attiny85/watchdog_1_s/external_oscillator_x/16m000000_hz/+115k2_baud/swio_rxb4_txb3/led+b1/README.md)
and the [ATtiny84 based
Luminet](https://github.com/stefanrueger/urboot.hex/tree/main/boards/luminet/attiny84/watchdog_1_s/external_oscillator_x/16m000000_hz/+115k2_baud/swio_rxa3_txa2/led+a4/README.md).
Finally, there are pre-compiled urboot bootloaders for popular Arduino cores in the
[cores](https://github.com/stefanrueger/urboot.hex/tree/main/cores) directory of the
[urboot.hex](https://github.com/stefanrueger/urboot.hex) repository.

<p id="how-to-select"></p>

**How to select a bootloader.** Tips, tricks and some background
[here](https://github.com/stefanrueger/urboot/blob/main/docs/howtoselect.md).

<p id="usage"></p>

**Usage.** As with all bootloaders, one needs to first program them onto the board/chip using a
(physical) programmer and an uploader program, eg, through

```
avrdude -c usbasp -p m328p -U flash:w:bootloader.hex:i
```

If the part does *not* have hardware support for bootloaders (eg,
`ATtiny167`), then this is all that is needed. However, if your part *has*
hardware support for bootloaders (eg, `ATmega328p`), then ***particular
attention*** is needed as to whether the bootloader
 - Is a vector bootloader **(`j`),** in which case the fuses need to be programmed so that on reset
   the MCU jumps to the reset vector at address 0. Ensure the lock bits do not preclude writing to
   any of the boot sections (otherwise the space freed up by vector bootloaders cannot be used).
 - Assumes hardware support **(`h`)** and sits in a dedicated HW boot section, in which case the
   fuses need to be set to ensure that on reset the MCU jumps to the correct bootloader start;
   the boot section size fuse bits need to match the actual bootloader size (consult the data sheet
   of the part); the lock bits should protect the boot section from being written to.

| Bootloader | `FUSE_BOOTRST` | `FUSE_BOOTSZ` | Lock bits for boot section |
| --: | --: | --: | --: |
| `j` | Reset to memory start | Don't care | Read/write to everywhere|
| `h` | Reset to boot section | Match with usage in **words** | Protect boot section |

Strictly speaking vector bootloaders also need a `jmp` or `rjmp` from the reset vector at address 0
to the bootloader. However, if the chip was erased before putting the bootloader on, then this is
not necessary: Erased words read `0xffff`, and although not an official opcode, behave as `sbrs
r31,7` (skip one instruction if bit 7 in R31 is set). A reset to address 0 on an otherwise erased
flash will therefore eventually run into the start of the bootloader. Uploading the first
application with `avrdude -c urclock` will set the reset vector correctly to jump to the
bootloader.

Some parts also need the `SELFPRGEN` fuse set allowing the vital `spm` opcode (store to program
memory) to be executed without which bootloaders cannot write to flash.

Once the board has its bootloader the board can be directly connected to and programmed from the
host/laptop/PC (without a physical programmer), eg, through
```
 $ avrdude -c urclock -p m328p -b 115200 -P /dev/ttyUSB0 -U flash:w:mysketch.hex:i
 $ avrdude -c urclock -p m328p -b 115200 -P /dev/ttyUSB0 -U eeprom:r:backup.eep:I
 $ avrdude -c urclock -p m328p -b 115200 -P /dev/ttyUSB0 -t
```

Voila!

**Fun facts.** `avrdude -c urclock` can keep bootloaders in terminal mode `-t` without the
bootloader resetting itself through the watchdog timer. `avrdude -c urclock` keeps metadata of the
uploaded program, which are displayed with `avrdude -c urclock -xshowall`. Urboot bootloaders tell
avrdude *exactly* which part they were compiled for; hence, `-c urclock` is the *only* programmer
for which the part does not need specifying via `-p`.

<p id="trouble-shooting"></p>

**Trouble shooting.** So, you have selected the right bootloader for your
board and followed above but programming does not work as expected?

 - Carefully double check all fuse settings for your part; for example, if the bootloader requires
   hardware support then the bootloader size as given by command line tools or `hexls` will be in
   *bytes* but the data sheets often specify `BOOTSZ` in *words* (note the factor of 2).
 - Check the lock settings for your part; for example, if the bootloader is a vector bootloader
   then the lock bits should enable to write to everywhere, even the hardware bootloader section.
   Urboot bootloaders are sometimes smaller than the smallest hardware boot sections and they
   protect themselves from being overwritten; if the hardware lock for a large hardware
   boot section is enabled then this space cannot be used for applications.
 - Check the logical connections. If using a separate USB/serial converter check whether the
   correct I/O pins are hooked up: The **`RX`** pin that the bootloader was compiled with
   (typically, the `RX` pin of the processor's USART) needs to be connected to the **`TX`** pin
   (not the `RX` pin) of the USB/serial converter chip and vice versa. If the board has a
   USB/serial converter, check to which processor pins it is connected and verify these are the
   RX/TX pins that the bootloader was compiled with.
 - Check the electrical connections. Some USB/serial converters require a hardware pullup on the
   `TX` side of the board. Some boards run at a different voltage than 5V USB; they may need their
   own power supply (in which case connect GND, but do *not* connect the different Vcc); they  may
   need voltage level shifters.
 - Check the mechanical connections. Do the contacts fit tight?
 - Reset the board manually. Some boards have a hardware mechanism that allows avrdude to reset the
   board by plucking the RTS or the DTR line for a short while. Avrdude does both before starting
   communication, but if the board lacks the hardware for that, it needs to be reset manually
   before programming: *First* press the reset button of the board and release it; *then* start
   avrdude by pressing enter of the previously typed command line within the `WDTO` time (typically
   one second).
 - Ensure neither the actual F_CPU nor the used baud rate deviates by more than 1%
    + Does the bootloader match the actual F_CPU frequency? Check the crystal on the board and/or
      the board documentation. Has the `CKDIV8` fuse been programmed? Then the MCU runs slower by a
      factor of 8.
    + Does the `avrdude` baud rate match the bootloader's? Try an eighth of the nominal bootloader
      baud rate in case `CKDIV8` is programmed. Autobaud bootloaders only recognise 256 different
      baud rates: F_CPU/8n where n is 1..256; is the used baud rate close to an auto-detectable one?
    + Measure the F_CPU of the board, eg, by  measuring the blink frequency of the `blink.hex`
      sketch (uploaded with a physical programmer)
 - Swap out the USB/serial cable if there is no USB/serial chip on board


<p id="comparison"></p>

**Comparison.** The table below lists a sample of `urboot` bootloaders and their features alongside
vanilla optiboot and its larger bigboot variant for EEPROM access. They are all for a 16 MHz MCU
and 115,200 baud serial communication speed, except for the autobaud ones, which should work
for all `F_CPU` and a range of associated baudrates.

|Size|Usage|Version|Features|Hex file|
|:-:|:-:|:-:|:-:|:--|
|190|192|u8.0|`----jpr--`|[attiny2313_min.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/src/all/attiny2313_min.hex)|
|248|256|u8.0|`w---jPr--`|[urboot_m328p_1s_x16m0_115k2_uart0_rxd0_txd1_led+b5_pr.hex](https://raw.githubusercontent.com/stefanrueger/urboot.hex/main/mcus/atmega328p/watchdog_1_s/external_oscillator_x/16m000000_hz/+115k2_baud/uart0_rxd0_txd1/led+b5/urboot_m328p_1s_x16m0_115k2_uart0_rxd0_txd1_led+b5_pr.hex)|
|256|256|u8.0|`we--jpr--`|[attiny2313_emin.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/src/all/attiny2313_emin.hex)|
|384|384|u8.0|`weU-jPrac`|[urboot_m328p_1s_autobaud_uart0_rxd0_txd1_led+b5_pr_ee_ce.hex](https://raw.githubusercontent.com/stefanrueger/urboot.hex/main/mcus/atmega328p/watchdog_1_s/autobaud/uart0_rxd0_txd1/led+b5/urboot_m328p_1s_autobaud_uart0_rxd0_txd1_led+b5_pr_ee_ce.hex)|
|454|512|u7.7|`wes-hprac`|[urboot_m328p_1s_autobaud_uart0_rxd0_txd1_led+b5_ee_ce_hw_stk500.hex](https://raw.githubusercontent.com/stefanrueger/urboot.hex/main/u7.7/mcus/atmega328p/watchdog_1_s/autobaud/uart0_rxd0_txd1/led+b5/urboot_m328p_1s_autobaud_uart0_rxd0_txd1_led+b5_ee_ce_hw_stk500.hex)|
|474|512|o8.3|`--s-h-r--`|[optiboot_atmega328.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/src/all/optiboot_atmega328.hex) from the optiboot project|
|504|512|u8.0|`weUdhprac`|[urboot_m328p_1s_autobaud_uart0_rxd0_txd1_led+d5_csb0_dual_ee_ce_hw.hex](https://raw.githubusercontent.com/stefanrueger/urboot.hex/main/boards/urclockusb/atmega328p/watchdog_1_s/autobaud/uart0_rxd0_txd1/led+d5_csb0_dual/urboot_m328p_1s_autobaud_uart0_rxd0_txd1_led+d5_csb0_dual_ee_ce_hw.hex)|
|510|512|u8.1|`weUdhprac`|[atmega328p_ad.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/src/all/atmega328p_ad.hex)|
|710|1024|o8.3|`-es-h-r--`|[bigboot_328.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/src/all/bigboot_328.hex) from the optiboot project|

- **Size:** Bootloader code size including small table at top end
- **Usage:** How many bytes of flash are needed, ie, HW boot section or a multiple of the page size
- **Version:** For example, u7.6 is an urboot version, o5.2 is an optiboot version
- **Features:**
  + `w` bootloader provides `pgm_write_page(sram, flash)` for the application at `FLASHEND-4+1`
  + `e` EEPROM read/write support
  + `U` checks whether flash pages need writing before doing so
  + `s` uses skeleton of STK500v1 protocol (only u7.7); `-c urclock` and `-c arduino` both OK
  + `d` dual boot (over-the-air programming from external SPI flash)
  + `h` hardware boot section: make sure fuses are set for reset to jump to boot section
  + `j` vector bootloader: applications *need to be patched externally*, eg, using `avrdude -c urclock`
  + `p` bootloader protects itself from being overwritten
  + `P` vector bootloader only: protects itself and reset vector from being overwritten
  + `r` preserves reset flags for the application in the register R2
  + `a` autobaud detection (f_cpu/8n using discrete divisors, n = 1, 2, ..., 256)
  + `c` bootloader provides chip erase functionality (only recommended for large MCUs)
  + `-` corresponding feature not present

The naming convention for the `.hex` files is given in the [larger comparison table covering a range of MCUs](https://github.com/stefanrueger/urboot/blob/main/docs/comparison.md).
