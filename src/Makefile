# Makefile for urboot bootloader
#
# published under GNU General Public License, version 3 (GPL-3.0)
# author Stefan Rueger <stefan.rueger@urclocks.com>
# 2016-2022
#
# To make bootloader .hex files for known boards:
# $ make diecimila lilypad urclock
#
# To make a bootloader .hex file for a custom board:
# $ make MCU=attiny45 F_CPU=123456L SWIO=1 RX=AtmelPB0 TX=AtmelPB1 MOVETO=mysupderduperboard
#

SRC      = urboot.c

# urboot-gcc is a wrapper for avr-gcc that computes the "best" start address for the bootloader and
# more. If you don't have perl and therefore cannot run urboot-gcc, you can compile with something
# like (example atmega328):
#
#   avr-gcc -DSTART=0x7e00 -DVERSTART=0x7ffc -DRJMPWP=0xcfda -Wl,--section-start=.text=0x7e00 -Wl,--section-start=.version=0x7efc ...
#
# but then you need to verify by hand whether the RJMPWP opcode is the  right distance, by checking
# the pgm_page_write symbol in .elf. It's probably easier to install perl (and the Capture::Tiny
# module).


OPTIMIZE = -Os -fno-split-wide-types -mrelax
DEFS     =

# Override is only needed by avr-lib build system.
override CFLAGS        = -g -Wundef -Wall $(OPTIMIZE) -mmcu=$(MCU) -DF_CPU=$(F_CPU) $(DEFS) -Wno-clobbered
override LDFLAGS       = -Wl,--relax -nostartfiles -nostdlib

#
# Make command-line Options: permit commands like "make MCU=atmega328p VBL=3" to pass the
# appropriate options ("-DVBL=3") to urboot-gcc
#

##
# Defaults; they all can be overwritten on the command line
#
MCU ?= atmega328p
F_CPU ?= 16000000L
EEPROM ?= 1
DUAL ?= 0
FRILLS ?= 0
WDTO = 1S

# Basic naming convention, sth, like atmega328p_16mhz_we--2.hex
WHAT := $(MCU)_$(subst L,hz,$(subst 000L,khz,$(subst 000000L,mhz,$(F_CPU))))_
ifdef AUTOBAUD
WHAT := $(WHAT)$(subst 0,,$(subst 1,auto_,$(AUTOBAUD)))
endif

ifdef BAUD_RATE
WHAT := $(WHAT)$(BAUD_RATE)bps_
else
WHAT := $(WHAT)115200bps_
endif

WHAT := $(WHAT)$(subst 0,,$(subst 1,w,$(PGMWRITEPAGE)))
WHAT := $(WHAT)$(subst 0,,$(subst 1,e,$(EEPROM)))
WHAT := $(WHAT)$(subst 0,,$(subst 1,d,$(DUAL)))

# vector bootloader (able to program a chip that resets to addr 0)
ifdef VBL
WHAT := $(WHAT)$(subst 0,,$(subst 1,v1,$(subst 2,v2,$(subst 3,v3,$(VBL)))))
VBL_CMD = -DVBL=$(VBL)
endif
WHAT := $(WHAT)f$(FRILLS)

# Support EEPROM R/W
EEPROM_CMD = -DEEPROM=$(EEPROM)

# DUAL boot from external SPI memory (you must set SFMCS below if so)
DUAL_CMD = -DDUAL=$(DUAL)

# FRILLS=x on the command line sets defaults for the "lesser" options (which can be overwritten)
FRILLS_CMD = -DFRILLS=$(FRILLS)


# SFMCS chip select line of external SPI flash memory (eg, ArduinoPin8 or AtmelPB0)
ifdef SFMCS
SFMCS_CMD = -DSFMCS=$(SFMCS)
WHAT := $(WHAT)_cs$(subst Atmel,,$(subst ArduinoPin,,$(SFMCS)))
endif

# The vector that is being given up for VBL to save the jump to the application
ifdef VBL_VECT_NUM
VBL_VECT_NUM_CMD = -DVBL_VECT_NUM=$(VBL_VECT_NUM)
WHAT := $(WHAT)_vn$(VBL_VECT_NUM)
endif

# Provides code to emulate chip erase
ifdef CHIP_ERASE
CHIP_ERASE_CMD = -DCHIP_ERASE=$(CHIP_ERASE)
ifeq (1, $(CHIP_ERASE))
WHAT := $(WHAT)_ce
endif
endif

# Sets r2 to the reset flags before jumping to the application (so your application can use it)
ifdef RESETFLAGS
RESETFLAGS_CMD = -DRESETFLAGS=$(RESETFLAGS)
WHAT := $(WHAT)_rstf$(RESETFLAGS)
endif

# Provide pgm_write_page(sram, flash) function
ifdef PGMWRITEPAGE
PGMWRITEPAGE_CMD = -DPGMWRITEPAGE=$(PGMWRITEPAGE)
WHAT := $(WHAT)_p$(PGMWRITEPAGE)
endif

# QEXITEND reduces the wait on exit from programming to 16 ms
ifdef QEXITEND
QEXITEND_CMD = -DQEXITEND=$(QEXITEND)
WHAT := $(WHAT)_qxend$(QEXITEND)
endif

# QEXITERR reduces the wait on exit from synchronisation error to 16 ms
ifdef QEXITERR
QEXITERR_CMD = -DQEXITERR=$(QEXITERR)
WHAT := $(WHAT)_qxerr$(QEXITERR)
endif

# EXITFE refrains from resetting the watchdog on frame errors (thus the bootloader might exit eventually)
ifdef EXITFE
EXITFE_CMD = -DEXITFE=$(EXITFE)
WHAT := $(WHAT)_xfe$(EXITFE)
endif

# Reports the software version of urboot (otherwise it's always 7.7 or so)
ifdef RETSWVERS
RETSWVERS_CMD = -DRETSWVERS=$(RETSWVERS)
WHAT := $(WHAT)_swv$(RETSWVERS)
endif

# Protect reset for VBL
ifdef PROTECTRESET
PROTECTRESET_CMD = -DPROTECTRESET=$(PROTECTRESET)
WHAT := $(WHAT)_pres$(PROTECTRESET)
endif

# Test code
ifdef TESTING
TESTING_CMD = -DTESTING=$(TESTING)
WHAT := $(WHAT)_tt$(TESTING)
endif

# Autobaud or desired baud rate
ifdef AUTOBAUD
AUTOBAUD_CMD = -DAUTOBAUD=$(AUTOBAUD)
endif

ifdef BAUD_RATE
BAUD_RATE_CMD = -DBAUD_RATE=$(BAUD_RATE)
endif

# UART double speed mode (0 = don't use = save 6 bytes, 1 = let urboot decide, 2 = use that mode)
ifdef UART2X
UART2X_CMD = -DUART2X=$(UART2X)
WHAT := $(WHAT)_u2x$(UART2X)
endif

# UARTNUM are numbers UARTs from 0 onwards
ifdef UARTNUM
UARTNUM_CMD = -DUARTNUM=$(UARTNUM)
WHAT := $(WHAT)_uart$(UARTNUM)
endif

# UARTALT assigns different RX/TX pins to UART
ifdef UARTALT
UARTALT_CMD = -DUARTALT=$(UARTALT)
WHAT := $(WHAT)_alt$(UARTALT)
endif

# Software serial using TX and RX lines (see below)
ifdef SWIO
SWIO_CMD = -DSWIO=$(SWIO)
WHAT := $(WHAT)_swio$(SWIO)
endif

# Set TX line for software serial (eg, -DTX=ArduinoPin1 or -DRX=AtmelPD1)
ifdef TX
TX_CMD = -DTX=$(TX)
WHAT := $(WHAT)_tx$(subst Atmel,,$(subst ArduinoPin,,$(TX)))
endif

# Set RX line for software serial (eg, -DTX=ArduinoPin0 or -DRX=AtmelPD0)
ifdef RX
RX_CMD = -DRX=$(RX)
WHAT := $(WHAT)_rx$(subst Atmel,,$(subst ArduinoPin,,$(RX)))
endif

# BLINK toggles activity LED during data xfer
ifdef BLINK
BLINK_CMD = -DBLINK=$(BLINK)
# WHAT := $(WHAT)_blink$(BLINK)
endif

# Specify the LED pin; syntax as in, eg, -DLED=ArduinoPin9 or -DRX=AtmelPB1
ifdef LED
LED_CMD = -DLED=$(LED)
WHAT := $(WHAT)_led
ifdef LEDPOLARITY
ifeq (-1, $(LEDPOLARITY))
WHAT := $(WHAT)-
endif
endif
WHAT := $(WHAT)$(subst Atmel,,$(subst ArduinoPin,,$(LED)))
endif

# Specify the LED polarity, 1 for active-high LED, -1 for active-low LED
ifdef LEDPOLARITY
LEDPOLARITY_CMD = -DLEDPOLARITY=$(LEDPOLARITY)
# WHAT := $(WHAT)_pol$(LEDPOLARITY)
endif

# Start urboot by putting a square wave of DEBUG_FREQ Hz on FREQ_PIN for 5 cycles
ifdef DEBUG_FREQ
DEBUG_FREQ_CMD = -DDEBUG_FREQ=$(DEBUG_FREQ)
WHAT := $(WHAT)_$(DEBUG_FREQ)Hz
endif

ifdef EXACT_DF
EXACT_DF_CMD = -DEXACT_DF=$(EXACT_DF)
ifneq ($(EXACT_DF), 0)
WHAT := $(WHAT)x
endif
endif

ifdef FREQ_PIN
FREQ_PIN_CMD = -DFREQ_PIN=$(FREQ_PIN)
WHAT := $(WHAT)_fp$(subst Atmel,,$(subst ArduinoPin,,$(FREQ_PIN)))
endif

# Specify the FREQ_PIN polarity, 1 for active-high, -1 for active-low
ifdef FREQ_POLARITY
FREQ_POLARITY_CMD = -DFREQ_POLARITY=$(FREQ_POLARITY)
endif

ifdef FLASHWRAPS
FLASHWRAPS_CMD = -DFLASHWRAPS=$(FLASHWRAPS)
WHAT := $(WHAT)_fwrap$(FLASHWRAPS)
endif

# Watchdog timeout (16MS ... 8S, default 500MS)
ifdef WDTO
WDTO_CMD = -DWDTO=$(WDTO)
WHAT := $(WHAT)_wdto$(subst M,m,$(subst S,s,$(WDTO)))
endif

# Specify whether NOP codes are generated for LED and SFM CS (replaced when burning the bootloader)
ifdef TEMPLATE
TEMPLATE_CMD = -DTEMPLATE=$(TEMPLATE)
ifeq (1, $(TEMPLATE))
WHAT := $(WHAT)_otf
endif
endif

# Avoids generating code that is only needed for avrdude's arduino programmer (needs avrdude -c urclock)
ifdef URPROTOCOL
URPROTOCOL_CMD = -DURPROTOCOL=$(URPROTOCOL)
ifeq (1, $(URPROTOCOL))
WHAT := $(WHAT)_ur
endif
endif

# UPROTOCOL with EEPROM compiles much neater with 5.4.0
ifdef URPROTOCOL
ifeq (1, $(EEPROM))
override TOOLVER := 5.4.0
override GCCROOT = ./avr-toolchain/$(TOOLVER)/bin/
endif
endif

# Toolchain 4.8.1 does not know some MCUs, bump to 5.4.0
ifdef MCU
ifeq (/$(MCU)/, $(findstring /$(MCU)/, /ata5702m322/ata5782/ata5791/ata5831/ata8210/ata8510/atmega168pb/atmega328pb/atmega48pb/atmega88pb/atmega324pb/))
override TOOLVER := 5.4.0
override GCCROOT = ./avr-toolchain/$(TOOLVER)/bin/
endif
endif

# VERSION=n on the command sets the version, best set as octal number, eg, 075 for v 7.5
ifdef VERSION
VERSION_CMD = -DVERSION=$(VERSION)
WHAT := $(WHAT)_v$(VERSION)
endif

# Older avr-gcc versions such as 4.8.1 or 5.4.0 produce tighter code
TOOLVER ?= 4.8.1
GCCROOT ?= ./avr-toolchain/$(TOOLVER)/bin/
MK_OPTS  = TOOLVER=$(TOOLVER) GCCROOT=$(GCCROOT)

URBOOTGCC= ./urboot-gcc -toolchain=$(TOOLVER)
OBJCOPY        = $(GCCROOT)avr-objcopy
OBJDUMP        = $(GCCROOT)avr-objdump
SIZE           = $(GCCROOT)avr-size

COMMON_OPTIONS = $(WDTO_CMD) $(AUTOBAUD_CMD) $(BAUD_RATE_CMD) $(UART2X_CMD) $(LED_CMD) \
  $(LEDPOLARITY_CMD) $(TEMPLATE_CMD) $(BLINK_CMD) $(DUAL_CMD) $(SFMCS_CMD) $(EEPROM_CMD) \
  $(EXITFE_CMD) $(QEXITERR_CMD) $(QEXITEND_CMD) $(URPROTOCOL_CMD) $(VERSION_CMD) $(FRILLS_CMD) \
  $(RETSWVERS_CMD) $(VBL_CMD) $(VBL_VECT_NUM_CMD) $(CHIP_ERASE_CMD) $(RESETFLAGS_CMD) $(SWIO_CMD) \
  $(UARTNUM_CMD) $(UARTALT_CMD) $(TX_CMD) $(RX_CMD) $(TESTING_CMD) $(PGMWRITEPAGE_CMD) \
  $(DEBUG_FREQ_CMD) $(EXACT_DF_CMD) $(FREQ_PIN_CMD) $(FREQ_POLARITY_CMD) $(FLASHWRAPS_CMD) \
  $(PROTECTRESET_CMD)

bootloader: CFLAGS += $(COMMON_OPTIONS)
bootloader: $(WHAT).hex

min: attiny2313_min atmega1280_min atmega1284p_min atmega168_min atmega2560_min atmega32_min atmega328p_min \
  atmega644p_min atmega8_min atmega88_min attiny167_min attiny84_min attiny85_min
	./hexls -sort

all: attiny2313_min atmega168_min atmega2560_min atmega32_min \
  atmega644p_min atmega8_min atmega88_min attiny84_min \
  atmega328p_min \
  atmega328p_amin \
  atmega328p \
  atmega328p_ur \
  atmega328p_autobaud_ur \
  atmega328p_ur_testing \
  atmega328p_dur \
  atmega328p_adur \
  atmega328p_7875khz_swio_ur \
  atmega328p_8000khz_swio_ur \
  atmega328p_8125khz_swio_ur \
  atmega328p_led9_50Hz_fp9 \
  urclock urclock_cs8_dur \
  timeduino timeduino_cs8_dur  \
  jeenode \
  promini_led13_ur promini_led9_ur promini_led13 promini_led9  \
  moteino moteino_cs8_dur \
  anarduino anarduino_cs5_dur \
  \
  diecimila pro_16mhz pro_20mhz pro_8mhz \
  pro_16mhz_ur pro_20mhz_ur pro_8mhz_ur \
  \
  atmega1284p_min \
  atmega1284p_v2 \
  \
  atmega2560_min atmega2560_ur atmega2560_dur atmega2560_v3 \
  atmega1280_min atmega1280_ur atmega1280_dur \
  \
  atmega8_ur atmega32_ur atmega88_ur \
  atmega8 atmega32 atmega88 \
  attiny167_min attiny167_ur attiny167_dur digisparkpro \
  attiny85_min attiny85_ur digispark \
  lilypad_ur luminet_baud9600_ur
	./hexls --sort *.hex

# Examples
#   make MCU=atmega32 F_CPU=11059200L
#   make MCU=atmega88
#   make MCU=atmega328p

version:
	@$(URBOOTGCC) --version $(SRC) -o dummy.elf -mmcu=atmega328p


####
# 224 bytes
#
attiny2313_min:
	$(MAKE) $(MK_OPTS) MCU=attiny2313 URPROTOCOL=1 EEPROM=0 VBL=1 FRILLS=0 VBL_VECT_NUM=EEPROM_READY_vect_num MOVETO=$@


##
# ATmega328p based boards - those with external SPI memory appear again with dual boot capability further below
#

####
# 256 bytes == 2 pages (no EEPROM support, needs to be programmed with avrdude -c urclock)
#
atmega328p_min:
	$(MAKE) $(MK_OPTS) MCU=atmega328p URPROTOCOL=1 EEPROM=0 VBL=1 FRILLS=6 MOVETO=$@

atmega328p_amin:
	$(MAKE) $(MK_OPTS) MCU=atmega328p AUTOBAUD=1 URPROTOCOL=1 EEPROM=0 VBL=1 FRILLS=3 MOVETO=$@

####
# 384 bytes == 3 pages (needs to be programmed with avrdude -c urclock)
#
atmega328p_autobaud_ur:
	$(MAKE) $(MK_OPTS) MCU=atmega328p AUTOBAUD=1 URPROTOCOL=1 VBL=1 FRILLS=7 MOVETO=$@

atmega328p_ur:
	$(MAKE) $(MK_OPTS) MCU=atmega328p URPROTOCOL=1 VBL=1 FRILLS=7 MOVETO=$@

atmega328p_ur_testing:
	$(MAKE) $(MK_OPTS) MCU=atmega328p URPROTOCOL=1 VBL=1 FRILLS=7 TESTING=1 MOVETO=$@

####
# 384 bytes (needs to be programmed with avrdude -c urclock)
#
# Use the off-8 MHz frequencies for inaccurately calibrated internal 8 MHz resonators
#
# Note that 8 MHz Arduino with hardware UART have too large bitrate errors Try SWIO=1 TX=... RX=...
# for these. Software serial can adjust the bitrate to the tune of 1 cycle per bit length (69.44
# cycles at 115200 baud), so in theory should be able to get the error down to 0.44/69.44 = 0.63%.
#
atmega328p_7875khz_swio_ur:
	$(MAKE) $(MK_OPTS) MCU=atmega328p F_CPU=7875000L URPROTOCOL=1 SWIO=1 RX=ArduinoPin0 TX=ArduinoPin1 VBL=1 FRILLS=7 MOVETO=$@

atmega328p_8000khz_swio_ur:
	$(MAKE) $(MK_OPTS) MCU=atmega328p F_CPU=8000000L URPROTOCOL=1 SWIO=1 RX=ArduinoPin0 TX=ArduinoPin1 VBL=1 FRILLS=7 MOVETO=$@

atmega328p_8125khz_swio_ur:
	$(MAKE) $(MK_OPTS) MCU=atmega328p F_CPU=8125000L URPROTOCOL=1 SWIO=1 RX=ArduinoPin0 TX=ArduinoPin1 VBL=1 FRILLS=7 MOVETO=$@

####
# 512 bytes, regular bootloader
#
atmega328p:
	$(MAKE) $(MK_OPTS) MCU=atmega328p FRILLS=7 MOVETO=$@

# All bootloaders below can be derived from the single "template"  generic bootloader above

urclock urclockmini timeduino jeenode:
	$(MAKE) $(MK_OPTS) MCU=atmega328p LED=AtmelPB1 LEDPOLARITY=-1 FRILLS=7 MOVETO=$@

urclockusb:
	$(MAKE) $(MK_OPTS) MCU=atmega328p LED=AtmelPD5 FRILLS=7 MOVETO=$@

ursense:
	$(MAKE) $(MK_OPTS) MCU=atmega328p LED=AtmelPD5 LEDPOLARITY=-1 FRILLS=7 MOVETO=$@

anarduino moteino promini_led9:
	$(MAKE) $(MK_OPTS) MCU=atmega328p LED=AtmelPB1 FRILLS=7 MOVETO=$@

promini_led9_ur:
	$(MAKE) $(MK_OPTS) MCU=atmega328p AUTOBAUD=1 URPROTOCOL=1 VBL=1 LED=AtmelPB1 FRILLS=7 MOVETO=$@

uno rbbb promini_led13:
	$(MAKE) $(MK_OPTS) MCU=atmega328p LED=AtmelPB5 FRILLS=7 MOVETO=$@

promini_led13_ur:
	$(MAKE) $(MK_OPTS) MCU=atmega328p AUTOBAUD=1 URPROTOCOL=1 VBL=1 LED=AtmelPB5 FRILLS=7 MOVETO=$@

atmega328p_led9_50Hz_fp9:
	$(MAKE) $(MK_OPTS) MCU=atmega328p DEBUG_FREQ=50 LED=ArduinoPin9 FRILLS=7 MOVETO=$@


##
# Boards below have external spi flash memory, which we use for over-the-air programming
#

##
# 512 bytes and DUAL=1
#
atmega328p_dur:
	$(MAKE) $(MK_OPTS) MCU=atmega328p URPROTOCOL=1 DUAL=1 FRILLS=7 MOVETO=$@

atmega328p_adur:
	$(MAKE) $(MK_OPTS) MCU=atmega328p AUTOBAUD=1 URPROTOCOL=1 DUAL=1 FRILLS=7 MOVETO=$@

# These bootloaders can be derived from the version above
anarduino_cs5_dur:
	$(MAKE) $(MK_OPTS) MCU=atmega328p URPROTOCOL=1 SFMCS=ArduinoPin5 LED=AtmelPB1 DUAL=1 FRILLS=7 MOVETO=$@

timeduino_cs8_dur urclock_cs8_dur moteino_cs8_dur:
	$(MAKE) $(MK_OPTS) MCU=atmega328p URPROTOCOL=1 SFMCS=ArduinoPin8 LED=AtmelPB1 DUAL=1 FRILLS=7 MOVETO=$@

##
# With gcc v4.8.1 DUAL=1 still fits in 640 bytes (5 pages)
#
atmega328p_dv2:
	$(MAKE) $(MK_OPTS) MCU=atmega328p DUAL=1 VBL=2 FRILLS=4 MOVETO=$@

##
# With gcc v4.8.1 DUAL=1 still fits in 768 bytes (6 pages)
#
atmega328p_dv3:
	$(MAKE) $(MK_OPTS) MCU=atmega328p DUAL=1 VBL=3 FRILLS=7 MOVETO=$@


atmega16a_min:
	$(MAKE) $(MK_OPTS) MCU=atmega16a URPROTOCOL=1 EEPROM=0 VBL=0 FRILLS=6 MOVETO=$@

atmega163_min:
	$(MAKE) $(MK_OPTS) MCU=atmega162 URPROTOCOL=1 EEPROM=0 VBL=0 FRILLS=6 MOVETO=$@


##
# ATmega168 based boards - those with external SPI memory appear again with dual boot capability further below
#

####
# 256 bytes == 2 pages (no EEPROM support, needs to be programmed with avrdude -c urclock)
#
atmega168_min:
	$(MAKE) $(MK_OPTS) MCU=atmega168 URPROTOCOL=1 EEPROM=0 VBL=0 FRILLS=6 MOVETO=$@

pro_8mhz_ur lilypad_ur:
	$(MAKE) $(MK_OPTS) MCU=atmega168 F_CPU=8000000L URPROTOCOL=1 SWIO=1 RX=ArduinoPin0 TX=ArduinoPin1 VBL=1 FRILLS=7 MOVETO=$@

pro_20mhz_ur:
	$(MAKE) $(MK_OPTS) MCU=atmega168 F_CPU=20000000L URPROTOCOL=1 VBL=1 FRILLS=7 MOVETO=$@

pro_16mhz_ur diecimila_ur:
	$(MAKE) $(MK_OPTS) MCU=atmega168 URPROTOCOL=1 VBL=1 FRILLS=7 MOVETO=$@

pro_8mhz lilypad:
	$(MAKE) $(MK_OPTS) MCU=atmega168 F_CPU=8000000L SWIO=1 RX=ArduinoPin0 TX=ArduinoPin1 FRILLS=7 MOVETO=$@

pro_20mhz:
	$(MAKE) $(MK_OPTS) MCU=atmega168 F_CPU=20000000L FRILLS=7 MOVETO=$@

pro_16mhz diecimila:
	$(MAKE) $(MK_OPTS) MCU=atmega168 FRILLS=7 MOVETO=$@

##
# Yet other platforms (the digisparkpro documentation isn't great, so pin numbering may be
# different on your board - depending on the manufacturer)
#

##
# LED pin 1 (PB1) is the same as CIPO, so might clash with SPI
#
# 256 bytes
attiny85_min:
	$(MAKE) $(MK_OPTS) MCU=attiny85 SWIO=1 TX=AtmelPB3 RX=AtmelPB4 URPROTOCOL=1 EEPROM=0 VBL=1 FRILLS=4 MOVETO=$@

# 384 bytes
attiny85_ur digispark:
	$(MAKE) $(MK_OPTS) MCU=attiny85 URPROTOCOL=1 SWIO=1 TX=AtmelPB3 RX=AtmelPB4 VBL=1 FRILLS=7 MOVETO=$@

## 512 bytes
# attiny85_dur:
#	$(MAKE) $(MK_OPTS) MCU=attiny85 URPROTOCOL=1 SWIO=1 TX=AtmelPB3 RX=AtmelPB4 DUAL=1 VBL=1 TEMPLATE=1 FRILLS=7 MOVETO=$@


##
# digisparkpro LED=AtmelPB1
#

# 256 bytes
attiny167_min:
	$(MAKE) $(MK_OPTS) MCU=attiny167 URPROTOCOL=1 EEPROM=0 VBL=1 FRILLS=4 MOVETO=$@

# 384 bytes
attiny167_ur:
	$(MAKE) $(MK_OPTS) MCU=attiny167 URPROTOCOL=1 VBL=1 FRILLS=7 MOVETO=$@

tiny167debug_50Hz_fp2:
	$(MAKE) $(MK_OPTS) MCU=attiny167 URPROTOCOL=1 VBL=1 DEBUG_FREQ=50 FREQ_PIN=ArduinoPin2 FRILLS=7 MOVETO=$@

# 512 bytes
attiny167_dur:
	$(MAKE) $(MK_OPTS) MCU=attiny167 AUTOBAUD=1 URPROTOCOL=1 DUAL=1 VBL=1 FRILLS=7 QEXITERR=0 MOVETO=$@

digisparkpro:
	$(MAKE) $(MK_OPTS) MCU=attiny167 URPROTOCOL=1 VBL=1 LED=AtmelPB1 FRILLS=7 WDTO=4S MOVETO=$@

##
# Luminet is a 1 MHz clocked board (capable of 9600 baud)
#
attiny84_min:
	$(MAKE) $(MK_OPTS) MCU=attiny84 URPROTOCOL=1 SWIO=1 TX=AtmelPA2 RX=AtmelPA3 EEPROM=0 VBL=1 FRILLS=4 MOVETO=$@


# LED=AtmelPA4
luminet_baud9600_ur luminet_baud9600:
	$(MAKE) $(MK_OPTS) MCU=attiny84 F_CPU=1000000L BAUD_RATE=9600 URPROTOCOL=1 SWIO=1 TX=AtmelPA2 RX=AtmelPA3 VBL=1 FRILLS=7 MOVETO=$@

##
# ATmega8
#

##
# 256 bytes == 2 pages (no EEPROM support, needs to be programmed with avrdude -c urclock)
#
atmega8_min:
	$(MAKE) $(MK_OPTS) MCU=atmega8 URPROTOCOL=1 EEPROM=0 VBL=0 FRILLS=6 MOVETO=$@

##
# 320 bytes == 5 pages
#
atmega8_ur:
	$(MAKE) $(MK_OPTS) MCU=atmega8 URPROTOCOL=1 VBL=1 FRILLS=6 MOVETO=$@

atmega8:
	$(MAKE) $(MK_OPTS) MCU=atmega8 FRILLS=7 MOVETO=$@

##
# ATmega88
#

##
# 256 bytes == 4 pages (no EEPROM support, needs to be programmed with avrdude -c urclock)
#
atmega88_min:
	$(MAKE) $(MK_OPTS) MCU=atmega88 URPROTOCOL=1 EEPROM=0 VBL=1 FRILLS=6 MOVETO=$@

##
# 320 bytes == 5 pages (no EEPROM support, needs to be programmed with avrdude -c urclock)
#
atmega88_ur:
	$(MAKE) $(MK_OPTS) MCU=atmega88 URPROTOCOL=1 VBL=1 FRILLS=4 MOVETO=$@

##
# 512 bytes
#
atmega88:
	$(MAKE) $(MK_OPTS) MCU=atmega88 FRILLS=7 MOVETO=$@

##
# Atmega32 (used to be listed with unusual F_CPU=11059200L)
#

##
# 256 bytes == 2 pages (no EEPROM support, needs to be programmed with avrdude -c urclock)
#
atmega32_min:
	$(MAKE) $(MK_OPTS) MCU=atmega32 URPROTOCOL=1 EEPROM=0 VBL=1 FRILLS=6 MOVETO=$@


atmega32_ur:
	$(MAKE) $(MK_OPTS) MCU=atmega32 URPROTOCOL=1 VBL=1 FRILLS=7 MOVETO=$@

atmega32:
	$(MAKE) $(MK_OPTS) MCU=atmega32 FRILLS=7 MOVETO=$@

##
# 1284p/644p boards have a (too big) min bootloader size of 1k and a page size of 256
#

##
# 256 bytes == 1 page (no EEPROM support, needs to be programmed with avrdude -c urclock)
#
atmega1284p_min:
	$(MAKE) $(MK_OPTS) MCU=atmega1284p URPROTOCOL=1 EEPROM=0 VBL=1 FRILLS=3 MOVETO=$@

# 512 bytes
atmega1284p_ur:
	$(MAKE) $(MK_OPTS) MCU=atmega1284p URPROTOCOL=1 VBL=1 FRILLS=7 MOVETO=$@

atmega1284p_dur:
	$(MAKE) $(MK_OPTS) MCU=atmega1284p URPROTOCOL=1 DUAL=1 VBL=1 FRILLS=3 MOVETO=$@

atmega1284p_v2:
	$(MAKE) $(MK_OPTS) MCU=atmega1284p VBL=2 FRILLS=5 MOVETO=$@

# Named devices below can all be derived from above template bootloader
bobuino mighty1284:
	$(MAKE) $(MK_OPTS) MCU=atmega1284p LED=AtmelPB7 VBL=2 FRILLS=5 MOVETO=$@

wildfire:
	$(MAKE) $(MK_OPTS) MCU=atmega1284p LED=AtmelPB5 VBL=2 FRILLS=5 MOVETO=$@

wildfirev2:
	$(MAKE) $(MK_OPTS) MCU=atmega1284p BAUD_RATE=1000000 LED=AtmelPB7 VBL=2 FRILLS=5 MOVETO=$@

moteinomega:
	$(MAKE) $(MK_OPTS) MCU=atmega1284p LED=AtmelPD7 VBL=2 FRILLS=5 MOVETO=$@

urclockmega:
	$(MAKE) $(MK_OPTS) MCU=atmega1284p LED=AtmelPC7 VBL=2 FRILLS=5 MOVETO=$@

timeduinomega: # obsolete
	$(MAKE) $(MK_OPTS) MCU=atmega1284p LED=AtmelPD5 VBL=2 FRILLS=5 MOVETO=$@

# 768 bytes for dual boot
atmega1284p_dv3:
	$(MAKE) $(MK_OPTS) MCU=atmega1284p DUAL=1 VBL=3 FRILLS=6 MOVETO=$@

# Below can be derived from template bootloader
moteinomega_cs23_d:
	$(MAKE) $(MK_OPTS) MCU=atmega1284p SFMCS=ArduinoPin23 LED=AtmelPD7 DUAL=1 VBL=3 FRILLS=6 MOVETO=$@

urclockmega_cs3_d:
	$(MAKE) $(MK_OPTS) MCU=atmega1284p SFMCS=ArduinoPin3 LED=AtmelPC7 LEDPOLARITY=-1 DUAL=1 VBL=3 FRILLS=6 MOVETO=$@

timeduinomega_cs3_d: # obsolete
	$(MAKE) $(MK_OPTS) MCU=atmega1284p SFMCS=ArduinoPin3 LED=AtmelPD5 LEDPOLARITY=-1 DUAL=1 VBL=3 FRILLS=6 MOVETO=$@


# 256 bytes
atmega644p_min:
	$(MAKE) $(MK_OPTS) MCU=atmega644p URPROTOCOL=1 EEPROM=0 VBL=1 FRILLS=6 MOVETO=$@

# 512 bytes
atmega644p_ur:
	$(MAKE) $(MK_OPTS) MCU=atmega644p URPROTOCOL=1 VBL=1 FRILLS=7 MOVETO=$@

atmega644p_dur:
	$(MAKE) $(MK_OPTS) MCU=atmega644p URPROTOCOL=1 DUAL=1 VBL=1 FRILLS=5 MOVETO=$@

atmega644p_v3:
	$(MAKE) $(MK_OPTS) MCU=atmega644p VBL=3 FRILLS=3 MOVETO=$@

# Can be derived
sanguino:
	$(MAKE) $(MK_OPTS) MCU=atmega644p LED=AtmelPB0 VBL=3 FRILLS=3 MOVETO=$@

# 768 bytes
atmega644p_dv3:
	$(MAKE) $(MK_OPTS) MCU=atmega644p DUAL=1 VBL=3 FRILLS=7 MOVETO=$@


##
# MEGA1280/2560 boards
#
# Leds are typically on AtmelPB7
# _ur version not useful when same number of pages used (a page is 256 bytes)

# 256 bytes
atmega1280_min:
	$(MAKE) $(MK_OPTS) MCU=atmega1280 URPROTOCOL=1 EEPROM=0 VBL=1 FRILLS=3 MOVETO=$@

# 512 bytes
atmega1280_ur:
	$(MAKE) $(MK_OPTS) MCU=atmega1280 URPROTOCOL=1 VBL=1 FRILLS=7 MOVETO=$@

atmega1280_dur:
	$(MAKE) $(MK_OPTS) MCU=atmega1280 URPROTOCOL=1 DUAL=1 VBL=1 FRILLS=6 MOVETO=$@

atmega1280_v2:
	$(MAKE) $(MK_OPTS) MCU=atmega1280 VBL=2 FRILLS=3 MOVETO=$@

# 768 bytes = 3 pages
atmega1280_dv3:
	$(MAKE) $(MK_OPTS) MCU=atmega1280 DUAL=1 VBL=3 FRILLS=6 MOVETO=$@


# 256 bytes
atmega2560_min:
	$(MAKE) $(MK_OPTS) MCU=atmega2560 URPROTOCOL=1 EEPROM=0 VBL=1 FRILLS=3 MOVETO=$@

# 512 bytes
atmega2560_ur:
	$(MAKE) $(MK_OPTS) MCU=atmega2560 URPROTOCOL=1 VBL=1 FRILLS=7 MOVETO=$@

atmega2560_dur:
	$(MAKE) $(MK_OPTS) MCU=atmega2560 URPROTOCOL=1 DUAL=1 VBL=1 FRILLS=6 MOVETO=$@

# 768 bytes
atmega2560_v3:
	$(MAKE) $(MK_OPTS) MCU=atmega2560 VBL=3 FRILLS=7 MOVETO=$@

atmega2560_dv3:
	$(MAKE) $(MK_OPTS) MCU=atmega2560 DUAL=1 VBL=3 FRILLS=5 MOVETO=$@

#----------

%.hex: $(SRC) Makefile
	@$(URBOOTGCC)  $(CFLAGS) $(LDFLAGS) -o $*.elf $(SRC)
	$(OBJCOPY) -j .text -j .data -j .version --set-section-flags .version=alloc,load -O ihex $*.elf $@
	$(OBJDUMP) -h -S $*.elf > $*.lst
#	@$(SIZE) $*.elf
ifdef MOVETO
	mv $@ $(MOVETO).hex
	@mv $*.elf $(MOVETO).elf
	@mv $*.lst $(MOVETO).lst
	@echo -- `./hexls $(MOVETO).hex`
else
	@echo -- `./hexls $@`
endif
	@echo

clean:
	rm -rf *.o *.elf *.lst *.map *.sym *.lss *.eep *.srec *.bin *.hex *.tmp.sh *.out
