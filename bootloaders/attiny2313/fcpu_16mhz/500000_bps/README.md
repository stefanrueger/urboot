|Size|Usage|Version|Features|Hex file|
|:-:|:-:|:-:|:-:|:--|
|218|224|u7.6|`w-u-jpr`|[attiny2313_16mhz_500000bps_lednop_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/attiny2313/fcpu_16mhz/500000_bps/attiny2313_16mhz_500000bps_lednop_ur_vbl.hex)|
|232|256|u7.6|`w-u-jpr`|[attiny2313_16mhz_500000bps_lednop_fr_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/attiny2313/fcpu_16mhz/500000_bps/attiny2313_16mhz_500000bps_lednop_fr_ur_vbl.hex)|
|262|288|u7.6|`w-u-jpr`|[attiny2313_16mhz_500000bps_lednop_fr_ce_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/attiny2313/fcpu_16mhz/500000_bps/attiny2313_16mhz_500000bps_lednop_fr_ce_ur_vbl.hex)|
|282|288|u7.6|`weu-jpr`|[attiny2313_16mhz_500000bps_ee_lednop_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/attiny2313/fcpu_16mhz/500000_bps/attiny2313_16mhz_500000bps_ee_lednop_ur_vbl.hex)|
|300|320|u7.6|`weu-jpr`|[attiny2313_16mhz_500000bps_ee_lednop_fr_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/attiny2313/fcpu_16mhz/500000_bps/attiny2313_16mhz_500000bps_ee_lednop_fr_ur_vbl.hex)|
|326|352|u7.6|`weu-jpr`|[attiny2313_16mhz_500000bps_ee_lednop_fr_ce_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/attiny2313/fcpu_16mhz/500000_bps/attiny2313_16mhz_500000bps_ee_lednop_fr_ce_ur_vbl.hex)|

- **Size:** Bootloader code size including small table at top end
- **Usage:** How many bytes of flash are needed, ie, HW boot section or a multiple of the page size
- **Version:** For example, u7.6 is an urboot version, o5.2 is an optiboot version
- **Features:**
  + `w` urboot provides `pgm_write_page(sram, flash)` for the application at `FLASHEND-4+1`
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
