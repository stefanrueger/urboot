# Comparison table

The table below lists a sample of the range of 184 parts for which
[urboot](https://github.com/stefanrueger/urboot/blob/main/README.md) compiles. It shows their
features alongside vanilla optiboot and its larger bigboot variant for EEPROM access. They are all
for a 16 MHz MCU and 115,200 baud serial communication speed, except where noted differently.


|Size|Usage|Version|Features|Hex file|
|:-:|:-:|:-:|:-:|:--|
|214|224|u7.7|`w-u-jpr--`|[urboot+attiny2313+16m0000x++115k2_uart0_rxd0_txd1_lednop.hex](https://raw.githubusercontent.com/stefanrueger/urboot.hex/main/mcus/attiny2313/external_oscillator/fcpu+16m0000_Hz/br++115k2_bps/urboot+attiny2313+16m0000x++115k2_uart0_rxd0_txd1_lednop.hex)|
|240|256|u7.7|`w-u-hpr--`|[urboot+atmega16a+16m0000x++115k2_uart0_rxd0_txd1_lednop_fr_hw.hex](https://raw.githubusercontent.com/stefanrueger/urboot.hex/main/mcus/atmega16a/external_oscillator/fcpu+16m0000_Hz/br++115k2_bps/urboot+atmega16a+16m0000x++115k2_uart0_rxd0_txd1_lednop_fr_hw.hex)|
|240|256|u7.7|`w-u-jpr--`|[urboot+atmega1284p+16m0000x++115k2_uart0_rxd0_txd1_lednop.hex](https://raw.githubusercontent.com/stefanrueger/urboot.hex/main/mcus/atmega1284p/external_oscillator/fcpu+16m0000_Hz/br++115k2_bps/urboot+atmega1284p+16m0000x++115k2_uart0_rxd0_txd1_lednop.hex)|
|240|256|u7.7|`w-u-jpr--`|[urboot+atmega2560+16m0000x++115k2_uart0_rxe0_txe1_lednop.hex](https://raw.githubusercontent.com/stefanrueger/urboot.hex/main/mcus/atmega2560/external_oscillator/fcpu+16m0000_Hz/br++115k2_bps/urboot+atmega2560+16m0000x++115k2_uart0_rxe0_txe1_lednop.hex)|
|240|256|u7.7|`w-u-jPr--`|[urboot+atmega48a+16m0000x++115k2_uart0_rxd0_txd1_lednop.hex](https://raw.githubusercontent.com/stefanrueger/urboot.hex/main/mcus/atmega48a/external_oscillator/fcpu+16m0000_Hz/br++115k2_bps/urboot+atmega48a+16m0000x++115k2_uart0_rxd0_txd1_lednop.hex)|
|244|256|u7.7|`w-u-jpr--`|[urboot+attiny167++8m0000x++115k2_uart0_rxa0_txa1_lednop_fr.hex](https://raw.githubusercontent.com/stefanrueger/urboot.hex/main/mcus/attiny167/external_oscillator/fcpu++8m0000_Hz/br++115k2_bps/urboot+attiny167++8m0000x++115k2_uart0_rxa0_txa1_lednop_fr.hex)|
|244|256|u7.7|`w-u-hpr--`|[urboot+atmega88a+16m0000x++115k2_uart0_rxd0_txd1_lednop_fr_hw.hex](https://raw.githubusercontent.com/stefanrueger/urboot.hex/main/mcus/atmega88a/external_oscillator/fcpu+16m0000_Hz/br++115k2_bps/urboot+atmega88a+16m0000x++115k2_uart0_rxd0_txd1_lednop_fr_hw.hex)|
|244|256|u7.7|`w-u-hpr--`|[urboot+atmega168a+16m0000x++115k2_uart0_rxd0_txd1_lednop_fr_hw.hex](https://raw.githubusercontent.com/stefanrueger/urboot.hex/main/mcus/atmega168a/external_oscillator/fcpu+16m0000_Hz/br++115k2_bps/urboot+atmega168a+16m0000x++115k2_uart0_rxd0_txd1_lednop_fr_hw.hex)|
|244|256|u7.7|`w-u-jPr--`|[urboot+atmega328pb+16m0000x++115k2_uart1_rxb4_txb3_lednop.hex](https://raw.githubusercontent.com/stefanrueger/urboot.hex/main/mcus/atmega328pb/external_oscillator/fcpu+16m0000_Hz/br++115k2_bps/urboot+atmega328pb+16m0000x++115k2_uart1_rxb4_txb3_lednop.hex)|
|246|256|u7.7|`w-u-jpr--`|[urboot+attiny85+16m0000x++115k2_swio_rxb4_txb3_lednop.hex](https://raw.githubusercontent.com/stefanrueger/urboot.hex/main/mcus/attiny85/external_oscillator/fcpu+16m0000_Hz/br++115k2_bps/urboot+attiny85+16m0000x++115k2_swio_rxb4_txb3_lednop.hex)|
|246|256|u7.7|`w-u-jpra-`|[urboot+uno+autobaud_uart0_rxd0_txd1_led+b5.hex](https://raw.githubusercontent.com/stefanrueger/urboot.hex/main/boards/uno/autobaud/urboot+uno+autobaud_uart0_rxd0_txd1_led+b5.hex)|
|246|256|u7.7|`w-u-jpr--`|[urboot+attiny84+16m0000x++115k2_swio_rxa3_txa2_lednop.hex](https://raw.githubusercontent.com/stefanrueger/urboot.hex/main/mcus/attiny84/external_oscillator/fcpu+16m0000_Hz/br++115k2_bps/urboot+attiny84+16m0000x++115k2_swio_rxa3_txa2_lednop.hex)|
|252|256|u7.7|`w-u-jpr--`|[urboot+atmega328p++8m0000x++115k2_swio_rxd0_txd1_lednop.hex](https://raw.githubusercontent.com/stefanrueger/urboot.hex/main/mcus/atmega328p/external_oscillator/fcpu++8m0000_Hz/br++115k2_bps/urboot+atmega328p++8m0000x++115k2_swio_rxd0_txd1_lednop.hex)|
|370|384|u7.7|`weu-jPrac`|[urboot+atmega328p+autobaud_uart0_rxd0_txd1_ee_lednop_fr_ce.hex](https://raw.githubusercontent.com/stefanrueger/urboot.hex/main/mcus/atmega328p/autobaud/urboot+atmega328p+autobaud_uart0_rxd0_txd1_ee_lednop_fr_ce.hex)|
|376|384|u7.7|`weu-jPr-c`|[urboot+atmega328p++8m0000x++115k2_swio_rxd0_txd1_ee_lednop_fr_ce.hex](https://raw.githubusercontent.com/stefanrueger/urboot.hex/main/mcus/atmega328p/external_oscillator/fcpu++8m0000_Hz/br++115k2_bps/urboot+atmega328p++8m0000x++115k2_swio_rxd0_txd1_ee_lednop_fr_ce.hex)|
|456|512|u7.7|`wes-hprac`|[urboot+atmega328p+autobaud_uart0_rxd0_txd1_ee_lednop_fr_ce_stk500_hw.hex](https://raw.githubusercontent.com/stefanrueger/urboot.hex/main/mcus/atmega328p/autobaud/urboot+atmega328p+autobaud_uart0_rxd0_txd1_ee_lednop_fr_ce_stk500_hw.hex)|
|474|512|o8.3|`--s-h-r--`|[optiboot_atmega328.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/src/all/optiboot_atmega328.hex)|
|496|512|u7.7|`weudhprac`|[urboot+urclockusb+autobaud_uart0_rxd0_txd1_ee_led+d5_csb0_dual_fr_ce_hw.hex](https://raw.githubusercontent.com/stefanrueger/urboot.hex/main/boards/urclockusb/autobaud/urboot+urclockusb+autobaud_uart0_rxd0_txd1_ee_led+d5_csb0_dual_fr_ce_hw.hex)|
|504|512|u7.7|`weudhprac`|[urboot+atmega328p+autobaud_uart0_rxd0_txd1_ee_template_dual_fr_ce_hw.hex](https://raw.githubusercontent.com/stefanrueger/urboot.hex/main/mcus/atmega328p/autobaud/urboot+atmega328p+autobaud_uart0_rxd0_txd1_ee_template_dual_fr_ce_hw.hex)|
|710|1024|o8.3|`-es-h-r--`|[bigboot_328.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/src/all/bigboot_328.hex)|

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
  + `c` bootloader provides chip erase functionality (only recommended for large MCUs)
  + `-` corresponding feature not present
- **Hex file:** typically MCU name, oscillator frequency (16 MHz default) and baud rate (115200 default) followed by
  + `autobaud` tries to match host baud rate; can be f/8, f/16, f/24, ..., f/2048 (f=F<sub>CPU</sub>)
  + `uart0` UART number, in this case `0`
  + `rxd0 txd1` I/O using, in this example, lines RX `D0` and TX `D1`
  + `ee` bootloader supports EEPROM read/write
  + `led-b1` toggles an active-low LED on pin `B1`, `+` designates an active-high LED
  + `lednop` is a template bootloader with `mov rx,rx` nops as placeholders for LED operations
  + `csb0` for dual boot uses, in this example, pin B0 as chip select of external SPI flash memory
  + `template` bootloaders contains `mov rx,rx` nops as placeholders for LED and CS operations
  + `dual` boot: serial and from external SPI flash memory
  + `fr` bootloader provides non-essential code for smoother error handing
  + `ce` bootloader provides a chip erase command
  + `ur` uses urprotocol and requires `avrdude -c urclock` for programming
  + `d` dual boot
  + `vbl` vector bootloader: set fuses to jump to reset, not the HW boot section


Note that the classic UART on an 8 MHz ATmega328P *cannot* produce 115200 baud within a 2.5%
tolerance; the 8 MHz examples shown use software I/O that allows for a finer baud granularity. In
contrast to this, the LIN/UART of an 8 MHz ATtiny167 *can* produce 115200 baud within 1%
tolerance.
