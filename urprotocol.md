# Urprotocol

The urprotocol is a variation of the [STK500v1
protocol](https://ww1.microchip.com/downloads/en/AppNotes/doc2525.pdf) that was once (until ca
2005) used by Atmel for their STK500 programmers and still today retains popularity for programming
bootloaders. For example, `avrdude -c arduino` communicates with STK500v1 bootloaders, and `avrdude
-c urclock` with urprotocol bootloaders. The main purpose of urprotocol is to simplify it by
removing unnecessary get and set parameter calls, streamlining address and parameter handling, and
to provide a side-channel information for the MCU id and up to 5 parameter bits. It has a
backward-compatibility mode, so that urprotocol can cope with traditional STK500v1 bootloaders. As
such `avrdude -c urclock` can handle bootloaders that would normally be programmed with `avrdude -c
arduino`.

## Why a new protocol?

Whilst a skeleton implementation STK500v1 protocol is used in one of the most popular bootloaders,
optiboot, this has the following disadvantages; it
 - Requests SW/HW version from the bootloader, which costs code bytes in the bootloader to decode
   the request send something back with little benefit
 - Humours not-needed get parameter and set parameter requests; when using bootloaders there is no
   intermediate device with parameters; decoding the requests and ignoring them costs unnecessary
   bootloader code
 - Expects a chip erase command to be honoured, although that is neither needed nor universally
   implemented in bootloaders because SPM flash programming *can* erase a page before writing it
 - Communicates 16-bit *word* addresses for flash page reads/writes and should send 16-bit *byte*
   addresses for EEPROM though for historic reasons avrdude sends a word address here too; the
   bootloader uses `spm` and `lpm` opcodes that need *byte* addresses and therefore needs to
   multiply the word address by two (4 bytes code); 128 kB flash parts can be  handled, but
   initialising the necessary `RAMPZ` register is not straightforward (more code bytes gone
 - Handles addresses that exceed 16-bit words through a separate load extended byte command (of a
   *word* address) that needs managing a *separate* variable in the bootloader, for which `RAMPZ`
   is  typically used; processing this extra command not only costs additional code but also can
   lead to [subtle errors in the protocol use that need correcting by the
   uploader](https://github.com/avrdudes/avrdude/blob/0f956e90eb66e0ba7a4fecdd59e6fab31f346963/src/stk500.c#L701-L711)
 - Does not support vector bootloaders, which want the reset vector to point to them and want
   another (never otherwise used) vector in (or just behind) the vector table to point to the
   application; patching the vector table by the bootloader itself during avrdude upload/verify is
   very costly (the optiboot source code reckons some 110 bytes), but this is a task that a
   dedicated avrdude `-c` programmer could easily do meaning vector bootloading should cost the
   bootloader *zero* bytes additional code
 - Identifies the to be programmed part by requesting signature bytes through a protocol command
   exchange that needs to be decoded and served; again, this puts some strain on the bootloader but
   there are other mechanisms costing less (or no) bootloader code

The urprotocol addresses all of the above shortcomings. As a consequence the bootloaders
utilising it can save between 76 and 112 bytes not counting the 110 byte savings for vector
bootloaders.

## Details of the urprotocol

The **explicit communication** between an uploader/downloader program (*the programmer*, think
`avrdude -c urclock`) and the bootloader is driven by the programmer, which sends command sequences
to the bootloader and evaluates their return sequences. A command sequence starts by a command
byte, followed by its parameters, followed by an end-of-parameter byte `UR_EOP`. In return the
bootloader sends a fixed byte `UR_INSYNC` to acknowledge the command, then executes it, possibly
returning data, followed by sending a different fixed byte `UR_OK`.

Although the `UR_INSYNC` and `UR_OK` are *fixed constants* for a particular bootloader, they *can
vary* between bootloaders to indicate
 - Which MCU the bootloader sits on (using one of up to 2040 predefined different MCU IDs)
 - Whether or not the bootloader provides a paged-read flash command
 - Whether or not the bootloader has implemented the chip erase command
 - Whether or not writing a memory page memory looks like programming NOR memory
 - Two more whether-or-not bits that are currently reserved

As `UR_INSYNC` and `UR_OK` should always differ, there are 256 $\cdot$ 255 possible combinations,
one of which is reserved for backward compatibility mode where `UR_INSYNC` and `UR_OK` coincide
with the respective STK500v1 constants. This protocol definition enables the bootloader to pass to
the programmer log<sub>2</sub>(256 $\cdot$ 255-1) bits = 15.994331... bits of configuration information
without having to spend a single additional byte of bootloader code. Subtracting the 5 bits for the
whether-or-not info leaves 10.994331... bits which allows 2040 $\approx$ 2<sup>10.99433...</sup>
MCU ids.

**Parameters.** Paged EEPROM/flash access commands and page erase are the only commands that need
parameters. In this case the parameters are the address, followed by the length of the block to
read or write and, if needed, followed by the bytes to be written. As in STK500v1, addresses are
given as little endian (low byte first) and length as big endian (high byte first). The address
always is a byte address (unless in compatibility mode). It is a 16-bit address for MCUs that have
65536 bytes flash or less, and a 24-bit address for MCUs with larger flash. Zero-length reads or
writes are not supported by the protocol. If the *flash* page size is 256 or less, then the length
parameter is sent as one byte (where 0 means 256 bytes). Otherwise the length parameter is sent as
two bytes (where 0 means 65536). Note, however, that the only valid length for the write flash page
command is the MCU page size; also the *maximum* valid length for EEPROM writes is 256 or the
*flash* page size, whichever is higher. EEPROM write page commands should never exceed the size of
half of SRAM though. The other two (read) paged-access commands are free to request any length
between 1 and 256, and 1 and 65536, respectively. However, the programmer must never ask for an
address block that would access bytes outside the range of EEPROM or flash on the device. Whilst
the number of parameter bytes differs between bootloaders, for a particular bootloader the address
and length is given always in the same way. This means that the EEPROM address on an MCU with a
large flash will be a 24-bit address even though the EEPROM might only have 8192 bytes. Even though
the write flash page command only allows one length, and page erase does not need a page at all, it
must always be specified. This is to simplify the bootloader effort to decode the programmer's
command sequences.


Urprotocol commands

 - **`UR_GET_SYNC`:** The bootloader does nothing except returning the two protocol bytes. Its
   purpose is to synchronise the programmer with the bootloader and to identify the type of
   bootloader and (some of) its properties. For synchronisation, the programmer should issue a
   number of `UR_GET_SYNC` commands until it receives consistent `UR_INSYNC` and `UR_OK` values.
   At this point the programmer knows whether or not to switch to backward compatibility mode
   using the STK500v1 protocol as in `-c arduino`, which MCU is to be programmed etc. It is
   advised the programmer sets its read timeout in the synchronisation phase to less than 100 ms
   when reading the bootloader reply to avoid triggering the bootloader's watchdog timer. It is
   also recommended that the input is drained after successfully reading two response bytes to
   ensure the response has not been brought about by an application program of the connected
   board before the board was reset into bootloader mode. This command can also be used
   periodically to prevent the bootloader from timing out.

 - **`UR_PROG_PAGE_FL`:** One flash page is written to the device. In the absence of a
   `UR_CHIP_ERASE` (see below), the bootloader is expected to program the flash page as atomic
   page erase, page load and page write. If the bootloader implements `UR_CHIP_ERASE`, it has the
   choice of erasing a flash page before programming it or not. In case the bootloader erases
   pages before writing them, the payload of the `UR_PROG_PAGE` is programmed exactly as is; the
   programmer should implement desired sub-page modifications by first reading the flash
   contents of the not-to-be-modified page parts to correctly pad the page payload. If the
   bootloader does not erase pages before writing them, effectively the payload is *and*ed to
   the existing contents of the page thereby exposing the physical property of the underlying
   NOR flash memory; sub-page modifications can be carried out by padding the page buffer
   payload with 0xff, as programming 0xff is a NOP for AVR NOR flash memories.

 - **`UR_CHIP_ERASE`** (optional): If implemented, the bootloader erases to 0xff all flash
   except itself. After issuing the chip erase request it is advised the programmer set its
   timeout for reading the next character to more time than the bootloader will need to erase
   flash to avoid the programmer resuming communication before the bootloader comes back from
   the chip erase. 20 s should be sufficient. If the bootloader does not implement chip erase
   then the programmer should ensure that flash is erased to 0xff by, eg, repeated
   `UR_PROG_PAGE` calls with 0xff-only contents or equivalent; this normally takes longer than
   bootloader chip erase but is otherwise functionally equivalent to a `UR_CHIP_ERASE`
   implementation in the bootloader. The protocol does not expect EEPROM to be erased in either
   case. However, when implementing `UR_CHIP_ERASE` the bootloader is free to read fuses to
   determine whether or not EEPROM should also be erased and erase EEPROM accordingly.

 - **`UR_READ_PAGE_FL`** (optional) returns n=length bytes of flash from the given address

 - **`UR_READ_PAGE_EE`** (optional) returns n=length bytes of EEPROM from the given address

 - **`UR_PROG_PAGE_EE`** (optional) writes n=length bytes to the EEPROM at the given address

 - **`UR_PAGE_ERASE`** (optional) erases to 0xff a page at the given address (length must be given
   but is ignored)

 - **`UR_LEAVE_PROGMODE`** (optional): some bootloaders reduce the Watchdog timeout so that the
   application is started faster after programming

 - **Any other command**, should behave like `UR_GET_SYNC`, ie, the bootloader returns
   `UR_INSYNC` and `UR_OK`.


**Error handling.** It is generally considered an error if the programmer asks for not
implemented functionality, as it knows after synchronisation how the bootloader is configured.
Hence, the bootloader WDT should reset on request of an optional, not implemented command.
Typically, the bootloader would need to save the payload of EEPROM/flash writes to SRAM; for
security reasons the bootloader should trigger a WDT reset if an illegitimate length of a paged
write could overwrite the stack (eg, a request for writing 256 bytes EEPROM on a part with only
256 bytes SRAM). A protocol error detected by the bootloader (failure to match `UR_EOP`) should
lead to a WDT reset. Protocol errors detected by the programmer (not matching `UR_INSYNC` or
`UR_OK`) should normally lead to a termination of programming attempts. Frame errors in serial
communication should also lead to a WDT reset or termination of programming, respectively. The
bootloader should protect itself from being overwritten through own page writes and page erases.


**Implicit communication** of further bootloader properties happens through a small table located
at the top of the bootloader section. Normally, the programmer can read this table after
establishing the MCU id, and therefore either the location of top flash of the part for which the
bootloader was compiled or the location of the bootloader end otherwise. The 6-byte table contains
(top to bottom):
  - Version number: one byte, minor version 0..7 in three lsb, major version 0..31 in the 5 msb
  - Capabilities byte detailing the following properties
     + Bit 7, if set in versions up to v7.6 indicates `pgm_write_page(sram, flash)` can be called
       from application at `FLASHEND+1-4`; from v7.7 Bit 7 indicates that the bootloader selects
       the baud rate automatically depending on the host baud rate; whether or not
       `pgm_write_page(sram, flash)` is available can still be seen from the `rjmp` entry below
     + Bit 6, if set, indicates EEPROM read/write support
     + Bit 5, if set, indicates urprotocol use that requires `avrdude -c urclock`
     + Bit 4, if set, indicates the bootloader supports dual boot from external SPI flash
     + Bits 3 and 2 indicate whether or not the bootloader is a vector bootloader
       | Bit 3 | Bit 2 | Bootloader ...|
       | :-: | :-: | :-- |
       | 0 | 0 | ... is not a vector bootloader, must set fuses to HW bootloader support |
       | 0 | 1 | ... is a vector bootloader, ie, starts the application via an interrupt vector instead of reset |
       | 1 | 0 | ... is a vbl that patches reset/interrupt vectors itself (expect an error on verify) |
       | 1 | 1 | ... is a vbl that patches reset/interrupt vectors *and* shows original ones on verify |
     + Bit 1, if set, indicates bootloader safeguards against overwriting itself
     + Bit 0, if set in versions up to v7.6, indicates bootloader will load reset flags into
       register R2 before starting the application; from v7.7 onwards this is always the case, and
       Bit 0 indicates whether or not the bootloader offers a chip erase function
  - Two-byte `rjmp` to a `pgm_write_page(ram, flash)` function or a `ret` opcode if not implemented
  - Number 1..127 of pages that the bootloader occupies
  - Vector number 1..127 for the `r/jmp` to the application if it is a vector bootloader, 0 otherwise

If the bootloader does not have read capabilities the user needs to supply necessary information
such as the bootloader size to the programmer on the command line. In `avrdude` this is via -x
extended parameters.

**Backward compatibility mode.** When urprotocol after synchronisation with the bootloader
settles on `UR_INSYNC` and `UR_OK` values that turn out to be the STK500v1 values of 0x14 and 0x10,
this triggers a backward compatibility mode. In this instance the programmer behaves (almost)
like the STK500v1 implementation in avrdude's arduino programmer, ie, it handles optiboot and
legacy bootloaders gracefully: in particular, the programmer can issue STK_READ_SIGN and two
STK_UNIVERSAL requests (load extended address and chip erase) that the bootloader must implement
in the backward compatibility mode. All EEPROM/flash addresses are sent as two-byte word
addresses *little* endian, all length arguments are two-byte *big* endian, etc. Unlike `avrdude
-c arduino` the programmer for the urprotocol should not pass on get and set hardware parameter
requests, enquire software and hardware versions etc, as these requests would be wasteful for
the bootloader. Under the urprotocol, bootloaders should be assured they do not need to even
provide code to ignore these requests, even if they operate in the backwards compatibility mode.


**Limitations.** Urprotocol only has provisions for reading EEPROM and flash, for writing EEPROM
and for writing flash other than the bootloader area. In particular, urprotocol has no
provisions for reading other memories such as the signature (other than in backward
compatibility mode), calibration bytes, locks or fuses, and neither for writing lock bytes. The
protocol does not consider sub-page flash writes, which are shifted to the programmer. If the
bootloader's flash write does *not* look like NOR programming *and* if the bootloader does *not*
provide flash read, then sub-page modifications simply cannot be done. Installing a bootloader
has security implications as it provides a means to modify flash thus weakening the Harvard
architecture of AVR microprocessors. Even bootloader implementations that are hardened against
prohibited address and length parameters have, out of necessity, somewhere a code sequence that
manipulates flash memory. A flawed application might still give an attacker a way to call these
code sequences, so be warned here be dragons.
