/*
 * Print baud rate quantisation error for 8-bit AVRs as set up in the bootloader urboot.c source
 *
 * Published under GNU General Public License, version 3 (GPL-3.0)
 * Author Stefan Rueger <stefan.rueger@urclocks.com>
 *
 * v1.0
 * 11.03.2023
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <errno.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <math.h>

#include "ur_uarttable.c"

char progname[]="bauderror";

void Usage(void);
void fatal(const char *fmt, ...);
extern int errno;

Uart_info *uinfo(const char *p) {
  for(size_t i=0; i<sizeof Uart_table/sizeof*Uart_table; i++)
    if(strcasecmp(Uart_table[i].avrname, p)==0)
      return Uart_table+i;
  return NULL;
}


// Max value of Baud Rate Register
int maxbrr(Uart_info *up) {
  return up->brr_is12bit? 4095: 255;
}

int rawuartbrr(Uart_info *up, long f_cpu, long br, int nsamples) {
  switch(up->uarttype) {
  case 1:
  case 2:
    return (f_cpu + nsamples*br/2)/(nsamples*br) - 1;
  }

  return 0;
}

// Baud rate register value given f_cpu, desired baud rated and number 8..63 of samples
int uartbrr(Uart_info *up, long f_cpu, long br, int nsamples) {
  int ret = rawuartbrr(up, f_cpu, br, nsamples), mxb = maxbrr(up);

  return ret<0? 0: ret > mxb? mxb: ret;
}

// Actual baud rate given f_cpu, desired baud rated and number 8..63 of samples
long uartbaud(Uart_info *up, long f_cpu, long br, int nsamples) {
  return f_cpu/(nsamples*(uartbrr(up, f_cpu, br, nsamples) + 1));
}

// Uart quanitisation error in per mille (signed)
int uartqerr(Uart_info *up, long f_cpu, long br, int nsamples) {
  long bdiff = (uartbaud(up, f_cpu, br, nsamples)-br)*1000L;
  return bdiff<0? -(-bdiff/br): bdiff/br;
}


// Return either UART2X=1 or UART2X=0 depending on f_cpu, desired baud rate and preference u2x
int uart2x(Uart_info *up, long f_cpu, long br, int u2x) {
  if(!u2x || !up->has_u2x) // No choice: must use 1x mode
    return 0;
  /*
   * Part can choose between UART2X = 1 (nsamples == 8) and UART2X = 0 (nsamples == 16) and the
   * user doesn't mind. Switch on 2x mode if error for normal mode is > 2.5% and error with 2x
   * less than normal mode considering that normal mode has higher tolerances than 2x speed mode.
   * The reason for a slight preference for UART2X=0 is that is costs less code in urboot.c.
   */
  int e1 = abs(uartqerr(up, f_cpu, br, 8)), e0 = abs(uartqerr(up, f_cpu, br, 16));

  return 20*e1 < 15*e0 && e0 > 25;
}


// Return l1 or l2, whichever causes less error; on same quantised error return the larger value
int linbetter2_ns(Uart_info *up, long f_cpu, long br, int l1, int l2) {
  int e1 = abs(uartqerr(up, f_cpu, br, l1)), e2 = abs(uartqerr(up, f_cpu, br, l2));
  return e1 < e2? l1: e1 > e2? l2: l1 > l2? l1: l2;
}

// Best of 4
int linbetter4_ns(Uart_info *up, long f_cpu, long br, int l1, int l2, int l3, int l4) {
  return linbetter2_ns(up, f_cpu, br,
    linbetter2_ns(up, f_cpu, br, l1, l2),
    linbetter2_ns(up, f_cpu, br, l3, l4));
}

// Best of 8
int linbetter8_ns(Uart_info *up, long f_cpu, long br,
  int l1, int l2, int l3, int l4, int l5, int l6, int l7, int l8) {
  return linbetter2_ns(up, f_cpu, br,
    linbetter4_ns(up, f_cpu, br, l1, l2, l3, l4),
    linbetter4_ns(up, f_cpu, br, l5, l6, l7, l8));
}

// Best of all possible ns = 8..63
int linbest_ns(Uart_info *up, long f_cpu, long br) {
  return linbetter8_ns(up, f_cpu, br,
    8,
    linbetter8_ns(up, f_cpu, br,  8,  9, 10, 11, 12, 13, 14, 15),
    linbetter8_ns(up, f_cpu, br, 16, 17, 18, 19, 20, 21, 22, 23),
    linbetter8_ns(up, f_cpu, br, 24, 25, 26, 27, 28, 29, 30, 31),
    linbetter8_ns(up, f_cpu, br, 32, 33, 34, 35, 36, 37, 38, 39),
    linbetter8_ns(up, f_cpu, br, 40, 41, 42, 43, 44, 45, 46, 47),
    linbetter8_ns(up, f_cpu, br, 48, 49, 50, 51, 52, 53, 54, 55),
    linbetter8_ns(up, f_cpu, br, 56, 57, 58, 59, 60, 61, 62, 63));
}


// Cycles per bit given the number of delay loop iterations for software I/O
long swio_cpb(Uart_info *up, int val) {
  int is_xmega = strncasecmp(up->avrname, "ATxmega", 7) == 0;

  if(!is_xmega && !up->pc_22bit)
    return 6L*val + 14+9;       // Classic MCU with 16-bit PC
  if(!is_xmega)
    return 6L*val + 18+9;       // Classic MCU with 22-bit PC
  if(!!up->pc_22bit)
    return 6L*val + 12+10;      // XMEGA with 16-bit PC

  return 6L*val + 16+10;        // XMEGA with 22-bit PC
}

// Number of delay loop iterations given the cycles per bit
int swio_b_value(Uart_info *up, int cpb, int b_off) {
  int is_xmega = strncasecmp(up->avrname, "ATxmega", 7) == 0;

  if(!is_xmega && !up->pc_22bit)
    return (cpb-14-9+b_off+60)/6-10;  // Classic MCU with 16-bit PC
  if(!is_xmega)
    return (cpb-18-9+b_off+60)/6-10;  // Classic MCU with 22-bit PC
  if(!!up->pc_22bit)
    return (cpb-12-10+b_off+60)/6-10; // XMEGA with 16-bit PC

  return (cpb-16-10+b_off+60)/6-10;   // XMEGA with 22-bit PC
}


int main(int argc, char **argv) {
  int filargc=1, i=0;
  char *p;

  int u2x = 1, swio = 0, intnum = 0, absnum = 0, verbose = 0, raw, mxb, smp;
  const char *mcu = "ATmega328P", *errstr = NULL, *mode = "UNKNOWN";
  long f_cpu = 16000000L, brate = 115200L, gotbaud = 123L;
  double err;

  while(--argc) {
    if(*(p=argv[++i]) == '-') {
      while(*++p) {
        switch(isascii(*p)? *p: '?') {
        case 'f':
          if(*++p=='=')
            p++;
          p = *p || argc<2? p: (argc--, argv[++i]);
          f_cpu = atol(p);
          p=""-1;
          break;
        case 'b':
          if(*++p=='=')
            p++;
          p = *p || argc<2? p: (argc--, argv[++i]);
          brate = atol(p);
          p=""-1;
          break;
        case 'u':
          u2x = 0;
          break;
        case 'v':
          verbose++;
          break;
        case 's':
          swio = 1;
          break;
        case 'a':
          absnum = 1;
          break;
        case 'i':
          intnum = 1;
          break;
        default:
          Usage();
          fatal("unknown option '%c'", *p);
        }
      }
    } else
      argv[filargc++]=argv[i];
  }

  if(filargc > 2)
    fatal("too many MCUs specified\n");

  if(filargc == 2)
    mcu = argv[1];

  Uart_info *up = uinfo(mcu);
  if(!up) {
    fprintf(stderr, "unknown part %s\n", mcu);
    exit(1);
  }
  mcu = up->avrname;

  if(up->uarttype == 2)         // Urboot.c only uses LINBRRL, not the high 4 bit
    up->brr_is12bit = 0;

  if(!swio) {
    switch(up->uarttype) {
    case 0:
      swio = 1;
      break;
    case 1:                     // Classic parts with regular UART
      smp = uart2x(up, f_cpu, brate, u2x)? 8: 16;
      raw = rawuartbrr(up, f_cpu, brate, smp);
      mxb = maxbrr(up);
      if(raw > mxb)
        errstr = "unachievable baud rate (too slow)";
      else if(raw < 0)
        errstr = "unachievable baud rate (too fast)";
      else
        gotbaud = uartbaud(up, f_cpu, brate, smp);
      mode = "UART";
      break;
    case 2:                     // Classic parts with LINUART
      if(f_cpu > brate*64L*(maxbrr(up)+1)) // Quantisation error 64/63, ie, ca 1.6%
        errstr = up->brr_is12bit?
          "baud rate too small for 12-bit LINBRR": "baud rate too small for 8-bit LINBRR";
      else if(f_cpu < 79L*brate/10L) // Quantisation error 79/80, ie, ca 1.25%
        errstr = "baud rate too big";
      else
        gotbaud = uartbaud(up, f_cpu, brate, linbest_ns(up, f_cpu, brate));
      mode = "LINUART";
      break;
    }
  }

  if(swio) {
    // Cycles per tx/rx bit
    long cpb = (f_cpu+brate/2)/brate;
    // Delay loop has granularity of 6 cycles - want around 1% accuracy
    int b_off = cpb > 600?
      3: // 3 centres the error: max error is +/- 3 cycles
      0; // Underestimate b_value and insert opcodes for extra cycles (0..5)
    int b_value = swio_b_value(up, cpb, b_off);
    int b_cpb = swio_cpb(up, b_value);
    int b_extra = cpb > 600? 0: cpb - b_cpb;

    if(b_value > 255)
      errstr = "baud rate too slow for SWIO";
    else if(b_value < 0)
     errstr = "baud rate too fast for SWIO";
    else if(b_extra > 5 || b_extra < 0)
      errstr = "baud rate incompatible with F_CPU for SWIO";
    else
      gotbaud = f_cpu/(swio_cpb(up, b_value) + b_extra);
    mode = "SWIO";
  }

  err = 100.0*(gotbaud - brate)/brate;
  if(absnum)
    err = fabs(err);
  if(errstr)
    err = 9999.9;
  else if(err > 1000)
    err = 1000;
  else if(err < -1000)
    err = -1000;

  if(verbose) {
    if(errstr)
      printf("The %s @ %ld Hz cannot create %s %ld baud: %s\n", mcu, f_cpu, mode, brate, errstr);
    else
      printf("The %s @ %ld Hz has a %s baud rate of %ld baud = %ld%+.2f%% baud.\n",
        mcu, f_cpu, mode, gotbaud, brate, err);
  } else if(intnum)
    printf("%ld\n", lround(10*err));
  else
    printf(absnum? "%.2f%%\n": "%+.2f%%\n", err);
}


void Usage() {
  static char *Use[]= {
    "Syntax: ", progname, " [<opts>] {[<mcuname>] [<opts]}\n",
    "Function: prints baud rate error of urboot bootloaders given part, F_CPU, baud rate etc\n",
    "Options:\n",
    "    -f=<F_CPU> Clock frequency (default 16 MHz) of <mcuname> (default ATmega328P)\n",
    "    -b=<baud>  Desired baud rate <baud> (default 115200 baud)\n",
    "    -s         Software I/O instead of UART\n",
    "    -a         Print absolute error, not a signed error\n"
    "    -i         Print integer error per mille, eg, -12 means -1.2%\n"
    "    -u         Enforce UART2X = 0 for classic UART part\n",
    "    -v         Verbose mode\n",
    NULL
  };

  int i=0;
  while(Use[i])
    fprintf(stderr, "%s", Use[i++]);
}


void fatal(const char *fmt, ...) {
  va_list args;
  char *err;

  fflush(stdout);
  fprintf(stderr, "%s: ", progname);
  va_start (args, fmt);
  vfprintf(stderr, fmt, args);
  va_end (args);
  if(errno && (err = strerror(errno)) && *err)
    fprintf(stderr, " (%c%s)", islower(err[1])? tolower(*err): *err, err+1);
  fprintf(stderr, "\n");

  fflush(stderr);
  exit(errno? errno: 1);
}
