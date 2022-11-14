|Size|Usage|Version|Features|Hex file|
|:-:|:-:|:-:|:-:|:--|
|254|256|u7.6|`w-u-jpr`|[attiny88_24mhz_19200bps_rxb0_txb1_lednop_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/attiny88/fcpu_24mhz/19200_bps/attiny88_24mhz_19200bps_rxb0_txb1_lednop_ur_vbl.hex)|
|304|320|u7.6|`w-u-jpr`|[attiny88_24mhz_19200bps_rxb0_txb1_lednop_fr_ce_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/attiny88/fcpu_24mhz/19200_bps/attiny88_24mhz_19200bps_rxb0_txb1_lednop_fr_ce_ur_vbl.hex)|
|316|320|u7.6|`weu-jpr`|[attiny88_24mhz_19200bps_rxb0_txb1_ee_lednop_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/attiny88/fcpu_24mhz/19200_bps/attiny88_24mhz_19200bps_rxb0_txb1_ee_lednop_ur_vbl.hex)|
|362|384|u7.6|`weu-jpr`|[attiny88_24mhz_19200bps_rxb0_txb1_ee_lednop_fr_ce_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/attiny88/fcpu_24mhz/19200_bps/attiny88_24mhz_19200bps_rxb0_txb1_ee_lednop_fr_ce_ur_vbl.hex)|

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
  + `rxd0 txd1` software I/O using, in this example, lines RX `D0` and TX `D1`
  + `ee` bootloader supports EEPROM read/write
  + `lednop` is a template bootloader with `mov rx,rx` nops as placeholders for LEDs operations
  + `fr` bootloader provides non-essential code for smoother error handing
  + `ce` bootloader provides a chip erase command
  + `ur` uses urprotocol and requires `avrdude -c urclock` for programming
  + `vbl` vector bootloader: set fuses to jump to reset, not the HW boot section
