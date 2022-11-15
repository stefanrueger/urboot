|Size|Usage|Version|Features|Hex file|
|:-:|:-:|:-:|:-:|:--|
|240|256|u7.6|`w-u-jpr`|[moteinomega_7mhz3728_230400bps_led+d7_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/board_moteinomega/fcpu_7mhz3728/230400_bps/moteinomega_7mhz3728_230400bps_led+d7_ur_vbl.hex)|
|364|512|u7.6|`weu-jpr`|[moteinomega_7mhz3728_230400bps_ee_led+d7_fr_ce_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/board_moteinomega/fcpu_7mhz3728/230400_bps/moteinomega_7mhz3728_230400bps_ee_led+d7_fr_ce_ur_vbl.hex)|
|472|512|u7.6|`w-udjpr`|[moteinomega_7mhz3728_230400bps_led+d7_csc7_dual_fr_ce_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/board_moteinomega/fcpu_7mhz3728/230400_bps/moteinomega_7mhz3728_230400bps_led+d7_csc7_dual_fr_ce_ur_vbl.hex)|
|490|512|u7.6|`weudjpr`|[moteinomega_7mhz3728_230400bps_ee_led+d7_csc7_dual_fr_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/board_moteinomega/fcpu_7mhz3728/230400_bps/moteinomega_7mhz3728_230400bps_ee_led+d7_csc7_dual_fr_ur_vbl.hex)|
|534|768|u7.6|`weudjpr`|[moteinomega_7mhz3728_230400bps_ee_led+d7_csc7_dual_fr_ce_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/board_moteinomega/fcpu_7mhz3728/230400_bps/moteinomega_7mhz3728_230400bps_ee_led+d7_csc7_dual_fr_ce_ur_vbl.hex)|
|364|1024|u7.6|`weu-hpr`|[moteinomega_7mhz3728_230400bps_ee_led+d7_fr_ce_ur.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/board_moteinomega/fcpu_7mhz3728/230400_bps/moteinomega_7mhz3728_230400bps_ee_led+d7_fr_ce_ur.hex)|
|480|1024|u7.6|`wes-hpr`|[moteinomega_7mhz3728_230400bps_ee_led+d7_fr_ce.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/board_moteinomega/fcpu_7mhz3728/230400_bps/moteinomega_7mhz3728_230400bps_ee_led+d7_fr_ce.hex)|
|534|1024|u7.6|`weudhpr`|[moteinomega_7mhz3728_230400bps_ee_led+d7_csc7_dual_fr_ce_ur.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/board_moteinomega/fcpu_7mhz3728/230400_bps/moteinomega_7mhz3728_230400bps_ee_led+d7_csc7_dual_fr_ce_ur.hex)|
|650|1024|u7.6|`wesdhpr`|[moteinomega_7mhz3728_230400bps_ee_led+d7_csc7_dual_fr_ce.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/board_moteinomega/fcpu_7mhz3728/230400_bps/moteinomega_7mhz3728_230400bps_ee_led+d7_csc7_dual_fr_ce.hex)|

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
  + `j` vector bootloader: uploaded applications *need to be patched externally*, eg, using `avrdude -c urclock`
  + `p` bootloader protects itself from being overwritten
  + `r` preserves reset flags for the application in the register R2
- **Hex file:** typically MCU name, oscillator frequency (16 MHz default) and baud rate (115200 default) followed by
  + `ee` bootloader supports EEPROM read/write
  + `led-b1` toggles an active-low LED on pin `B1`, `+` designates an active-high LED
  + `csb0` for dual boot uses, in this example, pin B0 as chip select of external SPI flash memory
  + `dual` boot: serial and from external SPI flash memory
  + `fr` bootloader provides non-essential code for smoother error handing
  + `ce` bootloader provides a chip erase command
  + `ur` uses urprotocol and requires `avrdude -c urclock` for programming
  + `d` dual boot
  + `vbl` vector bootloader: set fuses to jump to reset, not the HW boot section
