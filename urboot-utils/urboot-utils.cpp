/*
 * urboot-utils.cpp
 *
 * Urboot bootloader utility functions
 *
 * Published under GNU General Public License, version 3 (GPL-3.0)
 * Author Stefan Rueger
 * v 1.1
 * First version 18.12.2021
 */

#include "urboot-utils.h"
#include "hw_boot_sections.h"

/*
 * The most important urboot utility functions allow applications to write
 * flash, which can only been done from functions sitting in bootloader
 * space. Most urboot bootloaders provide that functionality.
 *
 * Copy a single page from SRAM to page-aligned PROGMEM via smr's urboot
 *
 * void urbootPageWrite(void *sram, progmem_t pgm);
 *
 * urstore:
 * avrdude -c urclock creates metadata below the bootloader with filename
 * and date of sketch but crucially also the end of the application. The
 * space between the application and the metadata can be used as a flash
 * store and read from/written to by the two routines below. Where counts
 * from 0 up to urstoreSize()-1, and is relative to the available space.
 *
 * void urstoreRead(uint8_t *sram, uintpgm_t where, size_t n);
 * void urstoreWrite(uint8_t *sram, uintpgm_t where, size_t n);
 *
 *
 * urflash:
 * The two routines below can read/write arbitrary areas in flash. The
 * parameter where is the actual absolute flash address. Note that urboot
 * bootloaders always protect the bootloader from being overwritten and
 * typically refuse to overwrite the reset vector.
 *
 * void urflashRead(uint8_t *sram, uintpgm_t where, size_t n);
 * void urflashWrite(uint8_t *sram, uintpgm_t where, size_t n);
 *
 */

// Put version string into a buffer of max 19 characters incl nul (normally 15-16 bytes incl nul)
void urbootPutVersion(char *buf, uint16_t ver, uint16_t rjmpwp) {
  uint8_t hi = ver>>8, type = ver & 0xff, flags;

  if(ver == 0xffff)             // Unknown provenance
    hi = type = 0;

  if(hi >= 072) {               // These are urboot versions
    sprintf_q(buf, "u%d.%d ", hi>>3, hi&7);
    buf += strlen(buf);
    *buf++ = (hi < 077 && (type & UR_PGMWRITEPAGE)) || (hi >= 077 && rjmpwp != ub_ret_opcode)? 'w': '-';
    *buf++ = type & UR_EEPROM? 'e': '-';
    if(hi >= 076) {             // From urboot version 7.6 URPROTOCOL has its own bit
      *buf++ = type & UR_URPROTOCOL? 'u': 's';
      *buf++ = type & UR_DUAL? 'd': '-';
    } else {
      *buf++ = '-';             // Dummy bit
      flags = (type/UR_DUAL) & 3;
      // D = Dual boot with SE & SPI restoration, d = dual boot with SE, f = dual boot only
      *buf++ = flags==3? 'D': flags==2? 'd': flags? 'f': '-';
    }
    flags = (type/UR_VBL) & 3;
    // V = VBL, patch & verify, v = VBL, patch only, j = VBL, jump only
    *buf++ = flags==3? 'V': flags==2? 'v': flags? 'j': 'h';
    *buf++ = hi < 077? (type & UR_PROTECTME? 'p': '-'): (type & UR_PROTECTME? 'P': 'p');
    *buf++ = (hi < 077 && (type & UR_RESETFLAGS)) || hi >= 077? 'r': '-';
    *buf++ = hi >= 077 && (type & UR_AUTOBAUD)? 'a': '-';                 // - means no
    *buf++ = hi >= 077 && (type & UR_HAS_CE)? 'c': hi >= 077? '-': '.';   // . means don't know
    *buf = 0;
  } else if(hi)                 // Version number in binary from optiboot v4.1
    sprintf_q(buf, "o%d.%d -?s-?-r--", hi, type);
  else
    sprintf_q(buf, "x0.0 .........");

  return;
}


void urbootPrintVersion(uint16_t ver, uint16_t  rjmpwp) {
  char buf[19];
  urbootPutVersion(buf, ver, rjmpwp);
  printf_q("%s", buf);
  return;
}


/********************
// Print explanation of capabilities of a bootloader
void urbootPrintCapabilitiesLong(uint16_t ver, uint16_t rjmpwp) {
  uint8_t hi = ver>>8, type = ver & 0xff;
  const char *rf  = PSTR(" r load reset flags into register R2 before jumping to the sketch\n");

  if(ver == 0xffff)             // Unknown provenance
    hi = type = 0;

  if(hi >= 072)
    printf_q("urboot v %d.%d\n", hi>>3, hi&7);
  else if(hi)
    printf_q("optiboot v %d.%d\n", hi, type);
  else
    printf_q("unknown bootloader type\n");

  if(hi >= 072) {
    const char *vbl = PSTR(" vector bootloader: ");
    const char *aup = PSTR("sketch upload patches reset/interrupt vectors ");
    const char *tec = PSTR(" protection from overwriting bootloader");
    if((hi < 077 && (type & UR_PGMWRITEPAGE)) || (hi >= 077 && rjmpwp != ub_ret_opcode)? 'w': '-')
      printf_q(" w pgm_write_page() can be called from sketch at FLASHEND+1-4\n");
    if(type & UR_EEPROM)
      printf_q(" e EEPROM read/write support\n");
    if(hi >= 076) {             // From urboot version 7.6 URPROTOCOL has its own bit
      if(type & UR_URPROTOCOL)
        printf_q(" u uses own protocol and requires avrdude -c urclock\n");
    }
    if(type & UR_DUAL)
      printf_q(" d dual boot\n");
    switch(type & UR_VBLMASK) {
    case UR_VBLPATCHVERIFY: printf_q(" V%S%Sand pretends original vectors on verify\n", vbl, aup); break;
    case UR_VBLPATCH: printf_q(" v%S%Sonly (expect an error on verify)\n", vbl, aup); break;
    case UR_VBL: printf_q(" j%Ssketch starts via interrupt vector instead of reset (must be patched externally)\n", vbl); break;
    }
    if((hi < 077 && (type & UR_PROTECTME)) || (hi >= 077 && !(type & UR_PROTECTME)))
      printf_q(" p%S\n", tec);
    if(hi >= 077 && (type & UR_PROTECTME))
      printf_q(" P%S and reset vector\n", tec);
    if((hi < 077 && (type & UR_RESETFLAGS)) || hi >= 077? 'r': '-')
      printf_q("%S", rf);
    if(hi >= 077 && (type & UR_AUTOBAUD))
      printf_q(" a autobaud detection\n");
    if(hi >= 077 && (type & UR_HAS_CE))
      printf_q(" c chip erase functionality\n");
  } else if(hi) {
    if(hi >= 4)
      printf_q("%S", rf);
  }

  return;
}
********************/

/*
 * Copy a single page from SRAM to page-aligned PROGMEM utilising smr's urboot
 *  - Takes ca 8.7 ms on an 16 MHz ATmega328p
 *  - Remember to flush serial buffers before calling
 *  - The bootloader alters the first bytes of where sram points to if it has
 *    reset protection and Page 0 is overwritten
 */
void urbootPageWrite(void *sram, progmem_t pgm) {
  stdio_flush();
#if DEBUG_TIMING
  uint32_t mi = micros();
#endif
  cli();
#if FLASHEND > 0x1FFFFul        // Set the correct memory segment for indirect jump
  EIND = ((FLASHEND+1UL-4UL)/2) >> 16;
#endif
  // Call the bootloader routine
  ((void (*)(void *sram, progmem_t pgm))((FLASHEND+1UL-4UL)/2))(sram, pgm);
#if FLASHEND > 0xFFFFul // See https://gcc.gnu.org/onlinedocs/gcc/AVR-Options.html
  RAMPZ = 0;
#endif
#if FLASHEND > 0x1FFFFul
  EIND = 0;
#endif
  sei();
#if DEBUG_TIMING
  mi = micros()-mi;
  printf_q("SPM: %lu us\n", mi);
#endif
}


/*
 * Code below makes use of the urclock programmer of avrdude. This puts just below the bootloader
 *  - Optional filename (or a description) of max 254 bytes including terminating nul
 *  - Optional date of the uploaded sketch as, in this order
 *      + 2 bytes year in [1, 2999] little endian
 *      + 1 byte month in [1, 12]
 *      + 1 byte day in [1, 31]
 *      + 1 byte hour in [0, 23]
 *      + 1 byte minute in [0, 59]
 *  - Optional PROGMEM store description: start in flash (which is the size of sketch) and its size
 *      + start is encoded as little endian number in 2 bytes (flash up to 64 k) or 4 bytes (larger)
 *      + size between sketch and metadata is encoded in the same way as start, ie in 2 or 4 bytes
 *  - One byte called mcode just below bootloader that encodes how much of above metadata is given
 *      + 255 means no metadata present and no store is used
 *      + 2...254 means all of above is present and the filename/description is this long incl nul
 *      + 1 means no filename/description present but date and store is there
 *      + 0 means only the PROGMEM store description is present
 */

// Return how many bytes metadata are needed given the mcode byte just below bootloader
uint16_t urbootNmetabytes(uint8_t mcode) {
  // The size of the structure that holds info about metadata (sits just below bootloader)
  const uint8_t nmetahd = 2*sizeof(progmem_t)+1;

  return mcode == 0xff? 0:      // No metadata at all
    mcode > 1? mcode+6+nmetahd: // Sketch filename, sketch date and structure describing pgm store
    mcode? 6+nmetahd:           // Sketch date and structure describing pgm store
      nmetahd;                  // Structure describing pgm store
}


#if !defined(THIS_BOOTLOADER_SIZE)
uintpgm_t urstoreBelowBoot() {
  uintpgm_t blstart = 0;

  if(uint8_t nblp = numblpages_ur())
    blstart = (uintpgm_t) FLASHEND - nblp*SPM_PAGESIZE;
#if defined(HW_BLSTART0)
  else
    blstart = (uintpgm_t) HW_BLSTART0;
#endif

  return blstart;
}
#endif


// Initialise the storedesc_t structure --- not needed for the other functions
void urstoreSet(storedesc_t &sd) {
  memcpy_Pnf(&sd, (progmem_t) (urstoreBelowBoot()-2*sizeof(uintpgm_t)), sizeof sd);
}


// Sanity: ensure parameters are within area bounded by store
static void __nstore(uintpgm_t where, size_t &n) {
  uintpgm_t size, avail;

  size = urstoreSize();
  avail = size - where;

  n = where >= size? 0: avail < n? avail: n;
}

void urstoreRead(uint8_t *sram, uintpgm_t where, size_t n) {
  __nstore(where, n);
  memcpy_Pnf(sram, (progmem_t) (urstoreStart() + where), n);
}

#if SPM_PAGESIZE & (SPM_PAGESIZE-1)
#error SPM_PAGESIZE is not a power of 2; code will not work
#endif

void urstoreWrite(uint8_t *sram, uintpgm_t where, size_t n) {
  __nstore(where, n);
  uintpgm_t start = urstoreStart() + where;
  // Write one page at a time; will need to read, modify & write
  while(n) {
    uint8_t page[SPM_PAGESIZE];
    size_t off, len;

    off = start & (SPM_PAGESIZE-1); // Offset of start pointer into a page
    start &= ~(uintpgm_t)(SPM_PAGESIZE-1); // Make start point to start of a page
    // Initialise page buffer with right page from PROGMEM (short code more important than speed)
    memcpy_Pnf(page, (progmem_t) start, SPM_PAGESIZE);

    // Now figure out what to copy from SRAM
    len = SPM_PAGESIZE - off;
    if(n < len)
      len = n;
    memcpy(page+off, sram, len);
    n -= len;
    sram += len;

    // Write page to memory if needed
    if(memcmp_Pnf(page, (progmem_t) start, SPM_PAGESIZE))
      urbootPageWrite(page, (progmem_t) start);

    start += SPM_PAGESIZE;
  }
}


// This would ensure parameters are within area bounded by flash, but forgoing check
void __nflash(uintpgm_t where, size_t &n) {
  uintpgm_t size, avail;

  size = urflashAvail();
  avail = size - where;

  n = where >= size? 0: avail < n? avail: n;
}

#ifndef UB_CHECK_FLASH_BOUNDARIES
#define UB_CHECK_FLASH_BOUNDARIES() // __nflash(where, n)
#endif

void urflashRead(uint8_t *sram, uintpgm_t where, size_t n) {
  UB_CHECK_FLASH_BOUNDARIES();
  memcpy_Pnf(sram, (progmem_t) (urflashStart() + where), n);
}

void urflashWrite(uint8_t *sram, uintpgm_t where, size_t n) {
  UB_CHECK_FLASH_BOUNDARIES();
  uintpgm_t start = urflashStart() + where;
  // Write one page at a time; will need to read, modify & write
  while(n) {
    uint8_t page[SPM_PAGESIZE];
    size_t off, len;

    off = start & (SPM_PAGESIZE-1); // Offset of start pointer into a page
    start &= ~(uintpgm_t)(SPM_PAGESIZE-1); // Make start point to start of a page
    // Initialise page buffer with right page from PROGMEM (short code more important than speed)
    memcpy_Pnf(page, (progmem_t) start, SPM_PAGESIZE);

    // Now figure out what to copy from sram
    len = SPM_PAGESIZE - off;
    if(n < len)
      len = n;
    memcpy(page+off, sram, len);
    n -= len;
    sram += len;

    // Write page to memory if needed
    if(memcmp_Pnf(page, (progmem_t) start, SPM_PAGESIZE))
      urbootPageWrite(page, (progmem_t) start);

    start += SPM_PAGESIZE;
  }
}



// Routines for avr opcodes, in particular jmp and rjmp for detecting the vector table


// Is the instruction word an rjmp instruction?
uint8_t ub_isRjmp(uint16_t opcode) {
  return (opcode & 0xf000) == 0xc000;
}

/*
 * Map distances to [-flashsize/2, flashsize/2) for smaller devices. As rjmp can go +/- 4 kB, so
 * smaller flash than 8k (eg, 4k) benefit from wrap around logic.
 */
int16_t ub_rjmpdistwrap(int16_t addis) {
  const int16_t size = FLASHEND+1UL > 8182? 8192: FLASHEND+1;

#if FLASHEND & (FLASHEND+1)
#error Flash size is not a power of 2; code will not work.
#endif
  addis &= size-1;
  if(addis >= size/2)
    addis -= size;

  return addis;
}


// Compute from rjmp opcode the relative distance, in bytes, of rjmp destination to rjmp address
int16_t ub_dist_rjmp(uint16_t rjmp) {
  int16_t dist;

  dist = (int16_t)(rjmp<<4) >> 3; // Sign-extend 12-bit word distance and multiply by 2

  return ub_rjmpdistwrap(dist+2); // Wraps around in flashes smaller than 8k
}

// rjmp opcode from byte distance (rjmp address minus destination address); 0xcfff is an endless loop, 0xc000 is a nop
uint16_t ub_rjmp_opcode(int16_t dist) {
  dist = ub_rjmpdistwrap(dist);
  return 0xc000 | (((dist >> 1) - 1) & 0x0fff);
}


// jmp opcode from byte address (jmp uses word address; hence, shift by that one extra bit more)
uint32_t ub_jmp_opcode(uintpgm_t addr) {
   uint32_t ret = ((uint32_t) ((addr >> 1) & 0xffff) << 16) | 0x940c;
#if UB_WITHIN_128k
  return ret;
#elif UB_WITHIN_256k
  return ret | (((addr >> 17) & 1) << 0);
#else
  return ret | (((addr >> 18) & 31)<<4) | (((addr >> 17) & 1)<<0);
#endif
}


// Byte address from jmp opcode
uintpgm_t ub_addr_jmp(uint32_t jmp) {
  uintpgm_t addr;

  addr  = jmp >> 16;            // Low 16 bit of word address are in upper word of op code
#if !UB_WITHIN_128k
  addr |= (jmp & 1) << 16;      // Add extra address bits from least significant bytes of op code
#if !UB_WITHIN_256k
  addr |= (jmp & 0x1f0) << (17-4);
#endif
#endif
  addr <<= 1;                   // Convert to byte address

  return addr;
}


// Is the instruction word the lower 16 bit part of a 32-bit instruction?
uint8_t ub_isop32(uint16_t opcode) {
  return
#if UB_WITHIN_128k
    opcode == 0x940c ||            // jmp to word address in high word
    opcode == 0x940e ||            // call to word address in high word
#else
    (opcode & 0xfe0e) == 0x940c || // jmp (larger parts need masking)
    (opcode & 0xfe0e) == 0x940e || // call (larger parts need masking)
#endif
    (opcode & 0xfe0f) == 0x9200 || // sts (register needs masking out)
    (opcode & 0xfe0f) == 0x9000;   // lds (register needs masking out)
}


// Is the instruction word the lower 16 bit part of a jmp instruction?
uint8_t ub_isJmp(uint16_t opcode) {
  return
#if UB_WITHIN_128k
    opcode == 0x940c;
#else
    (opcode & 0xfe0e) == 0x940c;
#endif
}
