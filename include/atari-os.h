/*
 * Atari OS definitions, mostly taken from cc65 for compatibility.
 * Combinations of antic, pia, etc for one include file.
 */

#ifndef __ATARI_OS_H
#define __ATARI_OS_H

struct __iocb {
    unsigned char   handler;    /* handler index number (0xff free) */
    unsigned char   drive;      /* device number (drive) */
    unsigned char   command;    /* command */
    unsigned char   status;     /* status of last operation */
    void*           buffer;     /* pointer to buffer */
    void*           put_byte;   /* pointer to device's PUT BYTE routine */
    unsigned int    buflen;     /* length of buffer */
    unsigned char   aux1;       /* 1st auxiliary byte */
    unsigned char   aux2;       /* 2nd auxiliary byte */
    unsigned char   aux3;       /* 3rd auxiliary byte */
    unsigned char   aux4;       /* 4th auxiliary byte */
    unsigned char   aux5;       /* 5th auxiliary byte */
    unsigned char   spare;      /* spare byte */
};

typedef struct __iocb iocb_t;

#define IOCB_OPEN        0x03  /* open */
#define IOCB_GETREC      0x05  /* get record */
#define IOCB_GETCHR      0x07  /* get character(s) */
#define IOCB_PUTREC      0x09  /* put record */
#define IOCB_PUTCHR      0x0B  /* put character(s) */
#define IOCB_CLOSE       0x0C  /* close */
#define IOCB_STATIS      0x0D  /* status */
#define IOCB_SPECIL      0x0E  /* special */
#define IOCB_DRAWLN      0x11  /* draw line */
#define IOCB_FILLIN      0x12  /* draw line with right fill */
#define IOCB_RENAME      0x20  /* rename disk file */
#define IOCB_DELETE      0x21  /* delete disk file */
#define IOCB_LOCKFL      0x23  /* lock file (set to read-only) */
#define IOCB_UNLOCK      0x24  /* unlock file */
#define IOCB_POINT       0x25  /* point sector */
#define IOCB_NOTE        0x26  /* note sector */
#define IOCB_GETFL       0x27  /* get file length */
#define IOCB_CHDIR_MYDOS 0x29  /* change directory (MyDOS) */
#define IOCB_MKDIR       0x2A  /* make directory (MyDOS/SpartaDOS) */
#define IOCB_RMDIR       0x2B  /* remove directory (SpartaDOS) */
#define IOCB_CHDIR_SPDOS 0x2C  /* change directory (SpartaDOS) */
#define IOCB_GETCWD      0x30  /* get current directory (MyDOS/SpartaDOS) */
#define IOCB_FORMAT      0xFE  /* format */

struct __dcb {
    unsigned char ddevic;           /* device id */
    unsigned char dunit;            /* unit number */
    unsigned char dcomnd;           /* command */
    unsigned char dstats;           /* command type / status return */
    void          *dbuf;            /* pointer to buffer */
    unsigned char dtimlo;           /* device timeout in seconds */
    unsigned char dunuse;           /* - unused - */
    unsigned int  dbyt;             /* # of bytes to transfer */

	/* Part of Union - not supported by kickc
	unsigned int  daux;             // auxiliary as word
	*/

    unsigned char daux1;            // 1st command auxiliary byte
    unsigned char daux2;            // 2nd command auxiliary byte

};

typedef struct __dcb dcb_t;


/*****************************************************************************/
/* Define a structure with the ANTIC coprocessor's register offsets          */
/*****************************************************************************/

struct __antic {
    unsigned char   dmactl; /* (W) direct memory access control */
    unsigned char   chactl; /* (W) character mode control */
    unsigned char   dlistl; /* display list pointer low-byte */
    unsigned char   dlisth; /* display list pointer high-byte */
    unsigned char   hscrol; /* (W) horizontal scroll enable */
    unsigned char   vscrol; /* (W) vertical scroll enable */
    unsigned char   unuse0; /* unused */
    unsigned char   pmbase; /* (W) msb of p/m base address (for when DMACTL has player and/or missile DMA enabled) */
    unsigned char   unuse1; /* unused */
    unsigned char   chbase; /* (W) msb of character set base address */
    unsigned char   wsync;  /* (W) wait for horizontal synchronization */
    unsigned char   vcount; /* (R) vertical line counter */
    unsigned char   penh;   /* (R) light pen horizontal position */
    unsigned char   penv;   /* (R) light pen vertical position */
    unsigned char   nmien;  /* (W) non-maskable interrupt enable */
    unsigned char   nmist;  /* (R) nmi status, (W) nmi reset - sorry, no unions in kickc yet */
};

/*****************************************************************************/
/* DMACTL register options                                                   */
/*****************************************************************************/

/* Initialized to 0x22: DMA fetch, normal playfield, no PMG DMA, double-line PMGs */

/* Playfield modes: */
#define DMACTL_PLAYFIELD_NONE     0x00
#define DMACTL_PLAYFIELD_NARROW   0x01 /* e.g., 32 bytes per scanline with thick borders */
#define DMACTL_PLAYFIELD_NORMAL   0x02 /* e.g., 40 bytes per scanline with normal borders */
#define DMACTL_PLAYFIELD_WIDE     0x03 /* e.g., 48 bytes per scanline with no borders (overscan) */

/* Other options: */

/* If not set, GTIA's GRAFP0 thru GRAFP3, and/or GRAFM registers are used for
** player & missile shapes, respectively.  (Modify the registers during the horizontal blank
** (Display List Interrupt), a la "racing the beam" on an Atari VCS/2600, )
** if set, ANTIC's PMBASE will be used to fetch shapes from memory via DMA.
*/
#define DMACTL_DMA_MISSILES    0x04
#define DMACTL_DMA_PLAYERS     0x08

/* Unless set, PMGs (as fetched via DMA) will be double-scanline resolution */
#define DMACTL_PMG_SINGLELINE  0x10

/* Unless set, ANTIC operation is disabled, since it cannot fetch
** Display List instructions
*/
#define DMACTL_DMA_FETCH       0x20


/*****************************************************************************/
/* CHACTL register options                                                   */
/*****************************************************************************/

/* Initialized to 2 (CHACTL_CHAR_NORMAL | CHACTL_INV_PRESENT) */

/* Inverted (upside-down) characters */
#define CHACTL_CHAR_NORMAL    0x00
#define CHACTL_CHAR_INVERTED  0x04

/* Inverse (reverse-video) characters */
#define CHACTL_INV_TRANS      0x00 /* chars with high-bit shown */
#define CHACTL_INV_OPAQUE     0x01 /* chars with high-bit appear as space */
#define CHACTL_INV_PRESENT    0x02 /* chars with high-bit are reverse-video */


/*****************************************************************************/
/* Values for NMIEN (enabling interrupts) & NMIST (cause for the interrupt)  */
/*****************************************************************************/

#define NMIEN_DLI   0x80

/* Vertical Blank Interrupt
** Called during every vertical blank; see SYSVBV, VVBLKI, CRITIC, and VVBLKD,
** as well as the SETVBV routine.
*/
#define NMIEN_VBI   0x40 

/* [Reset] key pressed */
#define NMIEN_RESET 0x20


/*****************************************************************************/
/* ANTIC instruction set                                                     */
/*****************************************************************************/

/* Absolute instructions (non mode lines) */
#define DL_JMP  ((unsigned char) 1)
#define DL_JVB  ((unsigned char) 65)

#define DL_BLK1 ((unsigned char) 0)   /* 1 blank scanline */
#define DL_BLK2 ((unsigned char) 16)  /* 2 blank scanlines */
#define DL_BLK3 ((unsigned char) 32)  /* ...etc. */
#define DL_BLK4 ((unsigned char) 48)
#define DL_BLK5 ((unsigned char) 64)
#define DL_BLK6 ((unsigned char) 80)
#define DL_BLK7 ((unsigned char) 96)
#define DL_BLK8 ((unsigned char) 112)


/* Absolute instructions (mode lines) */

/* Note: Actual width varies (e.g., 40 vs 32 vs 48) depending on
** normal vs narrow vs wide (overscan) playfield setting; see DMACTL
*/

/* Character modes (text, tile graphics, etc.) */

/* monochrome, 40 character & 8 scanlines per mode line (aka Atari BASIC GRAPHICS 0 via OS's CIO routines) */
#define DL_CHR40x8x1    ((unsigned char) 2)

/* monochrome, 40 character & 10 scanlines per mode line (like GR. 0, with descenders) */
#define DL_CHR40x10x1   ((unsigned char) 3)

/* colour, 40 character & 8 scanlines per mode line (GR. 12) */
#define DL_CHR40x8x4    ((unsigned char) 4)

/* colour, 40 character & 16 scanlines per mode line (GR. 13) */
#define DL_CHR40x16x4   ((unsigned char) 5)

/* colour (duochrome per character), 20 character & 8 scanlines per mode line (GR. 1) */
#define DL_CHR20x8x2    ((unsigned char) 6)

/* colour (duochrome per character), 20 character & 16 scanlines per mode line (GR. 2) */
#define DL_CHR20x16x2   ((unsigned char) 7)


/* Bitmap modes */

/* colour, 40 pixel & 8 scanlines per mode line (GR. 3) */
#define DL_MAP40x8x4    ((unsigned char) 8)

/* 'duochrome', 80 pixel & 4 scanlines per mode line (GR.4) */
#define DL_MAP80x4x2    ((unsigned char) 9)

/* colour, 80 pixel & 4 scanlines per mode line (GR.5) */
#define DL_MAP80x4x4    ((unsigned char) 10)

/* 'duochrome', 160 pixel & 2 scanlines per mode line (GR.6) */
#define DL_MAP160x2x2   ((unsigned char) 11)

/* 'duochrome', 160 pixel & 1 scanline per mode line (GR.14) */
#define DL_MAP160x1x2   ((unsigned char) 12)

/* 4 colours, 160 pixel & 2 scanlines per mode line (GR.7) */
#define DL_MAP160x2x4   ((unsigned char) 13)

/* 4 colours, 160 pixel & 1 scanline per mode line (GR.15) */
#define DL_MAP160x1x4   ((unsigned char) 14)

/* monochrome, 320 pixel & 1 scanline per mode line (GR.8) */
#define DL_MAP320x1x1   ((unsigned char) 15)


/* Equivalents, for people familiar with Atari 8-bit OS */

#define DL_GRAPHICS0    DL_CHR40x8x1
#define DL_GRAPHICS1    DL_CHR20x8x2
#define DL_GRAPHICS2    DL_CHR20x16x2
#define DL_GRAPHICS3    DL_MAP40x8x4
#define DL_GRAPHICS4    DL_MAP80x4x2
#define DL_GRAPHICS5    DL_MAP80x4x4
#define DL_GRAPHICS6    DL_MAP160x2x2
#define DL_GRAPHICS7    DL_MAP160x2x4
#define DL_GRAPHICS8    DL_MAP320x1x1
#define DL_GRAPHICS9    DL_MAP320x1x1  /* N.B.: GRAPHICS 9, 10, and 11 also involve GTIA's PRIOR register */
#define DL_GRAPHICS10   DL_MAP320x1x1
#define DL_GRAPHICS11   DL_MAP320x1x1
#define DL_GRAPHICS12   DL_CHR40x8x4   /* N.B.: Atari 400/800 OS didn't have GRAPHICS 12 or 13 */
#define DL_GRAPHICS13   DL_CHR40x16x4
#define DL_GRAPHICS14   DL_MAP160x1x2
#define DL_GRAPHICS15   DL_MAP160x1x4

/* Atari 400/800 OS didn't have GRAPHICS 14 or 15, so they were known by "6+" and "7+" */
#define DL_GRAPHICS6PLUS DL_GRAPHICS14
#define DL_GRAPHICS7PLUS DL_GRAPHICS15

/* Neither Atari 400/800 nor XL OS supported 10-scanline (descenders) text mode via CIO */
#define DL_GRAPHICS0_DESCENDERS  DL_CHR40x10x1

/* Modifiers to mode lines */
#define DL_HSCROL(x)    ((unsigned char)((x) | 16)) /* enable smooth horizontal scrolling on this line; see HSCROL */
#define DL_VSCROL(x)    ((unsigned char)((x) | 32)) /* enable smooth vertical scrolling on this line; see VSCROL */
#define DL_LMS(x)       ((unsigned char)((x) | 64)) /* Load Memory Scan (next two bytes must be the LSB/MSB of the data to load) */

/* General modifier */
#define DL_DLI(x)       ((unsigned char)((x) | 128)) /* enable Display List Interrupt on this mode line */


/*
 * PIA
 */

/* Define a structure with the PIA register offsets */
struct __pia {
	/* port A data r/w */
    unsigned char   porta;
    
    // disable the ROM between $C000-$CFFF and $D800-$FFFF by setting bit 0 to 0.
	// Bit 1 controls BASIC; if 0, BASIC is enabled, if 1, it is disabled
	// Bits 2 and 3 control the 1200XL LEDs; 0 means on, 1 means off
	// Bits 4-6 are reserved (unused) in the XL and 65XE.
	// Bits 4 and 5 in the 130XE are used to enable bank switching
	// Bit 7 controls the RAM region $5000-$57FF, normally enabled (1).
    unsigned char   portb;

    unsigned char   pactl;  /* port A control */
    unsigned char   pbctl;  /* port B control */
};

const char PORTB_SELFTEST_OFF = %10000000;
const char PORTB_BASIC_OFF    = %00000010;
const char PORTB_SYSTEM_ON    = %00000001;


#endif
