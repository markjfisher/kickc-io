#include "atari-os.h"

// Internal realtime clock at 0x12, 0x13, 0x14.
// Location 0x14 increments every stage 1 VBLANK interrupt
// (1/60 second = 1 jiffy) until it reaches #0xFF, then increments 0x13, etc.
char * const RTCLOK = 0x12;

// Left margin
char const * OS_lmargn = 0x52;

// Right margin
char const * OS_rmargn = 0x53;

// = $0202/$0203    PROCEED LINE IRQ VECTOR
void() ** const VPRCED = 0x0202;

// Shift Lock, 1 = on, 0 = off
char const * OS_shflok = 0x02BE;

// Keyboard Delay Rate - makes keys repeat sooner
char const * OS_krpdel = 0x02D9;

// Keyboard Repeat Rate - makes keys appear faster
char const * OS_keyrep = 0x02DA;


// 0x2EA to 0x2ED = DVSTAT
// Four device status registers used by the I/O status operation.
// 0x2EA: device error status and the command status byte

// Bits set to one return the folloWing error codes:
// Bit Decimal Error
//  0     1    An invalid command frame was received (error).
//  1     2    An invalid data frame was received.
//  2     4    An output operation was unsuccessful.
//  3     8    The disk is write-protected.
//  4    16    The system is inactive (on standby).
//  7    32    The peripheral controller is "intelligent" (has its own microprocessor: the disk drive).
//             All Atari devices are intelligent except the cassette recorder, so BIT 7 will normally
//             be one when a device is attached
unsigned char const * OS_dvstat = 0x2EA;

// 0x2EB: device status byte
// 0x2EC: maximum device time-out value in seconds. value of 60 = 64 seconds
// 0x2ED: number of bytes in output buffer.


// OS_dcb - device control block
#define OS_dcb (*(struct __dcb*) 0x300)

#define OS_iocb0 (*(struct __iocb*) 0x340)
#define OS_iocb1 (*(struct __iocb*) 0x350)
#define OS_iocb2 (*(struct __iocb*) 0x360)
#define OS_iocb3 (*(struct __iocb*) 0x370)
#define OS_iocb4 (*(struct __iocb*) 0x380)
#define OS_iocb5 (*(struct __iocb*) 0x390)
#define OS_iocb6 (*(struct __iocb*) 0x3A0)
#define OS_iocb7 (*(struct __iocb*) 0x3B0)


// (W) Priority selection register. PRIOR establishes which objects
// on the screen (players, missiles, and playfields) will be in front of
// other objects.
char * const PRIOR = 0xD01B;

// When this location is read, it acts as a random number
// generator. It reads the high order eight bits of the 17 bit
// polynomial counter (nine bit if BIT 7 of AUDCTL is set) for the
// value of the number.
char * const RANDOM = 0xD20A;

#define ANTIC (*(struct __antic*)0xD400)

// POKE with zero, and VBLANK and system clock are disabled, and shadowing is suspended
char * const NMIEN = 0xD40E;


// PIA related
#define PIA (*(struct __pia*)0xD300)


// disable the ROM between $C000-$CFFF and $D800-$FFFF by setting bit 0 to 0.
// Bit 1 controls BASIC; if 0, BASIC is enabled, if 1, it is disabled
// Bits 2 and 3 control the 1200XL LEDs; 0 means on, 1 means off
// Bits 4-6 are reserved (unused) in the XL and 65XE.
// Bits 4 and 5 in the 130XE are used to enable bank switching
// Bit 7 controls the RAM region $5000-$57FF, normally enabled (1).
char * const PORTB = 0xD301;

// Display list pointer. Tells the OS the address of the display list
// instructions about what screen mode(s) to display and where to
// find the screen data. See also shadow register SDLIST ($230, $231).
word * const DLIST = 0xD402;

// (W) Character base address; the location of the start of the
// character set, either the standard Atari set or a user-designed set.
// The default is 224 ($EO), which points to the start of the Atari
// ROM character set
char * const CHBASE = 0xD409;

// Non Maskable Interrupt Status - holds cause for the NMI interrupt in BITs 5, 6 and 7
void * const NMIST  = 0xD40F;

// Non Maskable Interrupt Reset - Reset for NMIST. Clears the interrupt request register and resets all of the NMI status together
void * const NMIRES = 0xD40F;

// Location of the main charset when OS is on
void * const ATARI_CHARSET = 0xE000;

void * const CIOV    = 0xE456;
void * const SIOV    = 0xE459;
void * const COLDSV  = 0xE477;

void * const NMIVEC = 0xFFFA;
void * const RESVEC = 0xFFFC;
void * const IRQVEC = 0xFFFE;
