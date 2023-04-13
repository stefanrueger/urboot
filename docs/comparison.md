# Comparison table

The table below lists a sample of the range of 184 parts for which
[urboot](https://github.com/stefanrueger/urboot/blob/main/README.md) compiles. It shows their
features alongside vanilla optiboot and its larger bigboot variant for EEPROM access. They are all
for a 16 MHz MCU and 115,200 baud serial communication speed, except where noted differently.


|Size|Usage|Version|Features|Hex file|
|:-:|:-:|:-:|:-:|:--|
|222|224|u7.7|`w-u-jpr--`|[urboot_t2313_1s_x16m0_115k2_uart0_rxd0_txd1_lednop.hex](https://raw.githubusercontent.com/stefanrueger/urboot.hex/main/mcus/attiny2313/watchdog_1_s/external_oscillator_x/16m000000_hz/+115k2_baud/uart0_rxd0_txd1/lednop/urboot_t2313_1s_x16m0_115k2_uart0_rxd0_txd1_lednop.hex)|
|238|256|u7.7|`w-u-hpr--`|[urboot_atmega16a_16m0x_+115k2_uart0_rxd0_txd1_lednop_fr_hw.hex](https://raw.githubusercontent.com/stefanrueger/urboot.hex/main/mcus/atmega16a/watchdog_1_s/external_oscillator_x/16m000000_hz/+115k2_baud/uart0_rxd0_txd1/lednop/urboot_m16a_1s_x16m0_115k2_uart0_rxd0_txd1_lednop_hw.hex)|
|242|256|u7.7|`w-u-hpr--`|[urboot_m168a_1s_x16m0_115k2_uart0_rxd0_txd1_lednop_hw.hex](https://raw.githubusercontent.com/stefanrueger/urboot.hex/main/mcus/atmega168a/watchdog_1_s/external_oscillator_x/16m000000_hz/+115k2_baud/uart0_rxd0_txd1/lednop/urboot_m168a_1s_x16m0_115k2_uart0_rxd0_txd1_lednop_hw.hex)|
|242|256|u7.7|`w-u-hpr--`|[urboot_m88a_1s_x16m0_115k2_uart0_rxd0_txd1_lednop_hw.hex](https://raw.githubusercontent.com/stefanrueger/urboot.hex/main/mcus/atmega88a/watchdog_1_s/external_oscillator_x/16m000000_hz/+115k2_baud/uart0_rxd0_txd1/lednop/urboot_m88a_1s_x16m0_115k2_uart0_rxd0_txd1_lednop_hw.hex)|
|252|256|u7.7|`w-u-jPr--`|[urboot_m328pb_1s_x16m0_115k2_uart1_rxb4_txb3_lednop.hex](https://raw.githubusercontent.com/stefanrueger/urboot.hex/main/mcus/atmega328pb/watchdog_1_s/external_oscillator_x/16m000000_hz/+115k2_baud/uart1_rxb4_txb3/lednop/urboot_m328pb_1s_x16m0_115k2_uart1_rxb4_txb3_lednop.hex)|
|254|256|u7.7|`w-u-jpra-`|[urboot_m328p_1s_autobaud_uart0_rxd0_txd1_led+b5.hex](https://raw.githubusercontent.com/stefanrueger/urboot.hex/main/boards/uno/atmega328p/watchdog_1_s/autobaud/uart0_rxd0_txd1/led+b5/urboot_m328p_1s_autobaud_uart0_rxd0_txd1_led+b5.hex)|
|254|256|u7.7|`w-u-jpr--`|[urboot_m328p_1s_x8m0_115k2_swio_rxd0_txd1_lednop.hex](https://raw.githubusercontent.com/stefanrueger/urboot.hex/main/mcus/atmega328p/watchdog_1_s/external_oscillator_x/+8m000000_hz/+115k2_baud/uart0_rxd0_txd1/lednop/urboot_m328p_1s_x8m0_115k2_swio_rxd0_txd1_lednop.hex)|
|254|256|u7.7|`w-u-jpr--`|[urboot_t84_1s_x16m0_115k2_swio_rxa3_txa2_lednop.hex](https://raw.githubusercontent.com/stefanrueger/urboot.hex/main/mcus/attiny84/watchdog_1_s/external_oscillator_x/16m000000_hz/+115k2_baud/swio_rxa3_txa2/lednop/urboot_t84_1s_x16m0_115k2_swio_rxa3_txa2_lednop.hex)|
|254|256|u7.7|`w-u-jPr--`|[urboot_t85_1s_x16m0_115k2_swio_rxb4_txb3_lednop.hex](https://raw.githubusercontent.com/stefanrueger/urboot.hex/main/mcus/attiny85/watchdog_1_s/external_oscillator_x/16m000000_hz/+115k2_baud/swio_rxb4_txb3/lednop/urboot_t85_1s_x16m0_115k2_swio_rxb4_txb3_lednop.hex)|
|256|256|u7.7|`w-u-jPr--`|[urboot_m1284p_1s_x16m0_115k2_uart0_rxd0_txd1_lednop.hex](https://raw.githubusercontent.com/stefanrueger/urboot.hex/main/mcus/atmega1284p/watchdog_1_s/external_oscillator_x/16m000000_hz/+115k2_baud/uart0_rxd0_txd1/lednop/urboot_m1284p_1s_x16m0_115k2_uart0_rxd0_txd1_lednop.hex)|
|256|256|u7.7|`w-u-jPr--`|[urboot_m2560_1s_x16m0_115k2_uart0_rxe0_txe1_lednop.hex](https://raw.githubusercontent.com/stefanrueger/urboot.hex/main/mcus/atmega2560/watchdog_1_s/external_oscillator_x/16m000000_hz/+115k2_baud/uart0_rxe0_txe1/lednop/urboot_m2560_1s_x16m0_115k2_uart0_rxe0_txe1_lednop.hex)|
|256|256|u7.7|`w-u-jPr--`|[urboot_m48a_1s_x16m0_115k2_uart0_rxd0_txd1_lednop.hex](https://raw.githubusercontent.com/stefanrueger/urboot.hex/main/mcus/atmega48a/watchdog_1_s/external_oscillator_x/16m000000_hz/+115k2_baud/uart0_rxd0_txd1/lednop/urboot_m48a_1s_x16m0_115k2_uart0_rxd0_txd1_lednop.hex)|
|256|256|u7.7|`w-u-jPr--`|[urboot_t167_1s_x8m0_115k2_uart0_rxa0_txa1_lednop.hex](https://raw.githubusercontent.com/stefanrueger/urboot.hex/main/mcus/attiny167/watchdog_1_s/external_oscillator_x/+8m000000_hz/+115k2_baud/uart0_rxa0_txa1/lednop/urboot_t167_1s_x8m0_115k2_uart0_rxa0_txa1_lednop.hex)|
|368|384|u7.7|`weu-jPrac`|[urboot_m328p_1s_autobaud_uart0_rxd0_txd1_lednop_pr_ee_ce.hex](https://raw.githubusercontent.com/stefanrueger/urboot.hex/main/mcus/atmega328p/watchdog_1_s/autobaud/uart0_rxd0_txd1/lednop/urboot_m328p_1s_autobaud_uart0_rxd0_txd1_lednop_pr_ee_ce.hex)|
|374|384|u7.7|`weu-jPr-c`|[urboot_m328p_1s_x8m0_115k2_swio_rxd0_txd1_lednop_pr_ee_ce.hex](https://raw.githubusercontent.com/stefanrueger/urboot.hex/main/mcus/atmega328p/watchdog_1_s/external_oscillator_x/+8m000000_hz/+115k2_baud/uart0_rxd0_txd1/lednop/urboot_m328p_1s_x8m0_115k2_swio_rxd0_txd1_lednop_pr_ee_ce.hex)|
|454|512|u7.7|`wes-hprac`|[urboot_m328p_1s_autobaud_uart0_rxd0_txd1_lednop_ee_ce_hw_stk500.hex](https://raw.githubusercontent.com/stefanrueger/urboot.hex/main/mcus/atmega328p/watchdog_1_s/autobaud/uart0_rxd0_txd1/lednop/urboot_m328p_1s_autobaud_uart0_rxd0_txd1_lednop_ee_ce_hw_stk500.hex)|
|474|512|o8.3|`--s-h-r--`|[optiboot_atmega328.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/src/all/optiboot_atmega328.hex)|
|496|512|u7.7|`weudhprac`|[urboot_m328p_1s_autobaud_uart0_rxd0_txd1_led+d5_csb0_dual_ee_ce_hw.hex](https://raw.githubusercontent.com/stefanrueger/urboot.hex/main/boards/urclockusb/atmega328p/watchdog_1_s/autobaud/uart0_rxd0_txd1/led+d5_csb0_dual/urboot_m328p_1s_autobaud_uart0_rxd0_txd1_led+d5_csb0_dual_ee_ce_hw.hex)|
|504|512|u7.7|`weudhprac`|[urboot_m328p_1s_autobaud_uart0_rxd0_txd1_template_dual_ee_ce_hw.hex](https://raw.githubusercontent.com/stefanrueger/urboot.hex/main/mcus/atmega328p/watchdog_1_s/autobaud/uart0_rxd0_txd1/template_dual/urboot_m328p_1s_autobaud_uart0_rxd0_txd1_template_dual_ee_ce_hw.hex)|
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
  + `c` bootloader provides chip erase functionality (recommended for large MCUs)
  + `-` corresponding feature not present
- **Hex file:** often qualified by the MCU name and/or configuration
  + `1s` watchdog timeout, ie, time window for upload after external reset
  + `x16m0` is F<sub>CPU</sub> of an external oscillator, here 16.0 MHz
  + `115k2` shows the fixed communication baud rate, here 115200 baud
  + `autobaud` detects host baud rate f/8, f/16, f/24, ..., f/2048 (f=F<sub>CPU</sub>)
  + `swio` software I/O (not UART)
  + `uart0` UART number
  + `rxd0 txd1` I/O using, in this example, lines RX `D0` and TX `D1`
  + `led+b5` toggles an active-high (`+`) LED on pin `B5`
  + `lednop` is a template bootloader with `mov rx,rx` nops as placeholders for LED operations
  + `csb0` uses pin B0 as chip select of external SPI flash memory for dual boot
  + `template` bootloaders contains `mov rx,rx` nops as placeholders for LED and CS operations
  + `dual` can upload from external SPI flash memory and from serial interface
  + `pr` vector bootloader protecting the reset vector
  + `ee` bootloader supports EEPROM read/write
  + `ce` bootloader provides a chip erase command
  + `hw` hardware supported bootloader: set fuses to jump to the HW boot section, not to addr 0
  + `stk500` uses deprecated STK500v1 protocol to communicate with bootloader

Note that the classic UART on an 8 MHz ATmega328P *cannot* produce 115200 baud within a 2.5%
tolerance; the 8 MHz examples shown use software I/O that allows for a finer baud granularity. In
contrast to this, the LIN/UART of an 8 MHz ATtiny167 *can* produce 115200 baud within 1%
tolerance.
