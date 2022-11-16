# Urboot
**Feature-rich small AVR bootloader using
[urprotocol](https://github.com/stefanrueger/urboot/blob/main/urprotocol.md)**

 - Tight code: most bootloaders fit into
     + 256 bytes albeit without EEPROM read/write support
     + 384 bytes with EEPROM read/write support
     + 512 bytes with dual-boot, EEPROM read/write support and Chip Erase capability
 - Highly modular feature set:
     + Dedicated protocol between programmer and bootloader (urprotocol)
     + EEPROM read/write support
     + Vector bootloader for devices without a boot section or to save space on devices with one
     + Software UART: no hardware UART required, but also useful when the chip's CPU frequency and
       the bootloader's desired baudrate are incompatible (eg, 8 MHz and 115,200 baud)
     + Subroutine `pgm_write_page(sram, progmem)` for applications so they can change themselves:
       on many MCUs the SPM write flash only works when called from the bootloader section (default on)
     + Dual programming from external SPI flash memory for over-the-air programming
     + Template bootloader with nops that can be replaced at bootloader-burning time on the fly
       with code to operate the right LED/TX/RX/CS pins (see accompanying urloader sketch once published)
     + Saves the reset flags in R2 for inspection by the application via the .init0 section (default on)
     + Bootloader protects itself from overwriting (default on)
     + Automatic host baud rate detection
     + Chip erase in bootloader (faster than -c urclock emulation)
 - Avrdude (from v7.1 onwards) supports urboot bootloaders with `-c urclock`

**Documentation.** In the first instance, the first 666 or so lines of `urboot.c`

**Compiling an urboot bootloader.** The small sizes of `urboot` bootloaders depend on many a trick
of the trade, most of which only work with the avr-gcc toolchain used for compilation. It turns out
that two older avr-toolchains, version 4.8.1 or 5.4.0, produce pretty tight code. Linux binaries of
these are located in the `src/avr-toolchain` directory. This project uses helper programs in perl: an
avr-gcc wrapper `urboot-gcc` and `hexls` that shows stats of the produced `.hex` files. So, all
things considered, compiling this project is easiest on Linux with perl installed (and some
required perl libraries, too; install them with `cpan`). All going well `make all` should produce
`.hex`, `.elf` and `.lst` files of a number of bootloaders. The created .hex files should coincide
with the corresponding ones in the [`src/all`
directory.](https://raw.githubusercontent.com/stefanrueger/urboot/main/all/). Once all these
hurdles are taken, it is pretty easy to create own bootloaders with commands such as
```
 $ make MCU=atmega328p F_CPU=16000000L BAUD_RATE=115200 EEPROM=1 URPROTOCOL=1 \
   LED=AtmelPB1 SFMCS=AtmelPB0 DUAL=1 FRILLS=7 MOVETO=moteino-dual
```

**Pre-compiled bootloaders.** If compiling bootloaders is not feasible, feel free to try out one of
the 27,736 *different* pre-compiled bootloaders in the directory tree
[`bootloaders`](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/). The tree
contains actually 84,227 .hex files, but they are somewhat redundant because a bootloader on
115,200 baud for 16 MHz is *exactly* the same as a bootloader on 57,600 baud for 8 MHz.

**How to select a bootloader.** It is tempting to go for the most feature-rich bootloader that
there is. However, consider carefully what you need: if there is no plan for over-the-air
programming, and no external SPI flash on your board either, there is little benefit of using a
dual-boot bootloader. Whilst they deal gracefully with a board that has no external SPI flash
memory, they will add a little delay at each external reset and each WDT reset. Also they toggle
the SPI and chip select lines at each reset (which not all projects can handle). Chip erase in the
bootloader is only of benefit on MCUs with large flash. `avrdude -c urclock` erases flash during
upload of a new application by filling unused flash with 0xff; the extra time needed is hardly
noticable on small devices up to 8k, so  Chip erase functionality in the bootloader is not
desirable here. If you have a fixed workflow where you can control the host baud rate, then the
extra 18 bytes for autobaud detection may not be  worth your while, particularly if you can save an
extra memory page for *every* application that you may want to upload. The same goes for the six
additional code bytes spent for LED toggling. Selecting an 's' type bootloader (one that implements
a skeleton STK500 protocol and that can also be programmed with `avrdude -c arduino`) does not
increase functionality but costs code space; they are only recommended when `avdude -c urclock` is
not available.


**Bugs.** The code still defaults to `URPROTOCOL=0`. The urprotocol should be chosen by default.


**Usage.** As with all bootloaders, one needs to first program them onto
the board/chip using a (physical) programmer and an uploader program, eg, through
```
$ avrdude -c usbasp -p m328p -U flash:w:bootloader.hex:i
```
Particular attention is needed as to whether
 - The bootloader assumes hardware support (`h`, see below) and sits in a dedicated HW boot
   section, in which case the fuses need to be set to ensure that on reset the MCU jumps to the
   correct bootloader start and that the lock bits do not preclude writing to the boot sections
   (otherwise this space cannot be used), or whether
 - The bootloader is a vector bootloader (`j`, `v` or `V`, see below), in which case the fuses need
   to be programmed so that on reset the MCU jumps to the reset vector 0. Strictly speaking vector
   bootloaders also need a `jmp` or `rjmp` from the reset vector at address 0 to the bootloader.
   However, if the chip was erased before putting the bootloader on, then this is not necessary:
   Erased words read `0xffff`, and although this is not an official opcode, it behaves as `sbrs
   r31,7` (skip one instruction if bit 7 in R31 is set). A reset to address 0 on an otherwise
   erased flash will therefore eventually run into the start of the bootloader. Uploading the first
   application with `avrdude -c urboot` will then set the reset vector correctly to jump to the
   bootloader.

Once the board has its bootloader it can be directly connected to and programmed from the
host/laptop/PC (without a physical programmer), eg, through
```
 $ avrdude -c urclock -p m328p -b 115200 -P /dev/ttyUSB0 -U flash:w:mysketch.hex:i
 $ avrdude -c urclock -p m328p -b 115200 -P /dev/ttyUSB0 -U eeprom:r:backup.eep:I
 $ avrdude -c urclock -p m328p -b 115200 -P /dev/ttyUSB0 -t
```

Voila!

**Fun fact.** `avrdude -c urclock` can keep bootloaders in terminal mode `-t` without the
bootloader resetting itself through the watchdog timer.

**Comparison.** The table below lists a sample of `urboot` bootloaders and their features alongside
vanilla optiboot. They are all for a 16 MHz MCU and 115,200 baud serial communication speed, except
where noted differently.

Size|Usage|Version|Features|Hex file|
|:-:|:-:|:-:|:-:|:--|
|252|256|u7.7|`w-u-jpr--`|[atmega328p_16mhz_115200bps_led+b5_fr_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/atmega328p/fcpu_16mhz/115200_bps/atmega328p_16mhz_115200bps_led+b5_fr_ur_vbl.hex)|
|346|384|u7.7|`weu-jpr-c`|[atmega328p_16mhz_115200bps_ee_led+b5_fr_ce_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/atmega328p/fcpu_16mhz/115200_bps/atmega328p_16mhz_115200bps_ee_led+b5_fr_ce_ur_vbl.hex)|
|446|512|u7.7|`wes-hpr-c`|[atmega328p_16mhz_115200bps_ee_led+b5_fr_ce.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/atmega328p/fcpu_16mhz/115200_bps/atmega328p_16mhz_115200bps_ee_led+b5_fr_ce.hex)|
|474|512|o8.3|`--s-h-r`|[optiboot_atmega328.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/all/optiboot_atmega328.hex)|
|486|512|u7.7|`weudhpr-c`|[atmega328p_16mhz_115200bps_ee_led+b1_csb0_dual_fr_ce_ur.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/atmega328p/fcpu_16mhz/115200_bps/atmega328p_16mhz_115200bps_ee_led+b1_csb0_dual_fr_ce_ur.hex)|


- **Size:** Bootloader code size including small table at top end
- **Usage:** How many bytes of flash are needed, ie, HW boot section or a multiple of the page size
- **Version:** For example, u7.6 is an urboot version, o5.2 is an optiboot version
- **Features:**
  + `w` urboot provides `pgm_write_page(sram, flash)` for the application at `FLASHEND-4+1`
  + `e` EEPROM read/write support
  + `u` uses urprotocol requiring `avrdude -c urclock` for programming
  + `s` uses skeleton of STK500v1 protocol (deprecated); `-c urclock` and `-c arduino` both work
  + `d` dual boot (over-the-air programming from external SPI flash)
  + `h` hardware boot section: make sure fuses are set for reset to jump to boot section
  + `j` vector bootloader: uploaded applications *need to be patched externally*, eg, using `avrdude -c urclock`
  + `p` bootloader protects itself from being overwritten
  + `r` preserves reset flags for the application in the register R2
- **CE** Whether the bootloader itself provides a chip erase functionality in the sense that it erases all flash below the bootloader
- **Hex file:** typically MCU name, oscillator frequency (16 MHz default) and baud rate (115200 default) followed by
  + `ee` bootloader supports EEPROM read/write
  + `led-b1` toggles an active-low LED on pin `B1`, `+` designates an active-high LED
  + `csb0` for dual boot uses, in this example, pin B0 as chip select of external SPI flash memory
  + `dual` boot: serial and from external SPI flash memory
  + `fr` bootloader provides non-essential code for smoother error handing
  + `ce` bootloader provides a chip erase command
  + `ur` uses urprotocol and requires `avrdude -c urclock` for programming
  + `vbl` vector bootloader: set fuses to jump to reset, not the HW boot section


Here a slightly larger table catering demonstrating the range of 183 parts for which `urboot`
currently compiles.

|Size|Usage|Version|Features|Hex file|
|:-:|:-:|:-:|:-:|:--|
|222|224|u7.7|`w-u-jpr--`|[attiny2313_16mhz_115200bps_lednop_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/attiny2313/fcpu_16mhz/115200_bps/attiny2313_16mhz_115200bps_lednop_ur_vbl.hex)|
|252|256|u7.7|`w-u-jpr--`|[atmega328p_16mhz_115200bps_lednop_fr_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/atmega328p/fcpu_16mhz/115200_bps/atmega328p_16mhz_115200bps_lednop_fr_ur_vbl.hex)|
|252|256|u7.7|`w-u-jpr--`|[atmega328p_16mhz_115200bps_led+b5_fr_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/atmega328p/fcpu_16mhz/115200_bps/atmega328p_16mhz_115200bps_led+b5_fr_ur_vbl.hex)|
|252|256|u7.7|`w-u-jpra-`|[uno_autobaud_led+b5_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/board_uno/autobaud/uno_autobaud_led+b5_ur_vbl.hex)|
|244|256|u7.7|`w-u-hpr--`|[atmega16a_16mhz_115200bps_lednop_fr_ur.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/atmega16a/fcpu_16mhz/115200_bps/atmega16a_16mhz_115200bps_lednop_fr_ur.hex)|
|244|256|u7.7|`w-u-hpr--`|[atmega8a_16mhz_115200bps_lednop_fr_ur.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/atmega8a/fcpu_16mhz/115200_bps/atmega8a_16mhz_115200bps_lednop_fr_ur.hex)|
|244|256|u7.7|`w-u-jpr--`|[atmega32a_16mhz_115200bps_lednop_fr_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/atmega32a/fcpu_16mhz/115200_bps/atmega32a_16mhz_115200bps_lednop_fr_ur_vbl.hex)|
|246|256|u7.7|`w-u-jpr--`|[attiny85_16mhz_115200bps_rxb4_txb3_lednop_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/attiny85/fcpu_16mhz/115200_bps/attiny85_16mhz_115200bps_rxb4_txb3_lednop_ur_vbl.hex)|
|244|256|u7.7|`w-u-jpr--`|[atmega1280_16mhz_115200bps_lednop_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/atmega1280/fcpu_16mhz/115200_bps/atmega1280_16mhz_115200bps_lednop_ur_vbl.hex)|
|244|256|u7.7|`w-u-jpr--`|[atmega1284p_16mhz_115200bps_lednop_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/atmega1284p/fcpu_16mhz/115200_bps/atmega1284p_16mhz_115200bps_lednop_ur_vbl.hex)|
|244|256|u7.7|`w-u-jpr--`|[atmega2560_16mhz_115200bps_led+b7_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/atmega2560/fcpu_16mhz/115200_bps/atmega2560_16mhz_115200bps_led+b7_ur_vbl.hex)|
|244|256|u7.7|`w-u-jpr--`|[atmega2560_16mhz_115200bps_lednop_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/atmega2560/fcpu_16mhz/115200_bps/atmega2560_16mhz_115200bps_lednop_ur_vbl.hex)|
|248|256|u7.7|`w-u-jpr--`|[attiny167_16mhz_115200bps_rxb6_txb3_lednop_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/attiny167/fcpu_16mhz/115200_bps/attiny167_16mhz_115200bps_rxb6_txb3_lednop_ur_vbl.hex)|
|248|256|u7.7|`w-u-hpr--`|[atmega88a_16mhz_115200bps_lednop_fr_ur.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/atmega88a/fcpu_16mhz/115200_bps/atmega88a_16mhz_115200bps_lednop_fr_ur.hex)|
|252|256|u7.7|`w-u-jpr--`|[atmega328p_8mhz_115200bps_rxd0_txd1_lednop_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/atmega328p/fcpu_8mhz/115200_bps/atmega328p_8mhz_115200bps_rxd0_txd1_lednop_ur_vbl.hex)|
|248|256|u7.7|`w-u-jpr--`|[atmega48a_16mhz_115200bps_lednop_fr_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/atmega48a/fcpu_16mhz/115200_bps/atmega48a_16mhz_115200bps_lednop_fr_ur_vbl.hex)|
|250|256|u7.7|`w-u-jpr--`|[atmega644p_16mhz_115200bps_lednop_fr_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/atmega644p/fcpu_16mhz/115200_bps/atmega644p_16mhz_115200bps_lednop_fr_ur_vbl.hex)|
|254|256|u7.7|`w-u-jpr--`|[attiny84_16mhz_115200bps_rxa3_txa2_lednop_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/attiny84/fcpu_16mhz/115200_bps/attiny84_16mhz_115200bps_rxa3_txa2_lednop_ur_vbl.hex)|
|252|256|u7.7|`w-u-hpr--`|[atmega168p_16mhz_115200bps_lednop_fr_ur.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/atmega168p/fcpu_16mhz/115200_bps/atmega168p_16mhz_115200bps_lednop_fr_ur.hex)|
|346|384|u7.7|`weu-jpr-c`|[atmega328p_16mhz_115200bps_ee_lednop_fr_ce_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/atmega328p/fcpu_16mhz/115200_bps/atmega328p_16mhz_115200bps_ee_lednop_fr_ce_ur_vbl.hex)|
|364|384|u7.7|`weu-jpr-c`|[atmega328p_8mhz_115200bps_rxd0_txd1_ee_lednop_fr_ce_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/atmega328p/fcpu_8mhz/115200_bps/atmega328p_8mhz_115200bps_rxd0_txd1_ee_lednop_fr_ce_ur_vbl.hex)|
|464|512|u7.7|`wes-hprac`|[atmega328p_autobaud_ee_lednop_fr_ce.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/atmega328p/autobaud/atmega328p_autobaud_ee_lednop_fr_ce.hex)|
|474|512|o8.3|`--s-h-r`| |[optiboot_atmega328.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/src/all/optiboot_atmega328.hex)|
|494|512|u7.7|`weudhpr-c`|[atmega328p_16mhz_115200bps_ee_template_dual_fr_ce_ur.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/atmega328p/fcpu_16mhz/115200_bps/atmega328p_16mhz_115200bps_ee_template_dual_fr_ce_ur.hex)|
|504|512|u7.7|`weudhprac`|[urclockusb_autobaud_ee_led+d5_csb0_dual_fr_ce_ur.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/board_urclockusb/autobaud/urclockusb_autobaud_ee_led+d5_csb0_dual_fr_ce_ur.hex)|
|512|512|u7.7|`weudhprac`|[atmega328p_autobaud_ee_template_dual_fr_ce_ur.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/atmega328p/autobaud/atmega328p_autobaud_ee_template_dual_fr_ce_ur.hex)|

- **Hex file** naming convention: as above, plus
  + `rxd0 txd1` software I/O using, in this example, lines RX `D0` and TX `D1`
  + `lednop` is a template bootloader with `mov rx,rx` nops as placeholders for LEDs operations
  + `template` bootloaders contains `mov rx,rx` nops as placeholders for LED and CS operations

Note that the USART on an 8 MHz ATmega328P cannot produce 115200 baud within a 2.5% tolerance; the
8 MHz examples shown use software I/O that allows for a finer baud granularity.
