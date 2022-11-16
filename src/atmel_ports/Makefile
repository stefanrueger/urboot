# atmel_ports.h is created 
#
# Also Creates pins_atmel.h headers from pins_arduino.h with mappings for
# all the boards/variants. Copy the source header files to this dir first:
# $ cp -a ~/etc/clock/ur/variants .
#

DIRS := $(wildcard variants/*)
PINS_ATMEL_MAP := $(addsuffix /pins_atmel_map.h, $(DIRS))

atmel_ports.h: mk_atmel_ports.pl variants/standard/pins_atmel_map.h variants/MoteinoMEGA/pins_atmel_map.h \
  variants/mega/pins_atmel_map.h variants/digispark/pins_atmel_map.h variants/digisparkpro/pins_atmel_map.h \
  variants/attiny84/pins_atmel_map.h variants/atmega32/pins_atmel_map.h
	mk_atmel_ports.pl >$@

%/mypins_arduino.h: %/pins_arduino.h
	grep -v avr/pgmspace.h $< | sed -e "s_/\(pins.arduino.h\)_/my\1_" -e "s/.uint16_t. *\&[PD][IOD][NR]T*\(.\),/'\1',/" >$@

%/pins_atmel_map.h: %/mypins_arduino.h pins_atmel_map.c
	cp $< pins_define.h
	@cc -I $(dir $@) pins_atmel_map.c -DVARIANT='"'`dirname $@ | sed s_variants/__`'"'
	./a.out >$@
	@rm a.out pins_define.h

all: $(PINS_ATMEL_MAP)
	
clean:
	@touch $(PINS_ATMEL_MAP)
	rm $(PINS_ATMEL_MAP)

