# Comparison table

The table below lists a sample of the range of 184 parts for which
[urboot](https://github.com/stefanrueger/urboot/blob/main/README.md) compiles. It shows their
features alongside vanilla optiboot and its larger bigboot variant for EEPROM access. They are all
for a 16 MHz MCU and 115,200 baud serial communication speed, except where noted differently.


|Size|Usage|Version|Features|Hex file|
|:-:|:-:|:-:|:-:|:--|
|214|224|u7.7|`w-u-jpr--`|[urboot_attiny2313_16mhz_115200bps_uart0_rxd0_txd1_lednop_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot.hex/main/mcus/attiny2313/fcpu_16mhz/115200_bps/urboot_attiny2313_16mhz_115200bps_uart0_rxd0_txd1_lednop_ur_vbl.hex)|
|240|256|u7.7|`w-u-hpr--`|[urboot_atmega16a_16mhz_115200bps_uart0_rxd0_txd1_lednop_fr_ur.hex](https://raw.githubusercontent.com/stefanrueger/urboot.hex/main/mcus/atmega16a/fcpu_16mhz/115200_bps/urboot_atmega16a_16mhz_115200bps_uart0_rxd0_txd1_lednop_fr_ur.hex)|
|240|256|u7.7|`w-u-jpr--`|[urboot_atmega1284p_16mhz_115200bps_uart0_rxd0_txd1_lednop_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot.hex/main/mcus/atmega1284p/fcpu_16mhz/115200_bps/urboot_atmega1284p_16mhz_115200bps_uart0_rxd0_txd1_lednop_ur_vbl.hex)|
|240|256|u7.7|`w-u-jpr--`|[urboot_atmega2560_16mhz_115200bps_uart0_rxe0_txe1_lednop_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot.hex/main/mcus/atmega2560/fcpu_16mhz/115200_bps/urboot_atmega2560_16mhz_115200bps_uart0_rxe0_txe1_lednop_ur_vbl.hex)|
|240|256|u7.7|`w-u-jPr--`|[urboot_atmega48a_16mhz_115200bps_uart0_rxd0_txd1_lednop_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot.hex/main/mcus/atmega48a/fcpu_16mhz/115200_bps/urboot_atmega48a_16mhz_115200bps_uart0_rxd0_txd1_lednop_ur_vbl.hex)|
|244|256|u7.7|`w-u-jpr--`|[urboot_attiny167_8mhz_115200bps_uart0_rxa0_txa1_lednop_fr_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot.hex/main/mcus/attiny167/fcpu_8mhz/115200_bps/urboot_attiny167_8mhz_115200bps_uart0_rxa0_txa1_lednop_fr_ur_vbl.hex)|
|244|256|u7.7|`w-u-hpr--`|[urboot_atmega88a_16mhz_115200bps_uart0_rxd0_txd1_lednop_fr_ur.hex](https://raw.githubusercontent.com/stefanrueger/urboot.hex/main/mcus/atmega88a/fcpu_16mhz/115200_bps/urboot_atmega88a_16mhz_115200bps_uart0_rxd0_txd1_lednop_fr_ur.hex)|
|244|256|u7.7|`w-u-hpr--`|[urboot_atmega168a_16mhz_115200bps_uart0_rxd0_txd1_lednop_fr_ur.hex](https://raw.githubusercontent.com/stefanrueger/urboot.hex/main/mcus/atmega168a/fcpu_16mhz/115200_bps/urboot_atmega168a_16mhz_115200bps_uart0_rxd0_txd1_lednop_fr_ur.hex)|
|244|256|u7.7|`w-u-jPr--`|[urboot_atmega328pb_16mhz_115200bps_uart1_rxb4_txb3_lednop_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot.hex/main/mcus/atmega328pb/fcpu_16mhz/115200_bps/urboot_atmega328pb_16mhz_115200bps_uart1_rxb4_txb3_lednop_ur_vbl.hex)|
|246|256|u7.7|`w-u-jpr--`|[urboot_attiny85_16mhz_115200bps_swio_rxb4_txb3_lednop_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot.hex/main/mcus/attiny85/fcpu_16mhz/115200_bps/urboot_attiny85_16mhz_115200bps_swio_rxb4_txb3_lednop_ur_vbl.hex)|
|246|256|u7.7|`w-u-jpra-`|[urboot_uno_autobaud_uart0_rxd0_txd1_led+b5_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot.hex/main/boards/uno/autobaud/urboot_uno_autobaud_uart0_rxd0_txd1_led+b5_ur_vbl.hex)|
|246|256|u7.7|`w-u-jpr--`|[urboot_attiny84_16mhz_115200bps_swio_rxa3_txa2_lednop_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot.hex/main/mcus/attiny84/fcpu_16mhz/115200_bps/urboot_attiny84_16mhz_115200bps_swio_rxa3_txa2_lednop_ur_vbl.hex)|
|252|256|u7.7|`w-u-jpr--`|[urboot_atmega328p_8mhz_115200bps_swio_rxd0_txd1_lednop_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot.hex/main/mcus/atmega328p/fcpu_8mhz/115200_bps/urboot_atmega328p_8mhz_115200bps_swio_rxd0_txd1_lednop_ur_vbl.hex)|
|370|384|u7.7|`weu-jPrac`|[urboot_atmega328p_autobaud_uart0_rxd0_txd1_ee_lednop_fr_ce_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot.hex/main/mcus/atmega328p/autobaud/urboot_atmega328p_autobaud_uart0_rxd0_txd1_ee_lednop_fr_ce_ur_vbl.hex)|
|376|384|u7.7|`weu-jPr-c`|[urboot_atmega328p_8mhz_115200bps_swio_rxd0_txd1_ee_lednop_fr_ce_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot.hex/main/mcus/atmega328p/fcpu_8mhz/115200_bps/urboot_atmega328p_8mhz_115200bps_swio_rxd0_txd1_ee_lednop_fr_ce_ur_vbl.hex)|
|456|512|u7.7|`wes-hprac`|[urboot_atmega328p_autobaud_uart0_rxd0_txd1_ee_lednop_fr_ce.hex](https://raw.githubusercontent.com/stefanrueger/urboot.hex/main/mcus/atmega328p/autobaud/urboot_atmega328p_autobaud_uart0_rxd0_txd1_ee_lednop_fr_ce.hex)|
|474|512|o8.3|`--s-h-r--`|[optiboot_atmega328.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/src/all/optiboot_atmega328.hex)|
|496|512|u7.7|`weudhprac`|[urboot_urclockusb_autobaud_uart0_rxd0_txd1_ee_led+d5_csb0_dual_fr_ce_ur.hex](https://raw.githubusercontent.com/stefanrueger/urboot.hex/main/boards/urclockusb/autobaud/urboot_urclockusb_autobaud_uart0_rxd0_txd1_ee_led+d5_csb0_dual_fr_ce_ur.hex)|
|504|512|u7.7|`weudhprac`|[urboot_atmega328p_autobaud_uart0_rxd0_txd1_ee_template_dual_fr_ce_ur.hex](https://raw.githubusercontent.com/stefanrueger/urboot.hex/main/mcus/atmega328p/autobaud/urboot_atmega328p_autobaud_uart0_rxd0_txd1_ee_template_dual_fr_ce_ur.hex)|
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
