|Size|Usage|Version|Features|Hex file|
|:-:|:-:|:-:|:-:|:--|
|240|256|u7.7|`w-u-jpr--`|[urclockmega_3mhz6864_19200bps_led+c7_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/board_urclockmega/fcpu_3mhz6864/19200_bps/urclockmega_3mhz6864_19200bps_led+c7_ur_vbl.hex)|
|382|512|u7.7|`weu-jPr-c`|[urclockmega_3mhz6864_19200bps_ee_led+c7_fr_ce_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/board_urclockmega/fcpu_3mhz6864/19200_bps/urclockmega_3mhz6864_19200bps_ee_led+c7_fr_ce_ur_vbl.hex)|
|484|512|u7.7|`w-udjPr-c`|[urclockmega_3mhz6864_19200bps_led+c7_csb3_dual_fr_ce_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/board_urclockmega/fcpu_3mhz6864/19200_bps/urclockmega_3mhz6864_19200bps_led+c7_csb3_dual_fr_ce_ur_vbl.hex)|
|502|512|u7.7|`weudjPr--`|[urclockmega_3mhz6864_19200bps_ee_led+c7_csb3_dual_fr_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/board_urclockmega/fcpu_3mhz6864/19200_bps/urclockmega_3mhz6864_19200bps_ee_led+c7_csb3_dual_fr_ur_vbl.hex)|
|546|768|u7.7|`weudjPr-c`|[urclockmega_3mhz6864_19200bps_ee_led+c7_csb3_dual_fr_ce_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/board_urclockmega/fcpu_3mhz6864/19200_bps/urclockmega_3mhz6864_19200bps_ee_led+c7_csb3_dual_fr_ce_ur_vbl.hex)|
|364|1024|u7.7|`weu-hpr-c`|[urclockmega_3mhz6864_19200bps_ee_led+c7_fr_ce_ur.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/board_urclockmega/fcpu_3mhz6864/19200_bps/urclockmega_3mhz6864_19200bps_ee_led+c7_fr_ce_ur.hex)|
|470|1024|u7.7|`wes-hpr-c`|[urclockmega_3mhz6864_19200bps_ee_led+c7_fr_ce.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/board_urclockmega/fcpu_3mhz6864/19200_bps/urclockmega_3mhz6864_19200bps_ee_led+c7_fr_ce.hex)|
|528|1024|u7.7|`weudhpr-c`|[urclockmega_3mhz6864_19200bps_ee_led+c7_csb3_dual_fr_ce_ur.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/board_urclockmega/fcpu_3mhz6864/19200_bps/urclockmega_3mhz6864_19200bps_ee_led+c7_csb3_dual_fr_ce_ur.hex)|
|634|1024|u7.7|`wesdhpr-c`|[urclockmega_3mhz6864_19200bps_ee_led+c7_csb3_dual_fr_ce.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/board_urclockmega/fcpu_3mhz6864/19200_bps/urclockmega_3mhz6864_19200bps_ee_led+c7_csb3_dual_fr_ce.hex)|

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
  + `c` bootloader provides chip erase functionality (only recommended for large MCUs)
  + `-` corresponding feature not present
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
