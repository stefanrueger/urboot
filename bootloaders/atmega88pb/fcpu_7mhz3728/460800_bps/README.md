|Size|Usage|Version|Features|Hex file|
|:-:|:-:|:-:|:-:|:--|
|256|256|u7.6|`w-u-hpr`|[atmega88pb_7mhz3728_460800bps_lednop_fr_ur.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/atmega88pb/fcpu_7mhz3728/460800_bps/atmega88pb_7mhz3728_460800bps_lednop_fr_ur.hex)|
|256|256|u7.6|`w-u-jpr`|[atmega88pb_7mhz3728_460800bps_lednop_fr_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/atmega88pb/fcpu_7mhz3728/460800_bps/atmega88pb_7mhz3728_460800bps_lednop_fr_ur_vbl.hex)|
|288|320|u7.6|`w-u-jpr`|[atmega88pb_7mhz3728_460800bps_lednop_fr_ce_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/atmega88pb/fcpu_7mhz3728/460800_bps/atmega88pb_7mhz3728_460800bps_lednop_fr_ce_ur_vbl.hex)|
|304|320|u7.6|`weu-jpr`|[atmega88pb_7mhz3728_460800bps_ee_lednop_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/atmega88pb/fcpu_7mhz3728/460800_bps/atmega88pb_7mhz3728_460800bps_ee_lednop_ur_vbl.hex)|
|350|384|u7.6|`weu-jpr`|[atmega88pb_7mhz3728_460800bps_ee_lednop_fr_ce_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/atmega88pb/fcpu_7mhz3728/460800_bps/atmega88pb_7mhz3728_460800bps_ee_lednop_fr_ce_ur_vbl.hex)|
|346|512|u7.6|`weu-hpr`|[atmega88pb_7mhz3728_460800bps_ee_lednop_fr_ce_ur.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/atmega88pb/fcpu_7mhz3728/460800_bps/atmega88pb_7mhz3728_460800bps_ee_lednop_fr_ce_ur.hex)|
|460|512|u7.6|`wes-hpr`|[atmega88pb_7mhz3728_460800bps_ee_lednop_fr_ce.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/atmega88pb/fcpu_7mhz3728/460800_bps/atmega88pb_7mhz3728_460800bps_ee_lednop_fr_ce.hex)|

- **Size:** Bootloader code size including small table at top end
- **Usage:** How many bytes of flash are needed, ie, HW boot section or a multiple of the page size
- **Version:** For example, u7.6 is an urboot version, o5.2 is an optiboot version
- **Features:**
  + `w` urboot provides `pgm_write_page(sram, flash)` for the application at `FLASHEND-4+1`
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
