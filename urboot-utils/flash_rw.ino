/*
 * Flash read/write example
 *
 * You need to provide your own I/O functions in config.h if you want to
 * see print messages. If not, you can still examine the flash after
 * programming the sketch and running it to verify that the program has
 * stored sawtooth function values in the unused flash space between the
 * application and the metadata.
 *
 * Published under GNU General Public License, version 3 (GPL-3.0)
 * author Stefan Rueger
 * v 1.0
 * 12.07.2023
 */

#include "config.h"
#include "urboot-utils.h"

const uint32_t flashnum PROGMEM = 0;

uint16_t sensor_value() {
  static uint16_t val;
  uint16_t ret = val;

  /*
   * A real application would wait until time is ready or until device has
   * a value.  Here we emulate a sawtooth function for demo purposes.
   */
  val = (val+1)%1024;          

  return ret;
}  

void setup() {
  // boardInit(1, "flash_rw...\n");

  // Modify a read-only variable in flash
  uint32_t num = pgm_read_b4(pgm_addr(flashnum));
  printf_q("Sketch has run %lu time%S since flashing\n", num, PSTR("s")+(num==1));
  num++;
  urflashWrite((uint8_t *) &num, (uintpgm_t) pgm_addr(flashnum), sizeof num);

  printf_q("Recording a time series into all flash between sketch and metadata... ");
  stdio_flush();
  uintpgm_t n = 0, size = urstoreSize();
  do {
    // Buffer sensor values in RAM to avoid wearing out flash by too many write calls
    uint16_t buffer[SPM_PAGESIZE/2];
    for(unsigned i=0; i<sizeof buffer/sizeof*buffer; i++)
      buffer[i] = sensor_value();
    // urstoreWrite() does not write beyond store size and only writes if contents changed
    urstoreWrite((uint8_t *) buffer, n, sizeof buffer);
    n += sizeof buffer;
  } while(n < size);
  printf_q(" done\n");
}

void loop() {
}