|Size|Usage|Version|Features|Hex file|
|:-:|:-:|:-:|:-:|:--|
|252|256|u7.7|`w-u-jpra-`|[atmega32u2_autobaud_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/atmega32u2/autobaud/atmega32u2_autobaud_ur_vbl.hex)|
|370|384|u7.7|`weu-jprac`|[atmega32u2_autobaud_ee_lednop_fr_ce_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/atmega32u2/autobaud/atmega32u2_autobaud_ee_lednop_fr_ce_ur_vbl.hex)|
|366|512|u7.7|`weu-hprac`|[atmega32u2_autobaud_ee_lednop_fr_ce_ur.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/atmega32u2/autobaud/atmega32u2_autobaud_ee_lednop_fr_ce_ur.hex)|
|470|512|u7.7|`wes-hprac`|[atmega32u2_autobaud_ee_lednop_fr_ce.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/atmega32u2/autobaud/atmega32u2_autobaud_ee_lednop_fr_ce.hex)|

- **Size:** Bootloader code size including small table at top end
- **Usage:** How many bytes of flash are needed, ie, HW boot section or a multiple of the page size
- **Version:** For example, u7.6 is an urboot version, o5.2 is an optiboot version
- **Features:**
  + `w` bootloader provides `pgm_write_page(sram, flash)` for the application at `FLASHEND-4+1`
  + `e` EEPROM read/write support
  + `u` uses urprotocol requiring `avrdude -c urclock` for programming
  + `s` uses skeleton of STK500v1 protocol (deprecated); `-c urclock` and `-c arduino` both work
  + `h` hardware boot section: make sure fuses are set for reset to jump to boot section
  + `j` vector bootloader: applications *need to be patched externally*, eg, using `avrdude -c urclock`
  + `p` bootloader protects itself from being overwritten
  + `r` preserves reset flags for the application in the register R2
  + `a` autobaud detection (f_cpu/8n using discrete divisors, n = 1, 2, ..., 256)
  + `c` bootloader provides chip erase functionality (only recommended for large MCUs)
  + `-` corresponding feature not present
- **Hex file:** typically MCU name, oscillator frequency (16 MHz default) and baud rate (115200 default) followed by
  + `autobaud` tries to match host baud rate; can be f/8, f/16, f/24, ..., f/2048 (f=F<sub>CPU</sub>)
  + `ee` bootloader supports EEPROM read/write
  + `lednop` is a template bootloader with `mov rx,rx` nops as placeholders for LEDs operations
  + `fr` bootloader provides non-essential code for smoother error handing
  + `ce` bootloader provides a chip erase command
  + `ur` uses urprotocol and requires `avrdude -c urclock` for programming
  + `vbl` vector bootloader: set fuses to jump to reset, not the HW boot section
