/*
 * Atari OS definitions, mostly taken from cc65 for compatibility
 */

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

// Substitute for various OS.xxx as we aren't defining the entire OS struct
#define OS_dcb (*(struct __dcb*) 0x300)

unsigned char * OS_dvstat = 0x2EA;
// #define OS_dvstat (*(unsigned char*) 0x2EA)
