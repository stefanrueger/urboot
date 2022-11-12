|Size|Usage|Version|Features|Hex file|
|:-:|:-:|:-:|:-:|:--|
|224|256|u7.6|`w-u-jpr`|[atmega48pa_7mhz3728_460800bps_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/atmega48pa/fcpu_7mhz3728/460800_bps/atmega48pa_7mhz3728_460800bps_ur_vbl.hex)|
|230|256|u7.6|`w-u-jpr`|[atmega48pa_7mhz3728_460800bps_lednop_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/atmega48pa/fcpu_7mhz3728/460800_bps/atmega48pa_7mhz3728_460800bps_lednop_ur_vbl.hex)|
|248|256|u7.6|`w-u-jpr`|[atmega48pa_7mhz3728_460800bps_lednop_fr_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/atmega48pa/fcpu_7mhz3728/460800_bps/atmega48pa_7mhz3728_460800bps_lednop_fr_ur_vbl.hex)|
|298|320|u7.6|`weu-jpr`|[atmega48pa_7mhz3728_460800bps_ee_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/atmega48pa/fcpu_7mhz3728/460800_bps/atmega48pa_7mhz3728_460800bps_ee_ur_vbl.hex)|
|304|320|u7.6|`weu-jpr`|[atmega48pa_7mhz3728_460800bps_ee_lednop_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/atmega48pa/fcpu_7mhz3728/460800_bps/atmega48pa_7mhz3728_460800bps_ee_lednop_ur_vbl.hex)|
|322|384|u7.6|`w-s-jpr`|[atmega48pa_7mhz3728_460800bps_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/atmega48pa/fcpu_7mhz3728/460800_bps/atmega48pa_7mhz3728_460800bps_vbl.hex)|
|322|384|u7.6|`weu-jpr`|[atmega48pa_7mhz3728_460800bps_ee_lednop_fr_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/atmega48pa/fcpu_7mhz3728/460800_bps/atmega48pa_7mhz3728_460800bps_ee_lednop_fr_ur_vbl.hex)|
|328|384|u7.6|`w-s-jpr`|[atmega48pa_7mhz3728_460800bps_lednop_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/atmega48pa/fcpu_7mhz3728/460800_bps/atmega48pa_7mhz3728_460800bps_lednop_vbl.hex)|
|350|384|u7.6|`weu-jpr`|[atmega48pa_7mhz3728_460800bps_ee_lednop_fr_ce_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/atmega48pa/fcpu_7mhz3728/460800_bps/atmega48pa_7mhz3728_460800bps_ee_lednop_fr_ce_ur_vbl.hex)|
|362|384|u7.6|`w-s-jpr`|[atmega48pa_7mhz3728_460800bps_lednop_fr_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/atmega48pa/fcpu_7mhz3728/460800_bps/atmega48pa_7mhz3728_460800bps_lednop_fr_vbl.hex)|
|386|448|u7.6|`wes-jpr`|[atmega48pa_7mhz3728_460800bps_ee_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/atmega48pa/fcpu_7mhz3728/460800_bps/atmega48pa_7mhz3728_460800bps_ee_vbl.hex)|
|392|448|u7.6|`wes-jpr`|[atmega48pa_7mhz3728_460800bps_ee_lednop_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/atmega48pa/fcpu_7mhz3728/460800_bps/atmega48pa_7mhz3728_460800bps_ee_lednop_vbl.hex)|
|422|448|u7.6|`wes-jpr`|[atmega48pa_7mhz3728_460800bps_ee_lednop_fr_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/atmega48pa/fcpu_7mhz3728/460800_bps/atmega48pa_7mhz3728_460800bps_ee_lednop_fr_vbl.hex)|
|460|512|u7.6|`wes-jpr`|[atmega48pa_7mhz3728_460800bps_ee_lednop_fr_ce_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/atmega48pa/fcpu_7mhz3728/460800_bps/atmega48pa_7mhz3728_460800bps_ee_lednop_fr_ce_vbl.hex)|

- **Size:** Bootloader code size including small table at top end
- **Usage:** How many bytes of flash are needed, ie, HW boot section or a multiple of the page size
- **Version:** For example, u7.6 is an urboot version, o5.2 is an optiboot version
- **Features:**
  + `w` urboot provides `pgm_write_page(sram, flash)` for the application at `FLASHEND-4+1`
  + `e` EEPROM read/write support
  + `u` uses urprotocol requiring `avrdude -c urclock` for programming
  + `s` uses skeleton of STK500v1 protocol; `-c urclock` and `-c arduino` both work
  + `j` vector bootloader: uploaded applications *need to be patched externally*, eg, using `avrdude -c urclock`
  + `p` bootloader protects itself from being overwritten
  + `r` preserves reset flags for the application in the register R2
- **Hex file:** typically MCU name, oscillator frequency (16 MHz default) and baud rate (115200 default) followed by
  + `ee` bootloader supports EEPROM read/write
  + `lednop` "template" bootloader contains `mov rx,rx` nops that can be replaced to toggle LEDs
  + `fr` bootloader provides non-essential code for smoother error handing
  + `ce` bootloader provides a chip erase command
  + `ur` uses urprotocol and requires `avrdude -c urclock` for programming
  + `vbl` vector bootloader: set fuses to jump to reset, not the HW boot section
