|Size|Usage|Version|Features|Hex file|
|:-:|:-:|:-:|:-:|:--|
|244|256|u7.7|`w-u-jpr--`|[moteinomega_3mhz6864_460800bps_led+d7_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/board_moteinomega/fcpu_3mhz6864/460800_bps/moteinomega_3mhz6864_460800bps_led+d7_ur_vbl.hex)|
|368|512|u7.7|`weu-jpr-c`|[moteinomega_3mhz6864_460800bps_ee_led+d7_fr_ce_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/board_moteinomega/fcpu_3mhz6864/460800_bps/moteinomega_3mhz6864_460800bps_ee_led+d7_fr_ce_ur_vbl.hex)|
|476|512|u7.7|`w-udjpr-c`|[moteinomega_3mhz6864_460800bps_led+d7_csc7_dual_fr_ce_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/board_moteinomega/fcpu_3mhz6864/460800_bps/moteinomega_3mhz6864_460800bps_led+d7_csc7_dual_fr_ce_ur_vbl.hex)|
|494|512|u7.7|`weudjpr--`|[moteinomega_3mhz6864_460800bps_ee_led+d7_csc7_dual_fr_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/board_moteinomega/fcpu_3mhz6864/460800_bps/moteinomega_3mhz6864_460800bps_ee_led+d7_csc7_dual_fr_ur_vbl.hex)|
|538|768|u7.7|`weudjpr-c`|[moteinomega_3mhz6864_460800bps_ee_led+d7_csc7_dual_fr_ce_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/board_moteinomega/fcpu_3mhz6864/460800_bps/moteinomega_3mhz6864_460800bps_ee_led+d7_csc7_dual_fr_ce_ur_vbl.hex)|
|368|1024|u7.7|`weu-hpr-c`|[moteinomega_3mhz6864_460800bps_ee_led+d7_fr_ce_ur.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/board_moteinomega/fcpu_3mhz6864/460800_bps/moteinomega_3mhz6864_460800bps_ee_led+d7_fr_ce_ur.hex)|
|474|1024|u7.7|`wes-hpr-c`|[moteinomega_3mhz6864_460800bps_ee_led+d7_fr_ce.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/board_moteinomega/fcpu_3mhz6864/460800_bps/moteinomega_3mhz6864_460800bps_ee_led+d7_fr_ce.hex)|
|538|1024|u7.7|`weudhpr-c`|[moteinomega_3mhz6864_460800bps_ee_led+d7_csc7_dual_fr_ce_ur.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/board_moteinomega/fcpu_3mhz6864/460800_bps/moteinomega_3mhz6864_460800bps_ee_led+d7_csc7_dual_fr_ce_ur.hex)|
|644|1024|u7.7|`wesdhpr-c`|[moteinomega_3mhz6864_460800bps_ee_led+d7_csc7_dual_fr_ce.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/board_moteinomega/fcpu_3mhz6864/460800_bps/moteinomega_3mhz6864_460800bps_ee_led+d7_csc7_dual_fr_ce.hex)|

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
