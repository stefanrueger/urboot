|Size|Usage|Version|Features|Hex file|
|:-:|:-:|:-:|:-:|:--|
|250|256|u7.7|`w-u-jpra-`|[urboot_moteino_autobaud_led+b1_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/board_moteino/autobaud/urboot_moteino_autobaud_led+b1_ur_vbl.hex)|
|376|384|u7.7|`weu-jPrac`|[urboot_moteino_autobaud_ee_led+b1_fr_ce_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/board_moteino/autobaud/urboot_moteino_autobaud_ee_led+b1_fr_ce_ur_vbl.hex)|
|358|512|u7.7|`weu-hprac`|[urboot_moteino_autobaud_ee_led+b1_fr_ce_ur.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/board_moteino/autobaud/urboot_moteino_autobaud_ee_led+b1_fr_ce_ur.hex)|
|450|512|u7.7|`w-udjPrac`|[urboot_moteino_autobaud_led+b1_csb0_dual_fr_ce_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/board_moteino/autobaud/urboot_moteino_autobaud_led+b1_csb0_dual_fr_ce_ur_vbl.hex)|
|462|512|u7.7|`wes-hprac`|[urboot_moteino_autobaud_ee_led+b1_fr_ce.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/board_moteino/autobaud/urboot_moteino_autobaud_ee_led+b1_fr_ce.hex)|
|488|512|u7.7|`weudjPra-`|[urboot_moteino_autobaud_ee_led+b1_csb0_dual_fr_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/board_moteino/autobaud/urboot_moteino_autobaud_ee_led+b1_csb0_dual_fr_ur_vbl.hex)|
|502|512|u7.7|`weudhprac`|[urboot_moteino_autobaud_ee_led+b1_csb0_dual_fr_ce_ur.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/board_moteino/autobaud/urboot_moteino_autobaud_ee_led+b1_csb0_dual_fr_ce_ur.hex)|
|508|512|u7.7|`w-sdhpra-`|[urboot_moteino_autobaud_led+b1_csb0_dual_fr.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/board_moteino/autobaud/urboot_moteino_autobaud_led+b1_csb0_dual_fr.hex)|
|606|1024|u7.7|`wesdhprac`|[urboot_moteino_autobaud_ee_led+b1_csb0_dual_fr_ce.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/board_moteino/autobaud/urboot_moteino_autobaud_ee_led+b1_csb0_dual_fr_ce.hex)|

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
  + `ee` bootloader supports EEPROM read/write
  + `led-b1` toggles an active-low LED on pin `B1`, `+` designates an active-high LED
  + `csb0` for dual boot uses, in this example, pin B0 as chip select of external SPI flash memory
  + `dual` boot: serial and from external SPI flash memory
  + `fr` bootloader provides non-essential code for smoother error handing
  + `ce` bootloader provides a chip erase command
  + `ur` uses urprotocol and requires `avrdude -c urclock` for programming
  + `d` dual boot
  + `vbl` vector bootloader: set fuses to jump to reset, not the HW boot section
