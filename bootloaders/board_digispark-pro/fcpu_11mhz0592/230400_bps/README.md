|Size|Usage|Version|Features|Hex file|
|:-:|:-:|:-:|:-:|:--|
|248|256|u7.7|`w-u-jpr--`|[digispark-pro_11mhz0592_230400bps_rxb6_txb3_led+b1_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/board_digispark-pro/fcpu_11mhz0592/230400_bps/digispark-pro_11mhz0592_230400bps_rxb6_txb3_led+b1_ur_vbl.hex)|
|360|384|u7.7|`weu-jpr-c`|[digispark-pro_11mhz0592_230400bps_rxb6_txb3_ee_led+b1_fr_ce_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/board_digispark-pro/fcpu_11mhz0592/230400_bps/digispark-pro_11mhz0592_230400bps_rxb6_txb3_ee_led+b1_fr_ce_ur_vbl.hex)|

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
  + `c` bootloader provides chip erase functionality (only recommended for large MCUs)
  + `-` corresponding feature not present
- **Hex file:** typically MCU name, oscillator frequency (16 MHz default) and baud rate (115200 default) followed by
  + `rxd0 txd1` software I/O using, in this example, lines RX `D0` and TX `D1`
  + `ee` bootloader supports EEPROM read/write
  + `led-b1` toggles an active-low LED on pin `B1`, `+` designates an active-high LED
  + `fr` bootloader provides non-essential code for smoother error handing
  + `ce` bootloader provides a chip erase command
  + `ur` uses urprotocol and requires `avrdude -c urclock` for programming
  + `vbl` vector bootloader: set fuses to jump to reset, not the HW boot section
