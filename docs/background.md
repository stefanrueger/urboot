# How bootloaders work

In common with bootloaders, an urboot bootloader runs after every reset and tries to communicate
with an external uploader program via a serial interface. Hence, only the serial I/O needs to be
connected from a Raspberry Pi/PC/laptop host to the board with the to-be-programmed MCU on it.
After every reset the bootloader code determines its cause: if that was an external reset as
opposed to power-up reset or watchdog timeout (WDT) it waits a small time for protocol
initialisation bytes from an uploader program. If there was was a valid handshake, the bootloader
carries out one of several possible tasks driven by the host's uploader such as receiving and
burning a new sketch, downloading an existing sketch on the MCU, and, if compiled for it, reading
or writing the EEPROM on the MCU. The bootloader finishes this part typically through a watchdog
timeout reset that kicks in when initially no valid handshake could be detected, when a serial
hardware or protocol error occurred during burning/verification or when the bootloader finished its
task successfully. Watchdog timeout resets the MCU just like an external reset. When the bootloader
is entered under this condition, though, it then normally jumps directly to the application.
However, when the bootloader was compiled with dual-boot support, it first checks external SPI
flash memory to see if it contains a new sketch, in which case a copy of it is burned from external
memory onto internal flash. The idea is that the application sketch could have been written to
receive a new version via a radio and have placed it onto the external SPI flash memory before
issuing a WDT reset. Hence, this dual-boot property is also called over the air programming,
although one could conceivably also plug a new external SPI flash into a board for programming.

## Condition to enter the bootloader

Multiple reset flags can be set on startup: watchdog, brown out, power on, external and jtag.
When should the application be started, when the serial bootloader be run, and when the external
flash memory be checked for a viable program image? In this implementation the first decision is
between serial bootloader code and starting the app. If the latter then the external flash
memory is checked if and only if the watchdog reset flag was set; that check happens before the
actual app is started. Here some choices for the first decision:

 - `if(ch != _BV(EXTRF))`

   Only run serial bootloader on 'external reset exclusively' - anything else: run the application
   (with a possible detour over checking dual booting from external SPI memory). This can lead to
   problems when the application happens to trigger the watchdog early on, which makes the "reset
   and nothing else" condition nearly impossible.

 - `if(!(ch & _BV(EXTRF)))`

   This runs the serial bootloader if and only if there was an external reset (irrespective of a
   watchdog timeout), otherwise run the application with a possible detour over checking dual
   booting from external SPI flash memory. This is the choice in urboot bootloaders.

 - `if(ch & (_BV(WDRF) | _BV(BORF) | _BV(PORF)))`

   This is the condition for starting the app in other bootloaders. It used to be that the serial
   bootloader also would run when nothing caused the reset (all `xxxRF` are 0), so you could run it
   by jumping direct to START from the application.

## Assumptions, limitations and caveats of *any* bootloader

  - Establishing communication invariably causes some small startup delay on external reset.

  - For bootloaders on ISP MCUs there is no need for a physical programmer, though in order to
    burn the bootloader itself onto the MCU, an SPI header and an SPI programmer is needed at
    least once in the beginning. Burning the bootloader using SPI necessitates the MCU be in
    reset mode: all chip select lines of attached SPI hardware (external flash memory, radios
    etc) need therefore be pulled high through resistors to ensure all external SPI devices are
    inactive during SPI programming of the bootloader.

  - In most cases the connection from the PC for uploading sketches via the bootloader will be
    through USB, which necessitates a USB to serial converter cable if the destination board
    does not have USB. Some PCs (most notably, the Raspberry Pi) have a native serial connection
    that also could be used for connecting to a board without USB.

  - The bootloader sets the TX line of the UART or the designated TX pin of the software serial
    communication to output after every reset. If the bootloader is compiled to blink an LED, to
    output a square wave for debugging or to communicate via SPI with external memory for dual
    programming, there will be other lines that are set to output shortly after each reset. Not all
    projects can deal gracefully with these short flares of output activity on some of the pins. If
    you use a bootloader for production settings it is best practice to carefully consider the
    hardware implications on the circuit.

  - As with all bootloaders they work best when the host has a way to reset the board, which is
    typically done via DTR/RTS lines of the host's serial port. Avrdude -c [arduino|urclock] does
    this automatically. The board needs to have hardware to facilitate the DTR/RTS connection for
    reset. Most boards do. If not, sometimes running a small dedicated reset program on the host
    just before running avrdude helps. This reset program somehow needs to pull the board's reset
    line low for a short time; on a Raspberry Pi external GPIO pins can be used for that. If that
    is not possible either, then setting the watchdog timeout to a long time (see the WDTO option
    below) may be helpful, so one can manually reset the board before calling avrdude. The default
    for the timeout is 1 s when using `make`, but the urclock programmer can utilise timeouts down
    to 256 ms. When compiling directly using the `urboot-gcc` wrapper the default timeout is 512 ms.


## Assumptions, limitations, caveats, tips and tricks for *this* bootloader

  - The uploading program is assumed to be AVRDUDE with the urclock programmer: use `avrdude -c
    urclock`

  - A bootloader with dual-boot support needs to know which port pin is assigned to the chip
    select of the SPI flash memory, which pin drives a blinking LED (if wanted), where the
    RX/TX lines are for serial communication, how long the watchdog timeout should be etc. This
    explodes the option space for this bootloader. Template bootloaders (see TEMPLATE option)
    provide space holders for code that sets/clears the right pin for LED and chip select for
    dual bootloaders. In most cases they won't work unmodified, though.

  - Remember that dual-boot bootloaders communicate with the SPI flash memory at all external
    and WDT resets. Therefore, all other attached SPI devices need their chip select pulled
    high. While in theory urboot.c could be changed so that it uses software pullup for these
    additional devices, this could cost additional code and will complicate the already too big
    option space. It is better practice to use hardware pullups in the board design.

  - A dual-boot bootloader deals gracefully with a board that has no external SPI flash memory:
    it simply reads 0xff values and decides there is nothing interesting there. However, this
    causes unnecessary delay at each external reset and each WDT reset, and it toggles the SPI
    and chip select lines (see above); it is therefore not recommended to burn the most-feature
    rich urboot bootloader onto all your boards. Carefully determine what you actually need.

  - When using external SPI memory on a board with a dual-boot bootloader, remember to reserve
    the first FLASHEND+1 bytes exclusively for sketches to be burned onto the MCU. These are
    recognised by the second byte being indicative of an rjmp or a jmp opcode. Placing  some
    random data in that area risks the MCU being programmed with just these random data.

  - Vector bootloaders are great for devices with no dedicated boot section. They assume the
      + Vector table of the MCU, and therefore the reset vector, resides at address zero
      + Compiler puts either a jmp or an rjmp at address zero
      + Compiler does not zap/shorten the vector table if no or few interrupts are used
      + Compiler does not utilise unused interrupt vectors to place code/data there

    This should be the case with all regular sketches produced by avr-gcc. Vector bootloaders are
    also useful for devices with boot section support to allow them to use less space than the
    smallest offered hardware-supported boot section. In this case, the fuses must be set to
    make the processor jump to the reset vector as opposed to the bootloader. Also, the
    protection bits in the lock byte should allow code be written into the bootloader section
    with SPM instructions. Otherwise the extra space in the boot section that is freed by smaller
    vector bootloaders cannot be used.

  - The code makes several assumptions that reduce the code size (eg, no interrupts can occur, the `USART`
    character size register defaults to 8N1). They are true after a hardware reset, but will not
    necessarily be true if the bootloader is called directly. So don't.

