|Size|Usage|Version|Features|Hex file|
|:-:|:-:|:-:|:-:|:--|
|248|256|u7.7|`w-u-hpr--`|[atmega164pa_7mhz3728_19200bps_lednop_fr_ur.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/atmega164pa/fcpu_7mhz3728/19200_bps/atmega164pa_7mhz3728_19200bps_lednop_fr_ur.hex)|
|342|384|u7.7|`weu-jpr-c`|[atmega164pa_7mhz3728_19200bps_ee_lednop_fr_ce_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/atmega164pa/fcpu_7mhz3728/19200_bps/atmega164pa_7mhz3728_19200bps_ee_lednop_fr_ce_ur_vbl.hex)|
|338|512|u7.7|`weu-hpr-c`|[atmega164pa_7mhz3728_19200bps_ee_lednop_fr_ce_ur.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/atmega164pa/fcpu_7mhz3728/19200_bps/atmega164pa_7mhz3728_19200bps_ee_lednop_fr_ce_ur.hex)|
|442|512|u7.7|`wes-hpr-c`|[atmega164pa_7mhz3728_19200bps_ee_lednop_fr_ce.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/atmega164pa/fcpu_7mhz3728/19200_bps/atmega164pa_7mhz3728_19200bps_ee_lednop_fr_ce.hex)|

- **Size:** Bootloader code size including small table at top end
- **Usage:** How many bytes of flash are needed, ie, HW boot section or a multiple of the page size
- **Version:** For example, u7.6 is an urboot version, o5.2 is an optiboot version
- **Features:**
  + `w` bootloader provides `pgm_write_page(sram, flash)` for the application at `FLASHEND-4+1`
  + `e` EEPROM read/write support
  + `u` uses urprotocol requiring `avrdude -c urclock` for programming
  + `s` uses skeleton of STK500v1 protocol (deprecated); `-c urclock` and `-c arduino` both work
  + `h` hardware boot section: make sure fuses are set for reset to jump to boot section
  + `j` vector bootloader: uploaded applications *need to be patched externally*, eg, using `avrdude -c urclock`
  + `p` bootloader protects itself from being overwritten
  + `r` preserves reset flags for the application in the register R2
- **Hex file:** typically MCU name, oscillator frequency (16 MHz default) and baud rate (115200 default) followed by
  + `ee` bootloader supports EEPROM read/write
  + `lednop` is a template bootloader with `mov rx,rx` nops as placeholders for LEDs operations
  + `fr` bootloader provides non-essential code for smoother error handing
  + `ce` bootloader provides a chip erase command
  + `ur` uses urprotocol and requires `avrdude -c urclock` for programming
  + `vbl` vector bootloader: set fuses to jump to reset, not the HW boot section
