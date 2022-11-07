|Size|Usage|Version|Features|Hex file|
|:-:|:-:|:-:|:-:|:--|
|252|256|u7.6|`w-u-jpr`|[ata5272_1mhz_9600bps_rxb0_txb1_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/ata5272/fcpu_1mhz/9600_bps/ata5272_1mhz_9600bps_rxb0_txb1_ur_vbl.hex)|
|262|384|u7.6|`w-u-jpr`|[ata5272_1mhz_9600bps_rxb0_txb1_lednop_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/ata5272/fcpu_1mhz/9600_bps/ata5272_1mhz_9600bps_rxb0_txb1_lednop_ur_vbl.hex)|
|318|384|u7.6|`weu-jpr`|[ata5272_1mhz_9600bps_rxb0_txb1_ee_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/ata5272/fcpu_1mhz/9600_bps/ata5272_1mhz_9600bps_rxb0_txb1_ee_ur_vbl.hex)|
|324|384|u7.6|`weu-jpr`|[ata5272_1mhz_9600bps_rxb0_txb1_ee_lednop_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/ata5272/fcpu_1mhz/9600_bps/ata5272_1mhz_9600bps_rxb0_txb1_ee_lednop_ur_vbl.hex)|
|342|384|u7.6|`weu-jpr`|[ata5272_1mhz_9600bps_rxb0_txb1_ee_lednop_fr_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/ata5272/fcpu_1mhz/9600_bps/ata5272_1mhz_9600bps_rxb0_txb1_ee_lednop_fr_ur_vbl.hex)|
|350|384|u7.6|`w-s-jpr`|[ata5272_1mhz_9600bps_rxb0_txb1_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/ata5272/fcpu_1mhz/9600_bps/ata5272_1mhz_9600bps_rxb0_txb1_vbl.hex)|
|356|384|u7.6|`w-s-jpr`|[ata5272_1mhz_9600bps_rxb0_txb1_lednop_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/ata5272/fcpu_1mhz/9600_bps/ata5272_1mhz_9600bps_rxb0_txb1_lednop_vbl.hex)|
|370|384|u7.6|`weu-jpr`|[ata5272_1mhz_9600bps_rxb0_txb1_ee_lednop_fr_ce_ur_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/ata5272/fcpu_1mhz/9600_bps/ata5272_1mhz_9600bps_rxb0_txb1_ee_lednop_fr_ce_ur_vbl.hex)|
|402|512|u7.6|`wes-jpr`|[ata5272_1mhz_9600bps_rxb0_txb1_ee_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/ata5272/fcpu_1mhz/9600_bps/ata5272_1mhz_9600bps_rxb0_txb1_ee_vbl.hex)|
|408|512|u7.6|`wes-jpr`|[ata5272_1mhz_9600bps_rxb0_txb1_ee_lednop_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/ata5272/fcpu_1mhz/9600_bps/ata5272_1mhz_9600bps_rxb0_txb1_ee_lednop_vbl.hex)|
|438|512|u7.6|`wes-jpr`|[ata5272_1mhz_9600bps_rxb0_txb1_ee_lednop_fr_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/ata5272/fcpu_1mhz/9600_bps/ata5272_1mhz_9600bps_rxb0_txb1_ee_lednop_fr_vbl.hex)|
|480|512|u7.6|`wes-jpr`|[ata5272_1mhz_9600bps_rxb0_txb1_ee_lednop_fr_ce_vbl.hex](https://raw.githubusercontent.com/stefanrueger/urboot/main/bootloaders/ata5272/fcpu_1mhz/9600_bps/ata5272_1mhz_9600bps_rxb0_txb1_ee_lednop_fr_ce_vbl.hex)|

- **Size:** Bootloader code size including small table at top end
- **Useage:** How many bytes of flash are needed, ie, HW boot section or a multiple of the page size
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
  + `rxd0 txd1` software I/O using, in this example, lines RX `D0` and TX `D1`
  + `ee` bootloader supports EEPROM read/write
  + `lednop` "template" bootloader contains `mov rx,rx` nops that can be replaced to toggle LEDs
  + `fr` bootloader provides non-essential code for smoother error handing
  + `ce` bootloader provides a chip erase command
  + `ur` uses urprotocol and requires `avrdude -c urclock` for programming
  + `vbl` vector bootloader: set fuses to jump to reset, not the HW boot section
