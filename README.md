# Urboot
**Feature-rich small AVR bootloader using
[urprotocol](https://github.com/stefanrueger/urboot/blob/main/urprotocol.md)**

 - Tight code: most bootloaders fit into
     + 256 bytes albeit without EEPROM read/write support
     + 384 bytes with EEPROM read/write support
     + 512 bytes with dual-boot, EEPROM read/write support and Chip Erase capability
 - High default baud rate 115200 for fast programming
 - Highly modular feature set:
     + EEPROM read/write support
     + Vector bootloader for devices without a boot section or to save space on devices with one
     + Software UART: no hardware UART required, but also useful when the chip's CPU frequency and
       the bootloader's desired baudrate are incompatible (eg, 8 MHz and 115200 baud)
     + Subroutine `pgm_write_page(sram, progmem)` for applications so they can change themselves:
       on many MCUs the SPM write flash only works when called from the bootloader section (default on)
     + Dual programming from external SPI flash memory for over-the-air programming
     + Template bootloader with nops that can be replaced at bootloader-burning time on the fly
       with code to operate the right LED/TX/RX/CS pins (see accompanying urloader sketch once published)
     + Saves the reset flags in R2 for inspection by the application via the .init0 section (default on)
     + Bootloader protects itself from overwriting (default on)
 - Avrdude supports urboot bootloaders with `-c urclock` from v7.2

**Documentation.** In the first instance, the first 600 or so lines of `urboot.c` give a good
insight.

**Compiling an urboot bootloader.** The small sizes of `urboot` bootloaders depend on many a trick
of the trade, most of which only work with the avr-gcc toolchain used for compilation. It turns out
that two older avr-toolchains, version 4.8.1 or 5.4.0, produce pretty tight code. Linux binaries of
these are located in the `avr-toolchain` directory. This project uses helper programs in perl: an
avr-gcc wrapper `urboot-gcc` and `hexls` that shows stats of the produced `.hex` files. So, all
things considered, compiling your own, is easiest on Linux with perl installed and some required
perl libraries, too (install them with `cpan`). All going well `make all` should produce `.hex`,
`.elf` and `.lst` files of a number of bootloaders. The created .hex files should coincide with the
corresponding ones in the [`all`
directory.](https://raw.githubusercontent.com/stefanrueger/urboot/main/all/). Once all these
hurdles are taken, it is pretty easy to create own bootloaders with commands such as
```
 $ make MCU=atmega328p F_CPU=16000000L BAUD_RATE=115200 EEPROM=1 URPROTOCOL=1 \
   LED=AtmelPB1 SFMCS=AtmelPB0 DUAL=1 FRILLS=7 MOVETO=moteino-dual
```

**Pre-compiled bootloaders.** If compiling bootloaders is not feasible, feel free to try out one of
the roughly 110,000 *different* pre-compiled bootloaders in the directory tree
[`bootloaders`](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/). The tree
contains are actually some 350,000 hex files, but they are somewhat redundant because a bootloader
on 115200 baud for 16 MHz is *exactly* the same as a bootloader on 57600 baud for 8 MHz.

**Usage.** As with all bootloaders, one needs to first program them onto
the board/chip using a (physical) programmer and an uploader program, eg, through
```
$ avrdude -c usbasp -p m328p -U flash:w:bootloader.hex:i
```
Particular attention is needed as to whether
 - The bootloader assumes hardware support (`h`, see below) and sits in a dedicated HW boot
   section, in which case the fuses need to be set to ensure that on reset the MCU jumps to the
   correct HW boot section, or whether
 - The bootloader is a vector bootloader (`j`, `v` or `V`, see below), in which case the fuses need
   to be programmed so that on reset the MCU jumps to the reset vector 0.

Then the board can be directly connected to and prgrammed from the
host/laptop/PC (without a physical programmer), eg, through
```
 $ avrdude -c urclock -p m328p -b 115200 -P /dev/ttyUSB0 -U flash:w:mysketch.hex:i
 $ avrdude -c urclock -p m328p -b 115200 -P /dev/ttyUSB0 -U eeprom:r:backup.eep:I
 $ avrdude -c urclock -p m328p -b 115200 -P /dev/ttyUSB0 -t
```

Voila!

**Comparison.** The table below lists a sample of `urboot` bootloaders and their features alongside
vanilla optiboot:

|Size|Usage|Version|Features|Hex file|
|:-:|:-:|:-:|:-:|:--|
|222|224|u7.6|`w-u-jpr`|[attiny2313_lednop_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/attiny2313/fcpu_16mhz/115200_bps/attiny2313_16mhz_115200bps_lednop_ur_vbl.hex)|
|232|256|u7.6|`w-u-jpr`|[atmega328p_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/atmega328p/fcpu_16mhz/115200_bps/atmega328p_16mhz_115200bps_ur_vbl.hex)|
|238|256|u7.6|`w-u-jpr`|[atmega328p_led+b5_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/atmega328p/fcpu_16mhz/115200_bps/atmega328p_16mhz_115200bps_led+b5_ur_vbl.hex)|
|244|256|u7.6|`w-u-hpr`|[atmega16a_lednop_fr_ur.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/atmega16a/fcpu_16mhz/115200_bps/atmega16a_16mhz_115200bps_lednop_fr_ur.hex)|
|244|256|u7.6|`w-u-hpr`|[atmega8a_lednop_fr_ur.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/atmega8a/fcpu_16mhz/115200_bps/atmega8a_16mhz_115200bps_lednop_fr_ur.hex)|
|244|256|u7.6|`w-u-jpr`|[atmega32a_lednop_fr_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/atmega32a/fcpu_16mhz/115200_bps/atmega32a_16mhz_115200bps_lednop_fr_ur_vbl.hex)|
|246|256|u7.6|`w-u-jpr`|[attiny85_rxb4_txb3_lednop_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/attiny85/fcpu_16mhz/115200_bps/attiny85_16mhz_115200bps_rxb4_txb3_lednop_ur_vbl.hex)|
|248|256|u7.6|`w-u-jpr`|[atmega1280_lednop_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/atmega1280/fcpu_16mhz/115200_bps/atmega1280_16mhz_115200bps_lednop_ur_vbl.hex)|
|248|256|u7.6|`w-u-jpr`|[atmega1284p_lednop_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/atmega1284p/fcpu_16mhz/115200_bps/atmega1284p_16mhz_115200bps_lednop_ur_vbl.hex)|
|248|256|u7.6|`w-u-jpr`|[atmega2560_led+b7_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/atmega2560/fcpu_16mhz/115200_bps/atmega2560_16mhz_115200bps_led+b7_ur_vbl.hex)|
|248|256|u7.6|`w-u-jpr`|[atmega2560_lednop_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/atmega2560/fcpu_16mhz/115200_bps/atmega2560_16mhz_115200bps_lednop_ur_vbl.hex)|
|248|256|u7.6|`w-u-jpr`|[attiny167_rxb6_txb3_lednop_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/attiny167/fcpu_16mhz/115200_bps/attiny167_16mhz_115200bps_rxb6_txb3_lednop_ur_vbl.hex)|
|252|256|u7.6|`w-u-hpr`|[atmega88a_lednop_fr_ur.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/atmega88a/fcpu_16mhz/115200_bps/atmega88a_16mhz_115200bps_lednop_fr_ur.hex)|
|252|256|u7.6|`w-u-jpr`|[atmega328p_8mhz_rxd0_txd1_lednop_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/atmega328p/fcpu_8mhz/115200_bps/atmega328p_8mhz_115200bps_rxd0_txd1_lednop_ur_vbl.hex)|
|252|256|u7.6|`w-u-jpr`|[atmega48a_lednop_fr_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/atmega48a/fcpu_16mhz/115200_bps/atmega48a_16mhz_115200bps_lednop_fr_ur_vbl.hex)|
|254|256|u7.6|`w-u-jpr`|[atmega644p_lednop_fr_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/atmega644p/fcpu_16mhz/115200_bps/atmega644p_16mhz_115200bps_lednop_fr_ur_vbl.hex)|
|254|256|u7.6|`w-u-jpr`|[attiny84_rxa3_txa2_lednop_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/attiny84/fcpu_16mhz/115200_bps/attiny84_16mhz_115200bps_rxa3_txa2_lednop_ur_vbl.hex)|
|256|256|u7.6|`w-u-hpr`|[atmega168p_lednop_fr_ur.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/atmega168p/fcpu_16mhz/115200_bps/atmega168p_16mhz_115200bps_lednop_fr_ur.hex)|
|256|256|u7.6|`w-u-jpr`|[atmega328p_led+b5_fr_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/atmega328p/fcpu_16mhz/115200_bps/atmega328p_16mhz_115200bps_led+b5_fr_ur_vbl.hex)|
|350|384|u7.6|`weu-jpr`|[atmega328p_16mhz_115200bps_ee_led+b5_fr_ce_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/atmega328p_16mhz_115200bps_ee_led+b5_fr_ce_ur_vbl.hex)|
|350|384|u7.6|`weu-jpr`|[atmega328p_ee_lednop_fr_ce_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/atmega328p/fcpu_16mhz/115200_bps/atmega328p_16mhz_115200bps_ee_lednop_fr_ce_ur_vbl.hex)|
|364|384|u7.6|`weu-jpr`|[atmega328p_8mhz_rxd0_txd1_ee_lednop_fr_ce_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/atmega328p/fcpu_8mhz/115200_bps/atmega328p_8mhz_115200bps_rxd0_txd1_ee_lednop_fr_ce_ur_vbl.hex)|
|460|512|u7.6|`wes-hpr`|[atmega328p_ee_lednop_fr_ce_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/atmega328p/fcpu_16mhz/115200_bps/atmega328p_16mhz_115200bps_ee_lednop_fr_ce_vbl.hex)|
|474|512|o8.3|`?-s-h?r`|[optiboot_atmega328.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/all/optiboot_atmega328.hex)|
|490|512|u7.6|`weudjpr`|[atmega328p_ee_led+b1_csb0_fr_ce_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/atmega328p/fcpu_16mhz/115200_bps/atmega328p_16mhz_115200bps_ee_led+b1_csb0_fr_ce_ur_vbl.hex)|
|498|512|u7.6|`weudjpr`|[atmega328p_ee_lednop_csnop_fr_ce_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/atmega328p/fcpu_16mhz/115200_bps/atmega328p_16mhz_115200bps_ee_lednop_csnop_fr_ce_ur_vbl.hex)|

- **Size:** Bootloader code size including small table at top end
- **Usage:** How many bytes of MCU flash are needed, ie, HW boot section or a multiple of the page size
- **Version:** For example, u7.6 is an urboot version, o5.2 is an optiboot version
- **Features:**
  + `w` urboot provides `pgm_write_page(sram, flash)` for the application at `FLASHEND-4+1`
  + `e` EEPROM read/write support
  + `u` uses urprotocol and requires `avrdude -c urclock` for programming
  + `s` uses skeleton of STK500v1 protocol; `-c urclock` or `-c arduino` both work
  + `d` dual boot (over-the-air programming from external SPI flash)
  + `h` hardware boot section: make sure fuses are set for reset to jump to boot section
  + `j` vector bootloader: uploaded applications *need to be changed externally*, eg, using `avrdude -c urclock`
  + `p` bootloader protects itself from being overwritten
  + `r` preserves reset flags for the application in the register R2
- **Hex file** naming convention: MCU, oscillator frequency and baud rate followed by
  + `rxd0 txd1` software I/O using, in this example, lines RX `D0` and TX `D1`
  + `ee` bootloader supports EEPROM read/write
  + `led-b1` toggles an active-low LED on pin `B1`, `+` designates an active-high LED
  + `lednop` "template" bootloader contains `mov rx,rx` nops that can be replaced to toggle LEDs
  + `csb0` for dual boot uses, in this example, pin B0 as chip select of external SPI flash memory
  + `csnop` like `lednop` generates nops that can be replaced to toggle the correct CS pin
  + `fr` bootloader provides non-essential code for smoother error handing
  + `ce` bootloader provides a chip erase command
  + `ur` uses urprotocol and requires `avrdude -c urclock` for programming
  + `vbl` vector bootloader: set fuses to jump to reset, not the HW boot section
