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
# $ make MCU=attiny45 F_CPU=123456L SWIO=1 RX=AtmelPB0 TX=AtmelPB1 NAME=mysupderduperboard
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
# Make command-line Options: permit commands like "make MCU=atmega328p VBL=1" to pass the
# appropriate options ("-DVBL=1") to urboot-gcc
#

##
# Defaults; they all can be overwritten on the command line
#
MCU ?= atmega328p
F_CPU ?= 16000000L
PGMWRITEPAGE ?= 1
EEPROM ?= 1
DUAL ?= 0
AUTOBAUD ?= 0
VBL ?= 0
WDTO = 1S

# Provide pgm_write_page(sram, flash) function
ifdef PGMWRITEPAGE
PGMWRITEPAGE_CMD = -DPGMWRITEPAGE=$(PGMWRITEPAGE)
endif

# Support EEPROM R/W
EEPROM_CMD = -DEEPROM=$(EEPROM)

# DUAL boot from external SPI memory (you must set SFMCS below if so)
DUAL_CMD = -DDUAL=$(DUAL)

# Autobaud or desired baud rate
AUTOBAUD_CMD = -DAUTOBAUD=$(AUTOBAUD)

# vector bootloader (able to program a chip that resets to addr 0)
VBL_CMD = -DVBL=$(VBL)

# Basic naming convention, eg, atmega328p_16mhz_115200bps_we_wdto1s.hex, overridden by NAME=...
WHAT := $(MCU)_$(subst L,hz,$(subst 000L,khz,$(subst 000000L,mhz,$(F_CPU))))

ifeq ($(AUTOBAUD),0)
ifdef BAUD_RATE
WHAT := $(WHAT)_$(BAUD_RATE)bps
else
WHAT := $(WHAT)_115200bps
endif
endif

FEAT :=        $(subst 0,,$(subst 1,w,$(PGMWRITEPAGE)))
FEAT := $(FEAT)$(subst 0,,$(subst 1,e,$(EEPROM)))
FEAT := $(FEAT)$(subst 0,,$(subst 1,d,$(DUAL)))
FEAT := $(FEAT)$(subst 0,,$(subst 1,v,$(VBL)))
FEAT := $(FEAT)$(subst 0,,$(subst 1,a,$(AUTOBAUD)))
# Provides code to emulate chip erase
ifdef CHIP_ERASE
CHIP_ERASE_CMD = -DCHIP_ERASE=$(CHIP_ERASE)
FEAT := $(FEAT)$(subst 0,,$(subst 1,c,$(CHIP_ERASE)))
endif

# Check flash page before writing it (from v8.0)
ifdef UPDATE_FL
UPDATE_FL_CMD = -DUPDATE_FL=$(UPDATE_FL)
FEAT := $(FEAT)$(subst 0,,$(subst 1,u,$(subst 2,u2,$(subst 3,u3,$(subst 4,u4,$(UPDATE_FL))))))
endif

# FRILLS=x on the command line sets defaults for the "lesser" options (which can be overwritten)
ifdef FRILLS
FEAT := $(FEAT)f$(FRILLS)
FRILLS_CMD = -DFRILLS=$(FRILLS)
# AUTOFRILLS=x,y,z uses the highest FRILL level of the list that fits into space occupied by FRILLS=x
else ifdef AUTOFRILLS
FEAT := $(FEAT)fa
AUTOFRILLS_CMD = -DAUTOFRILLS=$(AUTOFRILLS)
endif

ifneq ($(FEAT),)
WHAT := $(WHAT)_$(FEAT)
endif

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

# QEXITEND reduces the wait on exit from programming to 16 ms
ifdef QEXITEND
QEXITEND_CMD = -DQEXITEND=$(QEXITEND)
WHAT := $(WHAT)_qf$(QEXITEND)
endif

# QEXITERR reduces the wait on exit from synchronisation error to 16 ms
ifdef QEXITERR
QEXITERR_CMD = -DQEXITERR=$(QEXITERR)
WHAT := $(WHAT)_qe$(QEXITERR)
endif

# EXITFE refrains from resetting the watchdog on frame errors (thus the bootloader might exit eventually)
ifdef EXITFE
EXITFE_CMD = -DEXITFE=$(EXITFE)
WHAT := $(WHAT)_xf$(EXITFE)
endif

# Protect reset for VBL
ifdef PROTECTRESET
PROTECTRESET_CMD = -DPROTECTRESET=$(PROTECTRESET)
WHAT := $(WHAT)_pr$(PROTECTRESET)
endif

# Test code
ifdef TESTING
TESTING_CMD = -DTESTING=$(TESTING)
WHAT := $(WHAT)_tt$(TESTING)
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

# Start urboot by putting a square wave of DEBUG_FREQ Hz on FREQ_PIN for DEBUG_CYCLES cycles
ifdef DEBUG_FREQ
DEBUG_FREQ_CMD = -DDEBUG_FREQ=$(DEBUG_FREQ)
WHAT := $(WHAT)_$(DEBUG_FREQ)Hz
endif

# If set debug frequency has granularity of 1 CPU cycle at cost of, eg, inserted nops
ifdef EXACT_DF
EXACT_DF_CMD = -DEXACT_DF=$(EXACT_DF)
ifneq ($(EXACT_DF), 0)
WHAT := $(WHAT)x
endif
endif

# Number of initial blink periods for debugging
ifdef DEBUG_CYCLES
DEBUG_CYCLES_CMD = -DDEBUG_CYCLES=$(DEBUG_CYCLES)
WHAT := $(WHAT)_dc$(DEBUG_CYCLES)
endif

# Pin for debug square wave
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

# EEPROM compiles much neater with 5.4.0
ifeq (1, $(EEPROM))
override TOOLVER := 5.4.0
override GCCROOT = ./avr-toolchain/$(TOOLVER)/bin/
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

# Prefer NAME= name if given
ifdef NAME
WHAT := ${NAME}
else
  # Backward compatibility
  ifdef MOVETO
  WHAT := ${MOVETO}
  endif
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
  $(EXITFE_CMD) $(QEXITERR_CMD) $(QEXITEND_CMD) $(VERSION_CMD) $(FRILLS_CMD) \
  $(UPDATE_FL_CMD) $(VBL_CMD) $(VBL_VECT_NUM_CMD) $(CHIP_ERASE_CMD) $(SWIO_CMD) \
  $(UARTNUM_CMD) $(UARTALT_CMD) $(TX_CMD) $(RX_CMD) $(TESTING_CMD) $(PGMWRITEPAGE_CMD) \
  $(DEBUG_FREQ_CMD) $(EXACT_DF_CMD) $(DEBUG_CYCLES_CMD) $(FREQ_PIN_CMD) $(FREQ_POLARITY_CMD) \
  $(FLASHWRAPS_CMD) $(PROTECTRESET_CMD) $(AUTOFRILLS_CMD)


bootloader: CFLAGS += $(COMMON_OPTIONS)
bootloader: $(WHAT).hex

min: attiny2313_min attiny2313_emin atmega1280_min atmega1284p_min atmega168_min atmega2560_min atmega32_min \
  atmega328p_min atmega644p_min atmega8_min atmega88_min attiny167_min attiny84_min attiny85_min
	./hexls -sort

all: clean attiny2313_min attiny2313_emin atmega168_min atmega32_min \
  atmega2560_min atmega1280_min atmega1284p_min \
  atmega644p_min atmega8_min atmega88_min attiny84_min \
  atmega328p_min \
  atmega328p_amin \
  atmega328p \
  atmega328p_h \
  atmega328p_a \
  atmega328p_fa \
  atmega328p_swio \
  atmega328p_d \
  atmega328p_ad \
  atmega328p_7875khz_swio \
  atmega328p_8000khz_swio \
  atmega328p_8125khz_swio \
  atmega328p_led9_50Hz_fp9 \
  urclock urclock_cs8_ad \
  timeduino timeduino_cs8_ad \
  jeenode \
  promini_led13_a promini_led9_a promini_led13 promini_led9  \
  moteino moteino_cs8_ad \
  anarduino anarduino_cs5_ad \
  \
  diecimila pro_16mhz pro_20mhz pro_8mhz pro_a \
  \
  atmega644p atmega644p_a \
  atmega1284p atmega1284p_a atmega1284p_d atmega1284p_ad \
  atmega2560 atmega2560_d atmega2560_a atmega2560_ad \
  atmega1280 atmega1280_d atmega1280_a atmega1280_ad \
  \
  atmega8 atmega8_a atmega8_h \
  atmega32 atmega32_a \
  atmega88 atmega88_a atmega88_h \
  attiny167_min attiny167 attiny167_a attiny167_ad digisparkpro \
  attiny85_min attiny85 digispark \
  attiny84_min lilypad luminet_baud9600
	./hexls --sort *.hex


# Examples
#   make MCU=atmega32 F_CPU=11059200L
#   make MCU=atmega88
#   make MCU=atmega328p

version:
	@$(URBOOTGCC) --version $(SRC) -o dummy.elf -mmcu=atmega328p


####
# 192 bytes
#
attiny2313_min:
	$(MAKE) $(MK_OPTS) MCU=attiny2313 EEPROM=0 VBL=1 AUTOFRILLS=0..3 PGMWRITEPAGE=0 VBL_VECT_NUM=EEPROM_READY_vect_num NAME=$@

####
# 256 bytes with EEPROM support
#
attiny2313_emin:
	$(MAKE) $(MK_OPTS) MCU=attiny2313 EEPROM=1 VBL=1 AUTOFRILLS=0..3 PGMWRITEPAGE=1 VBL_VECT_NUM=EEPROM_READY_vect_num NAME=$@


##
# ATmega328p based boards - those with external SPI memory appear again with dual boot capability further below
#

####
# 256 bytes == 2 pages (no EEPROM support)
#
atmega328p_min:
	$(MAKE) $(MK_OPTS) MCU=atmega328p EEPROM=0 VBL=1 AUTOFRILLS=0..6 NAME=$@

atmega328p_amin:
	$(MAKE) $(MK_OPTS) MCU=atmega328p AUTOBAUD=1 EEPROM=0 VBL=1 AUTOFRILLS=0..6 NAME=$@

####
# 384 bytes == 3 pages
#
atmega328p_a:
	$(MAKE) $(MK_OPTS) MCU=atmega328p AUTOBAUD=1 VBL=1 AUTOFRILLS=5..10 NAME=$@

atmega328p_swio:
	$(MAKE) $(MK_OPTS) MCU=atmega328p AUTOBAUD=1 VBL=1 SWIO=1 RX=ArduinoPin0 TX=ArduinoPin1 AUTOFRILLS=5..10 NAME=$@

atmega328p_fa:
	$(MAKE) $(MK_OPTS) MCU=atmega328p PGMWRITEPAGE=0 AUTOBAUD=1 VBL=1 CHIP_ERASE=1 UPDATE_FL=1 AUTOFRILLS=1..6,8..10 NAME=$@

atmega328p:
	$(MAKE) $(MK_OPTS) MCU=atmega328p VBL=1 AUTOFRILLS=5..10 NAME=$@

# Bootloaders below can be derived from the single "template"  generic bootloader above

urclock urclockmini timeduino jeenode:
	$(MAKE) $(MK_OPTS) MCU=atmega328p LED=AtmelPB1 LEDPOLARITY=-1 VBL=1 AUTOFRILLS=5..10 NAME=$@

urclockusb:
	$(MAKE) $(MK_OPTS) MCU=atmega328p LED=AtmelPD5 VBL=1 AUTOFRILLS=5..10 NAME=$@

ursense:
	$(MAKE) $(MK_OPTS) MCU=atmega328p LED=AtmelPD5 LEDPOLARITY=-1 VBL=1 AUTOFRILLS=5..10 NAME=$@

anarduino moteino promini_led9:
	$(MAKE) $(MK_OPTS) MCU=atmega328p LED=AtmelPB1 VBL=1 AUTOFRILLS=5..10 NAME=$@

promini_led9_a:
	$(MAKE) $(MK_OPTS) MCU=atmega328p AUTOBAUD=1 LED=AtmelPB1 VBL=1 AUTOFRILLS=5..10 NAME=$@

uno rbbb promini_led13:
	$(MAKE) $(MK_OPTS) MCU=atmega328p LED=AtmelPB5 VBL=1 AUTOFRILLS=5..10 NAME=$@

promini_led13_a:
	$(MAKE) $(MK_OPTS) MCU=atmega328p AUTOBAUD=1 LED=AtmelPB5 VBL=1 AUTOFRILLS=5..10 NAME=$@

atmega328p_led9_50Hz_fp9:
	$(MAKE) $(MK_OPTS) MCU=atmega328p DEBUG_FREQ=50 DEBUG_CYCLES=5 LED=ArduinoPin9 VBL=1 AUTOFRILLS=6,7 NAME=$@

####
# 384 bytes
#
# Use the off-8 MHz frequencies for inaccurately calibrated internal 8 MHz resonators
#
# Note that 8 MHz Arduino with hardware UART have too large bitrate errors Try SWIO=1 TX=... RX=...
# for these. Software serial can adjust the bitrate to the tune of 1 cycle per bit length (69.44
# cycles at 115200 baud), so in theory should be able to get the error down to 0.44/69.44 = 0.63%.
#
atmega328p_7875khz_swio:
	$(MAKE) $(MK_OPTS) MCU=atmega328p F_CPU=7875000L SWIO=1 RX=ArduinoPin0 TX=ArduinoPin1 VBL=1 AUTOFRILLS=5..10 NAME=$@

atmega328p_8000khz_swio:
	$(MAKE) $(MK_OPTS) MCU=atmega328p F_CPU=8000000L SWIO=1 RX=ArduinoPin0 TX=ArduinoPin1 VBL=1 AUTOFRILLS=5..10 NAME=$@

atmega328p_8125khz_swio:
	$(MAKE) $(MK_OPTS) MCU=atmega328p F_CPU=8125000L SWIO=1 RX=ArduinoPin0 TX=ArduinoPin1 VBL=1 AUTOFRILLS=5..10 NAME=$@

####
# 512 bytes, hardware-supported bootloader
#
atmega328p_h:
	$(MAKE) $(MK_OPTS) MCU=atmega328p AUTOBAUD=1 AUTOFRILLS=2,8..10,3 NAME=$@

##
# Boards below have external spi flash memory, which we use for over-the-air programming
#

##
# 512 bytes and DUAL=1
#
atmega328p_d:
	$(MAKE) $(MK_OPTS) MCU=atmega328p DUAL=1 AUTOFRILLS=5..10 NAME=$@

atmega328p_ad:
	$(MAKE) $(MK_OPTS) MCU=atmega328p AUTOBAUD=1 DUAL=1 AUTOFRILLS=5..10 NAME=$@

# These bootloaders can be derived from the version above
anarduino_cs5_ad:
	$(MAKE) $(MK_OPTS) MCU=atmega328p AUTOBAUD=1 SFMCS=ArduinoPin5 LED=AtmelPB1 DUAL=1 AUTOFRILLS=5..10 NAME=$@

timeduino_cs8_ad urclock_cs8_ad moteino_cs8_ad:
	$(MAKE) $(MK_OPTS) MCU=atmega328p AUTOBAUD=1 SFMCS=ArduinoPin8 LED=AtmelPB1 DUAL=1 AUTOFRILLS=5..10 NAME=$@



atmega16a_min:
	$(MAKE) $(MK_OPTS) MCU=atmega16a EEPROM=0 AUTOFRILLS=0..10 NAME=$@

atmega163_min:
	$(MAKE) $(MK_OPTS) MCU=atmega162 EEPROM=0 AUTOFRILLS=0..10 NAME=$@


##
# ATmega168 based boards - those with external SPI memory appear again with dual boot capability further below
#

####
# 256 bytes == 2 pages (no EEPROM support)
#
atmega168_min:
	$(MAKE) $(MK_OPTS) MCU=atmega168 EEPROM=0 AUTOFRILLS=0..10 NAME=$@

pro_8mhz lilypad:
	$(MAKE) $(MK_OPTS) MCU=atmega168 F_CPU=8000000L SWIO=1 RX=ArduinoPin0 TX=ArduinoPin1 VBL=1 AUTOFRILLS=5..10 NAME=$@

pro_20mhz:
	$(MAKE) $(MK_OPTS) MCU=atmega168 F_CPU=20000000L VBL=1 AUTOFRILLS=5..10 NAME=$@

pro_16mhz diecimila:
	$(MAKE) $(MK_OPTS) MCU=atmega168 VBL=1 AUTOFRILLS=5..10 NAME=$@

pro_a diecimila_a:
	$(MAKE) $(MK_OPTS) MCU=atmega168 AUTOBAUD=1 VBL=1 AUTOFRILLS=5..10 NAME=$@

##
# Yet other platforms (the digisparkpro documentation isn't great, so pin numbering may be
# different on your board - depending on the manufacturer)
#

##
# LED pin 1 (PB1) is the same as CIPO, so might clash with SPI
#
# 256 bytes
attiny85_min:
	$(MAKE) $(MK_OPTS) MCU=attiny85 SWIO=1 TX=AtmelPB3 RX=AtmelPB4 EEPROM=0 VBL=1 AUTOFRILLS=0..5 NAME=$@

# 384 bytes
attiny85 digispark:
	$(MAKE) $(MK_OPTS) MCU=attiny85 SWIO=1 TX=AtmelPB3 RX=AtmelPB4 VBL=1 AUTOFRILLS=5..10 NAME=$@

# ATtiny85 unable to produce dual boot bootloader owing to lack of SPI interface

##
# digisparkpro LED=AtmelPB1
#

# 256 bytes
attiny167_min:
	$(MAKE) $(MK_OPTS) MCU=attiny167 EEPROM=0 VBL=1 AUTOFRILLS=0..5 NAME=$@

# 384 bytes
attiny167:
	$(MAKE) $(MK_OPTS) MCU=attiny167 VBL=1 AUTOFRILLS=5..10 NAME=$@

attiny167_a:
	$(MAKE) $(MK_OPTS) MCU=attiny167 AUTOBAUD=1 VBL=1 AUTOFRILLS=5..10 NAME=$@

tiny167debug_50Hz_fp2:
	$(MAKE) $(MK_OPTS) MCU=attiny167 VBL=1 DEBUG_FREQ=50 FREQ_PIN=ArduinoPin2 AUTOFRILLS=5..10 NAME=$@

# 512 bytes
attiny167_ad:
	$(MAKE) $(MK_OPTS) MCU=attiny167 AUTOBAUD=1 DUAL=1 VBL=1 AUTOFRILLS=5..10 NAME=$@

digisparkpro:
	$(MAKE) $(MK_OPTS) MCU=attiny167 VBL=1 LED=AtmelPB1 AUTOFRILLS=5..10 WDTO=4S NAME=$@

##
# Luminet is a 1 MHz clocked board (capable of 9600 baud)
#
attiny84_min:
	$(MAKE) $(MK_OPTS) MCU=attiny84 SWIO=1 TX=AtmelPA2 RX=AtmelPA3 EEPROM=0 VBL=1 AUTOFRILLS=0..5 NAME=$@


# LED=AtmelPA4
luminet_baud9600:
	$(MAKE) $(MK_OPTS) MCU=attiny84 F_CPU=1000000L BAUD_RATE=9600 SWIO=1 TX=AtmelPA2 RX=AtmelPA3 VBL=1 AUTOFRILLS=5..10 NAME=$@

##
# ATmega8
#

##
# 256 bytes == 2 pages (no EEPROM support)
#
atmega8_min:
	$(MAKE) $(MK_OPTS) MCU=atmega8 EEPROM=0 AUTOFRILLS=0..5 NAME=$@

##
# 320 bytes == 5 pages
#
atmega8:
	$(MAKE) $(MK_OPTS) MCU=atmega8 VBL=1 AUTOFRILLS=0..5 NAME=$@

atmega8_a:
	$(MAKE) $(MK_OPTS) MCU=atmega8 AUTOBAUD=1 VBL=1 AUTOFRILLS=0..5 NAME=$@

##
# 512 bytes, hardware-supported bootloader
#
atmega8_h:
	$(MAKE) $(MK_OPTS) MCU=atmega8 AUTOBAUD=1 FRILLS=7 NAME=$@

##
# ATmega88
#

##
# 256 bytes == 4 pages (no EEPROM support)
#
atmega88_min:
	$(MAKE) $(MK_OPTS) MCU=atmega88 EEPROM=0 VBL=1 AUTOFRILLS=0..5 NAME=$@

##
# 320 bytes == 5 pages (no EEPROM support)
#
atmega88:
	$(MAKE) $(MK_OPTS) MCU=atmega88 VBL=1 AUTOFRILLS=0..5 NAME=$@

atmega88_a:
	$(MAKE) $(MK_OPTS) MCU=atmega88 AUTOBAUD=1 VBL=1 AUTOFRILLS=0..5 NAME=$@

##
# 512 bytes, hardware-supported bootloader
#
atmega88_h:
	$(MAKE) $(MK_OPTS) MCU=atmega88 AUTOBAUD=1 FRILLS=7 NAME=$@

##
# Atmega32 (used to be listed with unusual F_CPU=11059200L)
#

##
# 256 bytes == 2 pages (no EEPROM support)
#
atmega32_min:
	$(MAKE) $(MK_OPTS) MCU=atmega32 EEPROM=0 VBL=1 AUTOFRILLS=0..5 NAME=$@

##
# 384 bytes == 3 pages
#
atmega32:
	$(MAKE) $(MK_OPTS) MCU=atmega32 VBL=1 AUTOFRILLS=5..10 NAME=$@

atmega32_a:
	$(MAKE) $(MK_OPTS) MCU=atmega32 AUTOBAUD=1 VBL=1 AUTOFRILLS=5..10 NAME=$@

##
# 512 bytes, hardware-supported bootloader
#
atmega32_h:
	$(MAKE) $(MK_OPTS) MCU=atmega32 AUTOBAUD=1 FRILLS=7 NAME=$@

##
# 1284p/644p boards have a (too big) min bootloader size of 1k and a page size of 256
#

##
# 256 bytes == 1 page (no EEPROM support)
#
atmega1284p_min:
	$(MAKE) $(MK_OPTS) MCU=atmega1284p EEPROM=0 VBL=1 AUTOFRILLS=0..5 NAME=$@

# 512 bytes
atmega1284p:
	$(MAKE) $(MK_OPTS) MCU=atmega1284p VBL=1 AUTOFRILLS=5..10 NAME=$@

atmega1284p_d:
	$(MAKE) $(MK_OPTS) MCU=atmega1284p DUAL=1 VBL=1 AUTOFRILLS=0..10 NAME=$@

atmega1284p_a:
	$(MAKE) $(MK_OPTS) MCU=atmega1284p AUTOBAUD=1 VBL=1 AUTOFRILLS=5..10 NAME=$@

atmega1284p_ad:
	$(MAKE) $(MK_OPTS) MCU=atmega1284p AUTOBAUD=1 DUAL=1 VBL=1 AUTOFRILLS=0..10 NAME=$@

# Named devices below can all be derived from above template bootloader
bobuino mighty1284:
	$(MAKE) $(MK_OPTS) MCU=atmega1284p LED=AtmelPB7 VBL=1 AUTOFRILLS=0..10 NAME=$@

wildfire:
	$(MAKE) $(MK_OPTS) MCU=atmega1284p LED=AtmelPB5 VBL=1 AUTOFRILLS=0..10 NAME=$@

wildfirev2:
	$(MAKE) $(MK_OPTS) MCU=atmega1284p BAUD_RATE=1000000 LED=AtmelPB7 VBL=1 AUTOFRILLS=0..10 NAME=$@

moteinomega:
	$(MAKE) $(MK_OPTS) MCU=atmega1284p LED=AtmelPD7 VBL=1 AUTOFRILLS=0..10 NAME=$@

urclockmega:
	$(MAKE) $(MK_OPTS) MCU=atmega1284p LED=AtmelPC7 VBL=1 AUTOFRILLS=0..10 NAME=$@

timeduinomega: # obsolete
	$(MAKE) $(MK_OPTS) MCU=atmega1284p LED=AtmelPD5 VBL=1 AUTOFRILLS=0..10 NAME=$@

# Below can be derived from template bootloader
moteinomega_cs23_d:
	$(MAKE) $(MK_OPTS) MCU=atmega1284p SFMCS=ArduinoPin23 LED=AtmelPD7 DUAL=1 VBL=1 AUTOFRILLS=0..10 NAME=$@

urclockmega_cs3_d:
	$(MAKE) $(MK_OPTS) MCU=atmega1284p SFMCS=ArduinoPin3 LED=AtmelPC7 LEDPOLARITY=-1 DUAL=1 VBL=1 AUTOFRILLS=0..10 NAME=$@

timeduinomega_cs3_d: # obsolete
	$(MAKE) $(MK_OPTS) MCU=atmega1284p SFMCS=ArduinoPin3 LED=AtmelPD5 LEDPOLARITY=-1 DUAL=1 VBL=1 AUTOFRILLS=0..10 NAME=$@


# 256 bytes
atmega644p_min:
	$(MAKE) $(MK_OPTS) MCU=atmega644p EEPROM=0 VBL=1 AUTOFRILLS=0..10 NAME=$@

# 512 bytes
atmega644p:
	$(MAKE) $(MK_OPTS) MCU=atmega644p VBL=1 AUTOFRILLS=5..10 NAME=$@

atmega644p_a:
	$(MAKE) $(MK_OPTS) MCU=atmega644p AUTOBAUD=1 VBL=1 AUTOFRILLS=5..10 NAME=$@

atmega644p_d:
	$(MAKE) $(MK_OPTS) MCU=atmega644p DUAL=1 VBL=1 AUTOFRILLS=0..10 NAME=$@

atmega644p_ad:
	$(MAKE) $(MK_OPTS) MCU=atmega644p AUTOBAUD=1 DUAL=1 VBL=1 AUTOFRILLS=0..10 NAME=$@

# Can be derived
sanguino:
	$(MAKE) $(MK_OPTS) MCU=atmega644p LED=AtmelPB0 VBL=1 AUTOFRILLS=0..10 NAME=$@

##
# MEGA1280/2560 boards
#
# Leds are typically on AtmelPB7

# 256 bytes
atmega1280_min:
	$(MAKE) $(MK_OPTS) MCU=atmega1280 EEPROM=0 VBL=1 AUTOFRILLS=0..10 NAME=$@

# 512 bytes
atmega1280:
	$(MAKE) $(MK_OPTS) MCU=atmega1280 VBL=1 AUTOFRILLS=5..10 NAME=$@

atmega1280_d:
	$(MAKE) $(MK_OPTS) MCU=atmega1280 DUAL=1 VBL=1 AUTOFRILLS=0..10 NAME=$@

atmega1280_a:
	$(MAKE) $(MK_OPTS) MCU=atmega1280 AUTOBAUD=1 VBL=1 AUTOFRILLS=5..10 NAME=$@

atmega1280_ad:
	$(MAKE) $(MK_OPTS) MCU=atmega1280 AUTOBAUD=1 DUAL=1 VBL=1 AUTOFRILLS=0..10 NAME=$@

# 256 bytes
atmega2560_min:
	$(MAKE) $(MK_OPTS) MCU=atmega2560 EEPROM=0 VBL=1 AUTOFRILLS=0..10 NAME=$@

# 512 bytes
atmega2560:
	$(MAKE) $(MK_OPTS) MCU=atmega2560 VBL=1 AUTOFRILLS=5..10 NAME=$@

atmega2560_d:
	$(MAKE) $(MK_OPTS) MCU=atmega2560 DUAL=1 VBL=1 AUTOFRILLS=0..10 NAME=$@

atmega2560_a:
	$(MAKE) $(MK_OPTS) MCU=atmega2560 AUTOBAUD=1 VBL=1 AUTOFRILLS=5..10 NAME=$@

atmega2560_ad:
	$(MAKE) $(MK_OPTS) MCU=atmega2560 AUTOBAUD=1 DUAL=1 VBL=1 AUTOFRILLS=0..10 NAME=$@

#----------

%.hex: $(SRC) Makefile
	@$(URBOOTGCC)  $(CFLAGS) $(LDFLAGS) -o $*.elf $(SRC)
	$(OBJCOPY) -j .text -j .data -j .version --set-section-flags .version=alloc,load -O ihex $*.elf $@
	$(OBJDUMP) -h -S $*.elf > $*.lst
#	@$(SIZE) $*.elf
	@echo -- `./hexls $@`
	@echo

# Prints baud rate quantisation error for bootloader urboot.c implementation
bauderror: bauderror.c ur_uarttable.c
	gcc bauderror.c -lm -o $@

clean:
	rm -rf *.o *.elf *.lst *.map *.sym *.lss *.eep *.srec *.bin *.hex *.tmp.sh *.out
