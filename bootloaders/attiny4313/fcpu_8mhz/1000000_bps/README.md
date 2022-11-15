|Size|Usage|Version|Features|Hex file|
|:-:|:-:|:-:|:-:|:--|
|238|256|u7.7|`w-u-jpr--`|[attiny4313_8mhz_1000000bps_lednop_fr_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/attiny4313/fcpu_8mhz/1000000_bps/attiny4313_8mhz_1000000bps_lednop_fr_ur_vbl.hex)|
|270|320|u7.7|`w-u-jpr-c`|[attiny4313_8mhz_1000000bps_lednop_fr_ce_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/attiny4313/fcpu_8mhz/1000000_bps/attiny4313_8mhz_1000000bps_lednop_fr_ce_ur_vbl.hex)|
|306|320|u7.7|`weu-jpr--`|[attiny4313_8mhz_1000000bps_ee_lednop_fr_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/attiny4313/fcpu_8mhz/1000000_bps/attiny4313_8mhz_1000000bps_ee_lednop_fr_ur_vbl.hex)|
|334|384|u7.7|`weu-jpr-c`|[attiny4313_8mhz_1000000bps_ee_lednop_fr_ce_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/attiny4313/fcpu_8mhz/1000000_bps/attiny4313_8mhz_1000000bps_ee_lednop_fr_ce_ur_vbl.hex)|

- **Size:** Bootloader code size including small table at top end
- **Usage:** How many bytes of flash are needed, ie, HW boot section or a multiple of the page size
- **Version:** For example, u7.6 is an urboot version, o5.2 is an optiboot version
- **Features:**
  + `w` bootloader provides `pgm_write_page(sram, flash)` for the application at `FLASHEND-4+1`
  + `e` EEPROM read/write support
  + `u` uses urprotocol requiring `avrdude -c urclock` for programming
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
