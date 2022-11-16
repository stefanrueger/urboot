|Size|Usage|Version|Features|Hex file|
|:-:|:-:|:-:|:-:|:--|
|248|256|u7.7|`w-u-jpr--`|[anarduino_7mhz3728_230400bps_led+b1_fr_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/board_anarduino/fcpu_7mhz3728/230400_bps/anarduino_7mhz3728_230400bps_led+b1_fr_ur_vbl.hex)|
|342|384|u7.7|`weu-jpr-c`|[anarduino_7mhz3728_230400bps_ee_led+b1_fr_ce_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/board_anarduino/fcpu_7mhz3728/230400_bps/anarduino_7mhz3728_230400bps_ee_led+b1_fr_ce_ur_vbl.hex)|
|382|384|u7.7|`w-udjpr--`|[anarduino_7mhz3728_230400bps_led+b1_csd5_dual_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/board_anarduino/fcpu_7mhz3728/230400_bps/anarduino_7mhz3728_230400bps_led+b1_csd5_dual_ur_vbl.hex)|
|338|512|u7.7|`weu-hpr-c`|[anarduino_7mhz3728_230400bps_ee_led+b1_fr_ce_ur.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/board_anarduino/fcpu_7mhz3728/230400_bps/anarduino_7mhz3728_230400bps_ee_led+b1_fr_ce_ur.hex)|
|442|512|u7.7|`wes-hpr-c`|[anarduino_7mhz3728_230400bps_ee_led+b1_fr_ce.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/board_anarduino/fcpu_7mhz3728/230400_bps/anarduino_7mhz3728_230400bps_ee_led+b1_fr_ce.hex)|
|490|512|u7.7|`weudhpr-c`|[anarduino_7mhz3728_230400bps_ee_led+b1_csd5_dual_fr_ce_ur.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/board_anarduino/fcpu_7mhz3728/230400_bps/anarduino_7mhz3728_230400bps_ee_led+b1_csd5_dual_fr_ce_ur.hex)|
|496|512|u7.7|`w-sdhpr--`|[anarduino_7mhz3728_230400bps_led+b1_csd5_dual_fr.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/board_anarduino/fcpu_7mhz3728/230400_bps/anarduino_7mhz3728_230400bps_led+b1_csd5_dual_fr.hex)|
|594|1024|u7.7|`wesdhpr-c`|[anarduino_7mhz3728_230400bps_ee_led+b1_csd5_dual_fr_ce.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/board_anarduino/fcpu_7mhz3728/230400_bps/anarduino_7mhz3728_230400bps_ee_led+b1_csd5_dual_fr_ce.hex)|

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
