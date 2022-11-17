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
these are located in the `src/avr-toolchain` directory. This project uses helper programs in perl:
an avr-gcc wrapper `urboot-gcc` and `hexls` that shows stats of the produced `.hex` files. So, all
things considered, compiling this project is easiest on Linux with perl installed (and some
required perl libraries, too; install them with `cpan`). All going well `make all` should produce
`.hex`, `.elf` and `.lst` files of a number of bootloaders. The created .hex files should coincide
with the corresponding ones in the directory
[`src/all`](https://github.com/stefanrueger/urboot/tree/main/src/all). Once all these hurdles are
taken, it is pretty easy to create own bootloaders with commands such as
```
 $ make MCU=atmega328p F_CPU=16000000L BAUD_RATE=115200 EEPROM=1 URPROTOCOL=1 \
   LED=AtmelPB1 SFMCS=AtmelPB0 DUAL=1 FRILLS=7 MOVETO=moteino-dual
```

**Pre-compiled bootloaders.** If compiling bootloaders is not feasible, feel free to try out one of
the 27,736 *different* pre-compiled bootloaders in the directory tree
[`bootloaders`](https://github.com/stefanrueger/urboot/tree/main/bootloaders/). The tree
contains actually 84,227 hex files, but they are somewhat redundant because a bootloader on
115,200 baud for 16 MHz is *exactly* the same as a bootloader on 57,600 baud for 8 MHz.

**How to select a bootloader.** It is tempting to go for the most feature-rich bootloader that
there is. However, consider carefully what you need:
 - **`pgm_write_page(sram, flash)`** for the application at `FLASHEND-4+1` is a necessary feature
   for applications that want to use MCU flash behind the code as storage in a similar fashion to
   EEPROM. It has security implications, but no more than generally having a bootloader on the
   chip.
 - **EPROM** access is generally thought of as useful. Applications can read initialisation
   parameters from EEPROM and store results there in the safe knowledge that the bootloader gives
   the user independent access to EEPROM.
 - **Protocol `u` or `s`.** Bootloaders with the `s` bit implement a skeleton STK500 protocol; they
   can normally also be programmed using `avrdude -c arduino`; the `s` protocol does *not* increase
   functionality but costs *a lot* of bootloader code space; they are only recommended when no
   recent version of avrdude with `-c urclock` programmer is available. The general recommendation
   is to always use bootloaders with the `u` urprotocol bit set. They are the *native* bootloaders
   for `-c urclock`. Once newer avrdude versions with `-c urclock` are widely available the `s`
   protocol is likely to be deprecated improving the clarity of the urboot.c source.
 - **Dual boot.** If there is no plan for over-the-air programming, and no external SPI flash on
   your board either, there is little benefit of using a dual-boot bootloader. Whilst they deal
   gracefully with a board that has no external SPI flash memory, they will add a little delay at
   each external reset or WDT reset. Also they toggle the SPI and chip select lines at each reset,
   which is something that not all projects can handle.
 - **Vector bootloaders or not**. This question only arises on parts that have hardware support,
   otherwise it has got to be a vector bootloader. In general, vector bootloaders occupy a multiple
   of the device's flash memory page size. In contrast to this hardware-supported bootloader often
   have very large sizes, eg, the ATmega328p can have hw-supported bootloaders with 512, 1024, 2048
   and 4096 bytes size. Its flash page size is 128 bytes, so vector bootloaders can occupy any
   multiple of that. Normally, the best use of MCU flash space is to select `j`-type vector
   bootloaders (see below), and only select `h`-type hardware supported bootloaders if they happen
   to occupy the same space. When installing bootloaders take care to program the right fuses (see
   *Usage* below). There are three types of vector bootloaders that the urboot project offers:
    + `j` versions cost minimal to no extra space in the bootloader and need applications to be
      patched during upload. `avrdude -c urclock` does that auto-magically.
    + In `v` and `V` versions the bootloader patched the applications itself to various degrees of
      rigour. They generally consume a lot of extra space on the MCU at no run-time benefit; these
      types are not recommended, and urboot support for these are likely to be withdrawn in future.
 - **Protection** from overwriting itself. All urboot bootloaders are protected from overwriting
   themselves. Although this can in theory be switched off at compile time, the recommendation is
   to never do that even when using hardware-supported bootloaders that can be protected by lock
   bits (as the user may have chosen not to program the lock bits). As an aside, vector bootloaders
   have an Achilles' heel in that the reset vector needs to always point to the bootloader. The `-c
   urclock` bootloader does not normally allow the reset vector to be overwritten by applications
   during upload, and the `-c urclock -t` terminal also protects the reset vector of vector
   bootloaders. Self-modifying programs can overwrite the reset vector, though. If user application
   code manages to do that, the bootloader needs to be re-flashed.
 - **Autobaud.** If you have a fixed workflow where you can control the host baud rate, then the
   extra 16 bytes for autobaud detection may not be worth your while: vector bootloaders have to
   have a size of a multiple of the memory page size. If, by reducing the features of the
   bootloader, one can save a memory page for the bootloader that memory page is then available for
   *every* application that you may want to upload.
 - **Chip erase** in the bootloader is only of benefit on MCUs with large flash. If the bootloader
   does not offer chip erase `avrdude -c urclock` erases flash during upload of a new application
   by filling unused flash with 0xff; the extra time needed is hardly noticeable on small devices up
   to 8k flash making chip erase in the bootloader itself undesirable here.
 - **LED.** The same goes for the six additional code bytes spent for LED toggling. The novelty of
   the bootloader blinking during upload/download wears quickly off, and projects can not normally
   re-purpose the LED line without it being used as output during external reset.
 - **Template bootloaders.** Pre-compiled bootloaders labelled `_lednop` or `_template` contain nops
   as placeholders so that just before flashing them, another program can replace the nops with
   code to plug the right LED line and/or CS line needed for dual boot. They normally occupy the
   same space as bootloaders that are compiled for a specific LED and/or CS line, but can be
   slightly bigger, particularly when the LED and CS line are known to sit on the same port.
 - **Frills.** The label `_fr` indicated the presence of code that is not necessary for the working
   of the bootloader, for example that the application is started sooner after upload, or that frame
   errors in the serial communication get the bootloader to exit quickly. It is OK not to have these
   frills, particlarly when it means an extra memory page for *every* application.


**Bugs.** The code still defaults to `URPROTOCOL=0`. The urprotocol should be chosen by default.


**Usage.** As with all bootloaders, one needs to first program them onto
the board/chip using a (physical) programmer and an uploader program, eg, through
```
$ avrdude -c usbasp -p m328p -U flash:w:bootloader.hex:i
```
***Particular attention*** is needed as to whether the bootloader
 - Assumes hardware support **(`h`)** and sits in a dedicated HW boot section, in which case the
   fuses need to be set to ensure that on reset the MCU jumps to the correct bootloader start
 - Is a vector bootloader **(`j`, `v` or `V`),** in which case the fuses need to be programmed so
   that on reset the MCU jumps to the reset vector at address 0. The lock bits should not preclude
   writing to any of the boot sections (otherwise the space freed up by vector bootloaders cannot
   be used). Strictly speaking vector bootloaders also need a `jmp` or `rjmp` from the reset vector
   at address 0 to the bootloader. However, if the chip was erased before putting the bootloader
   on, then this is not necessary: Erased words read `0xffff`, and although this is not an official
   opcode, it behaves as `sbrs r31,7` (skip one instruction if bit 7 in R31 is set). A reset to
   address 0 on an otherwise erased flash will therefore eventually run into the start of the
   bootloader. Uploading the first application with `avrdude -c urboot` will then set the reset
   vector correctly to jump to the bootloader.

Once the board has its bootloader the board can be directly connected to and programmed from the
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
vanilla optiboot and its larger bigboot variant for EEPROM access. They are all for a 16 MHz MCU
and 115,200 baud serial communication speed, except where noted differently.

Size|Usage|Version|Features|Hex file|
|:-:|:-:|:-:|:-:|:--|
|252|256|u7.7|`w-u-jpr--`|[atmega328p_led+b5_fr_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/atmega328p/fcpu_16mhz/115200_bps/atmega328p_16mhz_115200bps_led+b5_fr_ur_vbl.hex)|
|346|384|u7.7|`weu-jpr-c`|[atmega328p_ee_led+b5_fr_ce_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/atmega328p/fcpu_16mhz/115200_bps/atmega328p_16mhz_115200bps_ee_led+b5_fr_ce_ur_vbl.hex)|
|446|512|u7.7|`wes-hpr-c`|[atmega328p_ee_led+b5_fr_ce.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/atmega328p/fcpu_16mhz/115200_bps/atmega328p_16mhz_115200bps_ee_led+b5_fr_ce.hex)|
|474|512|o8.3|`--s-h-r--`|[optiboot_atmega328.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/src/all/optiboot_atmega328.hex)|
|502|512|u7.7|`weudhprac`|[urclockusb_autobaud_ee_led+d5_csb0_dual_fr_ce_ur.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/board_urclockusb/autobaud/urclockusb_autobaud_ee_led+d5_csb0_dual_fr_ce_ur.hex)|
|710|1024|o8.3|`-es-h-r--`|[bigboot_328.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/src/all/bigboot_328.hex)|


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
  + `j` vector bootloader: applications *need to be patched externally*, eg, using `avrdude -c urclock`
  + `p` bootloader protects itself from being overwritten
  + `r` preserves reset flags for the application in the register R2
  + `a` autobaud detection (f_cpu/8n and less with discrete divisors, normally n = 2, 3, ...)
  + `c` bootloader provides chip erase functionality (only recommended for large MCUs)
  + `-` corresponding feature not present
- **Hex file:** typically MCU name and frequency (16 MHz default), baud rate (115,200 default) followed by
  + `autobaud` tries to match host baud rate; can be f/8, f/16, f/24, ..., f/2048 (f=F<sub>CPU</sub>)
  + `ee` bootloader supports EEPROM read/write
  + `led-b1` toggles an active-low LED on pin `B1`, `+` designates an active-high LED
  + `csb0` for dual boot uses, in this example, pin B0 as chip select of external SPI flash memory
  + `dual` boot: serial and from external SPI flash memory
  + `fr` bootloader provides non-essential code (frills), eg, for smoother error handing
  + `ce` bootloader provides a chip erase command
  + `ur` uses urprotocol and requires `avrdude -c urclock` for programming
  + `vbl` vector bootloader: set fuses to jump to reset, not the HW boot section


Here a slightly larger table catering demonstrating the range of 183 parts for which `urboot` compiles.

|Size|Usage|Version|Features|Hex file|
|:-:|:-:|:-:|:-:|:--|
|222|224|u7.7|`w-u-jpr--`|[attiny2313_lednop_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/attiny2313/fcpu_16mhz/115200_bps/attiny2313_16mhz_115200bps_lednop_ur_vbl.hex)|
|252|256|u7.7|`w-u-jpr--`|[atmega328p_lednop_fr_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/atmega328p/fcpu_16mhz/115200_bps/atmega328p_16mhz_115200bps_lednop_fr_ur_vbl.hex)|
|252|256|u7.7|`w-u-jpr--`|[atmega328p_led+b5_fr_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/atmega328p/fcpu_16mhz/115200_bps/atmega328p_16mhz_115200bps_led+b5_fr_ur_vbl.hex)|
|250|256|u7.7|`w-u-jpra-`|[uno_autobaud_led+b5_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/board_uno/autobaud/uno_autobaud_led+b5_ur_vbl.hex)|
|244|256|u7.7|`w-u-hpr--`|[atmega16a_lednop_fr_ur.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/atmega16a/fcpu_16mhz/115200_bps/atmega16a_16mhz_115200bps_lednop_fr_ur.hex)|
|244|256|u7.7|`w-u-hpr--`|[atmega8a_lednop_fr_ur.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/atmega8a/fcpu_16mhz/115200_bps/atmega8a_16mhz_115200bps_lednop_fr_ur.hex)|
|244|256|u7.7|`w-u-jpr--`|[atmega32a_lednop_fr_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/atmega32a/fcpu_16mhz/115200_bps/atmega32a_16mhz_115200bps_lednop_fr_ur_vbl.hex)|
|246|256|u7.7|`w-u-jpr--`|[attiny85_rxb4_txb3_lednop_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/attiny85/fcpu_16mhz/115200_bps/attiny85_16mhz_115200bps_rxb4_txb3_lednop_ur_vbl.hex)|
|244|256|u7.7|`w-u-jpr--`|[atmega1280_lednop_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/atmega1280/fcpu_16mhz/115200_bps/atmega1280_16mhz_115200bps_lednop_ur_vbl.hex)|
|244|256|u7.7|`w-u-jpr--`|[atmega1284p_lednop_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/atmega1284p/fcpu_16mhz/115200_bps/atmega1284p_16mhz_115200bps_lednop_ur_vbl.hex)|
|244|256|u7.7|`w-u-jpr--`|[atmega2560_led+b7_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/atmega2560/fcpu_16mhz/115200_bps/atmega2560_16mhz_115200bps_led+b7_ur_vbl.hex)|
|244|256|u7.7|`w-u-jpr--`|[atmega2560_lednop_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/atmega2560/fcpu_16mhz/115200_bps/atmega2560_16mhz_115200bps_lednop_ur_vbl.hex)|
|248|256|u7.7|`w-u-jpr--`|[attiny167_rxb6_txb3_lednop_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/attiny167/fcpu_16mhz/115200_bps/attiny167_16mhz_115200bps_rxb6_txb3_lednop_ur_vbl.hex)|
|248|256|u7.7|`w-u-hpr--`|[atmega88a_lednop_fr_ur.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/atmega88a/fcpu_16mhz/115200_bps/atmega88a_16mhz_115200bps_lednop_fr_ur.hex)|
|252|256|u7.7|`w-u-jpr--`|[atmega328p_8mhz_115200bps_rxd0_txd1_lednop_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/atmega328p/fcpu_8mhz/115200_bps/atmega328p_8mhz_115200bps_rxd0_txd1_lednop_ur_vbl.hex)|
|248|256|u7.7|`w-u-jpr--`|[atmega48a_lednop_fr_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/atmega48a/fcpu_16mhz/115200_bps/atmega48a_16mhz_115200bps_lednop_fr_ur_vbl.hex)|
|250|256|u7.7|`w-u-jpr--`|[atmega644p_lednop_fr_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/atmega644p/fcpu_16mhz/115200_bps/atmega644p_16mhz_115200bps_lednop_fr_ur_vbl.hex)|
|254|256|u7.7|`w-u-jpr--`|[attiny84_rxa3_txa2_lednop_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/attiny84/fcpu_16mhz/115200_bps/attiny84_16mhz_115200bps_rxa3_txa2_lednop_ur_vbl.hex)|
|252|256|u7.7|`w-u-hpr--`|[atmega168p_lednop_fr_ur.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/atmega168p/fcpu_16mhz/115200_bps/atmega168p_16mhz_115200bps_lednop_fr_ur.hex)|
|346|384|u7.7|`weu-jpr-c`|[atmega328p_ee_lednop_fr_ce_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/atmega328p/fcpu_16mhz/115200_bps/atmega328p_16mhz_115200bps_ee_lednop_fr_ce_ur_vbl.hex)|
|364|384|u7.7|`weu-jpr-c`|[atmega328p_8mhz_115200bps_rxd0_txd1_ee_lednop_fr_ce_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/atmega328p/fcpu_8mhz/115200_bps/atmega328p_8mhz_115200bps_rxd0_txd1_ee_lednop_fr_ce_ur_vbl.hex)|
|462|512|u7.7|`wes-hprac`|[atmega328p_autobaud_ee_lednop_fr_ce.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/atmega328p/autobaud/atmega328p_autobaud_ee_lednop_fr_ce.hex)|
|474|512|o8.3|`--s-h-r--`|[optiboot_atmega328.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/src/all/optiboot_atmega328.hex)|
|502|512|u7.7|`weudhprac`|[urclockusb_autobaud_ee_led+d5_csb0_dual_fr_ce_ur.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/board_urclockusb/autobaud/urclockusb_autobaud_ee_led+d5_csb0_dual_fr_ce_ur.hex)|
|510|512|u7.7|`weudhprac`|[atmega328p_autobaud_ee_template_dual_fr_ce_ur.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/atmega328p/autobaud/atmega328p_autobaud_ee_template_dual_fr_ce_ur.hex)|

- **Hex file** naming convention: as above, plus
  + `rxd0 txd1` software I/O using, in this example, lines RX `D0` and TX `D1`
  + `lednop` is a template bootloader with `mov rx,rx` nops as placeholders for LEDs operations
  + `template` bootloaders contains `mov rx,rx` nops as placeholders for LED and CS operations

Note that the USART on an 8 MHz ATmega328P cannot produce 115200 baud within a 2.5% tolerance; the
8 MHz examples shown use software I/O that allows for a finer baud granularity.
